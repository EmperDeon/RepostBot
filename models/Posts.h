/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_POSTS_H
#define REPOSTBOT_POSTS_H

#include <QtCore/QList>
#include "interfaces/Model.h"
#include "Post.h"


class Posts : public Model {
public:
    QList<Post *> posts;

    Posts() = default;

    void append(Post *post) {
        model_id = "1";
        posts.append(post);
    }

    void sendTo(const User &user) override {
        for (auto *post : posts)
            post->sendTo(user);
    }

    ~Posts() override {
        for (auto *post : posts)
            delete post;
    }
};

#endif //REPOSTBOT_POSTS_H
