/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include <vendor/catch2.hpp>


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    Catch::Session session;

    auto future = QtConcurrent::run([&session, argc, argv]() {
        session.run(argc, argv);
    });

    future.waitForFinished();

    return 0;
}
