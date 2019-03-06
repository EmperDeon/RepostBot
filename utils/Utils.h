/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_UTILS_H
#define REPOSTBOT_UTILS_H

#include <QtCore/QStringList>


namespace Utils {
    QStringList splitMessageTo(const QString &mess, int size = 4096);

    int lastIndexBefore(const QString &str, char symbol, int before = -1);

    bool fileExists(const std::string &name);

    template<class T = std::string>
    std::string join(const std::vector<T> &vec, char sep);

    std::vector<std::string> split(const std::string &str, char sep);
};


#endif //REPOSTBOT_UTILS_H
