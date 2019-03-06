/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <models/Posts.h>
#include <apis/handlers/VkHandler.h>
#include "VkAuth.h"

VkAuth::VkAuth(VkApi *a) : api(a) {}

void VkAuth::toggleAuth(QueueTask *task, bool adding) {
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

void VkAuth::setAuthCode(QueueTask *task, const std::string &code) {
    json &tokens = api->storage()["tokens"];

    if (code.empty()) {
        TASK_ERROR("No Code is passed");
    }

    json response = api->requestAuth("access_token", {{"code", code}});

    if (response.has_key("error")) {
        TASK_ERROR("Error, try reauth");
    }

    tokens[task->user.id] = response["access_token"];
    Storage::save();

    // TODO: Verify that user token works
    response = api->request("users.get", {{"user_ids", response["user_id"]}}, &task->user);

    TASK_STATUS(QString("Logged in as %1 %2")
                        .arg(response["/response/0/first_name"].get<QString>())
                        .arg(response["/response/0/last_name"].get<QString>()));
}

QString VkAuth::authUrl() {
    return QString(
            "https://oauth.vk.com/authorize?client_id=%1&scope=groups,offline&redirect_uri=%2&display=page&v=5.84&response_type=code")
            .arg(VK_AUTH_ID)
            .arg(VK_AUTH_URL);
}

bool VkAuth::isAuthUrlDefault() {
    return QString(VK_AUTH_URL) == VK_AUTH_DEFAULT_URL;
}