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
  - [month_abbr_to_int](#month_abbr_to_int)
  - [replace_all](#replace_all)
  - [to_lower](#to_lower)
  - [trim](#trim)
- [Building](#building)
- [Dependencies](#dependencies)
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
| Month abbreviation lookup | Converts three-letter month abbreviations to their numeric index |
| In-place substring replacement | Replaces all occurrences of a substring within a string |
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
void get_bool(std::string_view s, bool& b) noexcept;
```

Parses `s` as a boolean and writes the result into `b`. Only accepts `"TRUE"`, `"true"`, `"FALSE"`, or `"false"`. `b` is left unchanged if `s` is none of these.

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

### month_abbr_to_int

```cpp
int month_abbr_to_int(std::string_view s) noexcept;
```

Converts a three-letter month abbreviation (e.g. `"Jan"`, `"Dec"`) to its 1-based month number. Returns `-1` when `s` is not a recognized abbreviation.

[↑ Top](#table-of-contents)

### replace_all

```cpp
void replace_all(std::string& _string, std::string_view _original, std::string_view _replacement) noexcept;
```

Replaces every non-overlapping occurrence of `_original` in `_string` with `_replacement`, modifying `_string` in place.

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
```

Trims leading and trailing whitespace from `s` in place by adjusting the view's bounds. No copy or allocation occurs.

[↑ Top](#table-of-contents)

## Building

This library is built using [SlimLibraryPackager](https://codeberg.org/greergan/SlimLibraryPackager). See that repository for build instructions.

[↑ Top](#table-of-contents)

## Dependencies

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
using slim::common::utilities::month_abbr_to_int;
using slim::common::utilities::replace_all;
using slim::common::utilities::to_lower;
using slim::common::utilities::trim;

// Digit counting
std::size_t digits = count_digits(31536000); // -> 8

// Boolean parsing
bool flag = false;
get_bool("true", flag); // flag -> true

// Case-insensitive comparison
bool same = iequals("Session", "session"); // -> true

// Month abbreviation lookup
int month = month_abbr_to_int("Feb"); // -> 2

// In-place substring replacement
std::string s = "a-b-c";
replace_all(s, "-", "_"); // s -> "a_b_c"

// Lowercasing
std::string lower;
to_lower("Set-Cookie", lower); // lower -> "set-cookie"

// Trimming a view in place
std::string_view view = "   trim me   ";
trim(view); // view -> "trim me"
```

[↑ Top](#table-of-contents)
