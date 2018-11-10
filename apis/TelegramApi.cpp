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

void TelegramApi::sendMessage(int64_t to, const QString &message) {
    QStringList messages = Utils::splitMessageTo(message, 4096);

    for (const QString &split_message : messages) {
        try {
            tg_api->sendMessage(to, split_message.toStdString(), true,
                                0, std::make_shared<TgBot::GenericReply>(), "HTML", false);
        } catch (TgBot::TgException &e) {
            printf("Telegram Send error: %s\n", e.what());
        } catch (boost::system::system_error const &e) {
            printf("Boost Error: %s\n", e.what());
        } catch (std::runtime_error const &e) {
            printf("Curl Error: %s\n", e.what());
        }
    }
}

void TelegramApi::sendMedia(int64_t user, std::vector<TgBot::InputMedia::Ptr> attachments) {
    try {
        tg_api->sendMediaGroup(user, attachments);
    } catch (TgBot::TgException &e) {
        printf("Telegram Send Media error: %s\n", e.what());
    } catch (boost::system::system_error const &e) {
        printf("Boost Error: %s\n", e.what());
    } catch (std::runtime_error const &e) {
        printf("Curl Error: %s\n", e.what());
    }
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