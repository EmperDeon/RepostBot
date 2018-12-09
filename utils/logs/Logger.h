/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H

#include <utils/Singleton.h>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtCore/QDateTime>
#include <QtCore/QQueue>
#include <vendor/verdigris/src/wobjectdefs.h>
#include <QtCore/QDir>

#define logE(m) Utils::Logger::log(Utils::Logger::Error, QDateTime::currentDateTime(), Q_FUNC_INFO, m)
#define logW(m) Utils::Logger::log(Utils::Logger::Warning, QDateTime::currentDateTime(), Q_FUNC_INFO, m)
#define logI(m) Utils::Logger::log(Utils::Logger::Info, QDateTime::currentDateTime(), Q_FUNC_INFO, m)
#define logD(m) Utils::Logger::log(Utils::Logger::Debug, QDateTime::currentDateTime(), Q_FUNC_INFO, m)
#define logV(m) Utils::Logger::log(Utils::Logger::Verbose, QDateTime::currentDateTime(), Q_FUNC_INFO, m)

#define ULOG_LEVEL_LIST QList<Utils::Logger::Level> level_list = { Utils::Logger::Error, Utils::Logger::Warning, Utils::Logger::Info, Utils::Logger::Debug, Utils::Logger::Verbose };

namespace Utils {

    class Logger : public QObject, public Utils::Singleton<Logger> {

    public:
        enum Level {
            Error, Warning, Info, Debug, Verbose
        };

    protected:
        QMutex *mutex;
        QQueue<QString> queue;

        QDir logs_dir;

    public:
        Logger();

        static void log(Logger::Level lev, const QDateTime &time, const QString &func, const QString &mess);

        static QString toLogEntry(Logger::Level level, const QDateTime &time, const QString &func, const QString &mess);

        static QString levelToString(Logger::Level level);

        QString logs();

        void save();

        QString currentLogFile();
    };

}
#endif //UTILS_LOGGER_H
