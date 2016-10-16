#include "blocks.hpp"

#include <unordered_map>
#include <vector>

namespace {
  constexpr std::pair<char32_t, char32_t> make_point(char32_t point)
  {
    return std::pair<char32_t, char32_t>(point, point);
  }

  static const std::unordered_map<std::string, std::vector<std::pair<char32_t, char32_t>>> blocks
    {
      // BasicLatin (#x0000 - #x007F): (Basic Latin) U+0000 - U+007F
      {"BasicLatin", {{0, 0x7F}}},
      // Latin-1Supplement (#x0080-#x00FF): (Latin-1 Supplement) U+0080 - U+00FF
      {"Latin-1Supplement", {{0x80, 0xFF}}},
      // LatinExtended-A (#x0100-#x017F): (Latin Extended-A) U+0100 - U+017F
      {"LatinExtended-A", {{0x100, 0x17F}}},
      // LatinExtended-B (#x0180-#x024F): (Latin Extended-B) U+0180 - U+024F
      {"LatinExtended-B", {{0x180, 0x24F}}},
      // IPAExtensions (#x0250-#x02AF): (IPA Extensions) U+0250 - U+02AF
      {"IPAExtensions", {{0x250, 0x2AF}}},
      // SpacingModifierLetters (#x02B0-#x02FF): (Spacing Modifier Letters) U+02B0 - U+02FF
      {"SpacingModifierLetters", {{0x2B0, 0x2FF}}},
      // CombiningDiacriticalMarks (#x0300-#x036F): (Combining Diacritical Marks) U+0300 - U+036F
      {"CombiningDiacriticalMarks", {{0x300, 0x36F}}},
      // Greek (#x0370-#x03FF): (Greek and Coptic) U+0370 - U+0377, U+037A - U+037F, U+0384 - U+038A, U+038C, U+038E - U+03FF
      {"Greek", {{0x370, 0x377}, {0x37A, 0x37F}, {0x384, 0x38A}, make_point(0x38C), {0x38E, 0x3FF}}},
      // Cyrillic (#x0400-#x04FF): (Cyrillic) U+0400 - U+04FF
      {"Cyrillic", {{0x400, 0x4FF}}},
      // -- (Cyrillic Supplement) U+0500 - U+052F
      {"CyrillicSupplement)", {{0x500, 0x52F}}},
      // Armenian (#x0530-#x058F): (Armenian) U+0531 - U+0556, U+0559 - U+055F, U+0561 - U+0587, U+0589 - U+058A, U+058D - U+058F
      {"Armenian", {{0x531, 0x556}, {0x559, 0x55F}, {0x561, 0x587}, {0x589, 0x58A}, {0x58D, 0x58F}}},
      // Hebrew (#x0590-#x05FF): (Hebrew) U+0591 - U+05C7, U+05D0 - U+05EA, U+05F0 - U+05F4
      {"Hebrew", {{0x591, 0x5C7}, {0x5D0, 0x5EA}, {0x5F0, 0x5F4}}},
      // Arabic (#x0600-#x06FF): (Arabic) U+0600 - U+061C, U+061E - U+06FF
      {"Arabic", {{0x600, 0x61C}, {0x61E, 0x6FF}}},
      // Syriac (#x0700-#x074F): (Syriac) U+0700 - U+070D, U+070F - U+074A, U+074D - U+074F
      {"Syriac", {{0x700, 0x70D}, {0x70F, 0x74A}, {0x74D, 0x74F}}},
      // -- (Arabic Supplement) U+0750 - U+077F
      {"ArabicSupplement", {{0x750, 0x77F}}},
      // Thaana (#x0780-#x07BF): (Thaana) U+0780 - U+07B1
      {"Thaana", {{0x780, 0x7B1}}},
      // -- (NKo) U+07C0 - U+07FA (U+07FF)
      {"NKo", {{0x7C0, 0x7FA}}},
      // -- (Samaritan) U+0800 - U+082D, U+0830 - U+083E (U+083F)
      {"Samaritan", {{0x800, 0x82D}, {0x830, 0x83E}}},
      // -- (Mandaic) U+0840 - U+085B, U+085E (U+085F)
      {"Mandaic", {{0x840, 0x85B}, make_point(0x85E)}},
      // -- (Arabic Extended-A) U+08A0 - U+08B4, U+08E3 - U+08FF
      {"ArabicExtended-A", {{0x8A0, 0x8B4}, {0x8E3, 0x8FF}}},
      // Devanagari (#x0900-#x097F): (Devanagari) U+0900 - U+097F
      {"Devanagari", {{0x900, 0x97F}}},
      // Bengali (#x0980-#x09FF): (Bengali) U+0980 - U+0983, U+0985 - U+098C, U+098F - U+0990, U+0993 - U+09A8, U+09AA - U+09B0, U+09B2, U+09B6 - U+09B9, U+09BC - U+09C4, U+09C7 - U+09C8, U+09CB - U+09CE, U+09D7, U+09DC - U+09DD, U+09DF - U+09E3, U+09E6 - U+09FB
      {"Bengali", {{0x980, 0x983}, {0x985, 0x98C}, {0x98F, 0x990}, {0x993, 0x9A8}, {0x9AA, 0x9B0}, make_point(0x9B2), {0x9B6, 0x9B9}, {0x9BC, 0x9C4}, {0x9C7, 0x9C8}, {0x9CB, 0x9CE}, make_point(0x9D7), {0x9DC, 0x9DD}, {0x9DF, 0x9E3}, {0x9E6, 0x9FB}}},
      // Gurmukhi (#x0A00-#x0A7F): (Gurmukhi) U+0A01 - U+0A03, U+0A05 - U+0A0A, U+0A0F - U+0A10, U+0A13 - U+0A28, U+0A2A - U+0A30, U+0A32 - U+0A33, U+0A35 - U+0A36, U+0A38 - U+0A39, U+0A3C, U+0A3E - U+0A42, U+0A47 - U+0A48, U+0A4B - U+0A4D, U+0A51, U+0A59 - U+0A5C, U+0A5E, U+0A66 - U+0A75
      {"Gurmukhi", {{0xA01, 0xA03}, {0xA05, 0xA0A}, {0xA0F, 0xA10}, {0xA13, 0xA28}, {0xA2A, 0xA30}, {0xA32, 0xA33}, {0xA35, 0xA36}, {0xA38, 0xA39}, make_point(0xA3C), {0xA3E, 0xA42}, {0xA47, 0xA48}, {0xA4B, 0xA4D}, make_point(0xA51), {0xA59, 0xA5C}, make_point(0xA5E), {0xA66, 0xA75}}},
      // Gujarati (#x0A80-#x0AFF): (Gujarati) U+0A81 - U+0A83, U+0A85 - U+0A8D, U+0A8F - U+0A91, U+0A93 - U+0AA8, U+0AAA - U+0AB0, U+0AB2 - U+0AB3, U+0AB5 - U+0AB9, U+0ABC - U+0AC5, U+0AC7 - U+0AC9, U+0ACB - U+0ACD, U+0AD0, U+0AE0 - U+0AE3, U+0AE6 - U+0AF1, U+0AF9
      {"Gujarati", {{0xA81, 0xA83}, {0xA85, 0xA8D}, {0xA8F, 0xA91}, {0xA93, 0xAA8}, {0xAAA, 0xAB0}, {0xAB2, 0xAB3}, {0xAB5, 0xAB9}, {0xABC, 0xAC5}, {0xAC7, 0xAC9}, {0xACB, 0xACD}, make_point(0xAD0), {0xAE0, 0xAE3}, {0xAE6, 0xAF1}, make_point(0xAF9)}},
      // Oriya (#x0B00-#x0B7F): (Oriya) U+0B01 - U+0B03, U+0B05 - U+0B0C, U+0B0F - U+0B10, U+0B13 - U+0B28, U+0B2A - U+0B30, U+0B32 - U+0B33, U+0B35 - U+0B39, U+0B3C - U+0B44, U+0B47 - U+0B48, U+0B4B - U+0B4D, U+0B56 - U+0B57, U+0B5C - U+0B5D, U+0B5F - U+0B63, U+0B66 - U+0B77
      {"Oriya", {{0xB01, 0xB03}, {0xB05, 0xB0C}, {0xB0F, 0xB10}, {0xB13, 0xB28}, {0xB2A, 0xB30}, {0xB32, 0xB33}, {0xB35, 0xB39}, {0xB3C, 0xB44}, {0xB47, 0xB48}, {0xB4B, 0xB4D}, {0xB56, 0xB57}, {0xB5C, 0xB5D}, {0xB5F, 0xB63}, {0xB66, 0xB77}}},
      // Tamil (#x0B80-#x0BFF): (Tamil) U+0B82 - U+0B83, U+0B85 - U+0B8A, U+0B8E - U+0B90, U+0B92 - U+0B95, U+0B99 - U+0B9A, U+0B9C, U+0B9E - U+0B9F, U+0BA3 - U+0BA4, U+0BA8 - U+0BAA, U+0BAE - U+0BB9, U+0BBE - U+0BC2, U+0BC6 - U+0BC8, U+0BCA - U+0BCD, U+0BD0, U+0BD7, U+0BE6 - U+0BFA
      {"Tamil", {{0xB82, 0xB83}, {0xB85, 0xB8A}, {0xB8E, 0xB90}, {0xB92, 0xB95}, {0xB99, 0xB9A}, make_point(0xB9C), {0xB9E, 0xB9F}, {0xBA3, 0xBA4}, {0xBA8, 0xBAA}, {0xBAE, 0xBB9}, {0xBBE, 0xBC2}, {0xBC6, 0xBC8}, {0xBCA, 0xBCD}, {0xBD0, 0xBD7}, {0xBE6, 0xBFA}}},
      // Telugu (#x0C00-#x0C7F): (Telugu) U+0C00 - U+0C03, U+0C05 - U+0C0C, U+0C0E - U+0C10, U+0C12 - U+0C28, U+0C2A - U+0C39, U+0C3D - U+0C44, U+0C46 - U+0C48, U+0C4A - U+0C4D, U+0C55 - U+0C56, U+0C58 - U+0C5A, U+0C60 - U+0C63, U+0C66 - U+0C6F, U+0C78 - U+0C7F
      {"Telugu", {{0xC00, 0xC03}, {0xC05, 0xC0C}, {0xC0E, 0xC10}, {0xC12, 0xC28}, {0xC2A, 0xC39}, {0xC3D, 0xC44}, {0xC46, 0xC48}, {0xC4A, 0xC4D}, {0xC55, 0xC56}, {0xC58, 0xC5A}, {0xC60, 0xC63}, {0xC66, 0xC6F}, {0xC78, 0xC7F}}},
      // Kannada (#x0C80-#x0CFF): (Kannada) U+0C81 - U+0C83, U+0C85 - U+0C8C, U+0C8E - U+0C90, U+0C92 - U+0CA8, U+0CAA - U+0CB3, U+0CB5 - U+0CB9, U+0CBC - U+0CC4, U+0CC6 - U+0CC8, U+0CCA - U+0CCD, U+0CD5 - U+0CD6, U+0CDE, U+0CE0 - U+0CE3, U+0CE6 - U+0CEF, U+0CF1 - U+0CF2
      {"Kannada", {{0xC81, 0xC83}, {0xC85, 0xC8C}, {0xC8E, 0xC90}, {0xC92, 0xCA8}, {0xCAA, 0xCB3}, {0xCB5, 0xCB9}, {0xCBC, 0xCC4}, {0xCC6, 0xCC8}, {0xCCA, 0xCCD}, {0xCD5, 0xCD6}, make_point(0xCDE), {0xCE0, 0xCE3}, {0xCE6, 0xCEF}, {0xCF1, 0xCF2}}},
      // Malayalam (#x0D00-#x0D7F): (Malayalam) U+0D01 - U+0D03, U+0D05 - U+0D0C, U+0D0E - U+0D10, U+0D12 - U+0D3A, U+0D3D - U+0D44, U+0D46 - U+0D48, U+0D4A - U+0D4E, U+0D57, U+0D5F - U+0D63, U+0D66 - U+0D75, U+0D79 - U+0D7F
      {"Malayalam", {{0xD01, 0xD03}, {0xD05, 0xD0C}, {0xD0E, 0xD10}, {0xD12, 0xD3A}, {0xD3D, 0xD44}, {0xD46, 0xD48}, {0xD4A, 0xD4E}, make_point(0xD57), {0xD5F, 0xD63}, {0xD66, 0xD75}, {0xD79, 0xD7F}}},
      // Sinhala (#x0D80-#x0DFF): (Sinhala) U+0D82 - U+0D83, U+0D85 - U+0D96, U+0D9A - U+0DB1, U+0DB3 - U+0DBB, U+0DBD, U+0DC0 - U+0DC6, U+0DCA, U+0DCF - U+0DD4, U+0DD6, U+0DD8 - U+0DDF, U+0DE6 - U+0DEF, U+0DF2 - U+0DF4
      {"Sinhala", {{0xD82, 0xD83}, {0xD85, 0xD96}, {0xD9A, 0xDB1}, {0xDB3, 0xDBB}, make_point(0xDBD), {0xDC0, 0xDC6}, make_point(0xDCA), {0xDCF, 0xDD4}, make_point(0xDD6), {0xDD8, 0xDDF}, {0xDE6, 0xDEF}, {0xDF2, 0xDF4}}},
      // Thai (#x0E00-#x0E7F): (Thai) U+0E01 - U+0E3A, U+0E3F - U+0E5B
      {"Thai", {{0xE01, 0xE3A}, {0xE3F, 0xE5B}}},
      // Lao (#x0E80-#x0EFF): (Lao) U+0E81 - U+0E82, U+0E84, U+0E87 - U+0E88, U+0E8A, U+0E8D, U+0E94 - U+0E97, U+0E99 - U+0E9F, U+0EA1 - U+0EA3, U+0EA5, U+0EA7, U+0EAA - U+0EAB, U+0EAD - U+0EB9, U+0EBB - U+0EBD, U+0EC0 - U+0EC4, U+0EC6, U+0EC8 - U+0ECD, U+0ED0 - U+0ED9, U+0EDC - U+0EDF
      {"Lao", {{0xE81, 0xE82}, make_point(0xE84), {0xE87, 0xE88}, make_point(0xE8A), make_point(0xE8D), {0xE94, 0xE97}, {0xE99, 0xE9F}, {0xEA1, 0xEA3}, make_point(0xEA5), make_point(0xEA7), {0xEAA, 0xEAB}, {0xEAD, 0xEB9}, {0xEBB, 0xEBD}, {0xEC0, 0xEC4}, make_point(0xEC6), {0xEC8, 0xECD}, {0xED0, 0xED9}, {0xEDC, 0xEDF}}},
      // Tibetan (#x0F00-#x0FFF): (Tibetan) U+0F00 - U+0F47, U+0F49 - U+0F6C, U+0F71 - U+0F97, U+0F99 - U+0FBC, U+0FBE - U+0FCC, U+0FCE - U+0FDA
      {"Tibetan", {{0xF00, 0xF47}, {0xF49, 0xF6C}, {0xF71, 0xF97}, {0xF99, 0xFBC}, {0xFBE, 0xFCC}, {0xFCE, 0xFDA}}},
      // Myanmar (#x1000-#x109F): (Myanmar) U+1000 - U+109F
      {"Myanmar", {{0x1000, 0x109F}}},
      // Georgian (#x10A0-#x10FF): (Georgian) U+10A0 - U+10C5, U+10C7, U+10CD, U+10D0 - U+10FF
      {"Georgian", {{0x10A0, 0x10C5}, make_point(0x10C7), make_point(0x10CD), {0x10D0, 0x10FF}}},
      // HangulJamo (#x1100-#x11FF): (Hangul Jamo) U+1100 - U+11FF
      {"HangulJamo", {{0x1100, 0x11FF}}},
      // Ethiopic (#x1200-#x137F): (Ethiopic) U+1200 - U+1248, U+124A - U+124D, U+1250 - U+1256, U+1258, U+125A - U+125D, U+1260 - U+1288, U+128A - U+128D, U+1290 - U+12B0, U+12B2 - U+12B5, U+12B8 - U+12BE, U+12C0, U+12C2 - U+12C5, U+12C8 - U+12D6, U+12D8 - U+1310, U+1312 - U+1315, U+1318 - U+135A, U+135D - U+137C
      {"Ethiopic", {{0x1200, 0x1248}, {0x124A, 0x124D}, {0x1250, 0x1256}, make_point(0x1258), {0x125A, 0x125D}, {0x1260, 0x1288}, {0x128A, 0x128D}, {0x1290, 0x12B0}, {0x12B2, 0x12B5}, {0x12B8, 0x12BE}, make_point(0x12C0), {0x12C2, 0x12C5}, {0x12C8, 0x12D6}, {0x12D8, 0x1310}, {0x1312, 0x1315}, {0x1318, 0x135A}, {0x135D, 0x137C}}},
      // -- (Ethiopic Supplement) U+1380 - U+1399 (U+139F)
      {"EthiopicSupplement", {{0x1380, 0x1399}}},
      // Cherokee (#x13A0-#x13FF): (Cherokee) U+13A0 - U+13F5, U+13F8 - U+13FD
      {"Cherokee", {{0x13A0, 0x13F5}, {0x13F8, 0x13FD}}},
      // UnifiedCanadianAboriginalSyllabics (#x1400-#x167F): (Unified Canadian Aboriginal Syllabics) U+1400 - U+167F
      {"UnifiedCanadianAboriginalSyllabics", {{0x1400, 0x167F}}},
      // Ogham (#x1680-#x169F): (Ogham) U+1680 - U+169C
      {"ogham", {{0x1680, 0x169C}}},
      // Ogham (#x16A0-#x16FF): (Runic) U+16A0 - U+16F8
      {"Runic", {{0x16A0, 0x16F8}}},
      // -- (Tagalog) U+1700 - U+170C, U+170E - U+1714 (U+171F)
      {"Tagalog", {{0x1700, 0x170C}, {0x170E, 0x1714}}},
      // -- (Hanunoo) U+1720 - U+1736 (U+173F)
      {"Hanunoo", {{0x1720, 0x1736}}},
      // -- (Buhid) U+1740 - U+1753 (U+175F)
      {"Buhid", {{0x1740, 0x1753}}},
      // -- (Tagbanwa) U+1760 - U+176C, U+176E - U+1770, U+1772 - U+1773 (U+177F)
      {"Tagbanwa", {{0x1760, 0x176C}, {0x176E, 0x1770}, {0x1772, 0x1773}}},
      // Khmer (#x1780-#x17FF): (Khmer) U+1780 - U+17DD, U+17E0 - U+17E9, U+17F0 - U+17F9
      {"Khmer", {{0x1780, 0x17DD}, {0x17E0, 0x17E9}, {0x17F0, 0x17F9}}},
      // Mongolian (#x1800-#x18AF): (Mongolian) U+1800 - U+180E, U+1810 - U+1819, U+1820 - U+1877, U+1880 - U+18AA
      {"Mongolian", {{0x1800, 0x180E}, {0x1810, 0x1819}, {0x1820, 0x1877}, {0x1880, 0x18AA}}},
      // -- (Unified Canadian Aboriginal Syllabics Extended) U+18B0 - U+18F5 (U+18FF)
      {"UnifiedCanadianAboriginalSyllabicsExtended", {{0x18B0, 0x18F5}}},
      // -- (Limbu) U+1900 - U+191E, U+1920 - U+192B, U+1930 - U+193B, U+1940, U+1944 - U+194F
      {"Limbu", {{0x1900, 0x191E}, {0x1920, 0x192B}, {0x1930, 0x193B}, make_point(0x1940), {0x1944, 0x194F}}},
      // -- (Tai Le) U+1950 - U+196D, U+1970 - U+1974 (U+197F)
      {"TaiLe", {{0x1950, 0x196D}, {0x1970, 0x1974}}},
      // -- (New Tai Lue) U+1980 - U+19AB, U+19B0 - U+19C9, U+19D0 - U+19DA, U+19DE - U+19DF
      {"NewTaiLue", {{0x1980, 0x19AB}, {0x19B0, 0x19C9}, {0x19D0, 0x19DA}, {0x19DE, 0x19DF}}},
      // -- (Khmer Symbols) U+19E0 - U+19FF
      {"KhmerSymbols", {{0x19E0, 0x19FF}}},
      // -- (Buginese) U+1A00 - U+1A1B, U+1A1E - U+1A1F
      {"Buginese", {{0x1A00, 0x1A1B}, {0x1A1E, 0x1A1F}}},
      // -- (Tai Tham) U+1A20 - U+1A5E, U+1A60 - U+1A7C, U+1A7F - U+1A89, U+1A90 - U+1A99, U+1AA0 - U+1AAD
      {"TaiTham)", {{0x1A20, 0x1A5E}, {0x1A60, 0x1A7C}, {0x1A7F, 0x1A89}, {0x1A90, 0x1A99}, {0x1AA0, 0x1AAD}}},
      // -- (Combining Diacritical Marks Extended) U+1AB0 - U+1ABE (U+1AFF)
      {"CombiningDiacriticalMarksExtended)", {{0x1AB0, 0x1ABE}}},
      // -- (Balinese) U+1B00 - U+1B4B, U+1B50 - U+1B7C (U+1B7F)
      {"Balinese", {{0x1B00, 0x1B4B}, {0x1B50, 0x1B7C}}},
      // -- (Sundanese) U+1B80 - U+1BBF
      {"Sundanese", {{0x1B80, 0x1BBF}}},
      // -- (Batak) U+1BC0 - U+1BF3, U+1BFC - U+1BFF
      {"Batak", {{0x1BC0, 0x1BF3}, {0x1BFC, 0x1BFF}}},
      // -- (Lepcha) U+1C00 - U+1C37, U+1C3B - U+1C49, U+1C4D - U+1C4F
      {"Lepcha", {{0x1C00, 0x1C37}, {0x1C3B, 0x1C49}, {0x1C4D, 0x1C4F}}},
      // -- (Ol Chiki) U+1C50 - U+1C7F
      {"OlChiki", {{0x1C50, 0x1C7F}}},
      // -- (Sundanese Supplement) U+1CC0 - U+1CC7 (U+1CCF)
      {"SundaneseSupplement", {{0x1CC0, 0x1CC7}}},
      // -- (Vedic Extensions) U+1CD0 - U+1CF6, U+1CF8 - U+1CF9 (U+1CFF)
      {"VedicExtensions", {{0x1CD0, 0x1CF6}, {0x1CF8, 0x1CF9}}},
      // -- (Phonetic Extensions) U+1D00 - U+1D7F
      {"PhoneticExtensions", {{0x1D00, 0x1D7F}}},
      // -- (Phonetic Extensions Supplement) U+1D80 - U+1DBF
      {"PhoneticExtensionsSupplement", {{0x1D80, 0x1DBF}}},
      // -- (Combining Diacritical Marks Supplement) U+1DC0 - U+1DF5, U+1DFC - U+1DFF
      {"CombiningDiacriticalMarksSupplement", {{0x1DC0, 0x1DF5}, {0x1DFC, 0x1DFF}}},
      // LatinExtendedAdditional (#x1E00-#x1EFF): (Latin Extended Additional) U+1E00 - U+1EFF
      {"LatinExtendedAdditional", {{0x1E00, 0x1EFF}}},
      // GreekExtended (#x1F00-#x1FFF): (Greek Extended) U+1F00 - U+1F15, U+1F18 - U+1F1D, U+1F20 - U+1F45, U+1F48 - U+1F4D, U+1F50 - U+1F57, U+1F59, U+1F5B, U+1F5D, U+1F5F - U+1F7D, U+1F80 - U+1FB4, U+1FB6 - U+1FC4, U+1FC6 - U+1FD3, U+1FD6 - U+1FDB, U+1FDD - U+1FEF, U+1FF2 - U+1FF4, U+1FF6 - U+1FFE
      {"GreekExtended", {{0x1F00, 0x1F15}, {0x1F18, 0x1F1D}, {0x1F20, 0x1F45}, {0x1F48, 0x1F4D}, {0x1F50, 0x1F57}, make_point(0x1F59), make_point(0x1F5B), make_point(0x1F5D), {0x1F5F, 0x1F7D}, {0x1F80, 0x1FB4}, {0x1FB6, 0x1FC4}, {0x1FC6, 0x1FD3}, {0x1FD6, 0x1FDB}, {0x1FDD, 0x1FEF}, {0x1FF2, 0x1FF4}, {0x1FF6, 0x1FFE}}},
      // GeneralPunctuation (#x2000-#x206F): (General Punctuation) U+2000 - U+2064, U+2066 - U+206F
      {"GeneralPunctuation", {{0x2000, 0x2064}, {0x2066, 0x206F}}},
      // SuperscriptsandSubscripts (#x2070-#x209F): (Superscripts and Subscripts) U+2070 - U+2071, U+2074 - U+208E, U+2090 - U+209C
      {"SuperscriptsandSubscripts", {{0x2070, 0x2071}, {0x2074, 0x208E}, {0x2090, 0x209C}}},
      // CurrencySymbols (#x20A0-#x20CF): (Currency Symbols) U+20A0 - U+20BE
      {"CurrencySymbols", {{0x20A0, 0x20BE}}},
      // CombiningMarksforSymbols (#x20D0-#x20FF): (Combining Diacritical Marks for Symbols) U+20D0 - U+20F0
      {"CombiningMarksforSymbols", {{0x20D0, 0x20F0}}},
      // LetterlikeSymbols (#x2100-#x214F): (Letterlike Symbols) U+2100 - U+214F
      {"LetterlikeSymbols", {{0x2100, 0x214F}}},
      // NumberForms (#x2150-#x218F): (Number Forms) U+2150 - U+218B
      {"NumberForms", {{0x2150, 0x218B}}},
      // Arrows (#x2190-#x21FF): (Arrows) U+2190 - U+21FF
      {"Arrows", {{0x2190, 0x21FF}}},
      // MathematicalOperators (#x2200-#x22FF): (Mathematical Operators) U+2200 - U+22FF
      {"MathematicalOperators", {{0x2200, 0x22FF}}},
      // MiscellaneousTechnical (#x2300-#x23FF): (Miscellaneous Technical) U+2300 - U+23FA
      {"MiscellaneousTechnical", {{0x2300, 0x23FA}}},
      // ControlPictures (#x2400-#x243F): (Control Pictures) U+2400 - U+2426
      {"ControlPictures", {{0x2400, 0x2426}}},
      // OpticalCharacterRecognition (#x2440-#x245F): (Optical Character Recognition) U+2440 - U+244A
      {"OpticalCharacterRecognition", {{0x2440, 0x244A}}},
      // EnclosedAlphanumerics (#x2460-#x24FF): (Enclosed Alphanumerics) U+2460 - U+24FF
      {"EnclosedAlphanumerics", {{0x2460, 0x24FF}}},
      // BoxDrawing (#x2500-#x257F): (Box Drawing) U+2500 - U+257F
      {"BoxDrawing", {{0x2500, 0x257F}}},
      // BlockElements (#x2580-#x259F): (Block Elements) U+2580 - U+259F
      {"BlockElements", {{0x2580, 0x259F}}},
      // GeometricShapes (#x25A0-#x25FF): (Geometric Shapes) U+25A0 - U+25FF
      {"GeometricShapes", {{0x2580, 0x259F}}},
      // MiscellaneousSymbols (#x2600-#x26FF): (Miscellaneous Symbols) U+2600 - U+26FF
      {"MiscellaneousSymbols", {{0x2600, 0x26FF}}},
      // Dingbats (#x2700-#x27BF): (Dingbats) U+2700 - U+27BF
      {"Dingbats", {{0x2700, 0x27BF}}},
      // -- (Miscellaneous Mathematical Symbols-A) U+27C0 - U+27EF
      {"MiscellaneousMathematicalSymbols-A", {{0x27C0, 0x27EF}}},
      // -- (Supplemental Arrows-A) U+27F0 - U+27FF
      {"SupplementalArrows-A", {{0x27F0, 0x27FF}}},
      // BraillePatterns (#x2800-#x28FF): (Braille Patterns) U+2800 - U+28FF
      {"BraillePatterns", {{0x2800, 0x28FF}}},
      // -- (Supplemental Arrows-B) U+2900 - U+297F
      {"SupplementalArrows-B", {{0x2900, 0x297F}}},
      // -- (Miscellaneous Mathematical Symbols-B) U+2980 - U+29FF
      {"MiscellaneousMathematicalSymbols-B", {{0x2980, 0x29FF}}},
      // -- (Supplemental Mathematical Operators) U+2A00 - U+2AFF
      {"SupplementalMathematicalOperators", {{0x2A00, 0x2AFF}}},
      // -- (Miscellaneous Symbols and Arrows) U+2B00 - U+2B73, U+2B76 - U+2B95, U+2B98 - U+2BB9, U+2BBD - U+2BC8, U+2BCA - U+2BD1, U+2BEC - U+2BEF (U+2BFF)
      {"MiscellaneousSymbolsAndArrows", {{0x2B00, 0x2B73}, {0x2B76, 0x2B95}, {0x2B98, 0x2BB9}, {0x2BBD, 0x2BC8}, {0x2BCA, 0x2BD1}, {0x2BEC, 0x2BEF}}}, // TODO check if and is OK in lower case
      // -- (Glagolitic) U+2C00 - U+2C2E, U+2C30 - U+2C5E (U+2C5F)
      {"Glagolitic", {{0x2C00, 0x2C2E}, {0x2C30, 0x2C5E}}},
      // -- (Latin Extended-C) U+2C60 - U+2C7F
      {"LatinExtended-C", {{0x2C60, 0x2C7F}}},
      // -- (Coptic) U+2C80 - U+2CF3, U+2CF9 - U+2CFF
      {"Coptic", {{0x2C80, 0x2CF3}, {0x2CF9, 0x2CFF}}},
      // -- (Georgian Supplement) U+2D00 - U+2D25, U+2D27, U+2D2D (U+2D2F)
      {"GeorgianSupplement", {{0x2D00, 0x2D25}, make_point(0x2D27), make_point(0x2D2D)}},
      // -- (Tifinagh) U+2D30 - U+2D67, U+2D6F - U+2D70, U+2D7F
      {"Tifinagh", {{0x2D30, 0x2D67}, {0x2D6F, 0x2D70}, make_point(0x2D7F)}},
      // -- (Ethiopic Extended) U+2D80 - U+2D96, U+2DA0 - U+2DA6, U+2DA8 - U+2DAE, U+2DB0 - U+2DB6, U+2DB8 - U+2DBE, U+2DC0 - U+2DC6, U+2DC8 - U+2DCE, U+2DD0 - U+2DD6, U+2DD8 - U+2DDE (U+2DDF)
      {"EthiopicExtended", {{0x2D80, 0x2D96}, {0x2DA0, 0x2DA6}, {0x2DA8, 0x2DAE}, {0x2DB0, 0x2DB6}, {0x2DB8, 0x2DBE}, {0x2DC0, 0x2DC6}, {0x2DC8, 0x2DCE}, {0x2DD0, 0x2DD6}, {0x2DD8, 0x2DDE}}},
      // -- (Cyrillic Extended-A) U+2DE0 - U+2DFF
      {"CyrillicExtended-A", {{0x2DE0, 0x2DFF}}},
      // -- (Supplemental Punctuation) U+2E00 - U+2E42 (U+2E7F)
      {"SupplementalPunctuation", {{0x2E00, 0x2E42}}},
      // CJKRadicalsSupplement (#x2E80-#x2EFF): (CJK Radicals Supplement) U+2E80 - U+2E99, U+2E9B - U+2EF3
      {"CJKRadicalsSupplement", {{0x2E80, 0x2E99}, {0x2E9B, 0x2EF3}}},
      // KangxiRadicals (#x2F00-#x2FDF): (Kangxi Radicals) U+2F00 - U+2FD5
      {"KangxiRadicals", {{0x2F00, 0x2FD5}}},
      // IdeographicDescriptionCharacters (#x2FF0-#x2FFF): (Ideographic Description Characters) U+2FF0 - U+2FFB
      {"IdeographicDescriptionCharacters", {{0x2FF0, 0x2FFB}}},
      // CJKSymbolsandPunctuation (#x3000-#x303F): (CJK Symbols and Punctuation) U+3000 - U+303F
      {"CJKSymbolsandPunctuation", {{0x3000, 0x303F}}},
      // Hiragana (#x3040-#x309F): (Hiragana) U+3041 - U+3096, U+3099 - U+309F
      {"Hiragana", {{0x3041, 0x3096}, {0x3099, 0x309F}}},
      // Katakana (#x30A0-#x30FF): (Katakana) U+30A0 - U+30FF
      {"Katakana", {{0x30A0, 0x30FF}}},
      // Bopomofo (#x3100-#x312F): (Bopomofo) U+3105 - U+312D
      {"Bopomofo", {{0x3105, 0x312D}}},
      // HangulCompatibilityJamo (#x3130-#x318F): (Hangul Compatibility Jamo) U+3131 - U+318E
      {"HangulCompatibilityJamo", {{0x3131, 0x318E}}},
      // Kanbun (#x3190-#x319F): (Kanbun) U+3190 - U+319F
      {"Kanbun", {{0x3190, 0x319F}}},
      // BopomofoExtended (#x31A0-#x31BF): (Bopomofo Extended) U+31A0 - U+31BA
      {"BopomofoExtended", {{0x31A0, 0x31BA}}},
      // -- (CJK Strokes) U+31C0 - U+31E3 (U+31EF)
      {"CJKStrokes", {{0x31C0, 0x31E3}}},
      // -- (Katakana Phonetic Extensions) U+31F0 - U+31FF
      {"KatakanaPhoneticExtensions", {{0x31F0, 0x31FF}}},
      // EnclosedCJKLettersandMonths (#x3200-#x32FF): (Enclosed CJK Letters and Months) U+3200 - U+321E, U+3220 - U+32FE
      {"EnclosedCJKLettersandMonths", {{0x3200, 0x321E}, {0x3220, 0x32FE}}},
      // CJKCompatibility (#x3300-#x33FF): (CJK Compatibility) U+3300 - U+33FF
      {"CJKCompatibility", {{0x3300, 0x33FF}}},
      // CJKUnifiedIdeographsExtensionA (#x3400-#x4DB5): (CJK Unified Ideographs Extension A) U+3400 - U+3402, U+3404 - U+3406, U+340C, U+3416, U+341C, U+3421, U+3424 - U+3425, U+3427 - U+3429, U+342B - U+3435, U+3437 - U+343D, U+3440 - U+3445, U+3447 - U+3450, U+3454, U+3457 - U+345F, U+3463 - U+346B, U+346E - U+3471, U+3473 - U+348E, U+3491 - U+3497, U+3499 - U+34A1, U+34A4 - U+34AB, U+34AD, U+34AF - U+34B0, U+34B2 - U+34B9, U+34BB - U+34BF, U+34C1 - U+34C5, U+34C7 - U+34CC, U+34CE - U+34D1, U+34D3 - U+34D4, U+34D6 - U+34EA, U+34EC - U+34FE, U+3500 - U+3507, U+3509 - U+3513, U+3515, U+3517 - U+351A, U+351C - U+352A, U+352C - U+3531, U+3533 - U+353F, U+3541 - U+357D, U+3580 - U+3588, U+358A, U+358F - U+3598, U+359E - U+35AB, U+35AD - U+35AE, U+35B5 - U+35CF, U+35D1 - U+35DE, U+35E2 - U+35ED, U+35F0 - U+35F7, U+35FB - U+3602, U+3604 - U+360E, U+3610 - U+3611, U+3613 - U+361E, U+3620 - U+362D, U+3630 - U+363B, U+363D - U+3645, U+3647 - U+364A, U+364C - U+3655, U+3657 - U+366C, U+366E - U+367C, U+367E, U+3680 - U+3683, U+3685 - U+3687, U+3689 - U+368D, U+368F - U+36C8, U+36CA, U+36CD - U+36FF, U+3701 - U+3713, U+3715 - U+371E, U+3720 - U+3745, U+3747 - U+3767, U+3769 - U+3772, U+3774 - U+3776, U+3778 - U+379C, U+379E - U+37D4, U+37D6 - U+37E0, U+37E2, U+37E4 - U+37EE, U+37F0 - U+3802, U+3804 - U+381D, U+381F - U+3823, U+3825 - U+382A, U+382D - U+382F, U+3831 - U+3832, U+3834 - U+384C, U+384E - U+3863, U+3865 - U+386B, U+386D - U+3886, U+3888 - U+388B, U+388D - U+388F, U+3891 - U+38A1, U+38A3 - U+38AA, U+38AC - U+38B0, U+38B2 - U+38B6, U+38B8, U+38BA - U+38BE, U+38C0 - U+38C9, U+38CB - U+38D4, U+38D8 - U+38E0, U+38E2 - U+38E7, U+38EA - U+38ED, U+38EF - U+38F2, U+38F5 - U+38F7, U+38FA - U+393B, U+393D - U+3956, U+3958, U+395A - U+397A, U+397C - U+3987, U+3989 - U+399F, U+39A1 - U+39DF, U+39E1 - U+3A22, U+3A24 - U+3A2B, U+3A2D - U+3A41, U+3A43 - U+3A4E, U+3A50, U+3A52 - U+3A6A, U+3A6C - U+3AB1, U+3AB4 - U+3AC5, U+3ACA - U+3AE2, U+3AE4 - U+3AEB, U+3AEF - U+3AFE, U+3B01 - U+3B10, U+3B12 - U+3B15, U+3B17 - U+3B27, U+3B29 - U+3B36, U+3B38 - U+3B39, U+3B3B - U+3B3C, U+3B3F, U+3B41 - U+3B44, U+3B47 - U+3B4F, U+3B51 - U+3B55, U+3B57 - U+3B64, U+3B68 - U+3B72, U+3B74, U+3B77 - U+3B88, U+3B8B - U+3B9F, U+3BA1 - U+3BBC, U+3BBE - U+3BD0, U+3BD3 - U+3BE6, U+3BE9 - U+3BF4, U+3BF6 - U+3BFB, U+3BFD - U+3C12, U+3C14 - U+3C1B, U+3C1D - U+3C4F, U+3C52, U+3C54 - U+3C5C, U+3C5E - U+3C76, U+3C78 - U+3C8F, U+3C92 - U+3CA8, U+3CAA - U+3CAD, U+3CAF - U+3CD2, U+3CD4 - U+3CE2, U+3CE4 - U+3CF0, U+3CF4 - U+3D0B, U+3D0F - U+3D1E, U+3D25 - U+3D38, U+3D3B - U+3D46, U+3D4A - U+3D59, U+3D5D - U+3D7B, U+3D7D - U+3D81, U+3D84 - U+3D8A, U+3D8C - U+3D8F, U+3D91 - U+3D98, U+3D9A - U+3D9C, U+3D9E - U+3DA1, U+3DA3 - U+3DA8, U+3DAB - U+3DB0, U+3DB2 - U+3DB6, U+3DB9 - U+3DC0, U+3DC2 - U+3DDB, U+3DDD, U+3DDF - U+3DF0, U+3DF3 - U+3DF9, U+3DFB - U+3E01, U+3E03 - U+3E13, U+3E15 - U+3E68, U+3E6A - U+3E85, U+3E87 - U+3E98, U+3E9A - U+3EAE, U+3EB0 - U+3EB5, U+3EB7 - U+3EBA, U+3EBF - U+3EC5, U+3EC7, U+3EC9 - U+3ED8, U+3EDA - U+3EDB, U+3EDD - U+3EE2, U+3EE7 - U+3EFA, U+3EFC - U+3F00, U+3F02, U+3F04, U+3F06 - U+3F53, U+3F55 - U+3F75, U+3F77 - U+3FB1, U+3FB4 - U+3FC0, U+3FC2 - U+3FCF, U+3FD1 - U+403F, U+4041 - U+4060, U+4062 - U+408A, U+408C - U+40C2, U+40C5, U+40C7 - U+40CF, U+40D1 - U+40E7, U+40E9 - U+40FB, U+40FD - U+4109, U+410B - U+4116, U+4118 - U+411D, U+411F - U+4122, U+4124 - U+4133, U+4136 - U+416A, U+416C - U+4186, U+4188 - U+418B, U+418D - U+41FE, U+4200 - U+4227, U+422A - U+4246, U+4248 - U+42C4, U+42C6, U+42C8 - U+434E, U+4350 - U+439D, U+439F - U+43A4, U+43A6 - U+4455, U+4457 - U+446D, U+446F - U+4476, U+4478 - U+447D, U+447F - U+4498, U+449A - U+44AD, U+44B0 - U+44C0, U+44C2 - U+44FA, U+44FC - U+4525, U+4527, U+452A, U+452C - U+4541, U+4543 - U+454F, U+4551 - U+45AB, U+45AD - U+45EC, U+45EE - U+45F2, U+45F4 - U+4626, U+4628 - U+4631, U+4633 - U+464A, U+464C, U+464E - U+466A, U+466C - U+4675, U+4677 - U+468B, U+468D - U+46A3, U+46A5 - U+4701, U+4703 - U+4725, U+4727, U+4729 - U+4732, U+4734 - U+473E, U+4740 - U+4748, U+474A - U+478E, U+4790 - U+47B1, U+47B3 - U+47CD, U+47CF - U+47F1, U+47F4 - U+480B, U+480D - U+4839, U+483B - U+4877, U+4879 - U+4885, U+4887 - U+48B6, U+48B8 - U+48C8, U+48CA - U+48D1, U+48D3 - U+48D6, U+48D8 - U+48FF, U+4901 - U+4965, U+4967 - U+4978, U+497A - U+49A8, U+49AA - U+49B7, U+49B9 - U+49BE, U+49C1 - U+49C7, U+49C9 - U+49CE, U+49D0 - U+49E8, U+49EA - U+49EC, U+49EE - U+4A02, U+4A04 - U+4A4D, U+4A4F - U+4A9E, U+4AA0 - U+4AA9, U+4AAB - U+4AE6, U+4AE8 - U+4B4E, U+4B50 - U+4B5B, U+4B5D - U+4B84, U+4B86 - U+4BE8, U+4BEA - U+4C2D, U+4C2F - U+4C32, U+4C34 - U+4C68, U+4C6B - U+4C86, U+4C89 - U+4CE4, U+4CE6 - U+4D27, U+4D29 - U+4DAC, U+4DAE - U+4DB5
      {"CJKUnifiedIdeographsExtensionA", {{0x3400, 0x3402}, {0x3404, 0x3406}, make_point(0x340C), make_point(0x3416), make_point(0x341C), make_point(0x3421), {0x3424, 0x3425}, {0x3427, 0x3429}, {0x342B, 0x3435}, {0x3437, 0x343D}, {0x3440, 0x3445}, {0x3447, 0x3450}, make_point(0x3454), {0x3457, 0x345F}, {0x3463, 0x346B}, {0x346E, 0x3471}, {0x3473, 0x348E}, {0x3491, 0x3497}, {0x3499, 0x34A1}, {0x34A4, 0x34AB}, make_point(0x34AD), {0x34AF, 0x34B0}, {0x34B2, 0x34B9}, {0x34BB, 0x34BF}, {0x34C1, 0x34C5}, {0x34C7, 0x34CC}, {0x34CE, 0x34D1}, {0x34D3, 0x34D4}, {0x34D6, 0x34EA}, {0x34EC, 0x34FE}, {0x3500, 0x3507}, {0x3509, 0x3513}, make_point(0x3515), {0x3517, 0x351A}, {0x351C, 0x352A}, {0x352C, 0x3531}, {0x3533, 0x353F}, {0x3541, 0x357D}, {0x3580, 0x3588}, make_point(0x358A), {0x358F, 0x3598}, {0x359E, 0x35AB}, {0x35AD, 0x35AE}, {0x35B5, 0x35CF}, {0x35D1, 0x35DE}, {0x35E2, 0x35ED}, {0x35F0, 0x35F7}, {0x35FB, 0x3602}, {0x3604, 0x360E}, {0x3610, 0x3611}, {0x3613, 0x361E}, {0x3620, 0x362D}, {0x3630, 0x363B}, {0x363D, 0x3645}, {0x3647, 0x364A}, {0x364C, 0x3655}, {0x3657, 0x366C}, {0x366E, 0x367C}, make_point(0x367E), {0x3680, 0x3683}, {0x3685, 0x3687}, {0x3689, 0x368D}, {0x368F, 0x36C8}, make_point(0x36CA), {0x36CD, 0x36FF}, {0x3701, 0x3713}, {0x3715, 0x371E}, {0x3720, 0x3745}, {0x3747, 0x3767}, {0x3769, 0x3772}, {0x3774, 0x3776}, {0x3778, 0x379C}, {0x379E, 0x37D4}, {0x37D6, 0x37E0}, make_point(0x37E2), {0x37E4, 0x37EE}, {0x37F0, 0x3802}, {0x3804, 0x381D}, {0x381F, 0x3823}, {0x3825, 0x382A}, {0x382D, 0x382F}, {0x3831, 0x3832}, {0x3834, 0x384C}, {0x384E, 0x3863}, {0x3865, 0x386B}, {0x386D, 0x3886}, {0x3888, 0x388B}, {0x388D, 0x388F}, {0x3891, 0x38A1}, {0x38A3, 0x38AA}, {0x38AC, 0x38B0}, {0x38B2, 0x38B6}, make_point(0x38B8), {0x38BA, 0x38BE}, {0x38C0, 0x38C9}, {0x38CB, 0x38D4}, {0x38D8, 0x38E0}, {0x38E2, 0x38E7}, {0x38EA, 0x38ED}, {0x38EF, 0x38F2}, {0x38F5, 0x38F7}, {0x38FA, 0x393B}, {0x393D, 0x3956}, make_point(0x3958), {0x395A, 0x397A}, {0x397C, 0x3987}, {0x3989, 0x399F}, {0x39A1, 0x39DF}, {0x39E1, 0x3A22}, {0x3A24, 0x3A2B}, {0x3A2D, 0x3A41}, {0x3A43, 0x3A4E}, make_point(0x3A50), {0x3A52, 0x3A6A}, {0x3A6C, 0x3AB1}, {0x3AB4, 0x3AC5}, {0x3ACA, 0x3AE2}, {0x3AE4, 0x3AEB}, {0x3AEF, 0x3AFE}, {0x3B01, 0x3B10}, {0x3B12, 0x3B15}, {0x3B17, 0x3B27}, {0x3B29, 0x3B36}, {0x3B38, 0x3B39}, {0x3B3B, 0x3B3C}, make_point(0x3B3F), {0x3B41, 0x3B44}, {0x3B47, 0x3B4F}, {0x3B51, 0x3B55}, {0x3B57, 0x3B64}, {0x3B68, 0x3B72}, make_point(0x3B74), {0x3B77, 0x3B88}, {0x3B8B, 0x3B9F}, {0x3BA1, 0x3BBC}, {0x3BBE, 0x3BD0}, {0x3BD3, 0x3BE6}, {0x3BE9, 0x3BF4}, {0x3BF6, 0x3BFB}, {0x3BFD, 0x3C12}, {0x3C14, 0x3C1B}, {0x3C1D, 0x3C4F}, make_point(0x3C52), {0x3C54, 0x3C5C}, {0x3C5E, 0x3C76}, {0x3C78, 0x3C8F}, {0x3C92, 0x3CA8}, {0x3CAA, 0x3CAD}, {0x3CAF, 0x3CD2}, {0x3CD4, 0x3CE2}, {0x3CE4, 0x3CF0}, {0x3CF4, 0x3D0B}, {0x3D0F, 0x3D1E}, {0x3D25, 0x3D38}, {0x3D3B, 0x3D46}, {0x3D4A, 0x3D59}, {0x3D5D, 0x3D7B}, {0x3D7D, 0x3D81}, {0x3D84, 0x3D8A}, {0x3D8C, 0x3D8F}, {0x3D91, 0x3D98}, {0x3D9A, 0x3D9C}, {0x3D9E, 0x3DA1}, {0x3DA3, 0x3DA8}, {0x3DAB, 0x3DB0}, {0x3DB2, 0x3DB6}, {0x3DB9, 0x3DC0}, {0x3DC2, 0x3DDB}, make_point(0x3DDD), {0x3DDF, 0x3DF0}, {0x3DF3, 0x3DF9}, {0x3DFB, 0x3E01}, {0x3E03, 0x3E13}, {0x3E15, 0x3E68}, {0x3E6A, 0x3E85}, {0x3E87, 0x3E98}, {0x3E9A, 0x3EAE}, {0x3EB0, 0x3EB5}, {0x3EB7, 0x3EBA}, {0x3EBF, 0x3EC5}, make_point(0x3EC7), {0x3EC9, 0x3ED8}, {0x3EDA, 0x3EDB}, {0x3EDD, 0x3EE2}, {0x3EE7, 0x3EFA}, {0x3EFC, 0x3F00}, make_point(0x3F02), make_point(0x3F04), {0x3F06, 0x3F53}, {0x3F55, 0x3F75}, {0x3F77, 0x3FB1}, {0x3FB4, 0x3FC0}, {0x3FC2, 0x3FCF}, {0x3FD1, 0x403F}, {0x4041, 0x4060}, {0x4062, 0x408A}, {0x408C, 0x40C2}, make_point(0x40C5), {0x40C7, 0x40CF}, {0x40D1, 0x40E7}, {0x40E9, 0x40FB}, {0x40FD, 0x4109}, {0x410B, 0x4116}, {0x4118, 0x411D}, {0x411F, 0x4122}, {0x4124, 0x4133}, {0x4136, 0x416A}, {0x416C, 0x4186}, {0x4188, 0x418B}, {0x418D, 0x41FE}, {0x4200, 0x4227}, {0x422A, 0x4246}, {0x4248, 0x42C4}, make_point(0x42C6), {0x42C8, 0x434E}, {0x4350, 0x439D}, {0x439F, 0x43A4}, {0x43A6, 0x4455}, {0x4457, 0x446D}, {0x446F, 0x4476}, {0x4478, 0x447D}, {0x447F, 0x4498}, {0x449A, 0x44AD}, {0x44B0, 0x44C0}, {0x44C2, 0x44FA}, {0x44FC, 0x4525}, make_point(0x4527), make_point(0x452A), {0x452C, 0x4541}, {0x4543, 0x454F}, {0x4551, 0x45AB}, {0x45AD, 0x45EC}, {0x45EE, 0x45F2}, {0x45F4, 0x4626}, {0x4628, 0x4631}, {0x4633, 0x464A}, make_point(0x464C), {0x464E, 0x466A}, {0x466C, 0x4675}, {0x4677, 0x468B}, {0x468D, 0x46A3}, {0x46A5, 0x4701}, {0x4703, 0x4725}, make_point(0x4727), {0x4729, 0x4732}, {0x4734, 0x473E}, {0x4740, 0x4748}, {0x474A, 0x478E}, {0x4790, 0x47B1}, {0x47B3, 0x47CD}, {0x47CF, 0x47F1}, {0x47F4, 0x480B}, {0x480D, 0x4839}, {0x483B, 0x4877}, {0x4879, 0x4885}, {0x4887, 0x48B6}, {0x48B8, 0x48C8}, {0x48CA, 0x48D1}, {0x48D3, 0x48D6}, {0x48D8, 0x48FF}, {0x4901, 0x4965}, {0x4967, 0x4978}, {0x497A, 0x49A8}, {0x49AA, 0x49B7}, {0x49B9, 0x49BE}, {0x49C1, 0x49C7}, {0x49C9, 0x49CE}, {0x49D0, 0x49E8}, {0x49EA, 0x49EC}, {0x49EE, 0x4A02}, {0x4A04, 0x4A4D}, {0x4A4F, 0x4A9E}, {0x4AA0, 0x4AA9}, {0x4AAB, 0x4AE6}, {0x4AE8, 0x4B4E}, {0x4B50, 0x4B5B}, {0x4B5D, 0x4B84}, {0x4B86, 0x4BE8}, {0x4BEA, 0x4C2D}, {0x4C2F, 0x4C32}, {0x4C34, 0x4C68}, {0x4C6B, 0x4C86}, {0x4C89, 0x4CE4}, {0x4CE6, 0x4D27}, {0x4D29, 0x4DAC}, {0x4DAE, 0x4DB5}}},
      // -- (Yijing Hexagram Symbols) U+4DC0 - U+4DFF
      {"YijingHexagramSymbols", {{0x4DC0, 0x4DFF}}},
      // CJKUnifiedIdeographs (#x4E00-#x9FFF): (CJK Unified Ideographs) U+4E00 - U+9FB4, U+9FBC - U+9FC4, U+9FC6 - U+9FD0, U+9FD2 - U+9FD5
      {"CJKUnifiedIdeographs", {{0x4E00, 0x9FB4}, {0x9FBC, 0x9FC4}, {0x9FC6, 0x9FD0}, {0x9FD2, 0x9FD5}}},
      // YiSyllables (#xA000-#xA48F): (Yi Syllables) U+A000 - U+A48C
      {"YiSyllables", {{0xA000, 0xA48C}}},
      // YiRadicals (#xA490-#xA4CF): (Yi Radicals) U+A490 - U+A4C6
      {"YiRadicals", {{0xA490, 0xA4C6}}},
      // -- (Lisu) U+A4D0 - U+A4FF
      {"Lisu", {{0xA4D0, 0xA4FF}}},
      // -- (Vai) U+A500 - U+A62B (U+A63F)
      {"Vai", {{0xA500, 0xA62B}}},
      // -- (Cyrillic Extended-B) U+A640 - U+A69F
      {"CyrillicExtended-B", {{0xA640, 0xA69F}}},
      // -- (Bamum) U+A6A0 - U+A6F7 (U+A6FF)
      {"Bamum", {{0xA6A0, 0xA6F7}}},
      // -- (Modifier Tone Letters) U+A700 - U+A71F
      {"Modifier Tone Letters", {{0xA700, 0xA71F}}},
      // -- (Latin Extended-D) U+A720 - U+A7AD, U+A7B0 - U+A7B7, U+A7F7 - U+A7FF
      {"LatinExtended-D", {{0xA720, 0xA7AD}, {0xA7B0, 0xA7B7}, {0xA7F7, 0xA7FF}}},
      // -- (Syloti Nagri) U+A800 - U+A82B (U+A82F)
      {"SylotiNagri", {{0xA830, 0xA839}}},
      // -- (Common Indic Number Forms) U+A830 - U+A839 (U+A83F)
      {"CommonIndicNumberForms", {{0xA830, 0xA839}}},
      // -- (Phags-pa) U+A840 - U+A877 (U+A87F)
      {"Phags-pa", {{0xA840, 0xA877}}},
      // -- (Saurashtra) U+A880 - U+A8C4, U+A8CE - U+A8D9 (U+A8DF)
      {"Saurashtra", {{0xA880, 0xA8C4}, {0xA8CE, 0xA8D9}}},
      // -- (Devanagari Extended) U+A8E0 - U+A8FD (U+A8FF)
      {"DevanagariExtended", {{0xA8E0, 0xA8FD}}},
      // -- (Kayah Li) U+A900 - U+A92F
      {"KayahLi", {{0xA900, 0xA92F}}},
      // -- (Rejang) U+A930 - U+A953, U+A95F
      {"Rejang", {{0xA930, 0xA953}, make_point(0xA95F)}},
      // -- (Hangul Jamo Extended-A) U+A960 - U+A97C (U+A97F)
      {"HangulJamoExtended-A", {{0xA960, 0xA97C}}},
      // -- (Javanese) U+A980 - U+A9CD, U+A9CF - U+A9D9, U+A9DE - U+A9DF
      {"Javanese", {{0xA980, 0xA9CD}, {0xA9CF, 0xA9D9}, {0xA9DE, 0xA9DF}}},
      // -- (Myanmar Extended-B) U+A9E0 - U+A9FE (U+A9FF)
      {"MyanmarExtended-B", {{0xA9E0, 0xA9FE}}},
      // -- (Cham) U+AA00 - U+AA36, U+AA40 - U+AA4D, U+AA50 - U+AA59, U+AA5C - U+AA5F
      {"Cham", {{0xAA00, 0xAA36}, {0xAA40, 0xAA4D}, {0xAA50, 0xAA59}, {0xAA5C, 0xAA5F}}},
      // -- (Myanmar Extended-A) U+AA60 - U+AA7F
      {"MyanmarExtended-A", {{0xAA60, 0xAA7F}}},
      // -- (Tai Viet) U+AA80 - U+AAC2, U+AADB - U+AADF
      {"TaiViet", {{0xAA80, 0xAAC2}, {0xAADB, 0xAADF}}},
      // -- (Meetei Mayek Extensions) U+AAE0 - U+AAF6 (U+AAFF)
      {"MeeteiMayekExtensions", {{0xAAE0, 0xAAF6}}},
      // -- (Ethiopic Extended-A) (U+AB00) U+AB01 - U+AB06, U+AB09 - U+AB0E, U+AB11 - U+AB16, U+AB20 - U+AB26, U+AB28 - U+AB2E (U+AB2F)
      {"EthiopicExtended-A", {{0xAB01, 0xAB06}, {0xAB09, 0xAB0E}, {0xAB11, 0xAB16}, {0xAB20, 0xAB26}, {0xAB28, 0xAB2E}}},
      // -- (Latin Extended-E) U+AB30 - U+AB65 (U+AB6F)
      {"LatinExtended-E", {{0xAB30, 0xAB65}}},
      // -- (Cherokee Supplement) U+AB70 - U+ABBF
      {"CherokeeSupplement", {{0xAB70, 0xABBF}}},
      // -- (Meetei Mayek) U+ABC0 - U+ABED, U+ABF0 - U+ABF9 (U+ABFF)
      {"MeeteiMayek", {{0xABC0, 0xABED}, {0xABF0, 0xABF9}}},
      // HangulSyllables (#xAC00-#xD7A3): (Hangul Syllables) U+AC00, U+D7A3 (U+D7AF)
      {"HangulSyllables", {make_point(0xAC00), make_point(0xD7A3)}},
      // -- (Hangul Jamo Extended-B) U+D7B0 - U+D7C6, U+D7CB - U+D7FB (U+D7FF)
      {"HangulJamoExtended-B", {{0xD7B0, 0xD7C6}, {0xD7CB, 0xD7FB}}},
      // HighSurrogates (#xD800-#xDB7F): (High Surrogates) U+D800 - U+DB7F
      {"HighSurrogates", {{0xD800, 0xDB7F}}},
      // HighPrivateUseSurrogates (#xDB80-#xDBFF): (High Private Use Surrogates) U+DB80 - U+DBFF
      {"HighPrivateUseSurrogates", {{0xDB80, 0xDBFF}}},
      // LowSurrogates (#xDC00-#xDFFF): (Low Surrogates) U+DC00 - U+DFFF
      {"LowSurrogates", {{0xDC00, 0xDFFF}}},
      // PrivateUse (#xE000-#xF8FF): (Private Use Area) U+E000 - U+F8FF
      {"PrivateUse", {{0xE000, 0xF8FF}}},
      // CJKCompatibilityIdeographs (#xF900-#xFAFF): U+F900 - U+FA6D, U+FA70 - U+FAD9
      {"CJKCompatibilityIdeographs", {{0xF900, 0xFA6D}, {0xFA70, 0xFAD9}}},
      // AlphabeticPresentationForms (#xFB00-#xFB4F): U+FB00 - U+FB06, U+FB13 - U+FB17, U+FB1D - U+FB36, U+FB38 - U+FB3C, U+FB3E, U+FB40 - U+FB41, U+FB43 - U+FB44, U+FB46 - U+FB4F
      {"AlphabeticPresentationForms", {{0xFB00, 0xFB06}, {0xFB13, 0xFB17}, {0xFB1D, 0xFB36}, {0xFB38, 0xFB3C}, make_point(0xFB3E), {0xFB40, 0xFB41}, {0xFB43, 0xFB44}, {0xFB46, 0xFB4F}}},
      // ArabicPresentationForms-A (#xFB50-#xFDFF): U+FB50 - U+FBC1, U+FBD3 - U+FD3F, U+FD50 - U+FD8F, U+FD92 - U+FDC7, U+FDD0 - U+FDFD
      {"ArabicPresentationForms-A", {{0xFB50, 0xFBC1}, {0xFBD3, 0xFD3F}, {0xFD50, 0xFD8F}, {0xFD92, 0xFDC7}, {0xFDD0, 0xFDFD}}},
      // -- (Variation Selectors) U+FE00 - U+FE0F
      {"VariationSelectors", {{0xFE00, 0xFE0F}}},
      // -- (Vertical Forms) U+FE10 - U+FE19 (U+FE1F)
      {"VerticalForms", {{0xFE10, 0xFE19}}},
      // CombiningHalfMarks (#xFE20-#xFE2F): (Combining Half Marks) U+FE20 - U+FE2F
      {"CombiningHalfMarks", {{0xFE20, 0xFE2F}}},
      // CJKCompatibilityForms (#xFE30-#xFE4F): (CJK Compatibility Forms) U+FE30 - U+FE4F
      {"CJKCompatibilityForms", {{0xFE30, 0xFE4F}}},
      // SmallFormVariants (#xFE50-#xFE6F): (Small Form Variants) U+FE50 - U+FE52, U+FE54 - U+FE66, U+FE68 - U+FE6B
      {"SmallFormVariants", {{0xFE50, 0xFE52}, {0xFE54, 0xFE66}, {0xFE68, 0xFE6B}}},
      // ArabicPresentationForms-B (#xFE70-#xFEFE): (Arabic Presentation Forms-B) U+FE70 - U+FE74, U+FE76 - U+FEFC, U+FEFF
      {"ArabicPresentationForms-B", {{0xFE70, 0xFE74}, {0xFE76, 0xFEFC}, make_point(0xFEFF)}},
      // Specials (#xFEFF-#xFEFF): <- TODO wtf see 2 lines below... Specials once again...
      // HalfwidthandFullwidthForms (#xFF00-#xFFEF): U+FF01 - U+FFBE, U+FFC2 - U+FFC7, U+FFCA - U+FFCF, U+FFD2 - U+FFD7, U+FFDA - U+FFDC, U+FFE0 - U+FFE6, U+FFE8 - U+FFEE
      {"HalfwidthandFullwidthForms", {{0xFF01, 0xFFBE}, {0xFFC2, 0xFFC7}, {0xFFCA, 0xFFCF}, {0xFFD2, 0xFFD7}, {0xFFDA, 0xFFDC}, {0xFFE0, 0xFFE6}, {0xFFE8, 0xFFEE}}},
      // Specials (#xFFF0-#xFFFD): U+FFF9 - U+FFFF
      {"Specials", {{0xFFF9, 0xFFFF}}},
      // -- (Linear B Syllabary) U+10000 - U+1000B, U+1000D - U+10026, U+10028 - U+1003A, U+1003C - U+1003D, U+1003F - U+1004D, U+10050 - U+1005D (U+1007F)
      {"LinearBSyllabary", {{0x10000, 0x1000B}, {0x1000D, 0x10026}, {0x10028, 0x1003A}, {0x1003C, 0x1003D}, {0x1003F, 0x1004D}, {0x10050, 0x1005D}}},
      // -- (Linear B Ideograms) U+10080 - U+100FA (U+100FF)
      {"LinearBIdeograms", {{0x10080, 0x100FA}}},
      // -- (Aegean Numbers) U+10100 - U+10102, U+10107 - U+10133, U+10137 - U+1013F
      {"AegeanNumbers", {{0x10100, 0x10102}, {0x10107, 0x10133}, {0x10137, 0x1013F}}},
      // -- (Ancient Greek Numbers) U+10140 - U+1018C (U+1018F)
      {"AncientGreekNumbers", {{0x10140, 0x1018C}}},
      // -- (Ancient Symbols) U+10190 - U+1019B, U+101A0 (U+101CF)
      {"AncientSymbols", {{0x10190, 0x1019B}, make_point(0x101A0)}},
      // -- (Phaistos Disc) U+101D0 - U+101FD (U+101FF)
      {"PhaistosDisc", {{0x101D0, 0x101FD}}},
      // -- (Lycian) U+10280 - U+1029C (U+1029F)
      {"Lycian", {{0x10280, 0x1029C}}},
      // -- (Carian) U+102A0 - U+102D0 (U+102DF)
      {"Carian", {{0x102A0, 0x102D0}}},
      // -- (Coptic Epact Numbers) U+102E0 - U+102FB (U+102FF)
      {"CopticEpactNumbers", {{0x102E0, 0x102FB}}},
      // OldItalic (#x10300-#x1032F): U+10300 - U+10323
      {"OldItalic", {{0x10300, 0x10323}}},
      // Gothic (#x10330-#x1034F): U+10330 - U+1034A
      {"Gothic", {{0x10330, 0x1034A}}},
      // -- (Old Permic) U+10350 - U+1037A (U+1037F)
      {"OldPermic", {{0x10350, 0x1037A}}},
      // -- (Ugaritic) U+10380 - U+1039D, U+1039F
      {"Ugaritic", {{0x10380, 0x1039D}, make_point(0x1039F)}},
      // -- (Old Persian) U+103A0 - U+103C3, U+103C8 - U+103D5 (U+103DF)
      {"OldPersian", {{0x103A0, 0x103C3}, {0x103C8, 0x103D5}}},
      // Deseret (#x10400-#x1044F): U+10400 - U+1044F
      {"Deseret", {{0x10400, 0x1044F}}},
      // -- (Shavian) U+10450 - U+1047F
      {"Shavian", {{0x10450, 0x1047F}}},
      // -- (Osmanya) U+10480 - U+1049D, U+104A0 - U+104A9 (U+104AF)
      {"Osmanya", {{0x10480, 0x1049D}, {0x104A0, 0x104A9}}},
      // -- (Elbasan) U+10500 - U+10527 (U+1052F)
      {"Elbasan", {{0x10500, 0x10527}}},
      // -- (Caucasian Albanian) U+10530 - U+10563, U+1056F (U+1056F)
      {"CaucasianAlbanian", {{0x10530, 0x10563}, make_point(0x1056F)}},
      // -- (Linear A) U+10600 - U+10736, U+10740 - U+10755, U+10760 - U+10767 (U+1077F)
      {"LinearA", {{0x10600, 0x10736}, {0x10740, 0x10755}, {0x10760, 0x10767}}},
      // -- (Cypriot Syllabary) U+10800 - U+10805, U+10808, U+1080A - U+10835, U+10837 - U+10838, U+1083C, U+1083F
      {"CypriotSyllabary", {{0x10800, 0x10805}, make_point(0x10808), {0x1080A, 0x10835}, {0x10837, 0x10838}, make_point(0x1083C), make_point(0x1083F)}},
      // -- (Imperial Aramaic) U+10840 - U+10855, U+10857 - U+1085F
      {"ImperialAramaic", {{0x10840, 0x10855}, {0x10857, 0x1085F}}},
      // -- (Palmyrene) U+10860 - U+1087F
      {"Palmyrene", {{0x10860, 0x1087F}}},
      // -- (Nabataean) U+10880 - U+1089E, U+108A7 - U+108AF
      {"Nabataean", {{0x10880, 0x1089E}, {0x108A7, 0x108AF}}},
      // -- (Hatran) U+108E0 - U+108F2, U+108F4 - U+108F5, U+108FB - U+108FF
      {"Hatran", {{0x108E0, 0x108F2}, {0x108F4, 0x108F5}, {0x108FB, 0x108FF}}},
      // -- (Phoenician) U+10900 - U+1091B, U+1091F
      {"Phoenician", {{0x10900, 0x1091B}, make_point(0x1091F)}},
      // -- (Lydian) U+10920 - U+10939, U+1093F
      {"Lydian", {{0x10920, 0x10939}, make_point(0x1093F)}},
      // -- (Meroitic Hieroglyphs) U+10980 - U+1099F
      {"MeroiticHieroglyphs", {{0x10980, 0x1099F}}},
      // -- (Meroitic Cursive) U+109A0 - U+109B7, U+109BC - U+109CF, U+109D2 - U+109FF
      {"MeroiticCursive", {{0x109A0, 0x109B7}, {0x109BC, 0x109CF}, {0x109D2, 0x109FF}}},
      // -- (Kharoshthi) U+10A00 - U+10A03, U+10A05 - U+10A06, U+10A0C - U+10A13, U+10A15 - U+10A17, U+10A19 - U+10A33, U+10A38 - U+10A3A, U+10A3F - U+10A47, U+10A50 - U+10A58 (U+10A5F)
      {"Kharoshthi", {{0x10A00, 0x10A03}, {0x10A05, 0x10A06}, {0x10A0C, 0x10A13}, {0x10A15, 0x10A17}, {0x10A19, 0x10A33}, {0x10A38, 0x10A3A}, {0x10A3F, 0x10A47}, {0x10A50, 0x10A58}}},
      // -- (Old South Arabian) U+10A60 - U+10A7F
      {"OldSouthArabian", {{0x10A60, 0x10A7F}}},
      // -- (Old North Arabian) U+10A80 - U+10A9F
      {"OldNorthArabian", {{0x10A80, 0x10A9F}}},
      // -- (Manichaean) U+10AC0 - U+10AE6, U+10AEB - U+10AF6 (U+10AFF)
      {"Manichaean", {{0x10AC0, 0x10AE6}, {0x10AEB, 0x10AF6}}},
      // -- (Avestan) U+10B00 - U+10B35, U+10B39 - U+10B3F
      {"Avestan", {{0x10B00, 0x10B35}, {0x10B39, 0x10B3F}}},
      // -- (Inscriptional Parthian) U+10B40 - U+10B55, U+10B58 - U+10B5F
      {"InscriptionalParthian", {{0x10B40, 0x10B55}, {0x10B58, 0x10B5F}}},
      // -- (Inscriptional Pahlavi) U+10B60 - U+10B72, U+10B78 - U+10B7F
      {"InscriptionalPahlavi", {{0x10B60, 0x10B72}, {0x10B78, 0x10B7F}}},
      // -- (Psalter Pahlavi) U+10B80 - U+10B91, U+10B99 - U+10B9C, U+10BA9 - U+10BAF
      {"PsalterPahlavi", {{0x10B80, 0x10B91}, {0x10B99, 0x10B9C}, {0x10BA9, 0x10BAF}}},
      // -- (Old Turkic) U+10C00 - U+10C48 (U+10C4F)
      {"OldTurkic", {{0x10C00, 0x10C48}}},
      // -- (Old Hungarian) U+10C80 - U+10CB2, U+10CC0 - U+10CF2, U+10CFA - U+10CFF
      {"OldHungarian", {{0x10C80, 0x10CB2}, {0x10CC0, 0x10CF2}, {0x10CFA, 0x10CFF}}},
      // -- (Rumi Numeral Symbols) U+10E60 - U+10E7E (U+10E7F)
      {"RumiNumeralSymbols", {{0x10E60, 0x10E7E}}},
      // -- (Brahmi) U+11000 - U+1104D, U+11052 - U+1106F, U+1107F
      {"Brahmi", {{0x11000, 0x1104D}, {0x11052, 0x1106F}, make_point(0x1107F)}},
      // -- (Kaithi) U+11080 - U+110C1 (U+110CF)
      {"Kaithi", {{0x11080, 0x110C1}}},
      // -- (Sora Sompeng) U+110D0 - U+110E8, U+110F0 - U+110F9 (U+110FF)
      {"SoraSompeng", {{0x110D0, 0x110E8}, {0x110F0, 0x110F9}}},
      // -- (Chakma) U+11100 - U+11134, U+11136 - U+11143 (U+1114F)
      {"Chakma", {{0x11100, 0x11134}, {0x11136, 0x11143}}},
      // -- (Mahajani) U+11150 - U+11176 (U+1117F)
      {"Mahajani", {{0x11150, 0x11176}}},
      // -- (Sharada) U+11180 - U+111CD, U+111D0 - U+111DF
      {"Sharada", {{0x11180, 0x111CD}, {0x111D0, 0x111DF}}},
      // -- (Sinhala Archaic Numbers) (U+111E0) U+111E1 - U+111F4 (U+111FF)
      {"SinhalaArchaicNumbers", {{0x111E1, 0x111F4}}},
      // -- (Khojki) U+11200 - U+11211, U+11213 - U+1123D (U+1124F)
      {"Khojki", {{0x11200, 0x11211}, {0x11213, 0x1123D}}},
      // -- (Multani) U+11280 - U+11286, U+11288, U+1128A - U+1128D, U+1128F - U+1129D, U+1129F - U+112A9 (U+112AF)
      {"Multani", {{0x11280, 0x11286}, make_point(0x11288), {0x1128A, 0x1128D}, {0x1128F, 0x1129D}, {0x1129F, 0x112A9}}},
      // -- (Khudawadi) U+112B0 - U+112EA, U+112F0 - U+112F9 (U+112FF)
      {"Khudawadi", {{0x112B0, 0x112EA}, {0x112F0, 0x112F9}}},
      // -- (Grantha) U+11300 - U+11303, U+11305 - U+1130C, U+1130F - U+11310, U+11313 - U+11328, U+1132A - U+11330, U+11332 - U+11333, U+11335 - U+11339, U+1133C - U+11344, U+11347 - U+11348, U+1134B - U+1134D, U+11350, U+11357, U+1135D - U+11363, U+11366 - U+1136C, U+11370 - U+11374 (U+1137F)
      {"Grantha", {{0x11300, 0x11303}, {0x11305, 0x1130C}, {0x1130F, 0x11310}, {0x11313, 0x11328}, {0x1132A, 0x11330}, {0x11332, 0x11333}, {0x11335, 0x11339}, {0x1133C, 0x11344}, {0x11347, 0x11348}, {0x1134B, 0x1134D}, make_point(0x11350), make_point(0x11357), {0x1135D, 0x11363}, {0x11366, 0x1136C}, {0x11370, 0x11374}}},
      // -- (Tirhuta) U+11480 - U+114C7, U+114D0 - U+114D9 (U+114DF)
      {"Tirhuta", {{0x11480, 0x114C7}, {0x114D0, 0x114D9}}},
      // -- (Siddham) U+11580 - U+115B5, U+115B8 - U+115DD (U+115FF)
      {"Siddham", {{0x11580, 0x115B5}, {0x115B8, 0x115DD}}},
      // -- (Modi) U+11600 - U+11644, U+11650 - U+11659 (U+1165F)
      {"Modi", {{0x11600, 0x11644}, {0x11650, 0x11659}}},
      // -- (Takri) U+11680 - U+116B7, U+116C0 - U+116C9 (U+116CF)
      {"Takri", {{0x11680, 0x116B7}, {0x116C0, 0x116C9}}},
      // -- (Ahom) U+11700 - U+11719, U+1171D - U+1172B, U+11730 - U+1173F
      {"Ahom", {{0x11700, 0x11719}, {0x1171D, 0x1172B}, {0x11730, 0x1173F}}},
      // -- (Warang Citi) U+118A0 - U+118F2, U+118FF
      {"WarangCiti", {{0x118A0, 0x118F2}, make_point(0x118FF)}},
      // -- (Pau Cin Hau) U+11AC0 - U+11AF8 (U+11AFF)
      {"PauCinHau", {{0x11AC0, 0x11AF8}}},
      // -- (Cuneiform) U+12000 - U+12399 (U+123FF)
      {"Cuneiform", {{0x12000, 0x12399}}},
      // -- (Cuneiform Numbers and Punctuation) U+12400 - U+1246E, U+12470 - U+12474 (U+1247F)
      {"CuneiformNumbersAndPunctuation", {{0x12400, 0x1246E}, {0x12470, 0x12474}}}, // TODO check if upper cased and is OK
      // -- (Early Dynastic Cuneiform) U+12480 - U+12543 (U+1254F)
      {"Early Dynastic Cuneiform", {{0x12480, 0x12543}}},
      // -- (Egyptian Hieroglyphs) U+13000 - U+1342E (U+1342F)
      {"EgyptianHieroglyphs", {{0x13000, 0x1342E}}},
      // -- (Anatolian Hieroglyphs) U+14400 - U+14646 (U+1467F)
      {"AnatolianHieroglyphs", {{0x14400, 0x14646}}},
      // -- (Bamum Supplement) U+16800 - U+16A38 (U+16A3F)
      {"BamumSupplement", {{0x16800, 0x16A38}}},
      // -- (Mro) U+16A40 - U+16A5E, U+16A60 - U+16A69, U+16A6E - U+16A6F
      {"Mro", {{0x16A40, 0x16A5E}, {0x16A60, 0x16A69}, {0x16A6E, 0x16A6F}}},
      // -- (Bassa Vah) U+16AD0 - U+16AED, U+16AF0 - U+16AF5 (U+16AFF)
      {"BassaVah", {{0x16AD0, 0x16AED}, {0x16AF0, 0x16AF5}}},
      // -- (Pahawh Hmong) U+16B00 - U+16B45, U+16B50 - U+16B59, U+16B5B - U+16B61, U+16B63 - U+16B77, U+16B7D - U+16B8F
      {"PahawhHmong", {{0x16B00, 0x16B45}, {0x16B50, 0x16B59}, {0x16B5B, 0x16B61}, {0x16B63, 0x16B77}, {0x16B7D, 0x16B8F}}},
      // -- (Miao) U+16F00 - U+16F44, U+16F50 - U+16F7E, U+16F8F - U+16F9F
      {"Miao", {{0x16F00, 0x16F44}, {0x16F50, 0x16F7E}, {0x16F8F, 0x16F9F}}},
      // -- (Kana Supplement) U+1B000 - U+1B001 (U+1B0FF)
      {"KanaSupplement", {{0x1B000, 0x1B001}}},
      // -- (Duployan) U+1BC00 - U+1BC6A, U+1BC70 - U+1BC7C, U+1BC80 - U+1BC88, U+1BC90 - U+1BC99, U+1BC9C - U+1BC9F
      {"Duployan", {{0x1BC00, 0x1BC6A}, {0x1BC70, 0x1BC7C}, {0x1BC80, 0x1BC88}, {0x1BC90, 0x1BC99}, {0x1BC9C, 0x1BC9F}}},
      // -- (Shorthand Format Controls) U+1BCA0 - U+1BCA3 (U+1BCAF)
      {"ShorthandFormatControls", {{0x1BCA0, 0x1BCA3}}},
      // ByzantineMusicalSymbols (#x1D000-#x1D0FF): (Byzantine Musical Symbols) U+1D000 - U+1D0F5
      {"ByzantineMusicalSymbols", {{0x1D000, 0x1D0F5}}},
      // MusicalSymbols (#x1D100-#x1D1FF): (Musical Symbols) U+1D100 - U+1D126, U+1D129 - U+1D1E8
      {"MusicalSymbols", {{0x1D100, 0x1D126}, {0x1D129, 0x1D1E8}}},
      // -- (Ancient Greek Musical Notation) U+1D200 - U+1D245 (U+1D24F)
      {"AncientGreekMusicalNotation", {{0x1D200, 0x1D245}}},
      // -- (Tai Xuan Jing Symbols) U+1D300 - U+1D356 (U+1D35F)
      {"TaiXuanJingSymbols", {{0x1D300, 0x1D356}}},
      // -- (Counting Rod Numerals) U+1D360 - U+1D371 (U+1D37F)
      {"CountingRodNumerals", {{0x1D360, 0x1D371}}},
      // MathematicalAlphanumericSymbols (#x1D400-#x1D7FF): (Mathematical Alphanumeric Symbols) U+1D400 - U+1D454, U+1D456 - U+1D49C, U+1D49E - U+1D49F, U+1D4A2, U+1D4A5 - U+1D4A6, U+1D4A9 - U+1D4AC, U+1D4AE - U+1D4B9, U+1D4BB, U+1D4BD - U+1D4C3, U+1D4C5 - U+1D505, U+1D507 - U+1D50A, U+1D50D - U+1D514, U+1D516 - U+1D51C, U+1D51E - U+1D539, U+1D53B - U+1D53E, U+1D540 - U+1D544, U+1D546, U+1D54A - U+1D550, U+1D552 - U+1D6A5, U+1D6A8 - U+1D7CB, U+1D7CE - U+1D7FF
      {"MathematicalAlphanumericSymbols", {{0x1D400, 0x1D454}, {0x1D456, 0x1D49C}, {0x1D49E, 0x1D49F}, make_point(0x1D4A2), {0x1D4A5, 0x1D4A6}, {0x1D4A9, 0x1D4AC}, {0x1D4AE, 0x1D4B9}, make_point(0x1D4BB), {0x1D4BD, 0x1D4C3}, {0x1D4C5, 0x1D505}, {0x1D507, 0x1D50A}, {0x1D50D, 0x1D514}, {0x1D516, 0x1D51C}, {0x1D51E, 0x1D539}, {0x1D53B, 0x1D53E}, {0x1D540, 0x1D544}, make_point(0x1D546), {0x1D54A, 0x1D550}, {0x1D552, 0x1D6A5}, {0x1D6A8, 0x1D7CB}, {0x1D7CE, 0x1D7FF}}},
      // -- (Sutton SignWriting) U+1D800 - U+1DA8B, U+1DA9B - U+1DA9F, U+1DAA1 - U+1DAAF
      {"SuttonSignWriting", {{0x1D800, 0x1DA8B}, {0x1DA9B, 0x1DA9F}, {0x1DAA1, 0x1DAAF}}},
      // -- (Mende Kikakui) U+1E800 - U+1E8C4, U+1E8C7 - U+1E8D6 (U+1E8DF)
      {"MendeKikakui", {{0x1E800, 0x1E8C4}, {0x1E8C7, 0x1E8D6}}},
      // -- (Arabic Mathematical Alphabetic Symbols) U+1EE00 - U+1EE03, U+1EE05 - U+1EE1F, U+1EE21 - U+1EE22, U+1EE24, U+1EE27, U+1EE29 - U+1EE32, U+1EE34 - U+1EE37, U+1EE39, U+1EE3B, U+1EE42, U+1EE47, U+1EE49, U+1EE4B, U+1EE4D - U+1EE4F, U+1EE51 - U+1EE52, U+1EE54, U+1EE57, U+1EE59, U+1EE5B, U+1EE5D, U+1EE5F, U+1EE61 - U+1EE62, U+1EE64, U+1EE67 - U+1EE6A, U+1EE6C - U+1EE72, U+1EE74 - U+1EE77, U+1EE79 - U+1EE7C, U+1EE7E, U+1EE80 - U+1EE89, U+1EE8B - U+1EE9B, U+1EEA1 - U+1EEA3, U+1EEA5 - U+1EEA9, U+1EEAB - U+1EEBB, U+1EEF0 - U+1EEF1 (U+1EEFF)
      {"ArabicMathematicalAlphabeticSymbols", {{0x1EE00, 0x1EE03}, {0x1EE05, 0x1EE1F}, {0x1EE21, 0x1EE22}, make_point(0x1EE24), make_point(0x1EE27), {0x1EE29, 0x1EE32}, {0x1EE34, 0x1EE37}, make_point(0x1EE39), make_point(0x1EE3B), make_point(0x1EE42), make_point(0x1EE47), make_point(0x1EE49), make_point(0x1EE4B), {0x1EE4D, 0x1EE4F}, {0x1EE51, 0x1EE52}, make_point(0x1EE54), make_point(0x1EE57), make_point(0x1EE59), make_point(0x1EE5B), make_point(0x1EE5D), make_point(0x1EE5F), {0x1EE61, 0x1EE62}, make_point(0x1EE64), {0x1EE67, 0x1EE6A}, {0x1EE6C, 0x1EE72}, {0x1EE74, 0x1EE77}, {0x1EE79, 0x1EE7C}, make_point(0x1EE7E), {0x1EE80, 0x1EE89}, {0x1EE8B, 0x1EE9B}, {0x1EEA1, 0x1EEA3}, {0x1EEA5, 0x1EEA9}, {0x1EEAB, 0x1EEBB}, {0x1EEF0, 0x1EEF1}}},
      // -- (Mahjong Tiles) U+1F000 - U+1F02B (U+1F02F)
      {"MahjongTiles", {{0x1F000, 0x1F02B}}},
      // -- (Domino Tiles) U+1F030 - U+1F093 (U+1F09F)
      {"DominoTiles", {{0x1F030, 0x1F093}}},
      // -- (Playing Cards) U+1F0A0 - U+1F0AE, U+1F0B1 - U+1F0BF, U+1F0C1 - U+1F0CF, U+1F0D1 - U+1F0F5 (U+1F0FF)
      {"PlayingCards", {{0x1F0A0, 0x1F0AE}, {0x1F0B1, 0x1F0BF}, {0x1F0C1, 0x1F0CF}, {0x1F0D1, 0x1F0F5}}},
      // -- (Enclosed Alphanumeric Supplement) U+1F100 - U+1F10C, U+1F110 - U+1F12E, U+1F130 - U+1F16B, U+1F170 - U+1F19A, U+1F1E6 - U+1F1FF
      {"EnclosedAlphanumericSupplement", {{0x1F100, 0x1F10C}, {0x1F110, 0x1F12E}, {0x1F130, 0x1F16B}, {0x1F170, 0x1F19A}, {0x1F1E6, 0x1F1FF}}},
      // -- (Enclosed Ideographic Supplement) U+1F200 - U+1F202, U+1F210 - U+1F23A, U+1F240 - U+1F248, U+1F250 - U+1F251 (U+1F2FF)
      {"EnclosedIdeographicSupplement", {{0x1F200, 0x1F202}, {0x1F210, 0x1F23A}, {0x1F240, 0x1F248}, {0x1F250, 0x1F251}}},
      // -- (Miscellaneous Symbols and Pictographs) U+1F300 - U+1F579, U+1F57B - U+1F5A3, U+1F5A5 - U+1F5FF
      {"MiscellaneousSymbolsandPictographs", {{0x1F300, 0x1F579}, {0x1F57B, 0x1F5A3}, {0x1F5A5, 0x1F5FF}}},
      // -- (Emoticons) U+1F600 - U+1F64F
      {"Emoticons", {{0x1F600, 0x1F64F}}},
      // -- (Ornamental Dingbats) U+1F650 - U+1F67F
      {"OrnamentalDingbats", {{0x1F650, 0x1F67F}}},
      // -- (Transport and Map Symbols) U+1F680 - U+1F6D0, U+1F6E0 - U+1F6EC, U+1F6F0 - U+1F6F3 (U+1F6FF)
      {"TransportAndMapSymbols", {{0x1F680, 0x1F6D0}, {0x1F6E0, 0x1F6EC}, {0x1F6F0, 0x1F6F3}}}, // TODO check that upper cased and is OK
      // -- (Alchemical Symbols) U+1F700 - U+1F773 (U+1F77F)
      {"AlchemicalSymbols", {{0x1F700, 0x1F773}}},
      // -- (Geometric Shapes Extended) U+1F780 - U+1F7D4 (U+1F7FF)
      {"GeometricShapesExtended", {{0x1F780, 0x1F7D4}}},
      // -- (Supplemental Arrows-C) U+1F800 - U+1F80B, U+1F810 - U+1F847, U+1F850 - U+1F859, U+1F860 - U+1F887, U+1F890 - U+1F8AD (U+1F8FF)
      {"SupplementalArrows-C)", {{0x1F800, 0x1F80B}, {0x1F810, 0x1F847}, {0x1F850, 0x1F859}, {0x1F860, 0x1F887}, {0x1F890, 0x1F8AD}}},
      // -- (Supplemental Symbols and Pictographs) U+1F910 - U+1F918, U+1F980 - U+1F984, U+1F9C0 (U+1F9FF)
      {"SupplementalSymbolsAndPictographs", {{0x1F910, 0x1F918}, {0x1F980, 0x1F984}, make_point(0x1F9C0)}}, // TODO check if upper cased and is OK
      // CJKUnifiedIdeographsExtensionB (#x20000-#x2A6D6): (CJK Unified Ideographs Extension B) U+20000 - U+20861, U+20863 - U+2230A, U+2230C - U+22C80, U+22C82 - U+23562, U+23564 - U+2420A, U+2420E - U+243B8, U+243BA - U+24B56, U+24B58 - U+253C2, U+253C4 - U+26224, U+26226 - U+2622D, U+2622F - U+26232, U+26234 - U+2623D, U+26241 - U+26242, U+26246 - U+26249, U+2624E - U+26251, U+26255, U+26258 - U+2625A, U+2625D - U+26263, U+26265, U+26267, U+26269 - U+2780B, U+2780D - U+2883F, U+28841 - U+2A6D6
      {"CJKUnifiedIdeographsExtensionB", {{0x20000, 0x20861}, {0x20863, 0x2230A}, {0x2230C, 0x22C80}, {0x22C82, 0x23562}, {0x23564, 0x2420A}, {0x2420E, 0x243B8}, {0x243BA, 0x24B56}, {0x24B58, 0x253C2}, {0x253C4, 0x26224}, {0x26226, 0x2622D}, {0x2622F, 0x26232}, {0x26234, 0x2623D}, {0x26241, 0x26242}, {0x26246, 0x26249}, {0x2624E, 0x26251}, make_point(0x26255), {0x26258, 0x2625A}, {0x2625D, 0x26263}, make_point(0x26265), make_point(0x26267), {0x26269, 0x2780B}, {0x2780D, 0x2883F}, {0x28841, 0x2A6D6}}},
      // -- (CJK Unified Ideographs Extension C) U+2A700, U+2A79D, U+2A848, U+2A84F, U+2A8AE, U+2A9E6, U+2AA0A, U+2AA17, U+2AA9D, U+2ADFF, U+2AEB9, U+2AED0, U+2AFA2, U+2B061, U+2B088, U+2B099, U+2B0DC, U+2B128, U+2B138, U+2B230, U+2B2D0, U+2B300, U+2B328, U+2B359, U+2B35F, U+2B362, U+2B370, U+2B372, U+2B3CB, U+2B404, U+2B406, U+2B409, U+2B410, U+2B413, U+2B4B6, U+2B4E7, U+2B4E9, U+2B50E, U+2B5B8, U+2B5E0, U+2B5E6 - U+2B5E7, U+2B5EE, U+2B5F4, U+2B61D, U+2B623 - U+2B624, U+2B628, U+2B688 - U+2B689, U+2B692, U+2B694 - U+2B695, U+2B699, U+2B6DB, U+2B6DE, U+2B6E2, U+2B6F6, U+2B6F8, U+2B734 (U+2B73F)
      {"CJKUnifiedIdeographsExtensionC", {make_point(0x2A700), make_point(0x2A79D), make_point(0x2A848), make_point(0x2A84F), make_point(0x2A8AE), make_point(0x2A9E6), make_point(0x2AA0A), make_point(0x2AA17), make_point(0x2AA9D), make_point(0x2ADFF), make_point(0x2AEB9), make_point(0x2AED0), make_point(0x2AFA2), make_point(0x2B061), make_point(0x2B088), make_point(0x2B099), make_point(0x2B0DC), make_point(0x2B128), make_point(0x2B138), make_point(0x2B230), make_point(0x2B2D0), make_point(0x2B300), make_point(0x2B328), make_point(0x2B359), make_point(0x2B35F), make_point(0x2B362), make_point(0x2B370), make_point(0x2B372), make_point(0x2B3CB), make_point(0x2B404), make_point(0x2B406), make_point(0x2B409), make_point(0x2B410), make_point(0x2B413), make_point(0x2B4B6), make_point(0x2B4E7), make_point(0x2B4E9), make_point(0x2B50E), make_point(0x2B5B8), make_point(0x2B5E0), {0x2B5E6, 0x2B5E7}, make_point(0x2B5EE), make_point(0x2B5F4), make_point(0x2B61D), {0x2B623, 0x2B624}, make_point(0x2B628), {0x2B688, 0x2B689}, make_point(0x2B692), {0x2B694, 0x2B695}, make_point(0x2B699), make_point(0x2B6DB), make_point(0x2B6DE), make_point(0x2B6E2), make_point(0x2B6F6), make_point(0x2B6F8), make_point(0x2B734)}},
      // -- (CJK Unified Ideographs Extension D) U+2B740, U+2B746, U+2B751, U+2B753, U+2B75A, U+2B75C, U+2B765, U+2B776 - U+2B777, U+2B77C, U+2B782, U+2B789, U+2B78B, U+2B78E, U+2B794, U+2B7AC, U+2B7AF, U+2B7BD, U+2B7C9, U+2B7CF, U+2B7D2, U+2B7D8, U+2B7F0, U+2B80D, U+2B817, U+2B81A, U+2B81D (U+2B81F)
      {"CJKUnifiedIdeographsExtensionD", {make_point(0x2B740), make_point(0x2B746), make_point(0x2B751), make_point(0x2B753), make_point(0x2B75A), make_point(0x2B75C), make_point(0x2B765), {0x2B776, 0x2B777}, make_point(0x2B77C), make_point(0x2B782), make_point(0x2B789), make_point(0x2B78B), make_point(0x2B78E), make_point(0x2B794), make_point(0x2B7AC), make_point(0x2B7AF), make_point(0x2B7BD), make_point(0x2B7C9), make_point(0x2B7CF), make_point(0x2B7D2), make_point(0x2B7D8), make_point(0x2B7F0), make_point(0x2B80D), make_point(0x2B817), make_point(0x2B81A), make_point(0x2B81D)}},
      // -- (CJK Unified Ideographs Extension E) U+2B820 - U+2CEA1 (U+2CEAF)
      {"CJKUnifiedIdeographsExtensionE", {{0x2B820, 0x2CEA1}}},
      // CJKCompatibilityIdeographsSupplement (#x2F800-#x2FA1F): (CJK Compatibility Ideographs Supplement) U+2F800 - U+2FA1D
      {"CJKCompatibilityIdeographsSupplement", {{0x2F800, 0x2FA1D}}},
      // Tags (#xE0000-#xE007F): (Tags) U+E0001, U+E0020 - U+E007F
      {"Tags", {make_point(0xE0001), {0xE0020, 0xE007F}}},
      // -- (Variation Selectors Supplement) U+E0100 - U+E01EF
      {"VariationSelectorsSupplement", {{0xE0100, 0xE01EF}}},
      // PrivateUse (#xF0000-#x10FFFD): (Supplementary Private Use Area-A) U+F0000, U+FFFFD - U+FFFFF (TODO: I think that FFFFD is the last one and FFFFF does not belong) (Supplementary Private Use Area-B) U+100000, U+10FFFD, U+10FFFF
      {"PrivateUse", {make_point(0xF0000), {0xFFFFD, 0xFFFFF}, make_point(0x100000), {0x10FFFD, 0x10FFFF}}}
    };
} // namespace

char32_t unicode::blocks::get_random_char()
{
  return 0; // TODO implement
}

const char32_t* unicode::blocks::get_random_char(const std::string& block_name)
{
  return nullptr; // TODO implement
}

const char32_t* unicode::blocks::get_random_char(const std::string& begin, const std::string& end)
{
  return nullptr; // TODO implement
}

const char32_t* unicode::blocks::get_random_char(char32_t begin, char32_t end)
{
  return nullptr; // TODO implement
}

