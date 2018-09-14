/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_THREADABLE_H
#define REPOSTBOT_THREADABLE_H

#include <QtCore/QThread>


class Threadable {
public:
    virtual QThread *createThread() { return nullptr; };

    virtual bool isThreadable() { return false; };
};


#endif //REPOSTBOT_THREADABLE_H
