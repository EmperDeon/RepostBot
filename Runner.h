/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_RUNNER_H
#define REPOSTBOT_RUNNER_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <Storage.h>
#include <queue/QueueManager.h>
#include <tasks/TaskManager.h>
#include <apis/TelegramApi.h>
#include <interfaces/Bot.h>

class TaskManager;

class Runner : public QObject, public USingleton<Runner> {
    Storage *storage;
    QueueManager *manager;
    TaskManager *tasks_manager;

    TelegramApi *api_telegram;

    QMap<QString, Bot *> bots;
    QList<QueueHandler *> handlers;

public:
    Runner();

    void start();

    QueueManager *queue() const { return manager; }

    TaskManager *tasks() const { return tasks_manager; }

    TelegramApi *telegramApi() const { return api_telegram; }
};


#endif //REPOSTBOT_RUNNER_H
