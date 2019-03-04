/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_TESTHELPER_H
#define REPOSTBOT_TESTHELPER_H

#include <vendor/catch2.hpp>

#define SKIP_IF(condition, reason) if (condition) { WARN("Test skipped: " + std::string(reason)); return; }


#endif //REPOSTBOT_TESTHELPER_H
