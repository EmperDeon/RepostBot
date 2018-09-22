/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include "Status.h"

void Status::sendTelegram(int64_t from, const TgBot::Api *api) {
    api->sendMessage(from, string.toStdString());
}
