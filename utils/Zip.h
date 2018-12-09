/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_ZIP_H
#define REPOSTBOT_ZIP_H

#include <string>
#include <vector>

namespace Utils {

    class Zip {
    public:
        static void addFiles(const std::string &archive_path, const std::vector<std::string> &file_paths);
    };

};


#endif //REPOSTBOT_ZIP_H
