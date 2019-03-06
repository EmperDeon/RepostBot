/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <Storage.h>
#include <utils/logs/Logger.h>
#include <thread>
#include "VkApi.h"

nlohmann::json VkApi::request(const std::string &method, Curl::params_type params, User *user) {
    params["access_token"] = token_for_user(user);

    params["v"] = "5.92";

    return vk_request("https://api.vk.com/method/" + method, params);
}

nlohmann::json VkApi::requestAuth(const std::string &method, Curl::params_type params) {
    params["client_id"] = VK_AUTH_ID;
    params["client_secret"] = VK_AUTH_SECRET;
    params["redirect_uri"] = VK_AUTH_URL;
    params["v"] = "5.92";

    return vk_request("https://oauth.vk.com/" + method, params);
}

QList<Attachment *> VkApi::parseAttachments(const json &thing) {
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
        } else if (type == "doc") {
        } else if (type == "podcast") {
        } else if (type == "album") {
        } else if (type == "poll") {
        } else {
            logD("Type " + type + "is not supported by VkApi::parseAttachment");
            logD(attachment.dumpQ(2));
        }
    }

    return r;
}

json VkApi::vk_request(const std::string &url, Curl::params_type params) {
    QNetworkAccessManager manager;
    nlohmann::json json;

    logD("Requesting url: " + QString::fromStdString(url));

    std::string reply = Curl(url, params).get();

    if (!reply.empty()) {
        json = nlohmann::json::parse(reply);

        logD("Received response:");
        logV(json.dumpQ());
    } else {
        logW("Response is empty");
    }

    if (!no_delay)
        std::this_thread::sleep_for(VK_API_DELAY);

    return json;
}

std::string VkApi::token_for_user(User *user) {
    std::string user_token;
    auto tokens = storage()["tokens"];

    if (user != nullptr && !user->isEmpty() && tokens.has_key(user->id)) {
        user_token = tokens[user->id].get<std::string>();
    }

    if (user_token.empty()) {
        return default_token;
    } else {
        return user_token;
    }
}
