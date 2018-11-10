/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include "Runner.h"
#include <QDebug>
#include <tasks/vk/PostsVkTask.h>
#include <tests/TestUtils.h>
#include <apis/bots/TelegramBot.h>
#include <apis/handlers/VkHandler.h>

Runner::Runner() {
    storage = Storage::instance();
    storage->load();
    manager = new QueueManager;
    tasks_manager = new TaskManager(this);
}

void Runner::start() {

    handlers = {new VkHandler()};
    manager->addHandlers(handlers);

    bots = {{"Telegram", new TelegramBot(this)}};
    for (Bot *bot : bots.values()) {
        bot->start();
    }

    api_telegram = dynamic_cast<TelegramBot *>(bots["Telegram"])->api;

    tasks_manager->start();
}
