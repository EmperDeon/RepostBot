/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QFile>
#include "Storage.h"
#include "vars.h"
#include <QDebug>
#include <utils/logs/Logger.h>

void Storage::load() {
    QFile f(STORAGE_FILE.c_str());

    if (f.open(QFile::ReadOnly)) {
        object = json::parse(QString::fromUtf8(f.readAll()).toStdString());
    }
}

void Storage::save() {
    QFile f(STORAGE_FILE.c_str());

    if (f.open(QFile::WriteOnly)) {
        f.write(Storage::instance()->object.dumpQ(4).toUtf8());
    } else {
        logE("Couldn't open file: " + f.errorString());
    }

    f.close();
}
