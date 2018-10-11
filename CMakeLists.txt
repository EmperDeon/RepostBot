# Copyright (c) 2018 by Ilya Barykin
# Released under the MIT License.
# See the provided LICENSE.TXT file for details.

cmake_minimum_required(VERSION 3.10)
project(RepostBot)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/vendor")


find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

message(STATUS "Qt version: " ${Qt5_VERSION})

if (DEBUG_BUILD)
    add_definitions(-DDEBUG)
endif ()

include(cotire/CMake/cotire)
add_subdirectory(vendor/tgbot-cpp)
include_directories(vendor/tgbot-cpp/include)
include_directories(vendor/)

set(TEST_SOURCES
        tests/TestCase.h tests/queue/QueueHelper.cpp tests/queue/QueueHelper.h tests/TestHelper.cpp tests/TestHelper.h
        tests/queue/TestQueue1.cpp tests/queue/TestQueue1.h tests/queue/TestQueue2.cpp tests/queue/TestQueue2.h
        tests/TestUtils.cpp tests/TestUtils.h tests/TestAttachment.cpp tests/TestAttachment.h
        )

add_executable(RepostBot
        interfaces/Threadable.h interfaces/Model.h interfaces/Model.cpp interfaces/Exporter.h interfaces/Importer.h interfaces/User.h interfaces/Task.h
        exports/ETelegram.cpp exports/ETelegram.h
        imports/IVk.cpp imports/IVk.h vars.h
        models/Attachment.cpp models/Post.h models/Post.cpp models/Error.cpp models/Error.h models/Status.cpp models/Status.h

        Storage.cpp Storage.h json.hpp Runner.cpp Runner.h
        utils/USingleton.h utils/concurrent/UFuture.h

        queue/QueueManager.cpp queue/QueueManager.h queue/QueueTask.cpp queue/QueueTask.h queue/QueueHandler.h queue/QueueHandler.cpp

        tasks/TaskManager.cpp tasks/TaskManager.h
        tasks/vk/PostsVkTask.cpp tasks/vk/PostsVkTask.h

        utils/Utils.cpp utils/Utils.h

        ${TEST_SOURCES}

        main.cpp models/Attachment.h)

set_target_properties(RepostBot PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER OFF)
target_link_libraries(RepostBot Qt5::Core Qt5::Network TgBot ssl crypto)

cotire(RepostBot)
