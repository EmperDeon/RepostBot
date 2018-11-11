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
  Storage.cpp \
  Runner.cpp


//HEADERS += vars.h \
// crypt/CAes.h \
// crypt/CRsa.h \
// crypt/Crypt.h \
// crypt/CBytes.h \
// utils/Utils.h \
//  \
// tabs/editors/TEditor.h \
//  \
// tabs/encrypted/TEncryptedTab.h \
//  \
// tabs/files/TFileGroup.h \
//  \
// tabs/journals/TJournalTab.h \
//  \
// tabs/links/TLink.h \
// tabs/links/TLinksGroup.h \
//  \
// tabs/TabNew.h \
// tabs/Tab.h \
// tabs/TabsController.h \
// tabs/TList.h \
//  \
// network/NCrypt.h \
// network/Network.h \
// network/NSync.h \
//  \
// plugins/Plugin.h \
// plugins/tabs/PTabPlugin.h \
// plugins/tabs/PTabFile.h \
// plugins/tabs/PTabFiles.h \
//  \
// storage/SBackup.h \
// storage/SGroups.h \
// storage/SMap.h \
// storage/SMigrations.h \
// storage/Storage.h \
// storage/SSecure.h \
// storage/SSettings.h \
//  \
// storage/migrations/SMigration.h \
// storage/migrations/SMVer0.h \
// storage/migrations/SMVer01.h \
// storage/migrations/SMVer02.h \
//  \
// utils/ULoginDialog.h \
// utils/UOrderedMap.h \
// utils/UPassDialog.h \
// utils/UScrollArea.h \
// utils/USingleton.h \
// utils/UTray.h \
//  \
// utils/logs/ULogger.h \
//  \
// utils/widgets/dates/UDateItem.h \
// utils/widgets/UCenteredWidget.h \
// utils/widgets/UDateDialog.h \
// utils/widgets/UDatesWidget.h \
// utils/widgets/UTreeWidget.h \
//  \
// widgets/files/WFile.h \
// widgets/files/WFileIcon.h \
// widgets/files/WFiles.h \
//  \
// widgets/groups/WTGroupLabel.h \
// widgets/groups/WTGroups.h \
//  \
// widgets/settings/WSCheckBox.h \
// widgets/settings/WSComboBox.h \
// widgets/settings/WSettings.h \
// widgets/settings/WSetting.h \
// widgets/settings/WSLineEdit.h \
//  \
// widgets/sort/WTSorter.h \
//  \
// widgets/WMain.h \
// widgets/WSync.h \
// widgets/WTabs.h \
//  \
// vendor/additions.h


LIBS += -L$$PWD/vendor/libs-linux-64/

LIBS += -lTgBot -lboost_system -lcrypto -lssl -lcurl

INCLUDEPATH += $$PWD/vendor/
INCLUDEPATH += $$PWD/vendor/tgbot-cpp/include

DEPENDPATH += $$PWD/vendor/libs-linux-64/
