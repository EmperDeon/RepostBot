/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include "Post.h"

Post::Post(const nlohmann::json &obj) {
    model_id = QString::number(obj["id"].get<int>());
    text = obj["text"].get<QString>();
    group_name = obj["group_name"].get<QString>();
    group_link = obj["group_link"].get<QString>();
    wall_link = obj["wall_link"].get<QString>();
}

QString Post::toString() const {
    return QString("[%1](%2) - [Wall](%3) \n\n%4").arg(group_name).arg(group_link).arg(wall_link).arg(text);
}
