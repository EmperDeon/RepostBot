/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef UTILS_UFUTURE_H
#define UTILS_UFUTURE_H

#include <QtCore/QMutex>
#include <QtCore/QArgument>


template<class T>
class UFuture {
protected:
    QMutex *mutex;

    bool finished = false;
    T value;

public:
    UFuture() { mutex = new QMutex; };

    T result() {
        QMutexLocker _lock(mutex);
        return value;
    }

    bool isFinished() {
        QMutexLocker _lock(mutex);
        return finished;
    }

    virtual void setResult(T _value, bool _finished = true) {
        QMutexLocker _lock(mutex);

        value = _value;

        _lock.unlock();

        if (_finished) setFinished(_finished);
    }

    virtual void setFinished(bool _finished = true) {
        QMutexLocker _lock(mutex);

        finished = _finished;
    }

    ~UFuture() { delete mutex; }
};


#endif //UTILS_UFUTURE_H
