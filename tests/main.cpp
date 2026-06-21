#include <catch2/catch_test_macros.hpp>
#include <slim/common/utilities.h>

// ─── count_digits ────────────────────────────────────────────────────────────

TEST_CASE("count_digits", "[count_digits]") {
    using slim::common::utilities::count_digits;

    SECTION("zero") {
        REQUIRE(count_digits(0) == 1);
    }

    SECTION("single digit") {
        REQUIRE(count_digits(7) == 1);
    }

    SECTION("two digits") {
        REQUIRE(count_digits(42) == 2);
    }

    SECTION("powers of ten boundaries") {
        REQUIRE(count_digits(9) == 1);
        REQUIRE(count_digits(10) == 2);
        REQUIRE(count_digits(99) == 2);
        REQUIRE(count_digits(100) == 3);
    }

    SECTION("large value") {
        REQUIRE(count_digits(1000000000ULL) == 10);
    }

    SECTION("max-ish uint64") {
        REQUIRE(count_digits(18446744073709551615ULL) == 20);
    }
}

// ─── get_bool ────────────────────────────────────────────────────────────────

TEST_CASE("get_bool", "[get_bool]") {
    using slim::common::utilities::get_bool;

    SECTION("lowercase true") {
        bool b = false;
        REQUIRE(get_bool("true", b));
        REQUIRE(b == true);
    }

    SECTION("lowercase false") {
        bool b = true;
        REQUIRE(get_bool("false", b));
        REQUIRE(b == false);
    }

    SECTION("mixed case true") {
        bool b = false;
        REQUIRE(get_bool("TrUe", b));
        REQUIRE(b == true);
    }

    SECTION("surrounding whitespace is trimmed") {
        bool b = false;
        REQUIRE(get_bool("  true  ", b));
        REQUIRE(b == true);
    }

    SECTION("unrecognized value returns false and leaves b unchanged") {
        bool b = true;
        REQUIRE_FALSE(get_bool("not-a-bool", b));
        REQUIRE(b == true);

        bool b2 = false;
        REQUIRE_FALSE(get_bool("not-a-bool", b2));
        REQUIRE(b2 == false);
    }
}

// ─── iequals ─────────────────────────────────────────────────────────────────

TEST_CASE("iequals", "[iequals]") {
    using slim::common::utilities::iequals;

    SECTION("uppercase lhs matches lowercase rhs") {
        REQUIRE(iequals("HELLO", "hello"));
    }

    SECTION("already matching lowercase") {
        REQUIRE(iequals("hello", "hello"));
    }

    SECTION("different lengths are not equal") {
        REQUIRE_FALSE(iequals("hello", "hell"));
    }

    SECTION("rhs uppercase does not match") {
        REQUIRE_FALSE(iequals("hello", "HELLO"));
    }

    SECTION("mismatched content") {
        REQUIRE_FALSE(iequals("HELLO", "world"));
    }

    SECTION("empty strings are equal") {
        REQUIRE(iequals("", ""));
    }
}

// ─── iiequals ────────────────────────────────────────────────────────────────

TEST_CASE("iiequals", "[iiequals]") {
    using slim::common::utilities::iiequals;

    SECTION("both uppercase vs both lowercase") {
        REQUIRE(iiequals("HELLO", "hello"));
    }

    SECTION("mixed case on both sides") {
        REQUIRE(iiequals("HeLLo", "hElLO"));
    }

    SECTION("different lengths are not equal") {
        REQUIRE_FALSE(iiequals("hello", "hell"));
    }

    SECTION("mismatched content") {
        REQUIRE_FALSE(iiequals("HELLO", "WORLD"));
    }

    SECTION("empty strings are equal") {
        REQUIRE(iiequals("", ""));
    }
}

// ─── is_alnum ────────────────────────────────────────────────────────────────

