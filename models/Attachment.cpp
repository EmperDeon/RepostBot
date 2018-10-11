/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <QtNetwork/QNetworkReply>
#include <QtCore/QTemporaryFile>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <vars.h>
#include <QtCore/QDir>
#include "Attachment.h"

Attachment::Attachment(const QString &_type, const QString &_url) : type(_type), url(_url) {}

QString Attachment::downloadFile(const QString &url) {
    auto reply = manager.get(QNetworkRequest(url));
    QDir dir(CACHE_PATH);

    QEventLoop loop;
    QTimer::singleShot(240'000, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QFile file(dir.absoluteFilePath(url.mid(url.lastIndexOf('/') + 1)));
    if (file.open(QFile::WriteOnly)) {
        file.write(reply->readAll());

        reply->deleteLater();

        return file.fileName();

    } else {
        reply->deleteLater();

        return "";
    }
}

TgBot::InputMedia::Ptr Attachment::toTg() {
    TgBot::InputMedia::Ptr r = std::make_shared<TgBot::InputMedia>();

    if (type == "photo") {
        r->type = TgBot::InputMedia::TYPE::PHOTO;
    } else if (type == "video") {
        r->type = TgBot::InputMedia::TYPE::VIDEO;
    }

    r->media = url.toStdString();

    return r;
}
