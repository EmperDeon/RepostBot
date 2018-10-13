/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtCore/QCoreApplication>
#include "Runner.h"

class TCoreApplication : public QCoreApplication {
public:
    TCoreApplication(int argc, char **argv) : QCoreApplication(argc, argv) {};

    bool notify(QObject *object, QEvent *event) override {
        try {
            return QCoreApplication::notify(object, event);

        } catch (std::exception e) {
            qDebug() << "Error in Event handler: " << e.what();
        }

        return false;
    }
};

int main(int argc, char **argv) {
    TCoreApplication a(argc, argv);

    auto *storage = Storage::instance();
    auto *app = Runner::instance();

    app->start();

    return QCoreApplication::exec();
}
