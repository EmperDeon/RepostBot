/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <models/Posts.h>
#include <utils/logs/Logger.h>
#include <utils/Utils.h>
#include "VkHandler.h"

VkHandler::VkHandler(std::string default_token) {
    api = new VkApi(std::move(default_token));
    auth = new VkAuth(api);
}

void VkHandler::action(QueueTask *task) {
    const QString &name = task->action;
    const QStringList &params = task->params;

    if (name == "toggleAuth") {
        auth->toggleAuth(task, params[0] == "true");

    } else if (name == "setAuthCode") {
        auth->setAuthCode(task, params[0].toStdString());

    } else if (name == "updateGroupNames") {
        updateGroupNames(task);

    } else if (name == "getLastPost") {
        getLastPost(task, params[0], params.value(1, ""));

    } else if (name == "getLastPosts") {
        getLastPosts(task, params[0].toStdString(), params[1].toStdString());

    } else if (name == "toggleSubscription") {
        toggleSubscription(task, params[0].toStdString(), params[1] == "true");

    } else if (name == "listSubscriptions") {
        listSubscriptions(task);

    } else if (name == "fetchGroupsFromMe") {
        fetchGroupsFromMe(task);

    } else {
        logW("No such action: " + name);
    }

    emit manager->setAvailable(this);
}

void VkHandler::getLastPost(QueueTask *task, QString group_name, QString last_post_id) {
    nlohmann::json response, params;

    if (group_name.isEmpty() || group_name == "null") {
        TASK_ERROR("no group id");
    }

    params = {{"count",    "2"},
              {"extended", "1"},
              {"filter",   "owner"}};

    if (group_name[0].isDigit()) { // If group_name starts with digit, it's id
        params["owner_id"] = "-" + group_name;
    } else {
        params["domain"] = group_name;
    }

    Model *model;
    response = api->request("wall.get", params, &task->user);

    json group = response["/response/groups/0"_json_pointer],
            post = response["/response/items/0"_json_pointer];

    if (response.has_key("error") || post.empty()) {
        model = new Post;
        task->setResult(model);
        return;

    } else if (post["is_pinned"].get<int>(0)) { // If pinned
        post = response["/response/items/1"_json_pointer];
    }


    if (QString::number(post["id"].get<int>()) == last_post_id) {
        model = new Post;

    } else {
        model = new Post({
                                 {"id",         post["id"]},
                                 {"text",       post["text"]},
                                 {"group_name", group["name"]},
                                 {"group_link", "https://vk.com/public" + QString::number(group["id"].get<int>())},
                                 {"wall_link",  "https://vk.com/wall-" + QString::number(group["id"].get<int>()) + '_' +
                                                QString::number(post["id"].get<int>())}
                         });

        model->setAttachments(api->parseAttachments(post));
    }

    task->setResult(model);
}

void VkHandler::getLastPosts(QueueTask *task, std::string group_ids, std::string last_ids) {
    if (group_ids.empty() || last_ids.empty()) {
        TASK_ERROR("no group id");
    }

    // Request
    json groups;
    std::vector<std::string> parsed_group_ids = {"string"}, posts; // First item is ignored

    for (const std::string &id : Utils::split(group_ids, ','))
        parsed_group_ids.push_back(isdigit(id[0]) ? "-" + id : id);

    json response = api->request("execute.fetchLastPostFrom", {{"group_ids", Utils::join(parsed_group_ids, ',')},
                                                               {"post_ids",  last_ids}}, &task->user);

    if (response.has_key("error")) {
        logW("Error received from fetchLastPost");
        logD(response.dumpQ(1));

        task->setResult(new Post);
        return;
    }


    // Organize and reorder
    for (const json &post : response["response"]) {
        std::string owner = std::to_string(post["owner_id"].get<int>(0));

        groups[owner] += post;
    }


    // Collect to Posts
    Posts *model = new Posts;
    for (auto &it : groups.items()) {
        std::sort(it.value().begin(), it.value().end(), [](const json &post_1, const json &post_2) {
            return post_1["id"].get<int>() < post_2["id"].get<int>();
        });

        for (json &post : it.value()) {
//            Post *post_model = new Post({
//                                                {"id",       post["id"]},
//                                                {"text",     post["text"]},
//                                                {"owner_id", post["owner_id"]}
//                                        });
            post["group_name"] = api->storage()["group_names"][QString::number(post["owner_id"].get<int>()).mid(1)];

            Post *post_model = new Post(post);

            post_model->setAttachments(api->parseAttachments(post));
            model->append(post_model);
        }
    }

    task->setResult(model);
}

