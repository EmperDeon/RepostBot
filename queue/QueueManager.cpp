/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QTimer>
#include "QueueManager.h"

QueueManager::QueueManager() {
    mutex = new QMutex;
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

void QueueManager::setAvailable(QueueHandler *handler) {
    QMutexLocker locker(mutex);
    QueueTask *task;

    if ((task = takeTask(handler->name())) != nullptr) { // If next task is present, start it immediately
        startTask(handler, task);

    } else { // Else add to available
        available_handlers[handler->name()].append(handler);
    }
}

void QueueManager::addTask(QString handler_name, QueueTask *task, bool low_priority) {
    QMutexLocker locker(mutex);
    QueueHandler *handler;

    if ((handler = takeAvailable(handler_name)) != nullptr) { // If there is available handler, start task
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
    QTimer::singleShot(1, [handler, task]() {
        emit handler->handle(task);
    });
}
