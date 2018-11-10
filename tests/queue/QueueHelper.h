/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_QUEUEHELPER_H
#define REPOSTBOT_QUEUEHELPER_H

#include <queue/QueueTask.h>
#include <queue/QueueHandler.h>


class TestQueueHandler : public QueueHandler {
    int delay;

public:
    TestQueueHandler(int _delay = 0) : delay(_delay) {}

    static QString className() { return "Test"; }

    QString name() override { return className(); }

    void action(QueueTask *task) override;

    void test1(QueueTask *task, QString str);

    void test2(QueueTask *task, QString str, int i);
};


#endif //REPOSTBOT_QUEUEHELPER_H
