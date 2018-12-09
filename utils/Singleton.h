/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef UTILS_SINGLETON_H
#define UTILS_SINGLETON_H

namespace Utils {
    template<class T>
    class Singleton {
    public:
        static T *instance() {
            static T *instance = nullptr;

            if (instance == nullptr) {
                instance = new T;
            }

            return instance;
        }
    };
};

#endif //UTILS_SINGLETON_H
