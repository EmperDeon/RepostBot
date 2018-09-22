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


class Runner : public QObject, public USingleton<Runner> {
    Storage *storage;
    QueueManager *manager;

    QMap<QString, Exporter *> exporters;
    QList<QueueHandler *> handlers;
    QMap<QString, QThread *> threads;

public:
    Runner();

    void start();

    QueueManager *queue() const { return manager; }
};


#endif //REPOSTBOT_RUNNER_H
