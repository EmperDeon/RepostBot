/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef UTILS_USINGLETON_H
#define UTILS_USINGLETON_H

template<class T>
class USingleton {
public:
    static T *instance() {
        static T *instance = nullptr;

        if (instance == nullptr) {
            instance = new T;
        }

        return instance;
    }
};

#endif //UTILS_USINGLETON_H
