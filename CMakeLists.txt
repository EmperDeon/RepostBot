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

# Minizip
set(USE_BZIP2 OFF)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

message(STATUS "Qt version: " ${Qt5_VERSION})

## curl
find_package(CURL)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif ()

if (DEBUG_BUILD)
    add_definitions(-DDEBUG)
endif ()

include(cotire/CMake/cotire)
add_subdirectory(vendor/tgbot-cpp)
add_subdirectory(vendor/minizip)
include_directories(vendor/tgbot-cpp/include)
include_directories(vendor/)

set(TEST_SOURCES
        tests/queue/QueueHelper.cpp tests/TestHelper.cpp
        tests/queue/TestQueue1.cpp tests/queue/TestQueue2.cpp
        tests/TestUtils.cpp
        tests/TestAttachment.cpp
        )

add_executable(RepostBot
        apis/TelegramApi.cpp apis/bots/TelegramBot.cpp
        apis/VkApi.cpp apis/handlers/VkHandler.cpp apis/handlers/vk/VkAuth.cpp

        interfaces/Model.cpp

        models/Attachment.cpp models/Post.cpp models/Error.cpp models/Status.cpp

        Storage.cpp Runner.cpp
        utils/Zip.cpp utils/logs/Logger.cpp

        queue/QueueManager.cpp queue/QueueTask.cpp queue/QueueHandler.cpp

        tasks/TaskManager.cpp
        tasks/vk/PostsVkTask.cpp

        utils/Utils.cpp

        ${TEST_SOURCES}

        main.cpp)

set_target_properties(RepostBot PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER OFF)
target_link_libraries(RepostBot Qt5::Core Qt5::Network TgBot libminizip ssl crypto)

cotire(RepostBot)
