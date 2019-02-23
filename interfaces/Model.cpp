/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <interfaces/Model.h>
#include <queue/QueueTask.h>
#include <Runner.h>
#include <tgbot/Api.h>
#include <models/ReplyModel.h>

void Model::sendTo(const User &user) {
    if (user.isTelegram()) {
        auto *queue = Runner::instance()->queue();
        auto *task = new QueueTask(user, "sendModel");

        task->setResult(new ReplyModel(this), false);
        queue->addTask("Telegram", task);
    } else {
        puts("Unsupported User\n");
    }
}
