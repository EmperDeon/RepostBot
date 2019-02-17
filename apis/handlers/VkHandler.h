/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_VKHANDLER_H
#define REPOSTBOT_VKHANDLER_H

#include <apis/VkApi.h>
#include <Runner.h>
#include <apis/handlers/vk/VkAuth.h>


class VkHandler : public QueueHandler {
    VkApi *api;
    VkAuth *auth;

public:
    VkHandler();

    static QString className() { return "VK"; }

    QString name() override { return className(); }

    void action(QueueTask *task) override;

    /*
     * Get last post from group
     *
     * Returns Post without id if post_id is same as last
     * */
    void getLastPost(QueueTask *task, QString group_name, QString last_post_id);

    /*
     * Get last posts from groups
     * Vk limitation: number of groups should be equal or less 25
     *  For now, group_ids and last_ids are list, joined by ','
     *
     *  Returns posts list since last request(last_ids), grouped by Group
     * */
    void getLastPosts(QueueTask *task, QString group_ids, QString last_ids);

    /*
     * Fetch names for all group ids
     * */
    void updateGroupNames(QueueTask *task);

    /*
     * Toggle subscription for user
     *
     * Returns human name of group
     * */
    void toggleSubscription(QueueTask *task, const QString &group_name, bool value);

    /*
     * List subscriptions for user
     *
     * Returns human names of groups
     * */
    void listSubscriptions(QueueTask *task);

    /*
     * Add subscriptions from user
     *
     * Returns human names of groups
     * */
    void fetchGroupsFromMe(QueueTask *task);


protected:
    int groupsLimitFor(User *u);

    bool isReachedLimit(QueueTask *task);
};


#endif //REPOSTBOT_VKHANDLER_H
