/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QFile>
#include <vars.h>
#include <QtCore/QDebug>
#include <utils/Utils.h>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>
#include <QtCore/QSaveFile>
#include <vendor/verdigris/src/wobjectimpl.h>
#include <utils/Zip.h>
#include "Logger.h"

Utils::Logger::Logger() : logs_dir(LOGS_DIR) {
    mutex = new QMutex;

    auto *timer = new QTimer;

    connect(timer, &QTimer::timeout, this, &Logger::save);
    connect(qApp, &QApplication::aboutToQuit, this, &Logger::save);

    timer->start(1000);
}

void Utils::Logger::log(Utils::Logger::Level lev, const QDateTime &time, const QString &func, const QString &mess) {
    auto *logger = instance();
    QMutexLocker l(logger->mutex);

    const QString &entry = toLogEntry(lev, time, func, mess);
    logger->queue.enqueue(entry);
    l.unlock();

    if (lev == Error || lev == Warning)
        logger->save();

#ifdef DEBUG
    if (lev != Verbose)
        if (lev == Error || lev == Warning) {
            fprintf(stderr, "%s\n", entry.toStdString().c_str());
        } else {
            fprintf(stdout, "%s\n", entry.toStdString().c_str());
        }
#endif
}

void Utils::Logger::save() {
    QByteArray bytes = instance()->logs().toUtf8();

    if (bytes.isEmpty())
        return;

    QFile f(currentLogFile());
    f.open(QFile::Append);

    f.write(bytes);
    f.close();
}

QString
Utils::Logger::toLogEntry(Utils::Logger::Level level, const QDateTime &time, const QString &func, const QString &mess) {
    QRegExp regexp(R"(((\w+)::)?(\w+)\()");
    regexp.indexIn(func);

    QStringList func_components = regexp.capturedTexts();
    QString f_class = func_components[2],
            f_func = func_components[3],
            formatted = QString("[%1][%2][%3]: %4").arg(time.toString(LOG_DATETIME_FORMAT)).arg(levelToString(level));

#ifdef LOG_INCLUDE_METHOD
    if (!(f_class.isEmpty() || f_func.isEmpty())) {
        formatted = formatted.arg(f_class + "::" + f_func);
    } else if (f_class.isEmpty()) {
        formatted = formatted.arg(f_func);
    } else {
        formatted = formatted.arg("");
    }
#else
    formatted = formatted.arg(f_class);
#endif

    return formatted.arg(mess);
}

QString Utils::Logger::levelToString(Utils::Logger::Level level) {
    switch (level) {
        case Error:
            return "E";
        case Warning:
            return "W";
        case Info:
            return "I";
        case Debug:
            return "D";
        case Verbose:
            return "V";
        default:
            return "I";
    }
}

QString Utils::Logger::logs() {
    QMutexLocker l(mutex);
    QString r;

    while (!queue.isEmpty()) {
        r += queue.dequeue() + '\n';
    }

    return r;
}

QString Utils::Logger::currentLogFile() {
    QDate date = QDate::currentDate();
    QString file = QString("%1_%2.log").arg(date.year()).arg(date.weekNumber());

    file = logs_dir.absoluteFilePath(file);

    if (!logs_dir.exists()) {
        QDir(BASE_PATH).mkdir("logs");
    }

    auto files = logs_dir.entryInfoList({"*.log"}, QDir::Files, QDir::Name);
    if (!QFile::exists(file) && !files.empty()) { // If new file
        files.takeLast(); // Ignore last file

        // Compress old files
        std::vector<std::string> to_zip_files;
        for (auto to_zip : files)
            to_zip_files.push_back(to_zip.absoluteFilePath().toStdString());

        Utils::Zip::addFiles(logs_dir.absoluteFilePath(LOGS_ZIP).toStdString(), to_zip_files);

        for (auto to_remove : files)
            logs_dir.remove(to_remove.fileName());
    }

    return file;
};
