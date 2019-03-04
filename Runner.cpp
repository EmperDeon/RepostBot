/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include "Runner.h"
#include <QDebug>
#include <tasks/vk/PostsVkTask.h>
#include <apis/bots/TelegramBot.h>
#include <apis/handlers/VkHandler.h>
#include <apis/handlers/TelegramHandler.h>

Runner::Runner() {
    storage = Storage::instance();
    storage->load();
    manager = new QueueManager;
    tasks_manager = new TaskManager(this);
}

void Runner::start() {
    api_telegram = new TelegramApi;

    handlers = {new VkHandler, new TelegramHandler(api_telegram)};
    manager->addHandlers(handlers);

    bots = {{"Telegram", new TelegramBot(this, api_telegram)}};
    for (Bot *bot : bots.values()) {
        bot->start();
    }

    tasks_manager->start();
}