void VkHandler::updateGroupNames(QueueTask *task) {
    json response, &groups = api->storage()["groups"], &group_names = api->storage()["group_names"];
    std::vector<std::string> ids;

    if (!groups.empty()) {
        for (const auto &it : groups.items())
            for (const std::string &group : it.value())
                ids.push_back(group);

        response = api->request("groups.getById", {{"group_ids", Utils::join(ids, ',')}});

        if (response.has_key("error")) {
            TASK_ERROR(response.dumpQ(2));
        }
    }

    for (const json &group : response["response"]) {
        QString id = QString::number(group["id"].get<int>());
        group_names[id] = group["name"];

        for (const auto &it : groups.items()) {
            auto &item = it.value();

            if (item.has_value(group["screen_name"])) {
                item.eraseAllV(group["screen_name"]);
                item.push_back(id);
            }
        }
    }

    Storage::save();
}

void VkHandler::toggleSubscription(QueueTask *task, const std::string &group_name, bool value) {
    json &groups = api->storage()["groups"][task->user.id];
    json response = api->request("groups.getById", {{"group_id", group_name}}, &task->user);

    if (group_name.empty() || group_name == "null") {
        TASK_ERROR("no group id");
    }

    if (response.has_key("error")) {
        TASK_ERROR(response);
    }

    QString human_name = response["/response/0/name"_json_pointer];

    if (!value) {
        groups.eraseAllV(group_name);
        task->setResult(new Status("Unsubscribed from group \"" + human_name + "\""));

    } else if (!groups.has_value(group_name)) {
        if (isReachedLimit(task)) {
            TASK_ERROR("Reached limit of groups");
        }

        groups += group_name;
        task->setResult(new Status("Subscribed to group \"" + human_name + "\""));

    } else {
        TASK_ERROR("Already subscribed");
    }

    Storage::save();

    listSubscriptions(task);
}

void VkHandler::listSubscriptions(QueueTask *task) {
    json response, &groups = api->storage()["groups"][task->user.id];
    std::vector<std::string> ids;

    if (!groups.empty()) {
        for (const std::string &group : groups) ids.push_back(group);

        response = api->request("groups.getById", {{"group_ids", Utils::join(ids, ',')},
                                                   {"fields",    "name"}});

        if (response.has_key("error")) {
            TASK_ERROR(response.dumpQ(2));
        }
    }

    ids.clear();
    ids.push_back("Subscriptions:");
    for (const json &group : response["response"]) {
        QString s("%1 - %2");
        ids.push_back(s.arg(group["name"].get<QString>()).arg(group["screen_name"].get<QString>()).toStdString());
    }
    ids.push_back(QString("\nCan add %1 more groups").arg(
            groupsLimitFor(&task->user) - api->storage()["groups"][task->user.id].size()).toStdString());

    if (isReachedLimit(task)) {
        ids.push_back(
                "Due to limitations of VK wall.get API, server can successfully make only 5000 requests in 1 day, which is 'get each of 100 groups every 30 minutes'");
    }

    TASK_STATUS(Utils::join(ids, '\n').c_str());
}

void VkHandler::fetchGroupsFromMe(QueueTask *task) {
    json &groups = api->storage()["groups"][task->user.id];

    QStringList ids;
    for (const QString &group : groups) ids << group;

    auto user = task->user;
    if (user.isEmpty() || !api->storage()["tokens"].has_key(user.id)) {
        TASK_ERROR("User not logged in");
    }

    json response = api->request("groups.get", {{"count",    "100"},
                                                {"extended", "1"}}, &task->user);

    for (const json &group : response["/response/items"_json_pointer]) {
        if (isReachedLimit(task)) {
            break;
        }

        QString name = group["screen_name"];

        if (name.startsWith("club")) {
            ids << name.remove(0, 4);
        } else {
            ids << name;
        }
    }

    ids.removeDuplicates();

    groups.clear();
    for (const QString &id : ids) groups += id;

    Storage::save();

    listSubscriptions(task);
}

int VkHandler::groupsLimitFor(User *user) {
    bool has_token = user != nullptr && !user->isEmpty() && api->storage()["tokens"].has_key(user->id);
    return has_token ? 100 : 10;
}

bool VkHandler::isReachedLimit(QueueTask *task) {
    return api->storage()["groups"][task->user.id].size() >= groupsLimitFor(&task->user);
}
