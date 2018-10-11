/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <interfaces/Model.h>
#include <queue/QueueTask.h>
#include <Runner.h>
#include <tgbot/Api.h>


void from_json(const nlohmann::json &j, QString &p) {
    p = QString::fromStdString(j);
}

void to_json(nlohmann::json &j, const QString &p) {
    j = p.toStdString();
}

void Model::sendTo(const User &user, bool silent) {
    if (user.isTelegram()) {
        auto *tg = Runner::instance()->telegram();

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
