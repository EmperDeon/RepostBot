/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QTimer>
#include <utils/logs/Logger.h>
#include "QueueManager.h"

QueueManager::QueueManager() {
    mutex = new QMutex;
    connect(this, &QueueManager::setAvailable, this, &QueueManager::handlerAvailable);
}

void QueueManager::addHandler(QueueHandler *handler) {
    handlers[handler->name()].append(handler);
    available_handlers[handler->name()].append(handler);

    handler->setManager(this);
}

void QueueManager::addHandlers(QList<QueueHandler *> handlers) {
    for (QueueHandler *h : handlers) {
        addHandler(h);
    }
}

void QueueManager::handlerAvailable(QueueHandler *handler) {
    QMutexLocker locker(mutex);
    QueueTask *task;

    if ((task = takeTask(handler->name())) != nullptr) { // If next task is present, start it immediately
        logD(QString("Started task: %1, queue count: %2, thread_id: %3")
                     .arg(task->action)
                     .arg(low_queues[handler->name()].count() + high_queues[handler->name()].count())
                     .arg(reinterpret_cast<unsigned long long>(handler)));

        startTask(handler, task);

    } else { // Else add to available
        available_handlers[handler->name()].append(handler);
    }
}

void QueueManager::addTask(QString handler_name, QueueTask *task, bool low_priority) {
    QMutexLocker locker(mutex);
    QueueHandler *handler;

    if ((handler = takeAvailable(handler_name)) != nullptr) { // If there is available handler, start task
        logD(QString("Started task: %1, queue count: %2, thread_id: %3")
                     .arg(task->action)
                     .arg(low_queues[handler->name()].count() + high_queues[handler->name()].count())
                     .arg(reinterpret_cast<unsigned long long>(handler)));

        startTask(handler, task);

    } else if (low_priority) { // Else, add to queue
        low_queues[handler_name].enqueue(task);
    } else {
        high_queues[handler_name].enqueue(task);
    }
}

QueueHandler *QueueManager::takeAvailable(QString handler_name) {
    auto &available = available_handlers[handler_name];

    if (!available.isEmpty()) {
        return available.takeFirst();
    }

    return nullptr;
}

QueueTask *QueueManager::takeTask(QString handler_name) {
    auto &low = low_queues[handler_name], &high = high_queues[handler_name];

    if (!high.isEmpty()) {
        return high.dequeue();
    } else if (!low.isEmpty()) {
        return low.dequeue();
    }

    return nullptr;
}

void QueueManager::startTask(QueueHandler *handler, QueueTask *task) {
    emit handler->handle(task);
}

W_OBJECT_IMPL(QueueManager)
