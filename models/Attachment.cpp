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
#include <curl/curl.h>
#include <utils/Curl.h>
#include <utils/Utils.h>
#include "Attachment.h"

Attachment::Attachment(const QString &_type, const QString &_url) : type(_type), url(_url) {}

std::string Attachment::downloadFile(const QString &url) {
    QDir dir(CACHE_PATH.c_str());
    if (!dir.exists()) {
        QDir(BASE_PATH.c_str()).mkdir("cache");
    }

    std::string file_path = Curl(url.toStdString()).getToDir(dir.absolutePath().toStdString());

    if (Utils::fileExists(file_path)) {
        return file_path;
    } else {
        return "";
    }
}

TgBot::InputMedia::Ptr Attachment::toTg() {
    TgBot::InputMedia::Ptr r = std::make_shared<TgBot::InputMedia>();

    if (type == "photo") {
        r->type = TgBot::InputMedia::TYPE::PHOTO;
        r->media = url.toStdString();
    } else if (type == "video") {
        r->type = TgBot::InputMedia::TYPE::VIDEO;
        r->media = url.toStdString();
    } else if (type == "file") {
        r->type = TgBot::InputMedia::TYPE::DOCUMENT;
        r->media = "attach://" + url.toStdString();
    }

    return r;
}
