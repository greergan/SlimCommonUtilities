#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <slim/common/utilities.h>

namespace slim::common::utilities {

namespace {
struct AsciiTables {
    std::array<bool, 256>  is_alnum{};
    std::array<bool, 256>  is_alpha{};
    std::array<bool, 256>  is_cookie_char{};
    std::array<bool, 256>  is_date_delimiter{};
    std::array<bool, 256>  is_digit{};
    std::array<bool, 256>  is_form_url_safe{};
    std::array<bool, 256>  is_space{};
    std::array<bool, 256>  is_xdigit{};
    std::array<char, 256>  to_lower{};

    constexpr AsciiTables() noexcept {
        for (size_t i = 0; i < 256; ++i) {
            is_alpha[i]         = (i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z');
            is_alnum[i]         = is_alpha[i] || (i >= '0' && i <= '9');
            is_digit[i]         = (i >= '0' && i <= '9');
            is_form_url_safe[i] = is_alnum[i] || i == '*' || i == '-' || i == '.' || i == '_';
            is_space[i]         = (i == ' ' || i == '\t' || i == '\r' || i == '\n' || i == '\v' || i == '\f');
            is_xdigit[i]        = is_digit[i] || (i >= 'a' && i <= 'f') || (i >= 'A' && i <= 'F');
            to_lower[i]         = (i >= 'A' && i <= 'Z') ? static_cast<char>(i + 32) : static_cast<char>(i);

            unsigned char uc     = static_cast<unsigned char>(i);
            is_cookie_char[i]    = (uc == 0x21) || (uc >= 0x23 && uc <= 0x2B) || (uc >= 0x2D && uc <= 0x3A) ||
                                    (uc >= 0x3C && uc <= 0x5B) || (uc >= 0x5D && uc <= 0x7E);
            is_date_delimiter[i] = (i == 0x09) || (i >= 0x20 && i <= 0x2F) || (i >= 0x3B && i <= 0x40) ||
                                    (i >= 0x5B && i <= 0x60) || (i >= 0x7B && i <= 0x7E);
        }
    }
};

constexpr AsciiTables ascii{};

constexpr char hex_chars[] = "0123456789ABCDEF";

constexpr uint8_t hex_val(char c) noexcept {
    if (c >= '0' && c <= '9') { return static_cast<uint8_t>(c - '0'); }
    if (c >= 'a' && c <= 'f') { return static_cast<uint8_t>(c - 'a' + 10); }
    if (c >= 'A' && c <= 'F') { return static_cast<uint8_t>(c - 'A' + 10); }
    return 0;
}
} // namespace

std::size_t count_digits(std::uint_least64_t n) noexcept {
    if (n == 0) { return 1; }
    std::size_t digits = 0;
    if (n >= 1000000000000000000ULL) { digits += 18; n /= 1000000000000000000ULL; }
    if (n >= 1000000000ULL)          { digits += 9;  n /= 1000000000ULL; }
    if (n >= 100000ULL)              { digits += 5;  n /= 100000ULL; }
    if (n >= 100ULL)                 { digits += 2;  n /= 100ULL; }
    if (n >= 10ULL)                  { digits += 1;  n /= 10ULL; }
    return digits + 1;
}

bool get_bool(std::string_view s, bool& b) noexcept {
    trim(s);
    if (iequals(s, "true"))  { b = true;  return true; }
    if (iequals(s, "false")) { b = false; return true; }
    return false;
}

bool iequals(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size()) { return false; }
    for (size_t i = 0; i < a.size(); ++i) {
        if (ascii.to_lower[static_cast<unsigned char>(a[i])] != static_cast<unsigned char>(b[i])) { return false; }
    }
    return true;
}

bool iiequals(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size()) { return false; }
    for (size_t i = 0; i < a.size(); ++i) {
        if (ascii.to_lower[static_cast<unsigned char>(a[i])] != ascii.to_lower[static_cast<unsigned char>(b[i])]) { return false; }
    }
    return true;
}

bool is_alnum(char c) noexcept          { return ascii.is_alnum[static_cast<unsigned char>(c)]; }
bool is_alpha(char c) noexcept          { return ascii.is_alpha[static_cast<unsigned char>(c)]; }
bool is_cookie_char(char c) noexcept    { return ascii.is_cookie_char[static_cast<unsigned char>(c)]; }
bool is_date_delimiter(char c) noexcept { return ascii.is_date_delimiter[static_cast<unsigned char>(c)]; }
bool is_digit(char c) noexcept          { return ascii.is_digit[static_cast<unsigned char>(c)]; }
bool is_space(char c) noexcept          { return ascii.is_space[static_cast<unsigned char>(c)]; }
bool is_xdigit(char c) noexcept         { return ascii.is_xdigit[static_cast<unsigned char>(c)]; }

