/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include "TelegramHandler.h"

TelegramHandler::TelegramHandler(TelegramApi *a) : api(a) {}

void TelegramHandler::action(QueueTask *task) {
    const QString &name = task->action;
    const QStringList &params = task->params;

    if (name == "sendModel") {
        sendModel(task);
    }

    emit manager->setAvailable(this);
}

void TelegramHandler::sendModel(QueueTask *task) {
    User user = task->user;

    auto *model = task->result();
    auto attachments = model->attachments();

    if (user.isTelegram()) {
        api->sendMessage(user.toTg(), model->toString());

        if (!attachments.isEmpty()) {
            // FIXME: Different types of media are send as one array, add grouping by type
            std::vector<TgBot::InputMedia::Ptr> media;
            for (auto *attach : attachments) {
                media.push_back(attach->toTg());
            }

            api->sendMedia(user.toTg(), media);
        }
    }

    delete model;
    task->deleteLater();
}
