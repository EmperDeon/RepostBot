/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <vendor/catch2.hpp>
#include <utils/Utils.h>


TEST_CASE("splitMessage should work correctly", "[Utils]") {
    QString str1 = "str str str", str2 = "strstrstr";

    REQUIRE(Utils::lastIndexBefore(str1, ' ', -1) == 7);
    REQUIRE(Utils::lastIndexBefore(str1, ' ', 0) == -1);
    REQUIRE(Utils::lastIndexBefore(str1, ' ', 1) == -1);
    REQUIRE(Utils::lastIndexBefore(str1, ' ', 2) == -1);
    REQUIRE(Utils::lastIndexBefore(str1, ' ', 3) == 3);
    REQUIRE(Utils::lastIndexBefore(str1, ' ', 4) == 3);
    REQUIRE(Utils::lastIndexBefore(str1, ' ', 6) == 3);
    REQUIRE(Utils::lastIndexBefore(str1, ' ', 10) == 7);

    REQUIRE(Utils::lastIndexBefore(str2, ' ', -1) == -1);
    REQUIRE(Utils::lastIndexBefore(str2, ' ', 3) == -1);
    REQUIRE(Utils::lastIndexBefore(str2, ' ', 10) == -1);


    REQUIRE(Utils::splitMessageTo(str1, 1).count() == 9);
    REQUIRE(Utils::splitMessageTo(str1, 3).count() == 3);
    REQUIRE(Utils::splitMessageTo(str1, 20).count() == 1);

    REQUIRE(Utils::splitMessageTo(str2, 1).count() == 9);
    REQUIRE(Utils::splitMessageTo(str2, 3).count() == 3);
    REQUIRE(Utils::splitMessageTo(str2, 20).count() == 1);
}
