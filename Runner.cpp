/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <exports/ETelegram.h>
#include <imports/IVk.h>
#include "Runner.h"
#include <QDebug>

Runner::Runner() {
    storage = Storage::instance();
    storage->load();

    exporters = {
            {"Telegram", new ETelegram}
    };

    importers = {
            {"VK", new IVk}
    };
}

void Runner::start() {
    for (QString key : exporters.keys()) {
        auto value = exporters[key];

        if (!value->isThreadable()) {
            continue;
        }

        key = "E" + key;

        threads[key] = value->createThread();
    }

    for (QString key : importers.keys()) {
        auto value = importers[key];

        if (!value->isThreadable()) {
            continue;
        }

        key = "E" + key;

        threads[key] = value->createThread();
    }

    for (QThread *thread : threads.values()) {
        thread->start();
    }
}
