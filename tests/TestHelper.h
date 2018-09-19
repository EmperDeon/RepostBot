/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_TESTHELPER_H
#define REPOSTBOT_TESTHELPER_H

#include <interfaces/Model.h>


class TestModel : public Model {
    QString string;

public:
    explicit TestModel(const QString &s);

    QString toString() override;
};


#endif //REPOSTBOT_TESTHELPER_H
