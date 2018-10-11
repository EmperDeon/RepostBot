/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_POST_H
#define REPOSTBOT_POST_H

#include "interfaces/Model.h"


class Post : public Model {
    QString group_name, group_link, wall_link;
    QString text;

public:
    Post() = default;

    explicit Post(const nlohmann::json &obj);

    QString toString() const override;
};

#endif //REPOSTBOT_POST_H
