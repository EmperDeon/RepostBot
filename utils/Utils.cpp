/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include "Utils.h"
#include <QDebug>

/*
 * Splits `message` by `size`, aligning by space if any
 */
QStringList Utils::splitMessageTo(const QString &mess, int size) {
    if (mess.size() < size) { return {mess}; }

    QStringList r;
    QString temp = mess;

    while (temp.size() > size) {
        int index = lastIndexBefore(temp, ' ', size) + 1;
        index = index == 0 ? size : index;

        r << temp.mid(0, index).trimmed();
        temp.remove(0, index);
    }

    if (!temp.isEmpty()) {
        r << temp;
    }

    return r;
}

/*
 * Returns index of first `symbol` starting from `before` ang going backwards
 *
 * If none, returns -1
 * */
int Utils::lastIndexBefore(const QString &str, char symbol, int before) {
    if (str.size() < before || before == -1) {
        before = str.size() - 1;
    }

    for (int i = before; i >= 0; i--) {
        if (str[i] == symbol) {
            return i;
        }
    }

    return -1;
}
