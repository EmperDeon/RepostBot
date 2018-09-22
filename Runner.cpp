/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <exports/ETelegram.h>
#include <imports/IVk.h>
#include "Runner.h"
#include <QDebug>
#include <tests/queue/TestQueue2.h>

Runner::Runner() {
    storage = Storage::instance();
    storage->load();
    manager = new QueueManager;
}

void Runner::start() {
    handlers = {
            new IVk
    };

    manager->addHandlers(handlers);

    exporters = {
            {"Telegram", new ETelegram}
    };

    for (QString key : exporters.keys()) {
        auto value = exporters[key];

        if (!value->isThreadable()) {
            continue;
        }

        key = "E" + key;

        threads[key] = value->createThread();
    }

    for (QThread *thread : threads.values()) {
        thread->start();
    }

//    TestQueue2 test;
//    test.runTest();
}
