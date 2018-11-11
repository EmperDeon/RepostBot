/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include "Post.h"

Post::Post(const json &obj) {
    model_id = QString::number(obj["id"].get<int>());
    domain = obj["domain"].get<QString>();
    text = obj["text"].get<QString>();
    wall_link = "https://vk.com/wall" + QString::number(obj["owner_id"].get<int>()) + '_' + model_id;

    if (domain.startsWith('-'))
        domain.remove(0, 1);

    if (obj.has_key("group_name"))
        group_name = obj["group_name"].get<QString>(QString("Group name"));
}

QString Post::toString() const {
    return QString("<a href=\"%1\">%2</a>\n\n%3").arg(wall_link).arg(group_name).arg(text);
}
