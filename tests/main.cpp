/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include <vendor/catch2.hpp>
#include <curl/curl.h>


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    curl_global_init(CURL_GLOBAL_DEFAULT);

    Catch::Session session;

    auto future = QtConcurrent::run([&session, argc, argv]() {
        session.run(argc, argv);
    });

    future.waitForFinished();

    return 0;
}
