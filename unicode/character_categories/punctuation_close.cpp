#include "punctuation_close.hpp"

namespace {
  static const std::vector<char32_t> characters
    {
      0x29,
      0x5D,
      0x7D,
      0xF3B,
      0xF3D,
      0x169C,
      0x2046,
      0x207E,
      0x208E,
      0x2309,
      0x230B,
      0x232A,
      0x2769,
      0x276B,
      0x276D,
      0x276F,
      0x2771,
      0x2773,
      0x2775,
      0x27C6,
      0x27E7,
      0x27E9,
      0x27EB,
      0x27ED,
      0x27EF,
      0x2984,
      0x2986,
      0x2988,
      0x298A,
      0x298C,
      0x298E,
      0x2990,
      0x2992,
      0x2994,
      0x2996,
      0x2998,
      0x29D9,
      0x29DB,
      0x29FD,
      0x2E23,
      0x2E25,
      0x2E27,
      0x2E29,
      0x3009,
      0x300B,
      0x300D,
      0x300F,
      0x3011,
      0x3015,
      0x3017,
      0x3019,
      0x301B,

      0x301E, 0x301F,

      0xFD3E,
      0xFE18,
      0xFE36,
      0xFE38,
      0xFE3A,
      0xFE3C,
      0xFE3E,
      0xFE40,
      0xFE42,
      0xFE44,
      0xFE48,
      0xFE5A,
      0xFE5C,
      0xFE5E,
      0xFF09,
      0xFF3D,
      0xFF5D,
      0xFF60,
      0xFF63
    };
}

/*
Character Name
U+0029    RIGHT PARENTHESIS
U+005D    RIGHT SQUARE BRACKET
U+007D    RIGHT CURLY BRACKET
U+0F3B    TIBETAN MARK GUG RTAGS GYAS
U+0F3D    TIBETAN MARK ANG KHANG GYAS
U+169C    OGHAM REVERSED FEATHER MARK
U+2046    RIGHT SQUARE BRACKET WITH QUILL
U+207E    SUPERSCRIPT RIGHT PARENTHESIS
U+208E    SUBSCRIPT RIGHT PARENTHESIS
U+2309    RIGHT CEILING
U+230B    RIGHT FLOOR
U+232A    RIGHT-POINTING ANGLE BRACKET
U+2769    MEDIUM RIGHT PARENTHESIS ORNAMENT
U+276B    MEDIUM FLATTENED RIGHT PARENTHESIS ORNAMENT
U+276D    MEDIUM RIGHT-POINTING ANGLE BRACKET ORNAMENT
U+276F    HEAVY RIGHT-POINTING ANGLE QUOTATION MARK ORNAMENT
U+2771    HEAVY RIGHT-POINTING ANGLE BRACKET ORNAMENT
U+2773    LIGHT RIGHT TORTOISE SHELL BRACKET ORNAMENT
U+2775    MEDIUM RIGHT CURLY BRACKET ORNAMENT
U+27C6    RIGHT S-SHAPED BAG DELIMITER
U+27E7    MATHEMATICAL RIGHT WHITE SQUARE BRACKET
U+27E9    MATHEMATICAL RIGHT ANGLE BRACKET
U+27EB    MATHEMATICAL RIGHT DOUBLE ANGLE BRACKET
U+27ED    MATHEMATICAL RIGHT WHITE TORTOISE SHELL BRACKET
U+27EF    MATHEMATICAL RIGHT FLATTENED PARENTHESIS
U+2984    RIGHT WHITE CURLY BRACKET
U+2986    RIGHT WHITE PARENTHESIS
U+2988    Z NOTATION RIGHT IMAGE BRACKET
U+298A    Z NOTATION RIGHT BINDING BRACKET
U+298C    RIGHT SQUARE BRACKET WITH UNDERBAR
U+298E    RIGHT SQUARE BRACKET WITH TICK IN BOTTOM CORNER
U+2990    RIGHT SQUARE BRACKET WITH TICK IN TOP CORNER
U+2992    RIGHT ANGLE BRACKET WITH DOT
U+2994    RIGHT ARC GREATER-THAN BRACKET
U+2996    DOUBLE RIGHT ARC LESS-THAN BRACKET
U+2998    RIGHT BLACK TORTOISE SHELL BRACKET
U+29D9    RIGHT WIGGLY FENCE
U+29DB    RIGHT DOUBLE WIGGLY FENCE
U+29FD    RIGHT-POINTING CURVED ANGLE BRACKET
U+2E23    TOP RIGHT HALF BRACKET
U+2E25    BOTTOM RIGHT HALF BRACKET
U+2E27    RIGHT SIDEWAYS U BRACKET
U+2E29    RIGHT DOUBLE PARENTHESIS
U+3009    RIGHT ANGLE BRACKET
U+300B    RIGHT DOUBLE ANGLE BRACKET
U+300D    RIGHT CORNER BRACKET
U+300F    RIGHT WHITE CORNER BRACKET
U+3011    RIGHT BLACK LENTICULAR BRACKET
U+3015    RIGHT TORTOISE SHELL BRACKET
U+3017    RIGHT WHITE LENTICULAR BRACKET
U+3019    RIGHT WHITE TORTOISE SHELL BRACKET
U+301B    RIGHT WHITE SQUARE BRACKET
U+301E    DOUBLE PRIME QUOTATION MARK
U+301F    LOW DOUBLE PRIME QUOTATION MARK
U+FD3E    ORNATE LEFT PARENTHESIS
U+FE18    PRESENTATION FORM FOR VERTICAL RIGHT WHITE LENTICULAR BRAKCET
U+FE36    PRESENTATION FORM FOR VERTICAL RIGHT PARENTHESIS
U+FE38    PRESENTATION FORM FOR VERTICAL RIGHT CURLY BRACKET
U+FE3A    PRESENTATION FORM FOR VERTICAL RIGHT TORTOISE SHELL BRACKET
U+FE3C    PRESENTATION FORM FOR VERTICAL RIGHT BLACK LENTICULAR BRACKET
U+FE3E    PRESENTATION FORM FOR VERTICAL RIGHT DOUBLE ANGLE BRACKET
U+FE40    PRESENTATION FORM FOR VERTICAL RIGHT ANGLE BRACKET
U+FE42    PRESENTATION FORM FOR VERTICAL RIGHT CORNER BRACKET
U+FE44    PRESENTATION FORM FOR VERTICAL RIGHT WHITE CORNER BRACKET
U+FE48    PRESENTATION FORM FOR VERTICAL RIGHT SQUARE BRACKET
U+FE5A    SMALL RIGHT PARENTHESIS
U+FE5C    SMALL RIGHT CURLY BRACKET
U+FE5E    SMALL RIGHT TORTOISE SHELL BRACKET
U+FF09    FULLWIDTH RIGHT PARENTHESIS
U+FF3D    FULLWIDTH RIGHT SQUARE BRACKET
U+FF5D    FULLWIDTH RIGHT CURLY BRACKET
U+FF60    FULLWIDTH RIGHT WHITE PARENTHESIS
U+FF63    HALFWIDTH RIGHT CORNER BRACKET
*/
