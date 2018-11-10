/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_VKHANDLER_H
#define REPOSTBOT_VKHANDLER_H

#include <apis/VkApi.h>
#include <Runner.h>


#define VK_AUTH_DEFAULT_URL "https://oauth.vk.com/blank.html"

class VkHandler : public QueueHandler {
    VkApi *api;

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

    /*
     * Sends login instructions or removes existing Token
     * */
    void toggleAuth(QueueTask *task, bool adding);

    /*
     * Gets new token from authorization code
     * */
    void setAuthCode(QueueTask *task, const QString &code);


protected:
    QString authUrl();

    QString defaultAuthUrl() { return VK_AUTH_DEFAULT_URL; }

    bool isAuthUrlDefault();
};


#endif //REPOSTBOT_VKHANDLER_H
