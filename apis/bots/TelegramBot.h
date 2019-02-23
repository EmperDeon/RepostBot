/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_TELEGRAMBOT_H
#define REPOSTBOT_TELEGRAMBOT_H

#include <interfaces/Bot.h>
#include <apis/TelegramApi.h>
#include <Runner.h>


class TelegramBot : public Bot {
    TelegramApi *api;

    QueueManager *queue;
    QList<QueueTask *> tasks;

public:
    TelegramBot(Runner *runner, TelegramApi *a);

    void run() override;

    void addVkTask(TgBot::Message::Ptr message, const char *action, QStringList params = {});

    void handleFinished(QueueTask *task);

protected:
    QString part_at(TgBot::Message::Ptr in, int at);

    friend class Runner;
};


#endif //REPOSTBOT_TELEGRAMBOT_H
