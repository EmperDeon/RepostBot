/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <Storage.h>
#include "VkApi.h"

nlohmann::json VkApi::request(const QString &method, const nlohmann::json &params, User *user) {
    QUrl c("https://api.vk.com/method/" + method);
    QUrlQuery q;

    QString user_token;

    if (user != nullptr && !user->isEmpty()) {
        user_token = storage()["tokens"][user->id].get<QString>();
    }

    if (user_token.isEmpty()) {
        q.addQueryItem("access_token", VK_TOKEN);
    } else {
        q.addQueryItem("access_token", user_token);
    }

    q.addQueryItem("v", "5.84");

    for (const auto &it : params.items()) {
        q.addQueryItem(QString::fromStdString(it.key()), QString::fromStdString(it.value()));
    }

    c.setQuery(q);

    return vk_request(c);
}

nlohmann::json VkApi::requestAuth(const QString &method, const nlohmann::json &params) {
    nlohmann::json json;

    QUrl c("https://oauth.vk.com/" + method);
    QUrlQuery q;

    q.addQueryItem("client_id", VK_AUTH_ID);
    q.addQueryItem("client_secret", VK_AUTH_SECRET);
    q.addQueryItem("redirect_uri", VK_AUTH_URL);
    q.addQueryItem("v", "5.84");

    for (const auto &it : params.items()) {
        q.addQueryItem(QString::fromStdString(it.key()), QString::fromStdString(it.value()));
    }

    c.setQuery(q);

    return vk_request(c);
}

QList<Attachment *> VkApi::parseAttachments(const json &thing) {
    QList<Attachment *> r;

    if (!thing.has_key("attachments")) {
        return r;
    }

    for (const json &attachment : thing["attachments"]) {
        QString type = attachment["type"].get<QString>(QString());

        if (type == "photo") {
            r << new Attachment(type, attachment["/photo/sizes"_json_pointer].back()["url"].get<QString>());

        } else if (type == "video") {
        } else if (type == "audio") {
        } else if (type == "link") {
        } else if (type == "doc") {
        } else if (type == "podcast") {
        } else if (type == "album") {
        } else if (type == "poll") {
        } else {
            qDebug() << "Type " << type << "is not supported by VkApi::parseAttachment";
            qDebug() << attachment.dump(2).c_str();
        }
    }

    return r;
}

json VkApi::vk_request(const QUrl &url) {
    QNetworkAccessManager manager;
    nlohmann::json json;

    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    QEventLoop wait;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply * )), &wait, SLOT(quit()));
    QTimer::singleShot(10000, &wait, SLOT(quit()));
    wait.exec();

    QString resp = QString::fromUtf8(reply->readAll());
    reply->deleteLater();

    if (!resp.isEmpty()) {
        json = nlohmann::json::parse(resp.toStdString());

#ifdef DEBUG
        qDebug() << json.dumpQ().mid(0, 256).toStdString().c_str();
#endif
    } else {
        qDebug() << "VkApi: Response is empty";
    }

    QTimer::singleShot(3000, &wait, SLOT(quit()));
    wait.exec();

    return json;
}
