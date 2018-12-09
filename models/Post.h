/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_POST_H
#define REPOSTBOT_POST_H

#include <Storage.h>
#include "interfaces/Model.h"


class Post : public Model {
    QString group_name, wall_link;
    QString posted_at, text;

public:
    QString group_id;

    Post() = default;

    explicit Post(const json &obj);

    QString toString() const override;
};

#endif //REPOSTBOT_POST_H
