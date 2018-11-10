/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_UTILS_H
#define REPOSTBOT_UTILS_H


#include <QtCore/QStringList>

class Utils {
public:
    static QStringList splitMessageTo(const QString &mess, int size = 4096);

    static int lastIndexBefore(const QString &str, char symbol, int before = -1);
};


#endif //REPOSTBOT_UTILS_H
