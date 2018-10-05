/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QFile>
#include "Storage.h"
#include "vars.h"
#include <QDebug>

void Storage::load() {
    QFile f(STORAGE_FILE);

    if (f.open(QFile::ReadOnly)) {
        object = json::parse(QString::fromUtf8(f.readAll()).toStdString());
    }
}

void Storage::save() {
    QFile f(STORAGE_FILE);

    if (f.open(QFile::WriteOnly)) {
        f.write(object.dumpQ(4).toUtf8());
    } else {
        qDebug() << f.errorString();
    }

    f.close();
}
