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
#include <interfaces/Exporter.h>
#include <interfaces/Importer.h>
#include <queue/QueueManager.h>
#include <tasks/TaskManager.h>


class ETelegram;

class Runner : public QObject, public USingleton<Runner> {
    Storage *storage;
    QueueManager *manager;
    TaskManager *tasks_manager;

    ETelegram *e_telegram;

    QMap<QString, Exporter *> exporters;
    QList<QueueHandler *> handlers;
    QMap<QString, QThread *> threads;

public:
    Runner();

    void start();

    QueueManager *queue() const { return manager; }

    TaskManager *tasks() const { return tasks_manager; }

    ETelegram *telegram() const { return e_telegram; }
};


#endif //REPOSTBOT_RUNNER_H
