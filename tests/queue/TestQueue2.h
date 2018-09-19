/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_TESTQUEUE2_H
#define REPOSTBOT_TESTQUEUE2_H

#include <tests/TestCase.h>
#include <vendor/verdigris/src/wobjectdefs.h>
#include <QtCore/QMutex>

class TestQueue2 : public TestCase, public QObject {
    W_OBJECT(TestQueue2)

    QMutex mutex;
    int count_finished = 0, count = 0;

public:
    void runTest() override;

    void finished() W_SIGNAL(finished)

    void futureCatcher(QueueTask *task);
};


#endif //REPOSTBOT_TESTQUEUE2_H
