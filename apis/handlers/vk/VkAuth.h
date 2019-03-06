/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_VKAUTH_H
#define REPOSTBOT_VKAUTH_H

#include <apis/VkApi.h>
#include <Runner.h>
#include <apis/handlers/VkHandler.h>


#define VK_AUTH_DEFAULT_URL "https://oauth.vk.com/blank.html"

class VkAuth {
    VkApi *api;

public:
    explicit VkAuth(VkApi *a);

    /*
     * Sends login instructions or removes existing Token
     * */
    void toggleAuth(QueueTask *task, bool adding);

    /*
     * Gets new token from authorization code
     * */
    void setAuthCode(QueueTask *task, const std::string &code);

protected:
    QString defaultAuthUrl() { return VK_AUTH_DEFAULT_URL; }

    QString authUrl();

    bool isAuthUrlDefault();
};


#endif //REPOSTBOT_VKAUTH_H
