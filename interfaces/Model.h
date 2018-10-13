/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_MODEL_H
#define REPOSTBOT_MODEL_H

#include <QtCore/QString>
#include <json.hpp>
#include <exports/ETelegram.h>
#include "User.h"
#include "models/Attachment.h"


class QueueTask;

class Attachment;

class Model {
protected:
    QString model_id;
    QList<Attachment *> attachments;

public:
    // Getters/Setters
    QString id() const { return model_id; }

    // Text to send through messenger
    virtual QString toString() const { return ""; };

    virtual bool empty() { return model_id.isEmpty(); }

    void setAttachments(const QList<Attachment *> &list) { attachments = list; }

    // Sending
    virtual void sendTo(const User &user);
};

void from_json(const nlohmann::json &j, QString &p);

void to_json(nlohmann::json &j, const QString &p);

#endif //REPOSTBOT_MODEL_H
