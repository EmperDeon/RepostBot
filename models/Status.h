/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_STATUS_H
#define REPOSTBOT_STATUS_H

#include <QtCore/QString>
#include <interfaces/Model.h>


class Status : public Model {
    QString string;

public:
    explicit Status(const QString &s) : string(s) {}

    QString toString() const override { return string; }

    ~Status() override = default;
};


#endif //REPOSTBOT_STATUS_H
