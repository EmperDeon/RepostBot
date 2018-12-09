TEMPLATE = app
TARGET   = RepostBot

QT += core network

CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14


DEFINES += "HAVE_CURL"

SOURCES += main.cpp \
  interfaces/Model.cpp \
  apis/TelegramApi.cpp \
  apis/bots/TelegramBot.cpp \
  apis/VkApi.cpp \
  apis/handlers/VkHandler.cpp \
  models/Attachment.cpp \
  models/Post.cpp \
  models/Error.cpp \
  models/Status.cpp \
  queue/QueueManager.cpp \
  queue/QueueTask.cpp \
  queue/QueueHandler.cpp\
  tasks/TaskManager.cpp \
  tasks/vk/PostsVkTask.cpp \
  utils/Utils.cpp \
  utils/logs/Logger.cpp \
  utils/Zip.cpp \
  Storage.cpp \
  Runner.cpp


LIBS += -L$$PWD/vendor/libs-linux-64/

LIBS += -lTgBot -lcurl -lz -lssl -lcrypto -ldl -lboost_system -ljpeg -lcares_static -lminizip

INCLUDEPATH += $$PWD/vendor/
INCLUDEPATH += $$PWD/vendor/tgbot-cpp/include

DEPENDPATH += $$PWD/vendor/libs-linux-64/
