/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <exports/ETelegram.h>
#include <imports/IVk.h>
#include "Runner.h"
#include <QDebug>
#include <tasks/vk/PostsVkTask.h>
#include <tests/TestUtils.h>

Runner::Runner() {
    storage = Storage::instance();
    storage->load();
    manager = new QueueManager;
}

void Runner::start() {
    tasks_manager = new TaskManager;

    handlers = {
            new IVk
    };

    manager->addHandlers(handlers);

    e_telegram = new ETelegram;

    exporters = {
            {"Telegram", e_telegram}
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

    tasks_manager->start();

//    TestUtils test;
//    test.runTest();
}
