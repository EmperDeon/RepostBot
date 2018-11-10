/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_POSTSVKTASK_H
#define REPOSTBOT_POSTSVKTASK_H

#include <interfaces/Task.h>
#include <Storage.h>
#include <queue/QueueTask.h>
#include <queue/QueueManager.h>


class PostsVkTask : public Task {
    QueueManager *queue;
    QList<QueueTask *> tasks;

public:
    PostsVkTask(Runner *runner);

    static QString id() { return "task.vk.posts"; };

    void launch() override;

    void startTask(const QString &user, const QString &name, const QStringList &params = {});

    void handleFinished(QueueTask *task);

    QStringList usersWithGroup(const QString &group);

//    launch once in hour
    int interval() override { return 60 * 60; }

    json &storage() { return Storage::instance()->value("task.vk.posts"); }

    json &vk_storage() { return Storage::instance()->value("vk"); }
};


#endif //REPOSTBOT_POSTSVKTASK_H
