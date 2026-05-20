#include <catch2/catch_test_macros.hpp>
#include <slim/common/utilities.h>

// ─── replace_all ─────────────────────────────────────────────────────────────

TEST_CASE("replace_all - basic replacement", "[replace_all]") {
    std::string s = "hello world";
    slim::common::utilities::replace_all(s, "world", "there");
    REQUIRE(s == "hello there");
}

TEST_CASE("replace_all - multiple occurrences", "[replace_all]") {
    std::string s = "aaa";
    slim::common::utilities::replace_all(s, "a", "bb");
    REQUIRE(s == "bbbbbb");
}

TEST_CASE("replace_all - no match", "[replace_all]") {
    std::string s = "hello";
    slim::common::utilities::replace_all(s, "xyz", "abc");
    REQUIRE(s == "hello");
}

TEST_CASE("replace_all - empty string early-out", "[replace_all]") {
    std::string s = "";
    slim::common::utilities::replace_all(s, "a", "b");
    REQUIRE(s == "");
}

TEST_CASE("replace_all - empty original early-out", "[replace_all]") {
    std::string s = "hello";
    slim::common::utilities::replace_all(s, "", "b");
    REQUIRE(s == "hello");
}

TEST_CASE("replace_all - non-overlapping pattern", "[replace_all]") {
    std::string s = "aaaa";
    slim::common::utilities::replace_all(s, "aa", "a");
    REQUIRE(s == "aa");
}

TEST_CASE("replace_all - replace entire string", "[replace_all]") {
    std::string s = "foo";
    slim::common::utilities::replace_all(s, "foo", "bar");
    REQUIRE(s == "bar");
}

TEST_CASE("replace_all - longer replacement", "[replace_all]") {
    std::string s = "ab";
    slim::common::utilities::replace_all(s, "b", "bbb");
    REQUIRE(s == "abbb");
}

// ─── to_lower ────────────────────────────────────────────────────────────────

TEST_CASE("to_lower - all uppercase", "[to_lower]") {
    REQUIRE(slim::common::utilities::to_lower("HELLO") == "hello");
}

TEST_CASE("to_lower - already lowercase", "[to_lower]") {
    REQUIRE(slim::common::utilities::to_lower("hello") == "hello");
}

TEST_CASE("to_lower - mixed case", "[to_lower]") {
    REQUIRE(slim::common::utilities::to_lower("HeLLo WoRLd") == "hello world");
}

TEST_CASE("to_lower - empty string", "[to_lower]") {
    REQUIRE(slim::common::utilities::to_lower("") == "");
}

TEST_CASE("to_lower - numbers and letters", "[to_lower]") {
    REQUIRE(slim::common::utilities::to_lower("ABC123") == "abc123");
}

TEST_CASE("to_lower - symbols passthrough", "[to_lower]") {
    REQUIRE(slim::common::utilities::to_lower("!@#ABC") == "!@#abc");
}