/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_IVK_H
#define REPOSTBOT_IVK_H

#include <string>
#include <vector>
#include <models/Post.h>
#include <Storage.h>
#include <interfaces/User.h>
#include "interfaces/Importer.h"


class IVk : public Importer {
public:
    IVk() = default;

    static Model *getLastPost(std::string group_name);

    // Toggle subscription for user
    //
    // Returns human name of group
    static std::string toggleSubscription(User user, std::string group_name, bool value);

    // List subscriptions for user
    //
    // Returns human names of groups
    static QString listSubscriptions(User user);

protected:
    static nlohmann::json request(const QString &method, const nlohmann::json &params = {});
};


#endif //REPOSTBOT_IVK_H
