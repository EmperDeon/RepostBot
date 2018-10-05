/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include "Error.h"

Error::Error(const char *_text) : error(_text) {}

Error::Error(const QString &_text) : error(_text) {}

Error::Error(const nlohmann::json &obj) {
    error = "Ошибка: \n" + obj.dumpQ(2);
}

void Error::sendTelegram(int64_t from, ETelegram *tg, bool silent) {
    tg->sendMessage(from, error, silent);
}
