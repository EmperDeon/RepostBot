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
#include <queue/QueueTask.h>
#include <queue/QueueManager.h>
#include <models/Attachment.h>


class QueueTask;

class ETelegramThread : public QThread {
    const TgBot::Bot *bot;
    const TgBot::Api *api;

public:
    explicit ETelegramThread(const TgBot::Bot *_bot, const TgBot::Api *_api) : bot(_bot), api(_api) {};

    void run() override;
};

class ETelegram : public Exporter, public QObject {
    TgBot::Bot *bot;
    const TgBot::Api *api;
    TgBot::EventBroadcaster *events;

    QueueManager *queue;
    QList<QueueTask *> tasks;

public:
    ETelegram();

    void addVkTask(TgBot::Message::Ptr message, const char *action, QStringList params = {});

    void handleFinished(QueueTask *task);

    void sendHelp(int64_t to);

    void sendMessage(int64_t to, const QString &message);

    void sendMedia(int64_t user, std::vector<TgBot::InputMedia::Ptr> attachments);

    QThread *createThread() override { return new ETelegramThread(bot, api); };

    bool isThreadable() override { return true; };

    QString part_at(TgBot::Message::Ptr in, int at);
};

#endif //REPOSTBOT_ETELEGRAM_H
