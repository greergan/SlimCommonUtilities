#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <slim/common/utilities.h>

namespace slim::common::utilities {

namespace {
struct AsciiTables {
    std::array<char, 256> to_lower{};
    std::array<bool, 256> is_alnum{};
    std::array<bool, 256> is_digit{};
    std::array<bool, 256> is_space{};
    std::array<bool, 256> is_cookie_char{};
    std::array<bool, 256> is_date_delimiter{};

    constexpr AsciiTables() noexcept {
        for (size_t i = 0; i < 256; ++i) {
            to_lower[i] = (i >= 'A' && i <= 'Z') ? static_cast<char>(i + 32) : static_cast<char>(i);
            is_alnum[i] = (i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9');
            is_digit[i] = (i >= '0' && i <= '9');
            is_space[i] = (i == ' ' || i == '\t' || i == '\r' || i == '\n' || i == '\v' || i == '\f');

            unsigned char uc = static_cast<unsigned char>(i);
            is_cookie_char[i] = (uc == 0x21) || (uc >= 0x23 && uc <= 0x2B) || (uc >= 0x2D && uc <= 0x3A) ||
                                (uc >= 0x3C && uc <= 0x5B) || (uc >= 0x5D && uc <= 0x7E);

            is_date_delimiter[i] = (i == 0x09) || (i >= 0x20 && i <= 0x2F) || (i >= 0x3B && i <= 0x40) ||
                                    (i >= 0x5B && i <= 0x60) || (i >= 0x7B && i <= 0x7E);
        }
    }
};

constexpr AsciiTables ascii{};
} // namespace

constexpr std::size_t count_digits(std::uint_least64_t n) noexcept {
    if (n == 0) return 1;
    std::size_t digits = 0;
    if (n >= 1000000000000000000ULL) { digits += 18; n /= 1000000000000000000ULL; }
    if (n >= 1000000000ULL) { digits += 9; n /= 1000000000ULL; }
    if (n >= 100000ULL) { digits += 5; n /= 100000ULL; }
    if (n >= 100ULL) { digits += 2; n /= 100ULL; }
    if (n >= 10ULL) { digits += 1; n /= 10ULL; }
    return digits + 1;
}

constexpr void get_bool(std::string_view s, bool& b) noexcept {
    trim(s);
    if (iequals(s, "true")) { b = true; return; }
    if (iequals(s, "false")) { b = false; return; }
}

constexpr bool iequals(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i)
        if (ascii.to_lower[static_cast<unsigned char>(a[i])] != static_cast<unsigned char>(b[i])) return false;
    return true;
}

constexpr bool iiequals(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i)
        if (ascii.to_lower[static_cast<unsigned char>(a[i])] != ascii.to_lower[static_cast<unsigned char>(b[i])]) return false;
    return true;
}

constexpr int month_abbr_to_int(std::string_view s) noexcept {
    if (s.size() < 3) return -1;
    uint32_t val = (static_cast<uint32_t>(ascii.to_lower[static_cast<unsigned char>(s[0])]) << 16) |
                   (static_cast<uint32_t>(ascii.to_lower[static_cast<unsigned char>(s[1])]) << 8) |
                    static_cast<uint32_t>(ascii.to_lower[static_cast<unsigned char>(s[2])]);
    switch(val) {
        case 0x6a616e: return 0; case 0x666562: return 1; case 0x6d6172: return 2;
        case 0x617072: return 3; case 0x6d6179: return 4; case 0x6a756e: return 5;
        case 0x6a756c: return 6; case 0x617567: return 7; case 0x736570: return 8;
        case 0x6f6374: return 9; case 0x6e6f76: return 10; case 0x646563: return 11;
        default: return -1;
    }
}

void replace_all(std::string& _string, std::string_view _original, std::string_view _replacement) noexcept {
    if (_string.empty() || _original.empty() || _original == _replacement) return;

    size_t start_pos = 0;
    while ((start_pos = _string.find(_original, start_pos)) != std::string::npos) {
        _string.replace(start_pos, _original.length(), _replacement);
        start_pos += _replacement.length();
    }
}

inline void to_lower(std::string_view _string, std::string& out) noexcept {
    out.resize(_string.size());
    std::transform(_string.begin(), _string.end(), out.begin(), [](unsigned char c) {
        return ascii.to_lower[c];
    });
}

constexpr void trim(std::string_view& s) noexcept {
    while (!s.empty() && ascii.is_space[static_cast<unsigned char>(s.front())]) s.remove_prefix(1);
    while (!s.empty() && ascii.is_space[static_cast<unsigned char>(s.back())]) s.remove_suffix(1);
}

} // namespace slim::common::utilities
