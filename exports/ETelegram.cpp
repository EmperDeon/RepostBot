/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <string>
#include <tgbot/Bot.h>
#include <tgbot/net/TgLongPoll.h>
#include <tgbot/TgException.h>
#include "ETelegram.h"
#include "imports/IVk.h"
#include "../vars.h"


void ETelegramThread::run() {
    try {
        printf("Bot username: %s\n", api->getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(*bot);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
#pragma clang diagnostic pop

    } catch (TgBot::TgException &e) {
        printf("error: %s\n", e.what());
    }
}

ETelegram::ETelegram() : bot(new TgBot::Bot(TELEGRAM_TOKEN)), api(&bot->getApi()), events(&bot->getEvents()) {
    events->onCommand("start", [this](TgBot::Message::Ptr message) {
        this->api->sendMessage(message->chat->id, "Hi!");
    });

    events->onCommand("get_last_post", [this](TgBot::Message::Ptr message) {
        std::vector<std::string> parts;
        StringTools::split(message->text, ' ', parts);

        auto *model = IVk::getLastPost(parts[1]);
        model->sendTelegram(message->chat->id, this->api);
        delete model;
    });

    events->onCommand("subscribe", [this](TgBot::Message::Ptr message) {
        std::vector<std::string> parts;
        StringTools::split(message->text, ' ', parts);

        std::string response = IVk::toggleSubscription(User(message->chat->id), parts[1], true);
        this->api->sendMessage(message->chat->id, response);
    });

    events->onCommand("unsubscribe", [this](TgBot::Message::Ptr message) {
        std::vector<std::string> parts;
        StringTools::split(message->text, ' ', parts);

        std::string response = IVk::toggleSubscription(User(message->chat->id), parts[1], false);
        this->api->sendMessage(message->chat->id, response);
    });

    events->onCommand("list_subscriptions", [this](TgBot::Message::Ptr message) {
        QString groups = IVk::listSubscriptions(User(message->chat->id));
        this->api->sendMessage(message->chat->id, groups.toStdString());
    });

    events->onCommand("export", [this](TgBot::Message::Ptr message) {
        // Send list of commands to execute/json to import.
        // Json will be better i think
    });
}

// @BotFather commands
//
// get_last_post - Get last not-pinned post from Vk group
// subscribe - Subscribe to Vk group
// unsubscribe - Unsubscribe from Vk group
// list_subscriptions - List subscribed Vk groups
//
