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

    events->onCommand("start", [this](TgBot::Message::Ptr message) {
        this->api->sendMessage(message->chat->id, "Hi!");
    });

    events->onCommand("get_last_post", [this](TgBot::Message::Ptr message) {
        QStringList parts = QString::fromStdString(message->text).split(' ');

        if (parts.size() == 1) {
            parts << "null";
        }

        addVkTask(message, "getLastPost", {parts[1]});
    });

    events->onCommand("subscribe", [this](TgBot::Message::Ptr message) {
        QStringList parts = QString::fromStdString(message->text).split(' ');

        if (parts.size() == 1) {
            parts << "null";
        }

        addVkTask(message, "toggleSubscription", {parts[1], "true"});
    });

    events->onCommand("unsubscribe", [this](TgBot::Message::Ptr message) {
        QStringList parts = QString::fromStdString(message->text).split(' ');

        if (parts.size() == 1) {
            parts << "null";
        }

        addVkTask(message, "toggleSubscription", {parts[1], "false"});
    });

    events->onCommand("list_subscriptions", [this](TgBot::Message::Ptr message) {
        addVkTask(message, "listSubscriptions");
    });

    events->onCommand("export", [this](TgBot::Message::Ptr message) {
        // Send list of commands to execute/json to import.
        // Json will be better i think
    });

    events->onAnyMessage([this](TgBot::Message::Ptr message) {
        qDebug() << "Received message: " << message->text.c_str();
    });

#ifdef DEBUG
    events->onCommand("forceUpdateVkTask", [this](TgBot::Message::Ptr message) {
        Runner::instance()->tasks()->forceLaunch(PostsVkTask().id());
    });
#endif
}

void ETelegram::addVkTask(TgBot::Message::Ptr message, const char *action, QStringList params) {
    auto *task = new QueueTask(User(message->chat->id), action, params);

    tasks << task;
    connect(task, &QueueTask::hasFinished, this, &ETelegram::handleFinished);

    queue->addTask(IVk::className(), task);
}

void ETelegram::handleFinished(QueueTask *task) {
    const QString &name = task->action;
    const User &user = task->user;
    Model *result = task->result();

    if (name == "getLastPost" || name == "toggleSubscription" || name == "listSubscriptions") {
        result->sendTo(user);

    } else {
        qDebug() << "No handler in Telegram for action: " << name;
    }

    delete result;
    delete task;
}

void ETelegram::sendMessage(int64_t to, const QString &message, bool silent) {
    QStringList messages = Utils::splitMessageTo(message, 4096);

    for (const QString &split_message : messages) {
        try {
            api->sendMessage(to, split_message.toStdString(), false, 0, std::make_shared<TgBot::GenericReply>(),
                             "Markdown",
                             silent);
        } catch (TgBot::TgException &e) {
            printf("error: %s\n", e.what());
        }
    }
}

void ETelegram::sendMedia(int64_t user, std::vector<TgBot::InputMedia::Ptr> attachments) {
    api->sendMediaGroup(user, attachments);
}

// @BotFather commands
//
// get_last_post - Get last not-pinned post from Vk group
// subscribe - Subscribe to Vk group
// unsubscribe - Unsubscribe from Vk group
// list_subscriptions - List subscribed Vk groups
//
