/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_REPLYMODEL_H
#define REPOSTBOT_REPLYMODEL_H

#include <interfaces/Model.h>


class ReplyModel : public Model {
    QString text;

public:
    explicit ReplyModel(Model *origin) {
        text = std::move(origin->toString());
        model_attachments = std::move(origin->attachments());
    }

    QString toString() const override {
        return text;
    }
};

#endif //REPOSTBOT_REPLYMODEL_H
