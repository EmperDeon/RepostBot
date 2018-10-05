/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_TASK_H
#define REPOSTBOT_TASK_H

#include <QtCore/QString>


class Task : public QObject {
public:
    virtual QString id() = 0;

    virtual void launch() = 0;

    virtual int interval() = 0;
};

#endif //REPOSTBOT_TASK_H
