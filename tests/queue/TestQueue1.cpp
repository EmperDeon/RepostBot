/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include <queue/QueueManager.h>
#include "TestQueue1.h"
#include "QueueHelper.h"
#include <QDebug>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QTime>

#define TASK_COUNT 1024

void TestQueue1::runTest() {
    QTime time;
    time.start();

    auto *manager = new QueueManager;
    auto *hn1 = new TestQueueHandler(2);

    auto catcher = [manager, hn1](QueueTask *task) {
        static int finished = 0, count = 0;

        count++;

        if (task->isFinished()) {
            finished++;

//            qDebug() << task->result()->toString();
//            qDebug() << finished << count;

            task->deleteLater();
        }

        if (finished == TASK_COUNT) {
            qDebug() << "Count:" << count << ", Finished:" << finished;
            hn1->thread()->quit();
        }
    };

    manager->addHandler(hn1);

    for (int i = 0; i < TASK_COUNT; i++) {
        auto *task = new QueueTask(User(), "test1", {"test string"});
        task->connect(task, &QueueTask::hasFinished, catcher);

        manager->addTask(hn1->name(), task);
    }

    // Wait for finish

    QEventLoop loop;
    QObject::connect(hn1->thread(), &QThread::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << time.elapsed();

    qDebug() << "Finished";
}
