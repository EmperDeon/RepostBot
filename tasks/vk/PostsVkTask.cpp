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
    QStringList group_ids;

    for (const auto &it : vk_storage().items()) {
        for (const QString &id : it.value()) {
            group_ids << id;
        }
    }

    group_ids.removeDuplicates();

    // Start tasks
    for (const QString &id : group_ids) {
        startTask(id, storage()["last_ids"][id].get<QString>(QString()));
    }
}

void PostsVkTask::startTask(const QString &group, const QString &last_id) {
    auto *task = new QueueTask(User(), "getLastPost", {group, last_id});

    tasks << task;
    connect(task, &QueueTask::hasFinished, this, &PostsVkTask::handleFinished);

    queue->addTask(IVk::className(), task);
}

void PostsVkTask::handleFinished(QueueTask *task) {
    const QString &name = task->action, &group = task->params[0];
    Model *result = task->result();

    if (name == "getLastPost") {
        if (!result->empty()) {
            for (const auto &user_id : usersWithGroup(group)) {
                result->sendTo(User(user_id), true);
            }

            storage()["last_ids"][group] = result->id();
            Storage::instance()->save();
        }
    } else {
        qDebug() << "No handler in PostsVkTask for action: " << name;
    }

    delete result;
    delete task;
}

QStringList PostsVkTask::usersWithGroup(const QString &group) {
    QStringList r;

    for (const auto &it : vk_storage().items()) {
        if (it.value().has_value(group.toStdString())) {
            r << QString::fromStdString(it.key());
        }
    }

    return r;
}