TEST_CASE("is_alnum", "[is_alnum]") {
    using slim::common::utilities::is_alnum;

    SECTION("lowercase letters") {
        REQUIRE(is_alnum('a'));
        REQUIRE(is_alnum('z'));
    }

    SECTION("uppercase letters") {
        REQUIRE(is_alnum('A'));
        REQUIRE(is_alnum('Z'));
    }

    SECTION("digits") {
        REQUIRE(is_alnum('0'));
        REQUIRE(is_alnum('9'));
    }

    SECTION("hyphen is not alnum") {
        REQUIRE_FALSE(is_alnum('-'));
    }

    SECTION("punctuation is not alnum") {
        REQUIRE_FALSE(is_alnum('.'));
        REQUIRE_FALSE(is_alnum('_'));
    }

    SECTION("space is not alnum") {
        REQUIRE_FALSE(is_alnum(' '));
    }
}

// ─── is_cookie_char ──────────────────────────────────────────────────────────

TEST_CASE("is_cookie_char", "[is_cookie_char]") {
    using slim::common::utilities::is_cookie_char;

    SECTION("typical alnum chars are valid") {
        REQUIRE(is_cookie_char('a'));
        REQUIRE(is_cookie_char('Z'));
        REQUIRE(is_cookie_char('5'));
    }

    SECTION("0x21 boundary is valid") {
        REQUIRE(is_cookie_char(static_cast<char>(0x21)));
    }

    SECTION("double quote (0x22) is invalid") {
        REQUIRE_FALSE(is_cookie_char('"'));
    }

    SECTION("comma (0x2C) is invalid") {
        REQUIRE_FALSE(is_cookie_char(','));
    }

    SECTION("semicolon (0x3B) is invalid") {
        REQUIRE_FALSE(is_cookie_char(';'));
    }

    SECTION("backslash (0x5C) is invalid") {
        REQUIRE_FALSE(is_cookie_char('\\'));
    }

    SECTION("0x7E boundary is valid") {
        REQUIRE(is_cookie_char(static_cast<char>(0x7E)));
    }

    SECTION("space (0x20) is invalid") {
        REQUIRE_FALSE(is_cookie_char(' '));
    }

    SECTION("DEL (0x7F) is invalid") {
        REQUIRE_FALSE(is_cookie_char(static_cast<char>(0x7F)));
    }
}

// ─── is_date_delimiter ───────────────────────────────────────────────────────

TEST_CASE("is_date_delimiter", "[is_date_delimiter]") {
    using slim::common::utilities::is_date_delimiter;

    SECTION("tab is a delimiter") {
        REQUIRE(is_date_delimiter('\t'));
    }

    SECTION("space is a delimiter") {
        REQUIRE(is_date_delimiter(' '));
    }

    SECTION("hyphen and slash are delimiters") {
        REQUIRE(is_date_delimiter('-'));
        REQUIRE(is_date_delimiter('/'));
    }

    SECTION("comma is a delimiter") {
        REQUIRE(is_date_delimiter(','));
    }

    SECTION("colon is not a delimiter (used as time-token separator)") {
        REQUIRE_FALSE(is_date_delimiter(':'));
    }

    SECTION("letters are not delimiters") {
        REQUIRE_FALSE(is_date_delimiter('a'));
        REQUIRE_FALSE(is_date_delimiter('Z'));
    }

    SECTION("digits are not delimiters") {
        REQUIRE_FALSE(is_date_delimiter('5'));
    }
}

// ─── is_digit ────────────────────────────────────────────────────────────────

TEST_CASE("is_digit", "[is_digit]") {
    using slim::common::utilities::is_digit;

    SECTION("digit characters") {
        REQUIRE(is_digit('0'));
        REQUIRE(is_digit('5'));
        REQUIRE(is_digit('9'));
    }

    SECTION("letters are not digits") {
        REQUIRE_FALSE(is_digit('a'));
        REQUIRE_FALSE(is_digit('Z'));
    }

    SECTION("symbols are not digits") {
        REQUIRE_FALSE(is_digit('-'));
        REQUIRE_FALSE(is_digit('.'));
    }
}

// ─── is_space ────────────────────────────────────────────────────────────────

