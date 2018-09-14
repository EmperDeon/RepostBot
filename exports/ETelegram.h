/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_ETELEGRAM_H
#define REPOSTBOT_ETELEGRAM_H

#include "interfaces/Exporter.h"
#include "Storage.h"
#include <tgbot/Bot.h>


class ETelegramThread : public QThread {
    const TgBot::Bot *bot;
    const TgBot::Api *api;

public:
    explicit ETelegramThread(const TgBot::Bot *_bot, const TgBot::Api *_api) : bot(_bot), api(_api) {};

    void run() override;
};

class ETelegram : public Exporter {
    TgBot::Bot *bot;
    const TgBot::Api *api;
    TgBot::EventBroadcaster *events;

public:
    ETelegram();

    QThread *createThread() override { return new ETelegramThread(bot, api); };

    bool isThreadable() override { return true; };
};

#endif //REPOSTBOT_ETELEGRAM_H
