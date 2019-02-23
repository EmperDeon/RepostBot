/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_TELEGRAMHANDLER_H
#define REPOSTBOT_TELEGRAMHANDLER_H

#include <queue/QueueHandler.h>
#include <apis/TelegramApi.h>


class TelegramHandler : public QueueHandler {
    TelegramApi *api;

public:
    explicit TelegramHandler(TelegramApi *a);

    static QString className() { return "Telegram"; }

    QString name() override { return className(); }

    void action(QueueTask *task) override;


    /*
     * Sends model text and attachments
     */
    void sendModel(QueueTask *task);
};


#endif //REPOSTBOT_TELEGRAMHANDLER_H
