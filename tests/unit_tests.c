#include <catch2/catch2_test_macros.hpp>
#include "lexer.h"

TEST_CASE("Lexer") {
    TokenArr arr;
    if (!initialize_token_arr(&arr)) REQUIRE(false);
    REQUIRE(true);
}