TEST_CASE("is_space", "[is_space]") {
    using slim::common::utilities::is_space;

    SECTION("recognized whitespace characters") {
        REQUIRE(is_space(' '));
        REQUIRE(is_space('\t'));
        REQUIRE(is_space('\r'));
        REQUIRE(is_space('\n'));
        REQUIRE(is_space('\v'));
        REQUIRE(is_space('\f'));
    }

    SECTION("non-whitespace characters") {
        REQUIRE_FALSE(is_space('a'));
        REQUIRE_FALSE(is_space('0'));
        REQUIRE_FALSE(is_space('-'));
    }
}

// ─── month_abbr_to_int ───────────────────────────────────────────────────────

TEST_CASE("month_abbr_to_int", "[month_abbr_to_int]") {
    using slim::common::utilities::month_abbr_to_int;

    SECTION("all valid abbreviations, lowercase") {
        REQUIRE(month_abbr_to_int("jan") == 0);
        REQUIRE(month_abbr_to_int("feb") == 1);
        REQUIRE(month_abbr_to_int("mar") == 2);
        REQUIRE(month_abbr_to_int("apr") == 3);
        REQUIRE(month_abbr_to_int("may") == 4);
        REQUIRE(month_abbr_to_int("jun") == 5);
        REQUIRE(month_abbr_to_int("jul") == 6);
        REQUIRE(month_abbr_to_int("aug") == 7);
        REQUIRE(month_abbr_to_int("sep") == 8);
        REQUIRE(month_abbr_to_int("oct") == 9);
        REQUIRE(month_abbr_to_int("nov") == 10);
        REQUIRE(month_abbr_to_int("dec") == 11);
    }

    SECTION("case insensitive") {
        REQUIRE(month_abbr_to_int("JAN") == 0);
        REQUIRE(month_abbr_to_int("Dec") == 11);
    }

    SECTION("only first three characters are considered") {
        REQUIRE(month_abbr_to_int("january") == 0);
        REQUIRE(month_abbr_to_int("december") == 11);
    }

    SECTION("too short returns -1") {
        REQUIRE(month_abbr_to_int("ja") == -1);
        REQUIRE(month_abbr_to_int("") == -1);
    }

    SECTION("unrecognized abbreviation returns -1") {
        REQUIRE(month_abbr_to_int("xyz") == -1);
    }
}

// ─── replace_all ─────────────────────────────────────────────────────────────

TEST_CASE("replace_all", "[replace_all]") {
    using slim::common::utilities::replace_all;

    SECTION("basic replacement") {
        std::string s = "hello world";
        replace_all(s, "world", "there");
        REQUIRE(s == "hello there");
    }

    SECTION("multiple occurrences") {
        std::string s = "aaa";
        replace_all(s, "a", "bb");
        REQUIRE(s == "bbbbbb");
    }

    SECTION("no match") {
        std::string s = "hello";
        replace_all(s, "xyz", "abc");
        REQUIRE(s == "hello");
    }

    SECTION("empty string early-out") {
        std::string s = "";
        replace_all(s, "a", "b");
        REQUIRE(s == "");
    }

    SECTION("empty original early-out") {
        std::string s = "hello";
        replace_all(s, "", "b");
        REQUIRE(s == "hello");
    }

    SECTION("identical original and replacement early-out") {
        std::string s = "hello";
        replace_all(s, "l", "l");
        REQUIRE(s == "hello");
    }

    SECTION("non-overlapping pattern") {
        std::string s = "aaaa";
        replace_all(s, "aa", "a");
        REQUIRE(s == "aa");
    }

    SECTION("replace entire string") {
        std::string s = "foo";
        replace_all(s, "foo", "bar");
        REQUIRE(s == "bar");
    }

    SECTION("longer replacement") {
        std::string s = "ab";
        replace_all(s, "b", "bbb");
        REQUIRE(s == "abbb");
    }
}

// ─── to_lower ────────────────────────────────────────────────────────────────

