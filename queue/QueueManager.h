/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_QUEUEMANAGER_H
#define REPOSTBOT_QUEUEMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QQueue>
#include <tests/TestCase.h>
#include "QueueTask.h"
#include "QueueHandler.h"


class QueueHandler;

class QueueManager : public QObject {
    W_OBJECT(QueueManager)

    QMutex *mutex;

    QMap<QString, QList<QueueHandler *>> handlers, available_handlers;
    QMap<QString, QQueue<QueueTask *>> high_queues, low_queues;

public:
    QueueManager();

    // For handlers - adds handler to available list
    void setAvailable(QueueHandler *handler) W_SIGNAL(setAvailable, handler)

    // For Runner
    void addHandler(QueueHandler *handler);

    void addHandlers(QList<QueueHandler *> handlers);



    // For task creators - takes first available handler and gives it to him
    void addTask(QString handler_name, QueueTask *task, bool low_priority = true);


protected:
    void handlerAvailable(QueueHandler *handler);

    QueueHandler *takeAvailable(QString handler_name);

    QueueTask *takeTask(QString handler_name);

    void startTask(QueueHandler *handler, QueueTask *task);

TEST_HELPER
};


#endif //REPOSTBOT_QUEUEMANAGER_H
