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
#include "IVk.h"

Model *IVk::getLastPost(std::string group_name) {
    nlohmann::json response, params;

    if (group_name.empty()) {
        return new Error("нет id группы");
    }

    params = {{"count",    "2"},
              {"extended", "1"}};

    if (isdigit(group_name[0])) { // If group_name starts with digit, it's id
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
        return new Error(response);

    } else if (is_pinned) { // If pinned
        post = response["/response/items/0"_json_pointer];
    }


    return new Post({
                            {"id",         post["id"]},
                            {"text",       post["text"]},
                            {"group_name", group["name"]},
                            {"group_link", "https://vk.com/" + group_name}
                    });
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

std::string IVk::toggleSubscription(User user, std::string group_name, bool value) {
    json &groups = Storage::instance()->value("vk.groups")[user.id];
    json response = request("groups.getById", {{"group_id", group_name}});

    if (response.has_key("error")) {
        return "Group " + group_name + " not found";
    }

    std::string human_name = response["/response/0/name"_json_pointer];

    if (!value) {
        groups.eraseAllV(group_name);
        return "Unsubscribed from group \"" + human_name + "\"";

    } else if (!groups.has_value(group_name)) {
        groups += group_name;
        return "Subscribed to group \"" + human_name + "\"";

    } else {
        return "Already subscribed";
    }
}

QString IVk::listSubscriptions(User user) {
    json response, &groups = Storage::instance()->value("vk.groups")[user.id];
    QStringList ids;

    if (!groups.empty()) {
        for (const QString &group : groups) ids << group;

        response = request("groups.getById", {{"group_ids", ids.join(',')},
                                              {"fields",    "name"}});

        if (response.has_key("error")) {
            return "Error:\n" + response.dumpQ(2);
        }
    }

    ids.clear();
    ids << "Subscriptions:";
    for (const json &group : response["response"]) {
        ids << group["name"].get<QString>() + " - " + group["screen_name"].get<QString>();
    }

    return ids.join('\n');
}
