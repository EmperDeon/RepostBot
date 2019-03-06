/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QCoreApplication>
#include <utils/Zip.h>
#include <utils/logs/Logger.h>
#include <QtCore/QTimer>
#include <curl/curl.h>
#include <utils/Curl.h>
#include "Runner.h"

class TCoreApplication : public QCoreApplication {
public:
    TCoreApplication(int argc, char **argv) : QCoreApplication(argc, argv) {};

    bool notify(QObject *object, QEvent *event) override {
//        try {
        return QCoreApplication::notify(object, event);

//        } catch (std::exception e) {
//            qDebug() << "Error in Event handler: " << e.what();
//        }
//
//        return false;
    }
};

int main(int argc, char **argv) {
    TCoreApplication a(argc, argv);
    curl_global_init(CURL_GLOBAL_DEFAULT);

    logI("");
    logI("Started RepostBot");

    auto *logger = new Utils::Logger;
    auto *storage = Storage::instance();
    auto *app = Runner::instance();

    Q_UNUSED(logger)
    Q_UNUSED(storage)

    app->start();

    return QCoreApplication::exec();
}
