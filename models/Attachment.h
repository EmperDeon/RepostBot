/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_ATTACHMENT_H
#define REPOSTBOT_ATTACHMENT_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QFile>
#include <tgbot/types/InputMedia.h>


class Attachment : public QObject {
    QNetworkAccessManager manager;

    QString type, url, filename;

public:
    Attachment(const QString &_type, const QString &_url);

    virtual QString downloadFile(const QString &url);

    virtual TgBot::InputMedia::Ptr toTg();
};


#endif //REPOSTBOT_ATTACHMENT_H
