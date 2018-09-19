/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_QUEUETASK_H
#define REPOSTBOT_QUEUETASK_H

#include <interfaces/User.h>
#include <interfaces/Model.h>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <verdigris/src/wobjectdefs.h>
#include <utils/concurrent/UFuture.h>
#include <QDebug>

/*
 * Stores information about user/chat, action and parameters
 */
class QueueTask : public UFuture<Model *>, public QObject {
    W_OBJECT(QueueTask)

public:
    User user;
    QString action;
    QStringList params;

public /* signals */:
    void hasFinished(QueueTask *task) W_SIGNAL(hasFinished, task)

public:
    QueueTask() = default;

    QueueTask(User _user, QString _action, QStringList _params) : action(std::move(_action)),
                                                                  params(std::move(_params)) {}

    void setFinished(bool _finished) override {
        QMutexLocker _lock(mutex);

        finished = _finished;
        _lock.unlock();

        // FIXME: Calling thread (QueueHandler) waits until connected slot is finished, which can lead to bad performance if slot is slow
        emit hasFinished(this);
    }
};

W_REGISTER_ARGTYPE(QueueTask *)

#endif //REPOSTBOT_QUEUETASK_H
