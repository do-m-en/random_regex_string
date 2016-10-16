#ifndef UNICODE_BLOCKS_HPP_INCLUDED
#define UNICODE_BLOCKS_HPP_INCLUDED

namespace unicode {

class blocks
{
  // BasicLatin (#x0000 - #x007F): (Basic Latin) U+0000 - U+007F
  // Latin-1Supplement (#x0080-#x00FF): (Latin-1 Supplement) U+0080 - U+00FF
  // LatinExtended-A (#x0100-#x017F): (Latin Extended-A) U+0100 - U+017F
  // LatinExtended-B (#x0180-#x024F): (Latin Extended-B) U+0180 - U+024F 	
  // IPAExtensions (#x0250-#x02AF): (IPA Extensions) U+0250 - U+02AF
  // SpacingModifierLetters (#x02B0-#x02FF): (Spacing Modifier Letters) U+02B0 - U+02FF
  // CombiningDiacriticalMarks (#x0300-#x036F): (Combining Diacritical Marks) U+0300 - U+036F
  // Greek (#x0370-#x03FF): (Greek and Coptic) U+0370 - U+0377, U+037A - U+037F, U+0384 - U+038A, U+038C, U+038E - U+03FF
  // Cyrillic (#x0400-#x04FF): (Cyrillic) U+0400 - U+04FF
  // -- (Cyrillic Supplement) U+0500 - U+052F
  // Armenian (#x0530-#x058F): (Armenian) U+0531 - U+0556, U+0559 - U+055F, U+0561 - U+0587, U+0589 - U+058A, U+058D - U+058F
  // Hebrew (#x0590-#x05FF): (Hebrew) U+0591 - U+05C7, U+05D0 - U+05EA, U+05F0 - U+05F4
  // Arabic (#x0600-#x06FF): (Arabic) U+0600 - U+061C, U+061E - U+06FF
  // Syriac (#x0700-#x074F): (Syriac) U+0700 - U+070D, U+070F - U+074A, U+074D - U+074F
  // -- (Arabic Supplement) U+075 - U+077F
  // Thaana (#x0780-#x07BF): (Thaana) U+0780 - U+07B1
  // -- (NKo) U+07C0 - U+07FA (U+07FF)
  // -- (Samaritan) U+0800 - U+082D, U+0830 - U+083E (U+083F)
  // -- (Mandaic) U+0840 - U+085B, U+085E (U+085F)
  // -- (Arabic Extended-A) U+08A0 - U+08B4, U+08E3 - U+08FF
  // Devanagari (#x0900-#x097F): (Devanagari) U+0900 - U+097F
  // Bengali (#x0980-#x09FF): (Bengali) U+0980 - U+0983, U+0985 - U+098C, U+098F - U+0990, U+0993 - U+09A8, U+09AA - U+09B0, U+09B2, U+09B6 - U+09B9, U+09BC - U+09C4, U+09C7 - U+09C8, U+09CB - U+09CE, U+09D7, U+09DC - U+09DD, U+09DF - U+09E3, U+09E6 - U+09FB
  // Gurmukhi (#x0A00-#x0A7F): (Gurmukhi) U+0A01 - U+0A03, U+0A05 - U+0A0A, U+0A0F - U+0A10, U+0A13 - U+0A28, U+0A2A - U+0A30, U+0A32 - U+0A33, U+0A35 - U+0A36, U+0A38 - U+0A39, U+0A3C, U+0A3E - U+0A42, U+0A47 - U+0A48, U+0A4B - U+0A4D, U+0A51, U+0A59 - U+0A5C, U+0A5E, U+0A66 - U+0A75
  // Gujarati (#x0A80-#x0AFF): (Gujarati) U+0A81 - U+0A83, U+0A85 - U+0A8D, U+0A8F - U+0A91, U+0A93 - U+0AA8, U+0AAA - U+0AB0, U+0AB2 - U+0AB3, U+0AB5 - U+0AB9, U+0ABC - U+0AC5, U+0AC7 - U+0AC9, U+0ACB - U+0ACD, U+0AD0, U+0AE0 - U+0AE3, U+0AE6 - U+0AF1, U+0AF9
  // Oriya (#x0B00-#x0B7F): (Oriya) U+0B01 - U+0B03, U+0B05 - U+0B0C, U+0B0F - U+0B10, U+0B13 - U+0B28, U+0B2A - U+0B30, U+0B32 - U+0B33, U+0B35 - U+0B39, U+0B3C - U+0B44, U+0B47 - U+0B48, U+0B4B - U+0B4D, U+0B56 - U+0B57, U+0B5C - U+0B5D, U+0B5F - U+0B63, U+0B66 - U+0B77
  // Tamil (#x0B80-#x0BFF): (Tamil) U+0B82 - U+0B83, U+0B85 - U+0B8A, U+0B8E - U+0B90, U+0B92 - U+0B95, U+0B99 - U+0B9A, U+0B9C, U+0B9E - U+0B9F, U+0BA3 - U+0BA4, U+0BA8 - U+0BAA, U+0BAE - U+0BB9, U+0BBE - U+0BC2, U+0BC6 - U+0BC8, U+0BCA - U+0BCD, U+0BD0, U+0BD7, U+0BE6 - U+0BFA 	
  // Telugu (#x0C00-#x0C7F): (Telugu) U+0C00 - U+0C03, U+0C05 - U+0C0C, U+0C0E - U+0C10, U+0C12 - U+0C28, U+0C2A - U+0C39, U+0C3D - U+0C44, U+0C46 - U+0C48, U+0C4A - U+0C4D, U+0C55 - U+0C56, U+0C58 - U+0C5A, U+0C60 - U+0C63, U+0C66 - U+0C6F, U+0C78 - U+0C7F
  // Kannada (#x0C80-#x0CFF): (Kannada) U+0C81 - U+0C83, U+0C85 - U+0C8C, U+0C8E - U+0C90, U+0C92 - U+0CA8, U+0CAA - U+0CB3, U+0CB5 - U+0CB9, U+0CBC - U+0CC4, U+0CC6 - U+0CC8, U+0CCA - U+0CCD, U+0CD5 - U+0CD6, U+0CDE, U+0CE0 - U+0CE3, U+0CE6 - U+0CEF, U+0CF1 - U+0CF2
  // Malayalam (#x0D00-#x0D7F): (Malayalam) U+0D01 - U+0D03, U+0D05 - U+0D0C, U+0D0E - U+0D10, U+0D12 - U+0D3A, U+0D3D - U+0D44, U+0D46 - U+0D48, U+0D4A - U+0D4E, U+0D57, U+0D5F - U+0D63, U+0D66 - U+0D75, U+0D79 - U+0D7F
  // Sinhala (#x0D80-#x0DFF): (Sinhala) U+0D82 - U+0D83, U+0D85 - U+0D96, U+0D9A - U+0DB1, U+0DB3 - U+0DBB, U+0DBD, U+0DC0 - U+0DC6, U+0DCA, U+0DCF - U+0DD4, U+0DD6, U+0DD8 - U+0DDF, U+0DE6 - U+0DEF, U+0DF2 - U+0DF4
  // Thai (#x0E00-#x0E7F): (Thai) U+0E01 - U+0E3A, U+0E3F - U+0E5B
  // Lao (#x0E80-#x0EFF): (Lao) U+0E81 - U+0E82, U+0E84, U+0E87 - U+0E88, U+0E8A, U+0E8D, U+0E94 - U+0E97, U+0E99 - U+0E9F, U+0EA1 - U+0EA3, U+0EA5, U+0EA7, U+0EAA - U+0EAB, U+0EAD - U+0EB9, U+0EBB - U+0EBD, U+0EC0 - U+0EC4, U+0EC6, U+0EC8 - U+0ECD, U+0ED0 - U+0ED9, U+0EDC - U+0EDF
  // Tibetan (#x0F00-#x0FFF): (Tibetan) U+0F00 - U+0F47, U+0F49 - U+0F6C, U+0F71 - U+0F97, U+0F99 - U+0FBC, U+0FBE - U+0FCC, U+0FCE - U+0FDA
  // Myanmar (#x1000-#x109F): (Myanmar) U+1000 - U+109F
  // Georgian (#x10A0-#x10FF): (Georgian) U+10A0 - U+10C5, U+10C7, U+10CD, U+10D0 - U+10FF
  // HangulJamo (#x1100-#x11FF): (Hangul Jamo) U+1100 - U+11FF
  // Ethiopic (#x1200-#x137F): (Ethiopic) U+1200 - U+1248, U+124A - U+124D, U+1250 - U+1256, U+1258, U+125A - U+125D, U+1260 - U+1288, U+128A - U+128D, U+1290 - U+12B0, U+12B2 - U+12B5, U+12B8 - U+12BE, U+12C0, U+12C2 - U+12C5, U+12C8 - U+12D6, U+12D8 - U+1310, U+1312 - U+1315, U+1318 - U+135A, U+135D - U+137C
  // -- (Ethiopic Supplement) U+1380 - U+1399 (U+139F)
  // Cherokee (#x13A0-#x13FF): (Cherokee) U+13A0 - U+13F5, U+13F8 - U+13FD
  // UnifiedCanadianAboriginalSyllabics (#x1400-#x167F): (Unified Canadian Aboriginal Syllabics) U+1400 - U+167F
  // Ogham (#x1680-#x169F): (Ogham) U+1680 - U+169C
  // Runic (#x16A0-#x16FF): (Runic) U+16A0 - U+16F8
  // -- (Tagalog) U+1700 - U+170C, U+170E - U+1714 (U+171F)
  // -- (Hanunoo) U+1720 - U+1736 (U+173F)
  // -- (Buhid) U+1740 - U+1753 (U+175F)
  // -- (Tagbanwa) U+1760 - U+176C, U+176E - U+1770, U+1772 - U+1773 (U+177F)
  // Khmer (#x1780-#x17FF): (Khmer) U+1780 - U+17DD, U+17E0 - U+17E9, U+17F0 - U+17F9
  // Mongolian (#x1800-#x18AF): (Mongolian) U+1800 - U+180E, U+1810 - U+1819, U+1820 - U+1877, U+1880 - U+18AA
  // -- (Unified Canadian Aboriginal Syllabics Extended) U+18B0 - U+18F5 (U+18FF)
  // -- (Limbu) U+1900 - U+191E, U+1920 - U+192B, U+1930 - U+193B, U+1940, U+1944 - U+194F
  // -- (Tai Le) U+1950 - U+196D, U+1970 - U+1974 (U+197F)
  // -- (New Tai Lue) U+1980 - U+19AB, U+19B0 - U+19C9, U+19D0 - U+19DA, U+19DE - U+19DF
  // -- (Khmer Symbols) U+19E0 - U+19FF
  // -- (Buginese) U+1A00 - U+1A1B, U+1A1E - U+1A1F
  // -- (Tai Tham) U+1A20 - U+1A5E,U+1A60 - U+1A7C, U+1A7F - U+1A89, U+1A90 - U+1A99, U+1AA0 - U+1AAD
  // -- (Combining Diacritical Marks Extended) U+1AB0 - U+1ABE (U+1AFF)
  // -- (Balinese) U+1B00 - U+1B4B, U+1B50 - U+1B7C (U+1B7F)
  // -- (Sundanese) U+1B80 - U+1BBF
  // -- (Batak) U+1BC0 - U+1BF3, U+1BFC - U+1BFF
  // -- (Lepcha) U+1C00 - U+1C37, U+1C3B - U+1C49, U+1C4D - U+1C4F
  // -- (Ol Chiki) U+1C50 - U+1C7F
  // -- (Sundanese Supplement) U+1CC0 - U+1CC7 (U+1CCF)
  // -- (Vedic Extensions) U+1CD0 - U+1CF6, U+1CF8 - U+1CF9 (U+1CFF)
  // -- (Phonetic Extensions) U+1D00 - U+1D7F
  // -- (Phonetic Extensions Supplement) U+1D80 - U+1DBF
  // -- (Combining Diacritical Marks Supplement) U+1DC0 - U+1DF5, U+1DFC - U+1DFF
  // LatinExtendedAdditional (#x1E00-#x1EFF): (Latin Extended Additional) U+1E00 - U+1EFF
  // GreekExtended (#x1F00-#x1FFF): (Greek Extended) U+1F00 - U+1F15, U+1F18 - U+1F1D, U+1F20 - U+1F45, U+1F48 - U+1F4D, U+1F50 - U+1F57, U+1F59, U+1F5B, U+1F5D, U+1F5F - U+1F7D, U+1F80 - U+1FB4, U+1FB6 - U+1FC4, U+1FC6 - U+1FD3, U+1FD6 - U+1FDB, U+1FDD - U+1FEF, U+1FF2 - U+1FF4, U+1FF6 - U+1FFE
  // GeneralPunctuation (#x2000-#x206F): (General Punctuation) U+2000 - U+2064, U+2066 - U+206F
  // SuperscriptsandSubscripts (#x2070-#x209F): (Superscripts and Subscripts) U+2070 - U+2071, U+2074 - U+208E, U+2090 - U+209C
  // CurrencySymbols (#x20A0-#x20CF): (Currency Symbols) U+20A0 - U+20BE
  // CombiningMarksforSymbols (#x20D0-#x20FF): (Combining Diacritical Marks for Symbols) U+20D0 - U+20F0
  // LetterlikeSymbols (#x2100-#x214F): (Letterlike Symbols) U+2100 - U+214F
  // NumberForms (#x2150-#x218F): (Number Forms) U+2150 - U+218B
  // Arrows (#x2190-#x21FF): (Arrows) U+2190 - U+21FF
  // MathematicalOperators (#x2200-#x22FF): (Mathematical Operators) U+2200 - U+22FF
  // MiscellaneousTechnical (#x2300-#x23FF): (Miscellaneous Technical) U+2300 - U+23FA
  // ControlPictures (#x2400-#x243F): (Control Pictures) U+2400 - U+2426
  // OpticalCharacterRecognition (#x2440-#x245F): (Optical Character Recognition) U+2440 - U+244A
  // EnclosedAlphanumerics (#x2460-#x24FF): (Enclosed Alphanumerics) U+2460 - U+24FF
  // BoxDrawing (#x2500-#x257F): (Box Drawing) U+2500 - U+257F
  // BlockElements (#x2580-#x259F): (Block Elements) U+2580 - U+259F
  // GeometricShapes (#x25A0-#x25FF): (Geometric Shapes) U+25A0 - U+25FF
  // MiscellaneousSymbols (#x2600-#x26FF): (Miscellaneous Symbols) U+2600 - U+26FF
  // Dingbats (#x2700-#x27BF): (Dingbats) U+2700 - U+27BF
  // -- (Miscellaneous Mathematical Symbols-A) U+27C0 - U+27EF
  // -- (Supplemental Arrows-A) U+27F0 - U+27FF
  // BraillePatterns (#x2800-#x28FF): (Braille Patterns) U+2800 - U+28FF
  // -- (Supplemental Arrows-B) U+2900 - U+297F
  // -- (Miscellaneous Mathematical Symbols-B) U+2980 - U+29FF
  // -- (Supplemental Mathematical Operators) U+2A00 - U+2AFF
  // -- (Miscellaneous Symbols and Arrows) U+2B00 - U+2B73, U+2B76 - U+2B95, U+2B98 - U+2BB9, U+2BBD - U+2BC8, U+2BCA - U+2BD1, U+2BEC - U+2BEF (U+2BFF)
  // -- (Glagolitic) U+2C00 - U+2C2E, U+2C30 - U+2C5E (U+2C5F)
  // -- (Latin Extended-C) U+2C60 - U+2C7F
  // -- (Coptic) U+2C80 - U+2CF3, U+2CF9 - U+2CFF
  // -- (Georgian Supplement) U+2D00 - U+2D25, U+2D27, U+2D2D (U+2D2F)
  // -- (Tifinagh) U+2D30 - U+2D67, U+2D6F - U+2D70, U+2D7F
  // -- (Ethiopic Extended) U+2D80 - U+2D96, U+2DA0 - U+2DA6, U+2DA8 - U+2DAE, U+2DB0 - U+2DB6, U+2DB8 - U+2DBE, U+2DC0 - U+2DC6, U+2DC8 - U+2DCE, U+2DD0 - U+2DD6, U+2DD8 - U+2DDE (U+2DDF)
  // -- (Cyrillic Extended-A) U+2DE0 - U+2DFF
  // -- (Supplemental Punctuation) U+2E00 - U+2E42 (U+2E7F)
  // CJKRadicalsSupplement (#x2E80-#x2EFF): (CJK Radicals Supplement) U+2E80 - U+2E99, U+2E9B - U+2EF3
  // KangxiRadicals (#x2F00-#x2FDF): (Kangxi Radicals) U+2F00 - U+2FD5
  // IdeographicDescriptionCharacters (#x2FF0-#x2FFF): (Ideographic Description Characters) U+2FF0 - U+2FFB
  // CJKSymbolsandPunctuation (#x3000-#x303F): (CJK Symbols and Punctuation) U+3000 - U+303F
  // Hiragana (#x3040-#x309F): (Hiragana) U+3041 - U+3096, U+3099 - U+309F
  // Katakana (#x30A0-#x30FF): (Katakana) U+30A0 - U+30FF
  // Bopomofo (#x3100-#x312F): (Bopomofo) U+3105 - U+312D
  // HangulCompatibilityJamo (#x3130-#x318F): (Hangul Compatibility Jamo) U+3131 - U+318E
  // Kanbun (#x3190-#x319F): (Kanbun) U+3190 - U+319F
  // BopomofoExtended (#x31A0-#x31BF): (Bopomofo Extended) U+31A0 - U+31BA
  // -- (CJK Strokes) U+31C0 - U+31E3 (U+31EF)
  // -- (Katakana Phonetic Extensions) U+31F0 - U+31FF
  // EnclosedCJKLettersandMonths (#x3200-#x32FF): (Enclosed CJK Letters and Months) U+3200 - U+321E, U+3220 - U+32FE
  // CJKCompatibility (#x3300-#x33FF): (CJK Compatibility) U+3300 - U+33FF
  // CJKUnifiedIdeographsExtensionA (#x3400-#x4DB5): (CJK Unified Ideographs Extension A) U+3400 - U+3402, U+3404 - U+3406, U+340C, U+3416, U+341C, U+3421, U+3424 - U+3425, U+3427 - U+3429, U+342B - U+3435, U+3437 - U+343D, U+3440 - U+3445, U+3447 - U+3450, U+3454, U+3457 - U+345F, U+3463 - U+346B, U+346E - U+3471, U+3473 - U+348E, U+3491 - U+3497, U+3499 - U+34A1, U+34A4 - U+34AB, U+34AD, U+34AF - U+34B0, U+34B2 - U+34B9, U+34BB - U+34BF, U+34C1 - U+34C5, U+34C7 - U+34CC, U+34CE - U+34D1, U+34D3 - U+34D4, U+34D6 - U+34EA, U+34EC - U+34FE, U+3500 - U+3507, U+3509 - U+3513, U+3515, U+3517 - U+351A, U+351C - U+352A, U+352C - U+3531, U+3533 - U+353F, U+3541 - U+357D, U+3580 - U+3588, U+358A, U+358F - U+3598, U+359E - U+35AB, U+35AD - U+35AE, U+35B5 - U+35CF, U+35D1 - U+35DE, U+35E2 - U+35ED, U+35F0 - U+35F7, U+35FB - U+3602, U+3604 - U+360E, U+3610 - U+3611, U+3613 - U+361E, U+3620 - U+362D, U+3630 - U+363B, U+363D - U+3645, U+3647 - U+364A, U+364C - U+3655, U+3657 - U+366C, U+366E - U+367C, U+367E, U+3680 - U+3683, U+3685 - U+3687, U+3689 - U+368D, U+368F - U+36C8, U+36CA, U+36CD - U+36FF, U+3701 - U+3713, U+3715 - U+371E, U+3720 - U+3745, U+3747 - U+3767, U+3769 - U+3772, U+3774 - U+3776, U+3778 - U+379C, U+379E - U+37D4, U+37D6 - U+37E0, U+37E2, U+37E4 - U+37EE, U+37F0 - U+3802, U+3804 - U+381D, U+381F - U+3823, U+3825 - U+382A, U+382D - U+382F, U+3831 - U+3832, U+3834 - U+384C, U+384E - U+3863, U+3865 - U+386B, U+386D - U+3886, U+3888 - U+388B, U+388D - U+388F, U+3891 - U+38A1, U+38A3 - U+38AA, U+38AC - U+38B0, U+38B2 - U+38B6, U+38B8, U+38BA - U+38BE, U+38C0 - U+38C9, U+38CB - U+38D4, U+38D8 - U+38E0, U+38E2 - U+38E7, U+38EA - U+38ED, U+38EF - U+38F2, U+38F5 - U+38F7, U+38FA - U+393B, U+393D - U+3956, U+3958, U+395A - U+397A, U+397C - U+3987, U+3989 - U+399F, U+39A1 - U+39DF, U+39E1 - U+3A22, U+3A24 - U+3A2B, U+3A2D - U+3A41, U+3A43 - U+3A4E, U+3A50, U+3A52 - U+3A6A, U+3A6C - U+3AB1, U+3AB4 - U+3AC5, U+3ACA - U+3AE2, U+3AE4 - U+3AEB, U+3AEF - U+3AFE, U+3B01 - U+3B10, U+3B12 - U+3B15, U+3B17 - U+3B27, U+3B29 - U+3B36, U+3B38 - U+3B39, U+3B3B - U+3B3C, U+3B3F, U+3B41 - U+3B44, U+3B47 - U+3B4F, U+3B51 - U+3B55, U+3B57 - U+3B64, U+3B68 - U+3B72, U+3B74, U+3B77 - U+3B88, U+3B8B - U+3B9F, U+3BA1 - U+3BBC, U+3BBE - U+3BD0, U+3BD3 - U+3BE6, U+3BE9 - U+3BF4, U+3BF6 - U+3BFB, U+3BFD - U+3C12, U+3C14 - U+3C1B, U+3C1D - U+3C4F, U+3C52, U+3C54 - U+3C5C, U+3C5E - U+3C76, U+3C78 - U+3C8F, U+3C92 - U+3CA8, U+3CAA - U+3CAD, U+3CAF - U+3CD2, U+3CD4 - U+3CE2, U+3CE4 - U+3CF0, U+3CF4 - U+3D0B, U+3D0F - U+3D1E, U+3D25 - U+3D38, U+3D3B - U+3D46, U+3D4A - U+3D59, U+3D5D - U+3D7B, U+3D7D - U+3D81, U+3D84 - U+3D8A, U+3D8C - U+3D8F, U+3D91 - U+3D98, U+3D9A - U+3D9C, U+3D9E - U+3DA1, U+3DA3 - U+3DA8, U+3DAB - U+3DB0, U+3DB2 - U+3DB6, U+3DB9 - U+3DC0, U+3DC2 - U+3DDB, U+3DDD, U+3DDF - U+3DF0, U+3DF3 - U+3DF9, U+3DFB - U+3E01, U+3E03 - U+3E13, U+3E15 - U+3E68, U+3E6A - U+3E85, U+3E87 - U+3E98, U+3E9A - U+3EAE, U+3EB0 - U+3EB5, U+3EB7 - U+3EBA, U+3EBF - U+3EC5, U+3EC7, U+3EC9 - U+3ED8, U+3EDA - U+3EDB, U+3EDD - U+3EE2, U+3EE7 - U+3EFA, U+3EFC - U+3F00, U+3F02, U+3F04, U+3F06 - U+3F53, U+3F55 - U+3F75, U+3F77 - U+3FB1, U+3FB4 - U+3FC0, U+3FC2 - U+3FCF, U+3FD1 - U+403F, U+4041 - U+4060, U+4062 - U+408A, U+408C - U+40C2, U+40C5, U+40C7 - U+40CF, U+40D1 - U+40E7, U+40E9 - U+40FB, U+40FD - U+4109, U+410B - U+4116, U+4118 - U+411D, U+411F - U+4122, U+4124 - U+4133, U+4136 - U+416A, U+416C - U+4186, U+4188 - U+418B, U+418D - U+41FE, U+4200 - U+4227, U+422A - U+4246, U+4248 - U+42C4, U+42C6, U+42C8 - U+434E, U+4350 - U+439D, U+439F - U+43A4, U+43A6 - U+4455, U+4457 - U+446D, U+446F - U+4476, U+4478 - U+447D, U+447F - U+4498, U+449A - U+44AD, U+44B0 - U+44C0, U+44C2 - U+44FA, U+44FC - U+4525, U+4527, U+452A, U+452C - U+4541, U+4543 - U+454F, U+4551 - U+45AB, U+45AD - U+45EC, U+45EE - U+45F2, U+45F4 - U+4626, U+4628 - U+4631, U+4633 - U+464A, U+464C, U+464E - U+466A, U+466C - U+4675, U+4677 - U+468B, U+468D - U+46A3, U+46A5 - U+4701, U+4703 - U+4725, U+4727, U+4729 - U+4732, U+4734 - U+473E, U+4740 - U+4748, U+474A - U+478E, U+4790 - U+47B1, U+47B3 - U+47CD, U+47CF - U+47F1, U+47F4 - U+480B, U+480D - U+4839, U+483B - U+4877, U+4879 - U+4885, U+4887 - U+48B6, U+48B8 - U+48C8, U+48CA - U+48D1, U+48D3 - U+48D6, U+48D8 - U+48FF, U+4901 - U+4965, U+4967 - U+4978, U+497A - U+49A8, U+49AA - U+49B7, U+49B9 - U+49BE, U+49C1 - U+49C7, U+49C9 - U+49CE, U+49D0 - U+49E8, U+49EA - U+49EC, U+49EE - U+4A02, U+4A04 - U+4A4D, U+4A4F - U+4A9E, U+4AA0 - U+4AA9, U+4AAB - U+4AE6, U+4AE8 - U+4B4E, U+4B50 - U+4B5B, U+4B5D - U+4B84, U+4B86 - U+4BE8, U+4BEA - U+4C2D, U+4C2F - U+4C32, U+4C34 - U+4C68, U+4C6B - U+4C86, U+4C89 - U+4CE4, U+4CE6 - U+4D27, U+4D29 - U+4DAC, U+4DAE - U+4DB5
  // -- (Yijing Hexagram Symbols) U+4DC0 - U+4DFF
  // CJKUnifiedIdeographs (#x4E00-#x9FFF): (CJK Unified Ideographs) U+4E00 - U+9FB4, U+9FBC - U+9FC4, U+9FC6 - U+9FD0, U+9FD2 - U+9FD5
  // YiSyllables (#xA000-#xA48F): (Yi Syllables) U+A000 - U+A48C
  // YiRadicals (#xA490-#xA4CF): (Yi Radicals) U+A490 - U+A4C6
  // -- (Lisu) U+A4D0 - U+A4FF
  // -- (Vai) U+A500 - U+A62B (U+A63F)
  // -- (Cyrillic Extended-B) U+A640 - U+A69F
  // -- (Bamum) U+A6A0 - U+A6F7 (U+A6FF)
  // -- (Modifier Tone Letters) U+A700 - U+A71F
  // -- (Latin Extended-D) U+A720 - U+A7AD, U+A7B0 - U+A7B7, U+A7F7 - U+A7FF
  // -- (Syloti Nagri) U+A800 - U+A82B (U+A82F)
  // -- (Common Indic Number Forms) U+A830 - U+A839 (U+A83F)
  // -- (Phags-pa) U+A840 - U+A877 (U+A87F)
  // -- (Saurashtra) U+A880 - U+A8C4, U+A8CE - U+A8D9 (U+A8DF)
  // -- (Devanagari Extended) U+A8E0 - U+A8FD (U+A8FF)
  // -- (Kayah Li) U+A900 - U+A92F
  // -- (Rejang) U+A930 - U+A953, U+A95F
  // -- (Hangul Jamo Extended-A) U+A960 - U+A97C (U+A97F)
  // -- (Javanese) U+A980 - U+A9CD, U+A9CF - U+A9D9, U+A9DE - U+A9DF
  // -- (Myanmar Extended-B) U+A9E0 - U+A9FE (U+A9FF)
  // -- (Cham) U+AA00 - U+AA36, U+AA40 - U+AA4D, U+AA50 - U+AA59, U+AA5C - U+AA5F
  // -- (Myanmar Extended-A) U+AA60 - U+AA7F
  // -- (Tai Viet) U+AA80 - U+AAC2, U+AADB - U+AADF
  // -- (Meetei Mayek Extensions) U+AAE0 - U+AAF6 (U+AAFF)
  // -- (Ethiopic Extended-A) (U+AB00) U+AB01 - U+AB06, U+AB09 - U+AB0E, U+AB11 - U+AB16, U+AB20 - U+AB26, U+AB28 - U+AB2E (U+AB2F)
  // -- (Latin Extended-E) U+AB30 - U+AB65 (U+AB6F)
  // -- (Cherokee Supplement) U+AB70 - U+ABBF
  // -- (Meetei Mayek) U+ABC0 - U+ABED, U+ABF0 - U+ABF9 (U+ABFF)
  // HangulSyllables (#xAC00-#xD7A3): (Hangul Syllables) U+AC00, U+D7A3 (U+D7AF)
  // -- (Hangul Jamo Extended-B) U+D7B0 - U+D7C6, U+D7CB - U+D7FB (U+D7FF)
  // HighSurrogates (#xD800-#xDB7F): (High Surrogates) U+D800 - U+DB7F
  // HighPrivateUseSurrogates (#xDB80-#xDBFF): (High Private Use Surrogates) U+DB80 - U+DBFF
  // LowSurrogates (#xDC00-#xDFFF): (Low Surrogates) U+DC00 - U+DFFF
  // PrivateUse (#xE000-#xF8FF): (Private Use Area) U+E000 - U+F8FF
  // CJKCompatibilityIdeographs (#xF900-#xFAFF): U+F900 - U+FA6D, U+FA70 - U+FAD9
  // AlphabeticPresentationForms (#xFB00-#xFB4F): U+FB00 - U+FB06, U+FB13 - U+FB17, U+FB1D - U+FB36, U+FB38 - U+FB3C, U+FB3E, U+FB40 - U+FB41, U+FB43 - U+FB44, U+FB46 - U+FB4F
  // ArabicPresentationForms-A (#xFB50-#xFDFF): U+FB50 - U+FBC1, U+FBD3 - U+FD3F, U+FD50 - U+FD8F, U+FD92 - U+FDC7, U+FDD0 - U+FDFD
  // -- (Variation Selectors) U+FE00 - U+FE0F
  // -- (Vertical Forms) U+FE10 - U+FE19 (U+FE1F)
  // CombiningHalfMarks (#xFE20-#xFE2F): (Combining Half Marks) U+FE20 - U+FE2F
  // CJKCompatibilityForms (#xFE30-#xFE4F): (CJK Compatibility Forms) U+FE30 - U+FE4F
  // SmallFormVariants (#xFE50-#xFE6F): (Small Form Variants) U+FE50 - U+FE52, U+FE54 - U+FE66, U+FE68 - U+FE6B
  // ArabicPresentationForms-B (#xFE70-#xFEFE): (Arabic Presentation Forms-B) U+FE70 - U+FE74, U+FE76 - U+FEFC, U+FEFF
  // Specials (#xFEFF-#xFEFF): <- TODO wtf see 2 lines below... Specials once again...
  // HalfwidthandFullwidthForms (#xFF00-#xFFEF): U+FF01 - U+FFBE, U+FFC2 - U+FFC7, U+FFCA - U+FFCF, U+FFD2 - U+FFD7, U+FFDA - U+FFDC, U+FFE0 - U+FFE6, U+FFE8 - U+FFEE
  // Specials (#xFFF0-#xFFFD): U+FFF9 - U+FFFF
  // -- (Linear B Syllabary) U+10000 - U+1000B, U+1000D - U+10026, U+10028 - U+1003A, U+1003C - U+1003D, U+1003F - U+1004D, U+10050 - U+1005D (U+1007F)
  // -- (Linear B Ideograms) U+10080 - U+100FA (U+100FF)
  // -- (Aegean Numbers) U+10100 - U+10102, U+10107 - U+10133, U+10137 - U+1013F
  // -- (Ancient Greek Numbers) U+10140 - U+1018C (U+1018F)
  // -- (Ancient Symbols) U+10190 - U+1019B, U+101A0 (U+101CF)
  // -- (Phaistos Disc) U+101D0 - U+101FD (U+101FF)
  // -- (Lycian) U+10280 - U+1029C (U+1029F)
  // -- (Carian) U+102A0 - U+102D0 (U+102DF)
  // -- (Coptic Epact Numbers) U+102E0 - U+102FB (U+102FF)
  // OldItalic (#x10300-#x1032F): U+10300 - U+10323
  // Gothic (#x10330-#x1034F): U+10330 - U+1034A
  // -- (Old Permic) U+10350 - U+1037A (U+1037F)
  // -- (Ugaritic) U+10380 - U+1039D, U+1039F
  // -- (Old Persian) U+103A0 - U+103C3, U+103C8 - U+103D5 (U+103DF)
  // Deseret (#x10400-#x1044F): U+10400 - U+1044F
  // -- (Shavian) U+10450 - U+1047F
  // -- (Osmanya) U+10480 - U+1049D, U+104A0 - U+104A9 (U+104AF)
  // -- (Elbasan) U+10500 - U+10527 (U+1052F)
  // -- (Caucasian Albanian) U+10530 - U+10563, U+1056F (U+1056F)
  // -- (Linear A) U+10600 - U+10736, U+10740 - U+10755, U+10760 - U+10767 (U+1077F)
  // -- (Cypriot Syllabary) U+10800 - U+10805, U+10808, U+1080A - U+10835, U+10837 - U+10838, U+1083C, U+1083F
  // -- (Imperial Aramaic) U+10840 - U+10855, U+10857 - U+1085F
  // -- (Palmyrene) U+10860 - U+1087F
  // -- (Nabataean) U+10880 - U+1089E, U+108A7 - U+108AF
  // -- (Hatran) U+108E0 - U+108F2, U+108F4 - U+108F5, U+108FB - U+108FF
  // -- (Phoenician) U+10900 - U+1091B, U+1091F
  // -- (Lydian) U+10920 - U+10939, U+1093F
  // -- (Meroitic Hieroglyphs) U+10980 - U+1099F
  // -- (Meroitic Cursive) U+109A0 - U+109B7, U+109BC - U+109CF, U+109D2 - U+109FF
  // -- (Kharoshthi) U+10A00 - U+10A03, U+10A05 - U+10A06, U+10A0C - U+10A13, U+10A15 - U+10A17, U+10A19 - U+10A33, U+10A38 - U+10A3A, U+10A3F - U+10A47, U+10A50 - U+10A58 (U+10A5F)
  // -- (Old South Arabian) U+10A60 - U+10A7F
  // -- (Old North Arabian) U+10A80 - U+10A9F
  // -- (Manichaean) U+10AC0 - U+10AE6, U+10AEB - U+10AF6 (U+10AFF)
  // -- (Avestan) U+10B00 - U+10B35, U+10B39 - U+10B3F
  // -- (Inscriptional Parthian) U+10B40 - U+10B55, U+10B58 - U+10B5F
  // -- (Inscriptional Pahlavi) U+10B60 - U+10B72, U+10B78 - U+10B7F
  // -- (Psalter Pahlavi) U+10B80 - U+10B91, U+10B99 - U+10B9C, U+10BA9 - U+10BAF
  // -- (Old Turkic) U+10C00 - U+10C48 (U+10C4F)
  // -- (Old Hungarian) U+10C80 - U+10CB2, U+10CC0 - U+10CF2, U+10CFA - U+10CFF
  // -- (Rumi Numeral Symbols) U+10E60 - U+10E7E (U+10E7F)
  // -- (Brahmi) U+11000 - U+1104D, U+11052 - U+1106F, U+1107F
  // -- (Kaithi) U+11080 - U+110C1 (U+110CF)
  // -- (Sora Sompeng) U+110D0 - U+110E8, U+110F0 - U+110F9 (U+110FF)
  // -- (Chakma) U+11100 - U+11134, U+11136 - U+11143 (U+1114F)
  // -- (Mahajani) U+11150 - U+11176 (U+1117F)
  // -- (Sharada) U+11180 - U+111CD, U+111D0 - U+111DF
  // -- (Sinhala Archaic Numbers) (U+111E0) U+111E1 - U+111F4 (U+111FF)
  // -- (Khojki) U+11200 - U+11211, U+11213 - U+1123D (U+1124F)
  // -- (Multani) U+11280 - U+11286, U+11288, U+1128A - U+1128D, U+1128F - U+1129D, U+1129F - U+112A9 (U+112AF)
  // -- (Khudawadi) U+112B0 - U+112EA, U+112F0 - U+112F9 (U+112FF)
  // -- (Grantha) U+11300 - U+11303, U+11305 - U+1130C, U+1130F - U+11310, U+11313 - U+11328, U+1132A - U+11330, U+11332 - U+11333, U+11335 - U+11339, U+1133C - U+11344, U+11347 - U+11348, U+1134B - U+1134D, U+11350, U+11357, U+1135D - U+11363, U+11366 - U+1136C, U+11370 - U+11374 (U+1137F)
  // -- (Tirhuta) U+11480 - U+114C7, U+114D0 - U+114D9 (U+114DF)
  // -- (Siddham) U+11580 - U+115B5, U+115B8 - U+115DD (U+115FF)
  // -- (Modi) U+11600 - U+11644, U+11650 - U+11659 (U+1165F)
  // -- (Takri) U+11680 - U+116B7, U+116C0 - U+116C9 (U+116CF)
  // -- (Ahom) U+11700 - U+11719, U+1171D - U+1172B, U+11730 - U+1173F
  // -- (Warang Citi) U+118A0 - U+118F2, U+118FF
  // -- (Pau Cin Hau) U+11AC0 - U+11AF8 (U+11AFF)
  // -- (Cuneiform) U+12000 - U+12399 (U+123FF)
  // -- (Cuneiform Numbers and Punctuation) U+12400 - U+1246E, U+12470 - U+12474 (U+1247F)
  // -- (Early Dynastic Cuneiform) U+12480 - U+12543 (U+1254F)
  // -- (Egyptian Hieroglyphs) U+13000 - U+1342E (U+1342F)
  // -- (Anatolian Hieroglyphs) U+14400 - U+14646 (U+1467F)
  // -- (Bamum Supplement) U+16800 - U+16A38 (U+16A3F)
  // -- (Mro) U+16A40 - U+16A5E, U+16A60 - U+16A69, U+16A6E - U+16A6F
  // -- (Bassa Vah) U+16AD0 - U+16AED, U+16AF0 - U+16AF5 (U+16AFF)
  // -- (Pahawh Hmong) U+16B00 - U+16B45, U+16B50 - U+16B59, U+16B5B - U+16B61, U+16B63 - U+16B77, U+16B7D - U+16B8F
  // -- (Miao) U+16F00 - U+16F44, U+16F50 - U+16F7E, U+16F8F - U+16F9F
  // -- (Kana Supplement) U+1B000 - U+1B001 (U+1B0FF)
  // -- (Duployan) U+1BC00 - U+1BC6A, U+1BC70 - U+1BC7C, U+1BC80 - U+1BC88, U+1BC90 - U+1BC99, U+1BC9C - U+1BC9F
  // -- (Shorthand Format Controls) U+1BCA0 - U+1BCA3 (U+1BCAF)
  // ByzantineMusicalSymbols (#x1D000-#x1D0FF): (Byzantine Musical Symbols) U+1D000 - U+1D0F5
  // MusicalSymbols (#x1D100-#x1D1FF): (Musical Symbols) U+1D100 - U+1D126, U+1D129 - U+1D1E8
  // -- (Ancient Greek Musical Notation) U+1D200 - U+1D245 (U+1D24F)
  // -- (Tai Xuan Jing Symbols) U+1D300 - U+1D356 (U+1D35F)
  // -- (Counting Rod Numerals) U+1D360 - U+1D371 (U+1D37F)
  // MathematicalAlphanumericSymbols (#x1D400-#x1D7FF): (Mathematical Alphanumeric Symbols) U+1D400 - U+1D454, U+1D456 - U+1D49C, U+1D49E - U+1D49F, U+1D4A2, U+1D4A5 - U+1D4A6, U+1D4A9 - U+1D4AC, U+1D4AE - U+1D4B9, U+1D4BB, U+1D4BD - U+1D4C3, U+1D4C5 - U+1D505, U+1D507 - U+1D50A, U+1D50D - U+1D514, U+1D516 - U+1D51C, U+1D51E - U+1D539, U+1D53B - U+1D53E, U+1D540 - U+1D544, U+1D546, U+1D54A - U+1D550, U+1D552 - U+1D6A5, U+1D6A8 - U+1D7CB, U+1D7CE - U+1D7FF
  // -- (Sutton SignWriting) U+1D800 - U+1DA8B, U+1DA9B - U+1DA9F, U+1DAA1 - U+1DAAF
  // -- (Mende Kikakui) U+1E800 - U+1E8C4, U+1E8C7 - U+1E8D6 (U+1E8DF)
  // -- (Arabic Mathematical Alphabetic Symbols) U+1EE00 - U+1EE03, U+1EE05 - U+1EE1F, U+1EE21 - U+1EE22, U+1EE24, U+1EE27, U+1EE29 - U+1EE32, U+1EE34 - U+1EE37, U+1EE39, U+1EE3B, U+1EE42, U+1EE47, U+1EE49, U+1EE4B, U+1EE4D - U+1EE4F, U+1EE51 - U+1EE52, U+1EE54, U+1EE57, U+1EE59, U+1EE5B, U+1EE5D, U+1EE5F, U+1EE61 - U+1EE62, U+1EE64, U+1EE67 - U+1EE6A, U+1EE6C - U+1EE72, U+1EE74 - U+1EE77, U+1EE79 - U+1EE7C, U+1EE7E, U+1EE80 - U+1EE89, U+1EE8B - U+1EE9B, U+1EEA1 - U+1EEA3, U+1EEA5 - U+1EEA9, U+1EEAB - U+1EEBB, U+1EEF0 - U+1EEF1 (U+1EEFF)
  // -- (Mahjong Tiles) U+1F000 - U+1F02B (U+1F02F)
  // -- (Domino Tiles) U+1F030 - U+1F093 (U+1F09F)
  // -- (Playing Cards) U+1F0A0 - U+1F0AE, U+1F0B1 - U+1F0BF, U+1F0C1 - U+1F0CF, U+1F0D1 - U+1F0F5 (U+1F0FF)
  // -- (Enclosed Alphanumeric Supplement) U+1F100 - U+1F10C, U+1F110 - U+1F12E, U+1F130 - U+1F16B, U+1F170 - U+1F19A, U+1F1E6 - U+1F1FF
  // -- (Enclosed Ideographic Supplement) U+1F200 - U+1F202, U+1F210 - U+1F23A, U+1F240 - U+1F248, U+1F250 - U+1F251 (U+1F2FF)
  // -- (Miscellaneous Symbols and Pictographs) U+1F300 - U+1F579, U+1F57B - U+1F5A3, U+1F5A5 - U+1F5FF
  // -- (Emoticons) U+1F600 - U+1F64F
  // -- (Ornamental Dingbats) U+1F650 - U+1F67F
  // -- (Transport and Map Symbols) U+1F680 - U+1F6D0, U+1F6E0 - U+1F6EC, U+1F6F0 - U+1F6F3 (U+1F6FF)
  // -- (Alchemical Symbols) U+1F700 - U+1F773 (U+1F77F)
  // -- (Geometric Shapes Extended) U+1F780 - U+1F7D4 (U+1F7FF)
  // -- (Supplemental Arrows-C) U+1F800 - U+1F80B, U+1F810 - U+1F847, U+1F850 - U+1F859, U+1F860 - U+1F887, U+1F890 - U+1F8AD (U+1F8FF)
  // -- (Supplemental Symbols and Pictographs) U+1F910 - U+1F918, U+1F980 - U+1F984, U+1F9C0 (U+1F9FF)
  // CJKUnifiedIdeographsExtensionB (#x20000-#x2A6D6): (CJK Unified Ideographs Extension B) U+20000 - U+20861, U+20863 - U+2230A, U+2230C - U+22C80, U+22C82 - U+23562, U+23564 - U+2420A, U+2420E - U+243B8, U+243BA - U+24B56, U+24B58 - U+253C2, U+253C4 - U+26224, U+26226 - U+2622D, U+2622F - U+26232, U+26234 - U+2623D, U+26241 - U+26242, U+26246 - U+26249, U+2624E - U+26251, U+26255, U+26258 - U+2625A, U+2625D - U+26263, U+26265, U+26267, U+26269 - U+2780B, U+2780D - U+2883F, U+28841 - U+2A6D6
  // -- (CJK Unified Ideographs Extension C) U+2A700, U+2A79D, U+2A848, U+2A84F, U+2A8AE, U+2A9E6, U+2AA0A, U+2AA17, U+2AA9D, U+2ADFF, U+2AEB9, U+2AED0, U+2AFA2, U+2B061, U+2B088, U+2B099, U+2B0DC, U+2B128, U+2B138, U+2B230, U+2B2D0, U+2B300, U+2B328, U+2B359, U+2B35F, U+2B362, U+2B370, U+2B372, U+2B3CB, U+2B404, U+2B406, U+2B409, U+2B410, U+2B413, U+2B4B6, U+2B4E7, U+2B4E9, U+2B50E, U+2B5B8, U+2B5E0, U+2B5E6 - U+2B5E7, U+2B5EE, U+2B5F4, U+2B61D, U+2B623 - U+2B624, U+2B628, U+2B688 - U+2B689, U+2B692, U+2B694 - U+2B695, U+2B699, U+2B6DB, U+2B6DE, U+2B6E2, U+2B6F6, U+2B6F8, U+2B734 (U+2B73F)
  // -- (CJK Unified Ideographs Extension D) U+2B740, U+2B746, U+2B751, U+2B753, U+2B75A, U+2B75C, U+2B765, U+2B776 - U+2B777, U+2B77C, U+2B782, U+2B789, U+2B78B, U+2B78E, U+2B794, U+2B7AC, U+2B7AF, U+2B7BD, U+2B7C9, U+2B7CF, U+2B7D2, U+2B7D8, U+2B7F0, U+2B80D, U+2B817, U+2B81A, U+2B81D (U+2B81F)
  // -- (CJK Unified Ideographs Extension E) U+2B820 - U+2CEA1 (U+2CEAF)
  // CJKCompatibilityIdeographsSupplement (#x2F800-#x2FA1F): (CJK Compatibility Ideographs Supplement) U+2F800 - U+2FA1D
  // Tags (#xE0000-#xE007F): (Tags) U+E0001, U+E0020 - U+E007F
  // -- (Variation Selectors Supplement) U+E0100 - U+E01EF
  // PrivateUse (#xF0000-#x10FFFD): (Supplementary Private Use Area-A) U+F0000, U+FFFFD - U+FFFFF (TODO: I think that FFFFD is the last one and FFFFF does not belong) (Supplementary Private Use Area-B) U+100000, U+10FFFD - U+10FFFF
};

}

#endif // UNICODE_BLOCKS_HPP_INCLUDED
