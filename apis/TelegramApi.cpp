/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include <vars.h>
#include <tgbot/TgException.h>
#include <utils/Utils.h>
#include "TelegramApi.h"
#include <tgbot/net/CurlHttpClient.h>
#include <tgbot/net/BoostHttpOnlySslClient.h>
#include <utils/logs/Logger.h>
#include <QtCore/QThread>


TelegramApi::TelegramApi() {
#ifdef HAVE_CURL
    auto *client = new TgBot::CurlHttpClient;
#else
    auto *client = new TgBot::BoostHttpOnlySslClient;
#endif

    tg_bot = new TgBot::Bot(TELEGRAM_TOKEN, *client);
    tg_api = &tg_bot->getApi();
    tg_events = &tg_bot->getEvents();
}

void TelegramApi::sendMessage(int64_t user, const QString &message) {
    QStringList messages = Utils::splitMessageTo(message, 4096);

    for (const QString &split_message : messages) {
        send([user, split_message](const TgBot::Api *api) {
            api->sendMessage(user, split_message.toStdString(), true,
                                0, std::make_shared<TgBot::GenericReply>(), "HTML", false);
        });
    }
}

void TelegramApi::sendMedia(int64_t user, std::vector<TgBot::InputMedia::Ptr> attachments) {
    send([user, attachments](const TgBot::Api *api) {
        api->sendMediaGroup(user, attachments);
    });
}

void TelegramApi::sendFile(int64_t user, QString path) {
    send([user, path](const TgBot::Api *api) {
        api->sendDocument(user, TgBot::InputFile::fromFile(path.toStdString(), "text/plain"));
    });
}

void TelegramApi::sendHelp(int64_t to) {
    sendMessage(to, "Available commands:\n"
                    "/subscribe - Subscribe to Vk group\n"
                    "/unsubscribe - Unsubscribe from Vk group\n"
                    "/list_subscriptions - List subscribed Vk groups\n"
                    "/vk_login - Login to Vk\n"
                    "/vk_set_code - Enter code after vk login\n"
                    "/vk_logout - Logout from Vk\n"
                    "/fetch_groups_from_me - Subscribe to all groups from user, requires login");
}

void TelegramApi::send(std::function<void(const TgBot::Api *)> sender) {
    try {
        sender(tg_api);

        QThread::currentThread()->sleep(3);
    } catch (TgBot::TgException &e) {
        auto error = QString(e.what());
        logE("Telegram Send error: " + error);

        if (error.startsWith("Too Many Requests")) {
            QThread::currentThread()->sleep(30);
        }

    } catch (boost::system::system_error const &e) {
        logE("Boost Error: " + QString(e.what()));
    } catch (std::runtime_error const &e) {
        logE("Curl Error: " + QString(e.what()));
    }
}
