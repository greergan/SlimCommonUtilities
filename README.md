<a href="https://codeberg.org/greergan/SlimTS">
  <img src="https://raw.githubusercontent.com/greergan/SlimTS/master/assets/slimts_logo.png" width="75" alt="SlimTS Logo">
</a>   

# SlimCommonUtilities

A lightweight collection of small, dependency-free C++ string and numeric helper functions.  
Part of the [SlimCommon](https://codeberg.org/greergan/SlimCommon) library.  
Used internally by different [SlimCommon](https://codeberg.org/greergan/SlimCommon) micro-libraries.  
Built using [SlimLibraryPackager](https://codeberg.org/greergan/SlimLibraryPackager).  
CI/CD supplied by unified workflows provided by [SlimLibraryPackager](https://codeberg.org/greergan/SlimLibraryPackager).

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Core API](#core-api)
  - [count_digits](#count_digits)
  - [get_bool](#get_bool)
  - [iequals](#iequals)
  - [iiequals](#iiequals)
  - [is_alnum](#is_alnum)
  - [is_alpha](#is_alpha)
  - [is_cookie_char](#is_cookie_char)
  - [is_date_delimiter](#is_date_delimiter)
  - [is_digit](#is_digit)
  - [is_space](#is_space)
  - [month_abbr_to_int](#month_abbr_to_int)
  - [replace_all](#replace_all)
  - [split](#split)
  - [to_lower](#to_lower)
  - [trim](#trim)
- [Building](#building)
- [Dependencies](#dependencies)
  - [required_packages](#required_packages)
- [Examples](#examples)

## Overview

This library provides a small set of free functions used across the Slim* libraries for common string and numeric handling tasks, with:
- No dynamic allocation in hot paths (where possible)
- `noexcept` guarantees on every function
- Plain, predictable behavior with no hidden state
- No external dependencies
- `std::string_view`-first signatures to avoid unnecessary copies

[↑ Top](#table-of-contents)

## Features

| Feature | Description |
|--------|-------------|
| Digit counting | Fast digit count for unsigned 64-bit integers |
| Boolean parsing | String-to-bool conversion for `TRUE`/`true`/`FALSE`/`false` |
| Case-insensitive comparison | Two variants (`iequals` / `iiequals`) for comparing strings |
| Character classification | Single-character predicates for alphabetics, alphanumerics, cookie-octets, date delimiters, digits, and whitespace |
| Month abbreviation lookup | Converts three-letter month abbreviations to their numeric index |
| In-place substring replacement | Replaces all occurrences of a substring within a string |
| Delimiter splitting | Splits a view into non-empty string tokens on a delimiter character, written into a caller-supplied buffer |
| Lowercasing | Produces a lowercased copy of a string |
| Trimming | Removes leading/trailing whitespace from a view in place |

[↑ Top](#table-of-contents)

## Core API

All functions live in the `slim::common::utilities` namespace and are declared in `slim/common/utilities.h`.

### count_digits

```cpp
std::size_t count_digits(std::uint_least64_t n) noexcept;
```

Returns the number of base-10 digits in `n`. `0` is treated as a single digit.

[↑ Top](#table-of-contents)

### get_bool

```cpp
bool get_bool(std::string_view s, bool& b) noexcept;
```

Parses `s` as a boolean and writes the result into `b`. Only accepts `"TRUE"`, `"true"`, `"FALSE"`, or `"false"` (case-insensitive, with surrounding whitespace trimmed). Returns `true` on success; returns `false` and leaves `b` unchanged if `s` is none of these.

[↑ Top](#table-of-contents)

### iequals

```cpp
bool iequals(std::string_view a, std::string_view b) noexcept;
```

Case-insensitive equality comparison. `b` must already be lowercase.

[↑ Top](#table-of-contents)

### iiequals

```cpp
bool iiequals(std::string_view a, std::string_view b) noexcept;
```

Case-insensitive equality comparison. No assumption on either argument's case.

[↑ Top](#table-of-contents)

### is_alnum

```cpp
bool is_alnum(char c) noexcept;
```

Returns `true` if `c` is an ASCII letter (`a`–`z`, `A`–`Z`) or digit (`0`–`9`).

[↑ Top](#table-of-contents)

### is_alpha

```cpp
bool is_alpha(char c) noexcept;
```

Returns `true` if `c` is an ASCII letter (`a`–`z`, `A`–`Z`).

[↑ Top](#table-of-contents)

### is_cookie_char

```cpp
bool is_cookie_char(char c) noexcept;
```

Returns `true` if `c` is a valid unquoted cookie-octet per RFC 6265 — printable ASCII excluding control characters, space, double quote, comma, semicolon, and backslash.

[↑ Top](#table-of-contents)

### is_date_delimiter

```cpp
bool is_date_delimiter(char c) noexcept;
```

Returns `true` if `c` is a delimiter character used when tokenizing HTTP-date strings (e.g. spaces, hyphens, commas). Colon is deliberately excluded, since it separates components within a single `HH:MM:SS` time token rather than separating tokens.

[↑ Top](#table-of-contents)

### is_digit

```cpp
bool is_digit(char c) noexcept;
```

Returns `true` if `c` is an ASCII digit (`0`–`9`).

[↑ Top](#table-of-contents)

### is_space

```cpp
bool is_space(char c) noexcept;
```

Returns `true` if `c` is one of the recognized ASCII whitespace characters: space, `\t`, `\r`, `\n`, `\v`, or `\f`.

[↑ Top](#table-of-contents)

### month_abbr_to_int

```cpp
int month_abbr_to_int(std::string_view s) noexcept;
```

Converts a three-letter month abbreviation (e.g. `"Jan"`, `"Dec"`) to its 0-based month index (`"Jan"` → `0`, `"Dec"` → `11`). Only the first three characters of `s` are considered. Returns `-1` when `s` is shorter than three characters or not a recognized abbreviation.

[↑ Top](#table-of-contents)

### replace_all

```cpp
void replace_all(std::string& _string, std::string_view _original, std::string_view _replacement) noexcept;
```

Replaces every non-overlapping occurrence of `_original` in `_string` with `_replacement`, modifying `_string` in place.

[↑ Top](#table-of-contents)

### split

```cpp
void split(std::string_view s, char delim, std::vector<std::string>& buf) noexcept;
```

Splits `s` on `delim` into non-empty tokens, appended to `buf` as owned `std::string`s. Consecutive delimiters and leading/trailing delimiters do not produce empty tokens. An empty `s`, or one consisting only of delimiters, appends nothing.

This overload **appends** to `buf` rather than clearing it first — useful for accumulating tokens across multiple calls without per-call allocation of a new vector. Callers that want a fresh result set for each call should clear `buf` themselves beforehand.

[↑ Top](#table-of-contents)

### to_lower

```cpp
void to_lower(std::string_view _string, std::string& out) noexcept;
```

Writes a lowercased copy of `_string` into `out`. `_string` itself is not modified.

[↑ Top](#table-of-contents)

### trim

```cpp
void trim(std::string_view& s) noexcept;
void trim(std::string_view s, std::string& out) noexcept;
```

Trims leading and trailing whitespace.  
The first overload trims `s` in place by adjusting the view's bounds, with no copy or allocation.  
The second overload leaves `s` untouched and writes the trimmed result into `out`, for callers that need an owned `std::string` rather than a view.

[↑ Top](#table-of-contents)

## Building

This library is built using [SlimLibraryPackager](https://codeberg.org/greergan/SlimLibraryPackager). See that repository for build instructions.

[↑ Top](#table-of-contents)

## Dependencies

### required_packages

External package dependencies for this library are declared in the [`required_packages`](required_packages) file at the repository root. This file is read by [SlimLibraryPackager](https://codeberg.org/greergan/SlimLibraryPackager) during the build process to resolve dependencies and install them if not present.

```
none
```

[↑ Top](#table-of-contents)

## Examples

```cpp
#include <slim/common/utilities.h>

using slim::common::utilities::count_digits;
using slim::common::utilities::get_bool;
using slim::common::utilities::iequals;
using slim::common::utilities::is_alnum;
using slim::common::utilities::is_alpha;
using slim::common::utilities::is_cookie_char;
using slim::common::utilities::is_date_delimiter;
using slim::common::utilities::is_digit;
using slim::common::utilities::is_space;
using slim::common::utilities::month_abbr_to_int;
using slim::common::utilities::replace_all;
using slim::common::utilities::split;
using slim::common::utilities::to_lower;
using slim::common::utilities::trim;

// Digit counting
std::size_t digits = count_digits(31536000); // -> 8

// Boolean parsing
bool flag = false;
bool parsed = get_bool("true", flag); // parsed -> true, flag -> true

// Case-insensitive comparison
bool same = iequals("Session", "session"); // -> true

// Character classification
bool a1 = is_alnum('q');           // -> true
bool a2 = is_alpha('q');           // -> true
bool a3 = is_alpha('7');           // -> false
bool a4 = is_cookie_char('"');     // -> false
bool a5 = is_date_delimiter(':');  // -> false
bool a6 = is_digit('7');           // -> true
bool a7 = is_space('\t');          // -> true

// Month abbreviation lookup
int month = month_abbr_to_int("Feb"); // -> 1

// In-place substring replacement
std::string s = "a-b-c";
replace_all(s, "-", "_"); // s -> "a_b_c"

// Delimiter splitting into a caller-supplied buffer (appends across calls)
std::vector<std::string> buf;
split("a;;b;c;", ';', buf); // buf -> {"a", "b", "c"}
split("d;e", ';', buf);     // buf -> {"a", "b", "c", "d", "e"} (appended, not replaced)

// Lowercasing
std::string lower;
to_lower("Set-Cookie", lower); // lower -> "set-cookie"

// Trimming a view in place
std::string_view view = "   trim me   ";
trim(view); // view -> "trim me"

// Trimming into an owned string, leaving the source view untouched
std::string trimmed;
trim(std::string_view("   trim me too   "), trimmed); // trimmed -> "trim me too"
```

[↑ Top](#table-of-contents)
