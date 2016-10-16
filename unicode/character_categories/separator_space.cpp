#include "separator_space.hpp"

namespace {
  constexpr std::pair<char32_t, char32_t> make_point(char32_t point)
  {
    return std::pair<char32_t, char32_t>(point, point);
  }

  static const std::vector<std::pair<char32_t, char32_t>> characters
    {
      make_point(0x20),
      make_point(0xA0),
      make_point(0x1680),
      {0x2000, 0x200A},
      make_point(0x202F),
      make_point(0x205F),
      make_point(0x3000)
    };
}

/*
Character Name
U+0020    SPACE
U+00A0    NO-BREAK SPACE
U+1680    OGHAM SPACE MARK
U+2000    EN QUAD
U+2001    EM QUAD
U+2002    EN SPACE
U+2003    EM SPACE
U+2004    THREE-PER-EM SPACE
U+2005    FOUR-PER-EM SPACE
U+2006    SIX-PER-EM SPACE
U+2007    FIGURE SPACE
U+2008    PUNCTUATION SPACE
U+2009    THIN SPACE
U+200A    HAIR SPACE
U+202F    NARROW NO-BREAK SPACE
U+205F    MEDIUM MATHEMATICAL SPACE
U+3000    IDEOGRAPHIC SPACE
*/
