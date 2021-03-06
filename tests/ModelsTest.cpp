/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <vendor/catch2.hpp>
#include <models/Attachment.h>
#include <vars.h>

using Catch::Matchers::StartsWith;


TEST_CASE("should download file", "[Models]") {
    std::string filename = Attachment::downloadFile("https://ya.ru/");

    INFO(filename);
    REQUIRE_FALSE(filename.empty());
    REQUIRE_THAT(filename, StartsWith(CACHE_PATH));

    QFile file(QString::fromStdString(filename));

    REQUIRE(file.open(QFile::ReadOnly));
    REQUIRE(file.size() > 0);
    REQUIRE(file.remove());
}
