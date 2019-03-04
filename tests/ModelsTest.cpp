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
    QString filename = Attachment::downloadFile("https://ya.ru/index.html");

    INFO(filename.toStdString());
    REQUIRE_FALSE(filename.isEmpty());
    REQUIRE_THAT(filename.toStdString(), StartsWith((CACHE_PATH).toStdString()));

    QFile file(filename);

    REQUIRE(file.open(QFile::ReadOnly));
    REQUIRE(file.size() > 0);
    REQUIRE(file.remove());
}
