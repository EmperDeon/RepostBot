/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include "QueueHandler.h"

QueueHandler::QueueHandler() {
    thread_handle = new QThread;
    moveToThread(thread_handle);
    thread_handle->start();
    connect(this, &QueueHandler::handle, this, &QueueHandler::action);
}

void QueueHandler::setManager(QueueManager *_manager) {
    manager = _manager;
}

QueueHandler::~QueueHandler() {
    thread_handle->quit();
    thread_handle->deleteLater();
}

W_OBJECT_IMPL(QueueHandler)
