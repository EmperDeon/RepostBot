/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_TASKMANAGER_H
#define REPOSTBOT_TASKMANAGER_H

#include <interfaces/Task.h>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <Runner.h>


class Runner;

class TaskManager : public QObject {
    QMap<QString, Task *> tasks;


public:
    TaskManager(Runner *runner);

    void start();

    void forceLaunch(const QString &id);

    json &storage() { return Storage::instance()->value("tasks"); }

protected:
    void timerEvent(QTimerEvent *e);
};


#endif //REPOSTBOT_TASKMANAGER_H
