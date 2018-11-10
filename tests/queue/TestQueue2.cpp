/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include <queue/QueueManager.h>
#include "TestQueue2.h"
#include "QueueHelper.h"
#include <QDebug>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QMutexLocker>

#define TASK_COUNT 1024

void TestQueue2::runTest() {
    QTime time;
    time.start();

    auto *manager = new QueueManager;
    auto *hn1 = new TestQueueHandler(4), *hn2 = new TestQueueHandler(4), *hn3 = new TestQueueHandler(
            4), *hn4 = new TestQueueHandler(4);

    manager->addHandlers({hn1, hn2, hn3, hn4});

    for (int i = 0; i < TASK_COUNT; i++) {
        auto *task = new QueueTask(User(), "test1", {"test string"});
        task->connect(task, &QueueTask::hasFinished, this, &TestQueue2::futureCatcher);

        manager->addTask(hn1->name(), task);
    }

    // Wait for finish

    QEventLoop loop;
    QObject::connect(this, &TestQueue2::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << time.elapsed();

    qDebug() << "Finished";
    qDebug() << "Count:" << count << ", Finished:" << count_finished;
}

void TestQueue2::futureCatcher(QueueTask *task) {
    QMutexLocker lock(&this->mutex);

    count++;

    if (task->isFinished()) {
        count_finished++;

//        qDebug() << QThread::currentThread() << task->result()->toString();

        delete task->result();
        task->deleteLater();
    }

    if (count_finished == TASK_COUNT) {
        emit finished();
    }
}

W_OBJECT_IMPL(TestQueue2)
