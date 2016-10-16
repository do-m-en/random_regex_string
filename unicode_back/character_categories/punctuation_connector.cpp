#include "punctuation_connector.hpp"

namespace {
  static const std::vector<char32_t> characters
    {
      0x5F,
      0x203F,
      0x2040,
      0x2054,

      0xFE33,
      0xFE34,

      0xFE4D,
      0xFE4E,
      0xFE4F,

      0xFF3F
    };
}

/*
Character Name
U+005F    LOW LINE
U+203F    UNDERTIE
U+2040    CHARACTER TIE
U+2054    INVERTED UNDERTIE
U+FE33    PRESENTATION FORM FOR VERTICAL LOW LINE
U+FE34    PRESENTATION FORM FOR VERTICAL WAVY LOW LINE
U+FE4D    DASHED LOW LINE
U+FE4E    CENTRELINE LOW LINE
U+FE4F    WAVY LOW LINE
U+FF3F    FULLWIDTH LOW LINE
*/
