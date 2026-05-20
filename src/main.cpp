#include <algorithm>
#include <slim/common/utilities.h>

void slim::common::utilities::replace_all(std::string& _string, const std::string& _original, const std::string& _replacement) {
    if(_string.empty() || _original.empty() || _replacement.empty()) {
		return;
	}

    size_t start_pos = 0;
    while((start_pos = _string.find(_original, start_pos)) != std::string::npos) {
        _string.replace(start_pos, _original.length(), _replacement);
        start_pos += _replacement.length();
    }
}

std::string slim::common::utilities::to_lower(std::string_view _string) {
	std::string return_string;
	return_string.resize(_string.size());
	std::transform(_string.begin(), _string.end(), return_string.begin(), [](unsigned char c){ return std::tolower(c); });
	return return_string;
}