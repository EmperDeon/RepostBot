/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include "Post.h"

Post::Post(const nlohmann::json &obj) {
    id = QString::number(obj["id"].get<int>());
    text = obj["text"].get<QString>();
    group_name = obj["group_name"].get<QString>();
    group_link = obj["group_link"].get<QString>();

}

void Post::sendTelegram(int64_t from, const TgBot::Api *api) {
    api->sendMessage(from, toText().toStdString(), false, 0, std::make_shared<TgBot::GenericReply>(), "Markdown");
}

QString Post::toText() {
    return QString("[%1](%2)\n\n%3").arg(group_name).arg(group_link).arg(text);
}
