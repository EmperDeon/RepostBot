/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <vendor/tgbot-cpp/include/tgbot/net/TgLongPoll.h>
#include <vendor/tgbot-cpp/include/tgbot/TgException.h>
#include <apis/handlers/VkHandler.h>
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

#ifdef DEBUG
    events->onCommand("force_update_vk_task", [runner](TgBot::Message::Ptr message) {
        runner->tasks()->forceLaunch("task.vk.posts");
    });
#endif

    /*
     * Other
     * */

    events->onCommand({"start", "help"}, [this](TgBot::Message::Ptr message) {
        api->sendHelp(message->chat->id);
    });

    events->onCommand("export", [this](TgBot::Message::Ptr message) {
        // Send list of commands to execute/json to import.
        // Json will be better i think
    });

    events->onAnyMessage([this](TgBot::Message::Ptr message) {
        qDebug() << "Received message: " << message->text.c_str();
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
        qDebug() << "Result is null in Telegram for action: " << name;
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
        printf("Bot username: %s\n", api->api()->getMe()->username.c_str());
    } catch (boost::system::system_error const &e) {
        printf("Boost Error: %s\n", e.what());
        return;
    } catch (std::runtime_error const &e) {
        printf("Curl Error: %s\n", e.what());
        return;
    }

    TgBot::TgLongPoll longPoll(*api->bot());

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        try {
            longPoll.start();
        } catch (TgBot::TgException &e) {
            printf("Telegram Error: %s\n", e.what());
        } catch (boost::system::system_error const &e) {
            printf("Boost Error: %s\n", e.what());
        } catch (std::runtime_error const &e) {
            printf("Curl Error: %s\n", e.what());
        }
    }
#pragma clang diagnostic pop
}
