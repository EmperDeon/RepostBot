/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_VKAPI_H
#define REPOSTBOT_VKAPI_H

#include <vars.h>
#include <models/Post.h>
#include <interfaces/User.h>
#include <utils/Curl.h>


class VkApi {
    std::string default_token;
    bool no_delay;

public:
    VkApi(std::string default_token, bool no_delay = false) : default_token(std::move(default_token)),
                                                              no_delay(no_delay) {};

    json &storage() { return Storage::instance()->value("vk"); };


    /*
     * Do a Vk request
     *
     * If user is supplied, and has custom token, it will be used in place of default
     *
     * Returns json response
     * */
    json request(const std::string &method, Curl::params_type params = {}, User *user = nullptr);


    /*
     * Ordinary request, but without token
     * */
    json requestAuth(const std::string &method, Curl::params_type params = {});


    /*
     * Parse attachments from json
     *
     * Returns list of attachments
     * */
    QList<Attachment *> parseAttachments(const json &thing);

protected:

    /*
     * Do a request to url, and return parsed response
     */
    json vk_request(const std::string &url, Curl::params_type params);

    /*
     * Get User token or default
     */
    std::string token_for_user(User *user);
};


#endif //REPOSTBOT_VKAPI_H
