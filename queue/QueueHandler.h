/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_QUEUEHANDLER_H
#define REPOSTBOT_QUEUEHANDLER_H

#include <QtCore/QThread>
#include <verdigris/src/wobjectdefs.h>
#include <verdigris/src/wobjectimpl.h>
#include <models/Status.h>
#include <models/Error.h>
#include "QueueTask.h"
#include "QueueManager.h"


#define TASK_ERROR(s) task->setResult(new Error(s)); return
#define TASK_STATUS(s) task->setResult(new Status(s)); return

class QueueManager;

class QueueHandler : public QObject {
    W_OBJECT(QueueHandler)

    QThread *thread_handle;

protected:
    QueueManager *manager;

public /* signals */:
    void handle(QueueTask *task) W_SIGNAL(handle, task);

    QueueHandler();

    void setManager(QueueManager *_manager);

    ~QueueHandler();

    virtual QString name() {
        puts("name is not implemented in QueueHandler");
        return "Empty handler";
    }

    virtual void action(QueueTask *task) = 0;
};

W_REGISTER_ARGTYPE(QueueHandler *)

#endif //REPOSTBOT_QUEUEHANDLER_H
