/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_STORAGE_H
#define REPOSTBOT_STORAGE_H

#include <utils/USingleton.h>
#include "json.hpp"

using json = nlohmann::json;


class Storage : public USingleton<Storage> {
    json object;

public:
    Storage() = default;

    void load();

    void save();

    json &operator[](const QString &key) { return object[key.toStdString().c_str()]; }

    const json &operator[](const QString &key) const { return object[key.toStdString().c_str()]; }

    json &operator[](char *key) { return object[key]; }

    const json &operator[](char *key) const { return object[key]; }

    json &value(const char *key) { return object[key]; }

    const json &value(const char *key) const { return object[key]; }
};


#endif //REPOSTBOT_STORAGE_H
