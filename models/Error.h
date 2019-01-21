/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_ERROR_H
#define REPOSTBOT_ERROR_H

#include <interfaces/Model.h>
#include "Storage.h"


class Error : public Model {
    QString error;

public:
    explicit Error(const char *_text);

    explicit Error(const QString &_text);

    explicit Error(const json &obj);

    QString toString() const override { return error; }

    ~Error() override = default;
};


#endif //REPOSTBOT_ERROR_H
