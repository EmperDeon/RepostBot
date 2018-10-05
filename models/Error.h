/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_ERROR_H
#define REPOSTBOT_ERROR_H

#include <interfaces/Model.h>
#include <json.hpp>


class Error : public Model {
    QString error;

public:
    explicit Error(const char *_text);

    explicit Error(const QString &_text);

    explicit Error(const nlohmann::json &obj);

    void sendTelegram(int64_t from, ETelegram *tg, bool silent = false) override;
};


#endif //REPOSTBOT_ERROR_H
