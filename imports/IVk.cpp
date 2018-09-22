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
#include <QtCore/QUrlQuery>
#include <json.hpp>
#include <models/Post.h>
#include <vars.h>
#include <models/Error.h>
#include <models/Status.h>
#include "IVk.h"

void IVk::action(QueueTask *task) {
    const QString &name = task->action;
    const QStringList &params = task->params;

    if (name == "getLastPost") {
        getLastPost(task, params[0]);

    } else if (name == "toggleSubscription") {
        toggleSubscription(task, params[0], params[1] == "true");

    } else if (name == "listSubscriptions") {
        listSubscriptions(task);

    } else {
        qDebug() << "No such action: " << name;
    }

    emit manager->setAvailable(this);
}

void IVk::getLastPost(QueueTask *task, QString group_name) {
    nlohmann::json response, params;

    if (group_name.isEmpty()) {
        TASK_ERROR("нет id группы");
    }

    params = {{"count",    "2"},
              {"extended", "1"}};

    if (group_name[0].isDigit()) { // If group_name starts with digit, it's id
        params["owner_id"] = "-" + group_name;
    } else {
        params["domain"] = group_name;
        group_name = "public" + group_name;
    }

    response = request("wall.get", params);

    json group = response["/response/groups/0"_json_pointer],
            post = response["/response/items/0"_json_pointer];
    auto is_pinned = post["is_pinned"].get<int>(0);


    if (response.has_key("error") || post.empty()) {
        TASK_ERROR(response);

    } else if (is_pinned) { // If pinned
        post = response["/response/items/1"_json_pointer];
    }


    task->setResult(new Post({
                                     {"id",         post["id"]},
                                     {"text",       post["text"]},
                                     {"group_name", group["name"]},
                                     {"group_link", "https://vk.com/" + group_name}
                             }));
}

void IVk::toggleSubscription(QueueTask *task, const QString &group_name, bool value) {
    json &groups = Storage::instance()->value("vk.groups")[task->user.id];
    json response = request("groups.getById", {{"group_id", group_name}});

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
}

void IVk::listSubscriptions(QueueTask *task) {
    json response, &groups = Storage::instance()->value("vk.groups")[task->user.id];
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
        ids << group["name"].get<QString>() + " - " + group["screen_name"].get<QString>();
    }

    task->setResult(new Status(ids.join('\n')));
}

nlohmann::json IVk::request(const QString &method, const nlohmann::json &params) {
    nlohmann::json json;

    QNetworkAccessManager manager;
    QUrl c("https://api.vk.com/method/" + method);
    QUrlQuery q;

    q.addQueryItem("access_token", VK_TOKEN);
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

    return json;
}
