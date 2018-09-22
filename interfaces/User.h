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

    explicit User(int64_t telegram_id) : id(QString::number(telegram_id)) {}

    int64_t toTg() const { return id.toLong(); }
};

#endif //REPOSTBOT_USER_H
