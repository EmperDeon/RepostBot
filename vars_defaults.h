/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_VARS_DEFAULTS_H
#define REPOSTBOT_VARS_DEFAULTS_H

#define STORAGE_FILE (BASE_PATH + "config.json")
#define CACHE_PATH (BASE_PATH + "cache/")
#define LOGS_DIR (BASE_PATH + "logs/")
#define SSL_CA_PATH (BASE_PATH + "cacert.pem")


// Curl
#define SSL_CA_URL "https://curl.haxx.se/ca/cacert.pem"


// Modals
#define DATETIME_FORMAT "<i>dd.MM.yyyy HH:mm</i>"


// Logger
#define LOG_DATETIME_FORMAT "yyyy-MM-dd HH:mm:ss"
#define LOGS_ZIP "logs.zip"
#define LOG_INCLUDE_METHOD


// Filesystem
#if defined(WIN32) || defined(_WIN32)
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif


// Apis
#define VK_API_DELAY std::chrono::duration<long double>{3}
#define TG_API_DELAY std::chrono::duration<long double>{3}
#define TG_API_DELAY_LONG std::chrono::duration<long double>{30}


#endif //REPOSTBOT_VARS_DEFAULTS_H
