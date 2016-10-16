#include "separator_paragraph.hpp"

namespace {
  constexpr std::pair<char32_t, char32_t> make_point(char32_t point)
  {
    return std::pair<char32_t, char32_t>(point, point);
  }

  static const std::vector<std::pair<char32_t, char32_t>> characters
    {
      make_point(0x2029)
    };
}

/*
Character Name
U+2029    PARAGRAPH SEPARATOR
*/
