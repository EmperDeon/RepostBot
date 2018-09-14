/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QCoreApplication>
#include "Runner.h"

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);

    auto *storage = Storage::instance();
    auto *app = Runner::instance();

    app->start();

    return QCoreApplication::exec();
}
