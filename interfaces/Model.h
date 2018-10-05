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
#include <queue/QueueTask.h>
#include <Runner.h>
#include <exports/ETelegram.h>
#include "User.h"


class QueueTask;

class Model {
protected:
    QString model_id;

public:
    virtual bool empty() { return model_id.isEmpty(); }

    virtual void sendTelegram(int64_t from, ETelegram *tg, bool silent = false) {
        printf("Method sendTelegram has no realization!");
    };

    virtual void sendTo(const User &user, bool silent = false) {
        if (user.isTelegram()) {
            sendTelegram(user.toTg(), Runner::instance()->telegram(), silent);
        } else {
            puts("Unsupported User\n");
        }
    }

    QString id() const { return model_id; }

    virtual QString toString() const { return ""; };
};


void from_json(const nlohmann::json &j, QString &p);

void to_json(nlohmann::json &j, const QString &p);

#endif //REPOSTBOT_MODEL_H
