/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QTimer>
#include <QtCore/QEventLoop>
#include <QtCore/QUrlQuery>
#include <json.hpp>
#include <models/Post.h>
#include <models/Error.h>
#include <models/Status.h>
#include "IVk.h"

void IVk::action(QueueTask *task) {
    const QString &name = task->action;
    const QStringList &params = task->params;

    if (name == "toggleAuth") {
        toggleAuth(task, params[0] == "true");

    } else if (name == "setAuthCode") {
        setAuthCode(task, params[0]);

    } else if (name == "getLastPost") {
        if (params.size() > 1) {
            getLastPost(task, params[0], params.value(1, ""));
        }

    } else if (name == "toggleSubscription") {
        toggleSubscription(task, params[0], params[1] == "true");

    } else if (name == "listSubscriptions") {
        listSubscriptions(task);

    } else if (name == "fetchGroupsFromMe") {
        fetchGroupsFromMe(task);

    } else {
        qDebug() << "No such action: " << name;
    }

    emit manager->setAvailable(this);
}

void IVk::getLastPost(QueueTask *task, QString group_name, QString last_post_id) {
    nlohmann::json response, params;

    if (group_name.isEmpty() || group_name == "null") {
        TASK_ERROR("no group id");
    }

    params = {{"count",    "2"},
              {"extended", "1"}};

    if (group_name[0].isDigit()) { // If group_name starts with digit, it's id
        params["owner_id"] = "-" + group_name;
    } else {
        params["domain"] = group_name;
    }

    response = request("wall.get", params, &task->user);

    json group = response["/response/groups/0"_json_pointer],
            post = response["/response/items/0"_json_pointer];
    auto is_pinned = post["is_pinned"].get<int>(0);


    if (response.has_key("error") || post.empty()) {
        TASK_ERROR(response);

    } else if (is_pinned) { // If pinned
        post = response["/response/items/1"_json_pointer];
    }

    Model *model;

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

        model->setAttachments(parseAttachments(post));
    }

    task->setResult(model);
}

void IVk::toggleSubscription(QueueTask *task, const QString &group_name, bool value) {
    json &groups = storage()["groups"][task->user.id];
    json response = request("groups.getById", {{"group_id", group_name}}, &task->user);

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

void IVk::listSubscriptions(QueueTask *task) {
    json response, &groups = storage()["groups"][task->user.id];
    QStringList ids;

    if (!groups.empty()) {
        for (const QString &group : groups) ids << group;

        response = request("groups.getById", {{"group_ids", ids.join(',')},
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

void IVk::fetchGroupsFromMe(QueueTask *task) {
    json &groups = storage()["groups"][task->user.id];

    QStringList ids;
    for (const QString &group : groups) ids << group;

    json response = request("groups.get", {{"count",    "1000"},
                                           {"extended", "1"},
                                           {"user_id",  "381936777"}});

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
}


nlohmann::json IVk::request(const QString &method, const nlohmann::json &params, User *user) {
    nlohmann::json json;

    QNetworkAccessManager manager;
    QUrl c("https://api.vk.com/method/" + method);
    QUrlQuery q;

    QString user_token;

    if (user != nullptr && !user->isEmpty()) {
        user_token = storage()["tokens"][user->id].get<QString>();
    }

    if (user_token.isEmpty()) {
        q.addQueryItem("access_token", VK_TOKEN);
    } else {
        q.addQueryItem("access_token", user_token);
    }

    q.addQueryItem("v", "5.84");

    for (const auto &it : params.items()) {
        q.addQueryItem(QString::fromStdString(it.key()), QString::fromStdString(it.value()));
    }

    c.setQuery(q);

    QNetworkReply *reply = manager.get(QNetworkRequest(c));
    QEventLoop wait;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply * )), &wait, SLOT(quit()));
    QTimer::singleShot(10000, &wait, SLOT(quit()));
    wait.exec();
    QByteArray resp = reply->readAll();
    json = nlohmann::json::parse(QString::fromUtf8(resp).toStdString());
    reply->deleteLater();

#ifdef DEBUG
//    qDebug() << json.dump(1).c_str();
#endif

    QTimer::singleShot(1500, &wait, SLOT(quit()));
    wait.exec();

    return json;
}

nlohmann::json IVk::requestAuth(const QString &method, const nlohmann::json &params) {
    nlohmann::json json;

    QNetworkAccessManager manager;
    QUrl c("https://oauth.vk.com/" + method);
    QUrlQuery q;

    q.addQueryItem("client_id", VK_AUTH_ID);
    q.addQueryItem("client_secret", VK_AUTH_SECRET);
    q.addQueryItem("redirect_uri", VK_AUTH_URL);

    for (const auto &it : params.items()) {
        q.addQueryItem(QString::fromStdString(it.key()), QString::fromStdString(it.value()));
    }

    c.setQuery(q);

    QNetworkReply *reply = manager.get(QNetworkRequest(c));
    QEventLoop wait;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply * )), &wait, SLOT(quit()));
    QTimer::singleShot(10000, &wait, SLOT(quit()));
    wait.exec();
    QByteArray resp = reply->readAll();
    json = nlohmann::json::parse(QString::fromUtf8(resp).toStdString());
    reply->deleteLater();

#ifdef DEBUG
    qDebug() << json.dump(1).c_str();
#endif

    return json;
}

QList<Attachment *> IVk::parseAttachments(const json &thing) {
    QList<Attachment *> r;

    if (!thing.has_key("attachments")) {
        return r;
    }

    for (const json &attachment : thing["attachments"]) {
        QString type = attachment["type"].get<QString>(QString());

        if (type == "photo") {
            r << new Attachment(type, attachment["/photo/sizes"_json_pointer].back()["url"].get<QString>());

        } else if (type == "video") {
        } else if (type == "audio") {
        } else if (type == "link") {
        } else if (type == "poll") {
        } else {
            qDebug() << "Type " << type << "is not supported by IVk::parseAttachment";
            qDebug() << attachment.dump(2).c_str();
        }
    }

    return r;
}

void IVk::toggleAuth(QueueTask *task, bool adding) {
    json &tokens = storage()["tokens"];

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

void IVk::setAuthCode(QueueTask *task, const QString &code) {
    json &tokens = storage()["tokens"];

    if (code.isEmpty()) {
        TASK_ERROR("No Code is passed");
    }

    json response = requestAuth("access_token", {{"code", code}});

    if (response.has_key("error")) {
        TASK_ERROR("Error, try reauth");
    }

    tokens[task->user.id] = response["access_token"];
    Storage::save();

    // TODO: Verify that user token works
    response = request("users.get", {{"user_ids", std::to_string(response["user_id"].get<int>())}}, &task->user);

    TASK_STATUS(QString("Logged in as %1 %2").arg(response["/response/0/first_name"_json_pointer].get<QString>()).arg(
            response["/response/0/last_name"_json_pointer].get<QString>()));
}

QString IVk::authUrl() {
    return QString(
            "https://oauth.vk.com/authorize?client_id=%1&scope=groups,offline&redirect_uri=%2&display=page&v=5.84&response_type=code")
            .arg(VK_AUTH_ID)
            .arg(VK_AUTH_URL);
}

bool IVk::isAuthUrlDefault() {
    return QString(VK_AUTH_URL) == VK_AUTH_DEFAULT_URL;
}
