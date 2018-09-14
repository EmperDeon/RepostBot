/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_MODEL_H
#define REPOSTBOT_MODEL_H

#include <tgbot/Api.h>
#include <QtCore/QString>
#include <json.hpp>


class Model {
protected:
    QString id;

public:
    virtual void sendTelegram(int64_t from, const TgBot::Api *api) {
        printf("Method sendTelegram has no realization!");
    };
};


void from_json(const nlohmann::json &j, QString &p);

void to_json(nlohmann::json &j, const QString &p);

#endif //REPOSTBOT_MODEL_H
