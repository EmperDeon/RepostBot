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


class VkApi {

public:
    VkApi() = default;

    json &storage() { return Storage::instance()->value("vk"); };


    /*
     * Do a Vk request
     *
     * If user is supplied, and has custom token, it will be used in place of default
     *
     * Returns json response
     * */
    json request(const QString &method, const json &params = {}, User *user = nullptr);


    /*
     * Ordinary request, but without token
     * */
    json requestAuth(const QString &method, const json &params = {});


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
    json vk_request(const QUrl &url);
};


#endif //REPOSTBOT_VKAPI_H
