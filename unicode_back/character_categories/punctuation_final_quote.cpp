#include "punctuation_final_quote.hpp"

namespace {

  static const std::vector<char32_t> characters
    {
      0xBB,
      0x2019,
      0x201D,
      0x203A,
      0x2E03,
      0x2E05,
      0x2E0A,
      0x2E0D,
      0x2E1D,
      0x2E21
    };
}

/*
Character Name
U+00BB    RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
U+2019    RIGHT SINGLE QUOTATION MARK
U+201D    RIGHT DOUBLE QUOTATION MARK
U+203A    SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
U+2E03    RIGHT SUBSTITUTION BRACKET
U+2E05    RIGHT DOTTED SUBSTITUTION BRACKET
U+2E0A    RIGHT TRANSPOSITION BRACKET
U+2E0D    RIGHT RAISED OMISSION BRACKET
U+2E1D    RIGHT LOW PARAPHRASE BRACKET
U+2E21    RIGHT VERTICAL BAR WITH QUILL
*/
