/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include <QtCore/QDateTime>
#include <QDebug>
#include <models/Posts.h>
#include <Runner.h>
#include <apis/handlers/VkHandler.h>
#include <utils/logs/Logger.h>
#include "PostsVkTask.h"

PostsVkTask::PostsVkTask(Runner *runner) {
    queue = runner->queue();
}

void PostsVkTask::launch() {
    QMap<QString, QStringList> group_ids;
    QMap<QString, QStringList> post_ids;
    json tokens = vk_storage()["tokens"], last_ids = storage()["last_ids"];

    for (const auto &it : vk_storage()["groups"].items()) {
        QString user = QString::fromStdString(it.key());

        if (!tokens.has_key(user)) {
            user = "";
        }

        for (const QString &id : it.value()) {
            group_ids[user] << id;
        }
    }

    group_ids[""].removeDuplicates();

    // Group by 25 groups by user
    for (const QString &user_id : group_ids.keys()) {
        QStringList groups, posts, temp_groups = group_ids[user_id];

        while (temp_groups.count() > 0) {
            QStringList to_add = temp_groups.mid(0, 25), posts_to_add;

            for (const auto &group : to_add) {
                posts_to_add << last_ids[group].get<QStringList>(QStringList()).join(':');
                temp_groups.removeAll(group);
            }

            groups << to_add.join(',');
            posts << posts_to_add.join(',');
        }

        group_ids[user_id] = groups;
        post_ids[user_id] = posts;
    }


    startTask("", "updateGroupNames");

    // Start tasks
    for (const QString &user_id : group_ids.keys()) {
        for (int i = 0; i < group_ids[user_id].count(); i++) {
            startTask(user_id, "getLastPosts", {group_ids[user_id][i], post_ids[user_id][i]});
        }
    }
}

void PostsVkTask::startTask(const QString &user, const QString &name, const QStringList &params) {
    auto *task = new QueueTask(User(user), name, params);

    tasks << task;
    connect(task, &QueueTask::hasFinished, this, &PostsVkTask::handleFinished);

    queue->addTask(VkHandler::className(), task);
}

void PostsVkTask::handleFinished(QueueTask *task) {
    const QString &name = task->action, &group = task->params[0];
    Posts *result = dynamic_cast<Posts *>(task->result());
    auto &ids = storage()["last_ids"];

    if (result != nullptr && !result->empty()) {
        for (auto *model : result->posts) {
            if (task->user.isEmpty()) { // Sent to all
                for (const auto &user_id : usersWithGroup(group)) {
                    model->sendTo(User(user_id));
                }
            } else { // Sent to user
                model->sendTo(task->user);
            }

            auto &last_ids = ids[model->group_id];

            last_ids.push_back(model->id()); // Creates array if empty

            if (last_ids.size() > 10) {
                for (unsigned int i = 0; i < last_ids.size() - 10; i++) {
                    last_ids.erase(last_ids.begin()); // Pop front
                }
            }

            Storage::save();
        }
    } else if (result == nullptr) {
        logW("Result is null in PostsVkTask for action: " + name);
    }

    tasks.removeAll(task);

    if (tasks.isEmpty()) {
        logI("PostsVkTask has finished");
    }

    delete result;
    delete task;
}

QStringList PostsVkTask::usersWithGroup(const QString &group) {
    QStringList r;
    json tokens = vk_storage()["tokens"];

    for (const auto &it : vk_storage().items()) {
        QString user_id = QString::fromStdString(it.key());
        if (it.value().has_value(group.toStdString()) && tokens.has_key(user_id)) {
            r << user_id;
        }
    }

    return r;
}
