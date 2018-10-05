/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_USER_H
#define REPOSTBOT_USER_H

#include <QtCore/QString>


struct User {
    QString id;

    User() = default;

    explicit User(const QString &_id) : id(_id) {}

    explicit User(int64_t telegram_id) : id("T" + QString::number(telegram_id)) {}

    bool isTelegram() const { return id.startsWith('T'); }

    int64_t toTg() const {
        QString t_id = id;
        return t_id.remove(0, 1).toLong();
    }
};

#endif //REPOSTBOT_USER_H
