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
    std::string vk_token(VK_TOKEN);
    SKIP_IF(vk_token.empty(), "VK token isn't present");

    VkApi api(vk_token, true);

    json response = api.request("account.getInfo");

    REQUIRE(response.has_key("response"));
    REQUIRE_FALSE(response.has_key("error"));

    response = response["response"];
    REQUIRE_FALSE(response.empty());
}
