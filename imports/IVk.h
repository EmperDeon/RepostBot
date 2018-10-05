/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_IVK_H
#define REPOSTBOT_IVK_H

#include <models/Post.h>
#include <Storage.h>
#include <interfaces/User.h>
#include <queue/QueueHandler.h>
#include "interfaces/Importer.h"


class IVk : public QueueHandler {
public:
    IVk() = default;

    static QString className() { return "VK"; }

    QString name() override { return className(); }

    json &storage() { return Storage::instance()->value("vk.groups"); };

    void action(QueueTask *task) override;

    // Get last post from group
    //
    // Returns Post without id if post_id is same as last
    void getLastPost(QueueTask *task, QString group_name, QString last_post_id = "");

    // Toggle subscription for user
    //
    // Returns human name of group
    void toggleSubscription(QueueTask *task, const QString &group_name, bool value);

    // List subscriptions for user
    //
    // Returns human names of groups
    void listSubscriptions(QueueTask *task);

protected:
    nlohmann::json request(const QString &method, const nlohmann::json &params = {});
};


#endif //REPOSTBOT_IVK_H
