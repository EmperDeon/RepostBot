/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <models/Posts.h>
#include <utils/logs/Logger.h>
#include "VkHandler.h"

VkHandler::VkHandler() {
    api = new VkApi;
}

void VkHandler::action(QueueTask *task) {
    const QString &name = task->action;
    const QStringList &params = task->params;

    if (name == "toggleAuth") {
        toggleAuth(task, params[0] == "true");

    } else if (name == "setAuthCode") {
        setAuthCode(task, params[0]);

    } else if (name == "updateGroupNames") {
        updateGroupNames(task);

    } else if (name == "getLastPost") {
        getLastPost(task, params[0], params.value(1, ""));

    } else if (name == "getLastPosts") {
        getLastPosts(task, params[0], params[1]);

    } else if (name == "toggleSubscription") {
        toggleSubscription(task, params[0], params[1] == "true");

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

void VkHandler::getLastPosts(QueueTask *task, QString group_ids, QString last_ids) {
    if (group_ids.isEmpty() || last_ids.isEmpty()) {
        TASK_ERROR("no group id");
    }

    // Request
    json groups;
    QStringList parsed_group_ids = {"string"}, posts; // First item is ignored

    for (const QString &id : group_ids.split(','))
        parsed_group_ids << (id[0].isDigit() ? "-" + id : id);

    json response = api->request("execute.fetchLastPostFrom", {{"group_ids", parsed_group_ids.join(',')},
                                                               {"post_ids",  last_ids}}, &task->user);

    if (response.has_key("error")) {
        logW("Error received from fetchLastPost");
        logD(response.dumpQ(1));

        task->setResult(new Post);
        return;
    }


    // Organize and reorder
    for (const json &post : response["response"]) {
        QString owner = QString::number(post["owner_id"].get<int>(0));

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
    QStringList ids;

    if (!groups.empty()) {
        for (const auto &it : groups.items())
            for (const QString &group : it.value())
                ids << group;

        response = api->request("groups.getById", {{"group_ids", ids.join(',')}});

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

void VkHandler::toggleSubscription(QueueTask *task, const QString &group_name, bool value) {
    json &groups = api->storage()["groups"][task->user.id];
    json response = api->request("groups.getById", {{"group_id", group_name}}, &task->user);

    if (group_name.isEmpty() || group_name == "null") {
        TASK_ERROR("no group id");
    }

    if (response.has_key("error")) {
        TASK_ERROR(response);
    }

    QString human_name = response["/response/0/name"_json_pointer];

    if (!value) {
        groups.eraseAllV(group_name);
        task->setResult(new Status("Unsubscribed from group \"" + human_name + "\""));

    } else if (!groups.has_value(group_name.toStdString())) {
        groups += group_name;
        task->setResult(new Status("Subscribed to group \"" + human_name + "\""));

    } else {
        TASK_ERROR("Already subscribed");
    }

    Storage::save();
}

void VkHandler::listSubscriptions(QueueTask *task) {
    json response, &groups = api->storage()["groups"][task->user.id];
    QStringList ids;

    if (!groups.empty()) {
        for (const QString &group : groups) ids << group;

        response = api->request("groups.getById", {{"group_ids", ids.join(',')},
                                                   {"fields",    "name"}});

        if (response.has_key("error")) {
            TASK_ERROR(response.dumpQ(2));
        }
    }

    ids.clear();
    ids << "Subscriptions:";
    for (const json &group : response["response"]) {
        QString s("%1 - %2");
        ids << s.arg(group["name"].get<QString>()).arg(group["screen_name"].get<QString>());
    }

    TASK_STATUS(ids.join('\n'));
}

void VkHandler::fetchGroupsFromMe(QueueTask *task) {
    json &groups = api->storage()["groups"][task->user.id];

    QStringList ids;
    for (const QString &group : groups) ids << group;

    auto user = task->user;
    if (user.isEmpty() || !api->storage()["tokens"].has_key(user.id)) {
        TASK_ERROR("User not logged in");
    }

    json response = api->request("groups.get", {{"count",    "1000"},
                                                {"extended", "1"}}, &task->user);

    for (const json &group : response["/response/items"_json_pointer]) {
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

void VkHandler::toggleAuth(QueueTask *task, bool adding) {
    json &tokens = api->storage()["tokens"];

    if (task->user.isEmpty()) {
        TASK_ERROR("No User is passed");
    }

    if (adding) {
        if (!tokens.has_key(task->user.id)) {
            if (isAuthUrlDefault()) {
                QString message("<a href=\"%1\">Click to authorize in VK</a>\n\n"
                                "After successful authorization you will be redirected to %2\n"
                                "To finish authorization you need to copy parameter 'code' from address and send message /vk_set_code with code to me\n"
                                "Example: /vk_set_code 1a2b3c4d5e6f7g8h9i0j");
                TASK_STATUS(message.arg(authUrl()).arg(defaultAuthUrl()));
            } else {
                TASK_STATUS("<a href=\"" + authUrl() + "\">Click to authorize in VK</a>");
            }
        } else {
            TASK_STATUS("You are already authorized");
        }

    } else {
        if (tokens.has_key(task->user.id)) {
            tokens.erase(task->user.id);
            Storage::save();

            TASK_STATUS("Logged out");
        } else {
            TASK_STATUS("Not logged in");
        }
    }
}

void VkHandler::setAuthCode(QueueTask *task, const QString &code) {
    json &tokens = api->storage()["tokens"];

    if (code.isEmpty()) {
        TASK_ERROR("No Code is passed");
    }

    json response = api->requestAuth("access_token", {{"code", code}});

    if (response.has_key("error")) {
        TASK_ERROR("Error, try reauth");
    }

    tokens[task->user.id] = response["access_token"];
    Storage::save();

    // TODO: Verify that user token works
    response = api->request("users.get", {{"user_ids", std::to_string(response["user_id"].get<int>())}}, &task->user);

    TASK_STATUS(QString("Logged in as %1 %2").arg(response["/response/0/first_name"_json_pointer].get<QString>()).arg(
            response["/response/0/last_name"_json_pointer].get<QString>()));
}

QString VkHandler::authUrl() {
    return QString(
            "https://oauth.vk.com/authorize?client_id=%1&scope=groups,offline&redirect_uri=%2&display=page&v=5.84&response_type=code")
            .arg(VK_AUTH_ID)
            .arg(VK_AUTH_URL);
}

bool VkHandler::isAuthUrlDefault() {
    return QString(VK_AUTH_URL) == VK_AUTH_DEFAULT_URL;
}
