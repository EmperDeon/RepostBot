/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <tests/TestHelper.h>
#include <QtCore/QString>
#include <vars.h>
#include <apis/VkApi.h>


TEST_CASE("should make correct basic request", "[vk]") {
    QString vk_token(VK_TOKEN);
    SKIP_IF(vk_token.isEmpty(), "VK token isn't present");

    VkApi api;

    json response = api.request("account.getInfo");
    INFO(response.dump(1));

    REQUIRE(response.has_key("response"));
    REQUIRE_FALSE(response.has_key("error"));

    response = response["response"];

    REQUIRE_FALSE(response.empty());
}
