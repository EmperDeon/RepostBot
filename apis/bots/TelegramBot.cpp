/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <vendor/tgbot-cpp/include/tgbot/net/TgLongPoll.h>
#include <vendor/tgbot-cpp/include/tgbot/TgException.h>
#include <apis/handlers/VkHandler.h>
#include <utils/logs/Logger.h>
#include "TelegramBot.h"

/* @BotFather commands
 *
 * start
 * help
 * import
 * export
 *
 * vk_login - Login to Vk
 * vk_set_code - Enter code after vk login
 * vk_logout - Logout from Vk
 *
 * subscribe - Subscribe to Vk group
 * unsubscribe - Unsubscribe from Vk group
 * list_subscriptions - List subscribed Vk groups
 * fetch_groups_from_me - Subscribe to all groups from user, requires login
 * */

TelegramBot::TelegramBot(Runner *runner) {
    api = new TelegramApi;
    queue = runner->queue();
    auto *events = api->events();


    /*
     * Authorization
     * */

    events->onCommand({"vk_login", "vk_logout"}, [this](TgBot::Message::Ptr message) {
        addVkTask(message, "toggleAuth", {(message->text == "/vk_login" ? "true" : "false")});
    });

    events->onCommand("vk_set_code", [this](TgBot::Message::Ptr message) {
        addVkTask(message, "setAuthCode", {part_at(message, 1)});
    });


    /*
     * Subscription
     * */

    events->onCommand({"subscribe", "unsubscribe"}, [this](TgBot::Message::Ptr message) {
        addVkTask(message, "toggleSubscription",
                  {part_at(message, 1), (message->text == "/subscribe" ? "true" : "false")});
    });

    events->onCommand("list_subscriptions", [this](TgBot::Message::Ptr message) {
        addVkTask(message, "listSubscriptions");
    });

    events->onCommand("fetch_groups_from_me", [this](TgBot::Message::Ptr message) {
        addVkTask(message, "fetchGroupsFromMe");
    });

    events->onCommand("fetch_logs", [this](TgBot::Message::Ptr message) {
        if (message->chat->id == TELEGRAM_ADMIN) {
            api->sendFile(TELEGRAM_ADMIN, Utils::Logger::instance()->currentLogFile());
        } else {
            logW(QString("Wrong user_id, expected: %1, actual: %2").arg(TELEGRAM_ADMIN).arg(message->chat->id));
        }
    });

    events->onCommand("force_update_vk_task", [runner](TgBot::Message::Ptr message) {
        if (message->chat->id == TELEGRAM_ADMIN) {
            runner->tasks()->forceLaunch("task.vk.posts");
        } else {
            logW(QString("Wrong user_id, expected: %1, actual: %2").arg(TELEGRAM_ADMIN).arg(message->chat->id));
        }
    });


    /*
     * Other
     * */

    events->onCommand({"start", "help"}, [this](TgBot::Message::Ptr message) {
        api->sendHelp(message->chat->id);
    });

    events->onCommand("export", [this](TgBot::Message::Ptr) {
        // Send list of commands to execute/json to import.
        // Json will be better i think
    });

    events->onAnyMessage([this](TgBot::Message::Ptr message) {
        logI("Received message: " + QString::fromStdString(message->text));
    });
}

void TelegramBot::addVkTask(TgBot::Message::Ptr message, const char *action, QStringList params) {
    auto *task = new QueueTask(User(message->chat->id), action, params);

    tasks << task;
    connect(task, &QueueTask::hasFinished, this, &TelegramBot::handleFinished);

    queue->addTask(VkHandler::className(), task, false);
}

void TelegramBot::handleFinished(QueueTask *task) {
    const QString &name = task->action;
    const User &user = task->user;
    Model *result = task->result();

    if (result != nullptr) {
        result->sendTo(user);

    } else {
        logW("Result is null in Telegram for action: " + name);
    }

    delete result;
    delete task;
}

QString TelegramBot::part_at(TgBot::Message::Ptr in, int at) {
    QStringList parts = QString::fromStdString(in->text).split(' ');

    return parts.value(at, "null");
}

void TelegramBot::run() {
    try {
        logI("Bot username: " + QString::fromStdString(api->api()->getMe()->username));
    } catch (boost::system::system_error const &e) {
        logE("Boost Error: " + QString(e.what()));
        return;
    } catch (std::runtime_error const &e) {
        logE("Curl Error: " + QString(e.what()));
        return;
    }

    TgBot::TgLongPoll longPoll(*api->bot());

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        try {
            longPoll.start();
        } catch (TgBot::TgException &e) {
            logE("Telegram Error: " + QString(e.what()));
        } catch (boost::system::system_error const &e) {
            logE("Boost Error: " + QString(e.what()));
        } catch (std::runtime_error const &e) {
            logE("Curl Error: " + QString(e.what()));
        }
    }
#pragma clang diagnostic pop
}
