/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <vars.h>
#include "Post.h"

Post::Post(const json &obj) {
    model_id = QString::number(obj["id"].get<int>());
    group_id = QString::number(obj["from_id"].get<int>());
    text = obj["text"].get<QString>();
    posted_at = QDateTime::fromSecsSinceEpoch(obj["date"].get<int>()).toString(DATETIME_FORMAT);
    wall_link = "https://vk.com/wall" + QString::number(obj["owner_id"].get<int>()) + '_' + model_id;

    if (group_id.startsWith('-'))
        group_id.remove(0, 1);

    if (obj.has_key("group_name"))
        group_name = obj["group_name"].get<QString>(QString("Group name"));
}

QString Post::toString() const {
    return QString("<a href=\"%1\">%2</a> - %3\n\n%4").arg(wall_link).arg(group_name).arg(posted_at).arg(text);
}
