/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include "QueueHelper.h"
#include <QDebug>
#include <models/Status.h>

void TestQueueHandler::action(QueueTask *task) {
    const QString &name = task->action;
    const QStringList &params = task->params;

    if (name == "test1") {
        test1(task, params[0]);
    } else if (name == "test2") {
        test2(task, params[0], params[1].toInt());
    } else {
        qDebug() << "No such action: " << name;
    }

    if (delay > 0) {
        QThread::msleep(delay);
    }

    emit manager->setAvailable(this);
}

void TestQueueHandler::test1(QueueTask *task, QString str) {
    str = QString("String - %1").arg(str);
    task->setResult(new Status(str), true);
}

void TestQueueHandler::test2(QueueTask *task, QString str, int i) {
    str = QString("String - %1, Int - %2").arg(str).arg(i);
    task->setResult(new Status(str), true);
}