bool is_valid_percent_encoding(const char* p, std::size_t remaining) noexcept {
    return remaining > 2 && is_xdigit(p[1]) && is_xdigit(p[2]);
}

int month_abbr_to_int(std::string_view s) noexcept {
    if (s.size() < 3) { return -1; }
    uint32_t val = (static_cast<uint32_t>(ascii.to_lower[static_cast<unsigned char>(s[0])]) << 16) |
                   (static_cast<uint32_t>(ascii.to_lower[static_cast<unsigned char>(s[1])]) << 8) |
                    static_cast<uint32_t>(ascii.to_lower[static_cast<unsigned char>(s[2])]);
    switch (val) {
        case 0x6a616e: return 0;  case 0x666562: return 1;  case 0x6d6172: return 2;
        case 0x617072: return 3;  case 0x6d6179: return 4;  case 0x6a756e: return 5;
        case 0x6a756c: return 6;  case 0x617567: return 7;  case 0x736570: return 8;
        case 0x6f6374: return 9;  case 0x6e6f76: return 10; case 0x646563: return 11;
        default: return -1;
    }
}

void percent_decode(std::string_view s, std::string& out) noexcept {
    out.clear();
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        const auto c = s[i];
        if (c == '+') {
            out += ' ';
        } else if (c == '%' && is_valid_percent_encoding(s.data() + i, s.size() - i)) {
            out += static_cast<char>((hex_val(s[i + 1]) << 4) | hex_val(s[i + 2]));
            i += 2;
        } else {
            out += c;
        }
    }
}

void percent_encode(std::string_view s, std::string& out) noexcept {
    out.clear();
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        const auto uc = static_cast<unsigned char>(s[i]);
        if (ascii.is_form_url_safe[uc]) {
            out += static_cast<char>(uc);
        } else if (uc == ' ') {
            out += '+';
        } else {
            out += '%';
            out += hex_chars[(uc >> 4) & 0x0F];
            out += hex_chars[uc & 0x0F];
        }
    }
}

void replace_all(std::string& _string, std::string_view _original, std::string_view _replacement) noexcept {
    if (_string.empty() || _original.empty() || _original == _replacement) { return; }
    size_t start_pos = 0;
    while ((start_pos = _string.find(_original, start_pos)) != std::string::npos) {
        _string.replace(start_pos, _original.length(), _replacement);
        start_pos += _replacement.length();
    }
}

std::vector<std::string_view> split(std::string_view s, char delim) noexcept {
    std::vector<std::string_view> tokens;
    size_t start = 0;
    size_t end   = s.find(delim);
    while (end != std::string_view::npos) {
        if (start != end) { tokens.push_back(s.substr(start, end - start)); }
        start = end + 1;
        end   = s.find(delim, start);
    }
    if (start < s.size()) { tokens.push_back(s.substr(start)); }
    return tokens;
}

void split(std::string_view s, char delim, std::vector<std::string>& buf) noexcept {
    size_t start = 0;
    size_t end   = s.find(delim);
    while (end != std::string_view::npos) {
        if (start != end) {
            std::string_view view = s.substr(start, end - start);
            trim(view);
            buf.emplace_back(view);
        }
        start = end + 1;
        end   = s.find(delim, start);
    }
    if (start < s.size()) {
        std::string_view view = s.substr(start);
        trim(view);
        buf.emplace_back(view);
    }
}

void to_lower(std::string_view _string, std::string& out) noexcept {
    out.resize(_string.size());
    std::transform(_string.begin(), _string.end(), out.begin(), [](unsigned char c) {
        return ascii.to_lower[c];
    });
}

void trim(std::string_view& s) noexcept {
    while (!s.empty() && ascii.is_space[static_cast<unsigned char>(s.front())]) { s.remove_prefix(1); }
    while (!s.empty() && ascii.is_space[static_cast<unsigned char>(s.back())]) { s.remove_suffix(1); }
}

void trim(std::string_view s, std::string& out) noexcept {
    trim(s);
    out.assign(s);
}

} // namespace slim::common::utilities
