/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include <tasks/vk/PostsVkTask.h>
#include <QtCore/QDateTime>
#include "TaskManager.h"
#include <QDebug>

TaskManager::TaskManager(Runner *runner) {
    auto *vk_posts = new PostsVkTask(runner);

    tasks = {
            {PostsVkTask::id(), vk_posts}
    };
}

void TaskManager::start() {
    startTimer(30000);
}

void TaskManager::timerEvent(QTimerEvent *) {
    json &last_launched = storage()["last_launched"];

    if (last_launched.is_null()) {
        last_launched = {};
    }

    for (auto task_id : tasks.keys()) {
        auto *task = tasks[task_id];
        QDateTime last = QDateTime::fromSecsSinceEpoch(last_launched[task_id].get<long long>(0));
        last = last.addSecs(task->interval());

        // Skip, if not enough time is passed
        if (last >= QDateTime::currentDateTime()) {
            continue;
        }

        task->launch();
        last_launched[task_id] = QDateTime::currentSecsSinceEpoch();

        Storage::save();
    }
}

void TaskManager::forceLaunch(const QString &id) {
    auto task = tasks.value(id, nullptr);

    if (task == nullptr) {
        return;
    }

    task->launch();
    storage()["last_launched"][id] = QDateTime::currentSecsSinceEpoch();

    Storage::save();
}
