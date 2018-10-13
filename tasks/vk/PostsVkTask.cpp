/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include <QtCore/QDateTime>
#include <QDebug>
#include <imports/IVk.h>
#include "PostsVkTask.h"

PostsVkTask::PostsVkTask() {
    queue = Runner::instance()->queue();
}

void PostsVkTask::launch() {
    QMap<QString, QStringList> group_ids;
    json tokens = vk_storage()["tokens"];

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

    // Start tasks
    for (const QString &user_id : group_ids.keys()) {
        for (const QString &group_id : group_ids[user_id]) {
            startTask(user_id, group_id, storage()["last_ids"][group_id].get<QString>(QString()));
        }
    }
}

void PostsVkTask::startTask(const QString &user, const QString &group, const QString &last_id) {
    auto *task = new QueueTask(User(user), "getLastPost", {group, last_id});

    tasks << task;
    connect(task, &QueueTask::hasFinished, this, &PostsVkTask::handleFinished);

    queue->addTask(IVk::className(), task);
}

void PostsVkTask::handleFinished(QueueTask *task) {
    const QString &name = task->action, &group = task->params[0];
    Model *result = task->result();

    if (result != nullptr && !result->empty()) {
        if (task->user.isEmpty()) { // Sent to all
            for (const auto &user_id : usersWithGroup(group)) {
                result->sendTo(User(user_id));
            }
        } else { // Sent to user
            result->sendTo(task->user);
        }

        storage()["last_ids"][group] = result->id();
        Storage::save();

    } else if (result == nullptr) {
        qDebug() << "Result is null in PostsVkTask for action: " << name;
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
