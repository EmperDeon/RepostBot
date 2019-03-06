/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include "Utils.h"
#include <fstream>
#include <sstream>

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

bool Utils::fileExists(const std::string &name) {
    std::ifstream f(name);
    return f.good();
}

template<class T>
std::string Utils::join(const std::vector<T> &vec, char sep) {
    if (vec.empty())
        return "";

    std::stringstream ss;

    bool empty = true;
    for (auto item : vec) {
        if (empty)
            ss << item;
        else
            ss << sep << item;

        empty = false;
    }

    return ss.str();
}

std::vector<std::string> Utils::split(const std::string &str, char sep) {
    std::vector<std::string> out;

    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, sep)) {
        out.push_back(item);
    }

    return out;
}
