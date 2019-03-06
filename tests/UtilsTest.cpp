/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <vendor/catch2.hpp>
#include <utils/Utils.h>
#include <utils/Curl.h>
#include <vars.h>

/*
 * Namespace
 */

TEST_CASE("splitMessage should work correctly", "[Utils][no_netw]") {
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

TEST_CASE("join should work correctly", "[Utils][no_netw]") {
    std::string joined = Utils::join({}, ',');

    REQUIRE(joined.empty());

    joined = Utils::join({"string"}, ',');

    REQUIRE(joined.find(',') == -1);

    joined = Utils::join({"1", "2", "3", "4", "5"}, ',');

    REQUIRE(joined.find(',') == 1);
}

TEST_CASE("split should work correctly", "[Utils][no_netw]") {
    std::vector<std::string> split = Utils::split("", ',');

    REQUIRE(split.empty());

    split = Utils::split("string", ',');

    REQUIRE(split.size() == 1);

    split = Utils::split("1,2,3,4,5", ',');

    REQUIRE(split.size() == 5);
}

/*
 * Curl
 */

TEST_CASE("Curl should fetch webpage", "[Utils]") {
    std::string reply = Curl("http://ya.ru/").get();

    REQUIRE_FALSE(reply.empty());
    REQUIRE(Utils::fileExists(SSL_CA_PATH));
}
