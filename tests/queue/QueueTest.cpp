/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include <vendor/catch2.hpp>
#include <tests/helpers/QueueHelper.h>
#include <queue/QueueManager.h>
#include <QtCore/QEventLoop>


TEST_CASE("should handle 1 thread correctly", "[Queue]") {
    int task_count = GENERATE(1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024);
    auto *manager = new QueueManager;
    auto *counter = new TestQueueCounter(task_count);
    auto *hn1 = new TestQueueHandler(2);

    manager->addHandler(hn1);

    BENCHMARK("finish tasks: " + std::to_string(task_count)) {
        for (int i = 0; i < task_count; i++) {
            auto *task = new QueueTask(User(), "test1", {"test string"});
            task->connect(task, &QueueTask::hasFinished, counter, &TestQueueCounter::futureCatcher);

            manager->addTask(hn1->name(), task);
        }

        // Wait for finish
        QEventLoop loop;
        QObject::connect(counter, &TestQueueCounter::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }

    INFO(counter->counts());
    REQUIRE(counter->successful());
}


TEST_CASE("should handle multiple threads", "[Queue]") {
    int task_count = GENERATE(1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024);
    auto *manager = new QueueManager;
    auto *counter = new TestQueueCounter(task_count);
    auto *hn1 = new TestQueueHandler(2),
            *hn2 = new TestQueueHandler(2),
            *hn3 = new TestQueueHandler(2),
            *hn4 = new TestQueueHandler(2);

    manager->addHandlers({hn1, hn2, hn3, hn4});

    BENCHMARK("finish tasks: " + std::to_string(task_count)) {
        for (int i = 0; i < task_count; i++) {
            auto *task = new QueueTask(User(), "test1", {"test string"});
            task->connect(task, &QueueTask::hasFinished, counter, &TestQueueCounter::futureCatcher);

            manager->addTask(hn1->name(), task);
        }

        // Wait for finish
        QEventLoop loop;
        QObject::connect(counter, &TestQueueCounter::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }

    INFO(counter->counts());
    REQUIRE(counter->successful());
}
