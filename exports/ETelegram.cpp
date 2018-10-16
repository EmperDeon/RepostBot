/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <string>
#include <tgbot/Bot.h>
#include <tgbot/net/TgLongPoll.h>
#include <tgbot/TgException.h>
#include <utils/Utils.h>
#include <tasks/vk/PostsVkTask.h>
#include "Runner.h"
#include "ETelegram.h"
#include "imports/IVk.h"
#include "../vars.h"


void ETelegramThread::run() {
    try {
        printf("Bot username: %s\n", api->getMe()->username.c_str());
    } catch (boost::system::system_error const &e) {
        printf("Boost Error: %s\n", e.what());
        return;
    }

    TgBot::TgLongPoll longPoll(*bot);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        try {
            longPoll.start();
        } catch (TgBot::TgException &e) {
            printf("Telegram Error: %s\n", e.what());
        } catch (boost::system::system_error const &e) {
            printf("Boost Error: %s\n", e.what());
        }
    }
#pragma clang diagnostic pop
}

ETelegram::ETelegram() : bot(new TgBot::Bot(TELEGRAM_TOKEN)), api(&bot->getApi()), events(&bot->getEvents()) {
    queue = Runner::instance()->queue();

    //
    // Authorization
    //
    events->onCommand({"vk_login", "vk_logout"}, [this](TgBot::Message::Ptr message) {
        addVkTask(message, "toggleAuth", {(message->text == "/vk_login" ? "true" : "false")});
    });

    events->onCommand("vk_set_code", [this](TgBot::Message::Ptr message) {
        addVkTask(message, "setAuthCode", {part_at(message, 1)});
    });

    //
    // Subscription
    //
    events->onCommand("get_last_post", [this](TgBot::Message::Ptr message) {
        addVkTask(message, "getLastPost", {"156450176"});
        addVkTask(message, "getLastPost", {"143655281"});
    });

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
    events->onCommand("force_update_vk_task", [this](TgBot::Message::Ptr message) {
        Runner::instance()->tasks()->forceLaunch(PostsVkTask().id());
    });
#endif

    //
    // Other
    //

    events->onCommand({"start", "help"}, [this](TgBot::Message::Ptr message) {
        sendHelp(message->chat->id);
    });

    events->onCommand("export", [this](TgBot::Message::Ptr message) {
        // Send list of commands to execute/json to import.
        // Json will be better i think
    });

    events->onAnyMessage([this](TgBot::Message::Ptr message) {
        qDebug() << "Received message: " << message->text.c_str();
    });
}

void ETelegram::addVkTask(TgBot::Message::Ptr message, const char *action, QStringList params) {
    auto *task = new QueueTask(User(message->chat->id), action, params);

    tasks << task;
    connect(task, &QueueTask::hasFinished, this, &ETelegram::handleFinished);

    queue->addTask(IVk::className(), task, false);
}

void ETelegram::handleFinished(QueueTask *task) {
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

void ETelegram::sendMessage(int64_t to, const QString &message) {
    QStringList messages = Utils::splitMessageTo(message, 4096);

    for (const QString &split_message : messages) {
        try {
            api->sendMessage(to, split_message.toStdString(), false, 0, std::make_shared<TgBot::GenericReply>(),
                             "HTML", false);
        } catch (TgBot::TgException &e) {
            printf("Telegram Send error: %s\n", e.what());
        }
    }
}

void ETelegram::sendMedia(int64_t user, std::vector<TgBot::InputMedia::Ptr> attachments) {
    try {
        api->sendMediaGroup(user, attachments);
    } catch (TgBot::TgException &e) {
        printf("Telegram Send Media error: %s\n", e.what());
    }
}

QString ETelegram::part_at(TgBot::Message::Ptr in, int at) {
    QStringList parts = QString::fromStdString(in->text).split(' ');

    return parts.value(at, "null");
}

void ETelegram::sendHelp(int64_t to) {
    sendMessage(to, "Available commands:\n"
                    "/subscribe - Subscribe to Vk group\n"
                    "/unsubscribe - Unsubscribe from Vk group\n"
                    "/list_subscriptions - List subscribed Vk groups\n"
                    "/vk_login - Login to Vk\n"
                    "/vk_set_code - Enter code after vk login\n"
                    "/vk_logout - Logout from Vk\n"
                    "/fetch_groups_from_me - Subscribe to all groups from user, requires login");
}

// @BotFather commands
//
// subscribe - Subscribe to Vk group
// unsubscribe - Unsubscribe from Vk group
// list_subscriptions - List subscribed Vk groups
// vk_login - Login to Vk
// vk_set_code - Enter code after vk login
// vk_logout - Logout from Vk
// fetch_groups_from_me - Subscribe to all groups from user, requires login
//
