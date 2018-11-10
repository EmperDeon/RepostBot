/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <interfaces/Model.h>
#include <queue/QueueTask.h>
#include <Runner.h>
#include <tgbot/Api.h>

void Model::sendTo(const User &user) {
    if (user.isTelegram()) {
        auto *tg = Runner::instance()->telegramApi();

        tg->sendMessage(user.toTg(), toString());

        if (!attachments.isEmpty()) {
            std::vector<TgBot::InputMedia::Ptr> media;
            for (auto *attach : attachments) {
                media.push_back(attach->toTg());
            }

            tg->sendMedia(user.toTg(), media);
        }
    } else {
        puts("Unsupported User\n");
    }
}
