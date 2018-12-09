/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_TELEGRAMAPI_H
#define REPOSTBOT_TELEGRAMAPI_H

#include <tgbot/Bot.h>
#include <QtCore/QString>


class TelegramApi {
    TgBot::Bot *tg_bot;
    const TgBot::Api *tg_api;
    TgBot::EventBroadcaster *tg_events;

public:
    TelegramApi();

    TgBot::Bot *bot() { return tg_bot; }

    const TgBot::Api *api() { return tg_api; }

    TgBot::EventBroadcaster *events() { return tg_events; }


    void sendHelp(int64_t to);

    void sendMessage(int64_t to, const QString &message);

    void sendMedia(int64_t user, std::vector<TgBot::InputMedia::Ptr> attachments);

    void sendFile(int64_t user, QString path);
};


#endif //REPOSTBOT_TELEGRAMAPI_H