TEST_CASE("to_lower", "[to_lower]") {
    using slim::common::utilities::to_lower;

    SECTION("all uppercase") {
        std::string out;
        to_lower("HELLO", out);
        REQUIRE(out == "hello");
    }

    SECTION("already lowercase") {
        std::string out;
        to_lower("hello", out);
        REQUIRE(out == "hello");
    }

    SECTION("mixed case") {
        std::string out;
        to_lower("HeLLo WoRLd", out);
        REQUIRE(out == "hello world");
    }

    SECTION("empty string") {
        std::string out;
        to_lower("", out);
        REQUIRE(out == "");
    }

    SECTION("numbers and letters") {
        std::string out;
        to_lower("ABC123", out);
        REQUIRE(out == "abc123");
    }

    SECTION("symbols passthrough") {
        std::string out;
        to_lower("!@#ABC", out);
        REQUIRE(out == "!@#abc");
    }

    SECTION("out parameter is resized to match input, overwriting prior content") {
        std::string out = "this content should be replaced entirely";
        to_lower("HI", out);
        REQUIRE(out == "hi");
    }
}

// ─── trim ────────────────────────────────────────────────────────────────────

TEST_CASE("trim", "[trim]") {
    using slim::common::utilities::trim;

    SECTION("leading whitespace") {
        std::string_view s = "   hello";
        trim(s);
        REQUIRE(s == "hello");
    }

    SECTION("trailing whitespace") {
        std::string_view s = "hello   ";
        trim(s);
        REQUIRE(s == "hello");
    }

    SECTION("leading and trailing whitespace") {
        std::string_view s = "  hello  ";
        trim(s);
        REQUIRE(s == "hello");
    }

    SECTION("mixed whitespace characters") {
        std::string_view s = "\t\r\n hello \v\f";
        trim(s);
        REQUIRE(s == "hello");
    }

    SECTION("no whitespace") {
        std::string_view s = "hello";
        trim(s);
        REQUIRE(s == "hello");
    }

    SECTION("all whitespace becomes empty") {
        std::string_view s = "    ";
        trim(s);
        REQUIRE(s.empty());
    }

    SECTION("empty string stays empty") {
        std::string_view s = "";
        trim(s);
        REQUIRE(s.empty());
    }

    SECTION("internal whitespace is preserved") {
        std::string_view s = "  hello   world  ";
        trim(s);
        REQUIRE(s == "hello   world");
    }

    SECTION("out-parameter overload trims into a string without mutating the source view") {
        std::string_view s = "  hello  ";
        std::string out;
        trim(s, out);
        REQUIRE(out == "hello");
        REQUIRE(s == "  hello  ");
    }

    SECTION("out-parameter overload on all-whitespace input yields an empty string") {
        std::string out;
        trim(std::string_view("    "), out);
        REQUIRE(out.empty());
    }

    SECTION("out-parameter overload overwrites prior content") {
        std::string out = "this content should be replaced entirely";
        trim(std::string_view("  hi  "), out);
        REQUIRE(out == "hi");
    }
}

// ─── split ───────────────────────────────────────────────────────────────────

TEST_CASE("split", "[split]") {
    using slim::common::utilities::split;

    SECTION("basic split on delimiter") {
        auto tokens = split("a;b;c", ';');
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "a");
        REQUIRE(tokens[1] == "b");
        REQUIRE(tokens[2] == "c");
    }

    SECTION("no delimiter present returns single token") {
        auto tokens = split("hello", ';');
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0] == "hello");
    }

    SECTION("empty string returns no tokens") {
        auto tokens = split("", ';');
        REQUIRE(tokens.empty());
    }

    SECTION("consecutive delimiters produce no empty tokens") {
        auto tokens = split("a;;b", ';');
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0] == "a");
        REQUIRE(tokens[1] == "b");
    }

    SECTION("leading delimiter is skipped") {
        auto tokens = split(";a;b", ';');
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0] == "a");
        REQUIRE(tokens[1] == "b");
    }

    SECTION("trailing delimiter produces no trailing empty token") {
        auto tokens = split("a;b;", ';');
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0] == "a");
        REQUIRE(tokens[1] == "b");
    }

    SECTION("string of only delimiters returns no tokens") {
        auto tokens = split(";;;", ';');
        REQUIRE(tokens.empty());
    }

    SECTION("different delimiter character") {
        auto tokens = split("name=value", '=');
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0] == "name");
        REQUIRE(tokens[1] == "value");
    }
}
