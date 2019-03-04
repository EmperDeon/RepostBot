# Copyright (c) 2018 by Ilya Barykin
# Released under the MIT License.
# See the provided LICENSE.TXT file for details.

cmake_minimum_required(VERSION 3.10)
project(RepostBot)

#
# Options
#

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/vendor")

# Minizip
set(USE_BZIP2 OFF)

#
# Includes
#

find_package(Qt5 COMPONENTS Core Network REQUIRED)
message(STATUS "Qt version: " ${Qt5_VERSION})

## curl
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
add_definitions(-DHAVE_CURL)

include(cotire/CMake/cotire)
add_subdirectory(vendor/tgbot-cpp)
add_subdirectory(vendor/minizip)
include_directories(vendor/tgbot-cpp/include)
include_directories(vendor)

if (DEBUG_BUILD)
    add_definitions(-DDEBUG)
endif ()

#
# Sources
#

set(SOURCES
        apis/TelegramApi.cpp apis/bots/TelegramBot.cpp apis/handlers/TelegramHandler.cpp
        apis/VkApi.cpp apis/handlers/VkHandler.cpp apis/handlers/vk/VkAuth.cpp

        interfaces/Model.cpp

        models/Attachment.cpp models/Post.cpp models/Error.cpp models/Status.cpp

        queue/QueueManager.cpp queue/QueueTask.cpp queue/QueueHandler.cpp

        tasks/TaskManager.cpp
        tasks/vk/PostsVkTask.cpp

        utils/Utils.cpp
        utils/Zip.cpp utils/logs/Logger.cpp

        Storage.cpp Runner.cpp
        )

set(TEST_SOURCES
        tests/main.cpp

        # Helpers
        tests/helpers/QueueHelper.cpp

        # Tests
        tests/queue/QueueTest.cpp
        tests/apis/VkApiTest.cpp

        tests/ModelsTest.cpp
        tests/UtilsTest.cpp
        )

#
# Targets
#

add_executable(RepostBot main.cpp ${SOURCES})
add_executable(RepostBot_test ${SOURCES} ${TEST_SOURCES})

set_target_properties(RepostBot PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER OFF)
target_link_libraries(RepostBot Qt5::Core Qt5::Network TgBot libminizip ssl crypto)

target_compile_definitions(RepostBot_test PRIVATE CATCH_CONFIG_RUNNER=1)
set_target_properties(RepostBot_test PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER OFF)
target_link_libraries(RepostBot_test Qt5::Core Qt5::Network TgBot libminizip ssl crypto)

cotire(RepostBot)
cotire(RepostBot_test)

#
# Testing
#

enable_testing()
add_test(NAME RunTests COMMAND ReportBot_test_unity)
