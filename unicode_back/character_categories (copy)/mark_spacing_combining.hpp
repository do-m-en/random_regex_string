#ifndef UNICODE_MARK_SPACING_COMBINING_HPP_INCLUDED
#define UNICODE_MARK_SPACING_COMBINING_HPP_INCLUDED

namespace unicode {

// Mc
class mark_spacing_combining
{
/*
Character Name
U+0903    DEVANAGARI SIGN VISARGA
U+093B    DEVANAGARI VOWEL SIGN OOE
U+093E    DEVANAGARI VOWEL SIGN AA
U+093F    DEVANAGARI VOWEL SIGN I
U+0940    DEVANAGARI VOWEL SIGN II
U+0949    DEVANAGARI VOWEL SIGN CANDRA O
U+094A    DEVANAGARI VOWEL SIGN SHORT O
U+094B    DEVANAGARI VOWEL SIGN O
U+094C    DEVANAGARI VOWEL SIGN AU
U+094E    DEVANAGARI VOWEL SIGN PRISHTHAMATRA E
U+094F    DEVANAGARI VOWEL SIGN AW
U+0982    BENGALI SIGN ANUSVARA
U+0983    BENGALI SIGN VISARGA
U+09BE    BENGALI VOWEL SIGN AA
U+09BF    BENGALI VOWEL SIGN I
U+09C0    BENGALI VOWEL SIGN II
U+09C7    BENGALI VOWEL SIGN E
U+09C8    BENGALI VOWEL SIGN AI
U+09CB    BENGALI VOWEL SIGN O
U+09CC    BENGALI VOWEL SIGN AU
U+09D7    BENGALI AU LENGTH MARK
U+0A03    GURMUKHI SIGN VISARGA
U+0A3E    GURMUKHI VOWEL SIGN AA
U+0A3F    GURMUKHI VOWEL SIGN I
U+0A40    GURMUKHI VOWEL SIGN II
U+0A83    GUJARATI SIGN VISARGA
U+0ABE    GUJARATI VOWEL SIGN AA
U+0ABF    GUJARATI VOWEL SIGN I
U+0AC0    GUJARATI VOWEL SIGN II
U+0AC9    GUJARATI VOWEL SIGN CANDRA O
U+0ACB    GUJARATI VOWEL SIGN O
U+0ACC    GUJARATI VOWEL SIGN AU
U+0B02    ORIYA SIGN ANUSVARA
U+0B03    ORIYA SIGN VISARGA
U+0B3E    ORIYA VOWEL SIGN AA
U+0B40    ORIYA VOWEL SIGN II
U+0B47    ORIYA VOWEL SIGN E
U+0B48    ORIYA VOWEL SIGN AI
U+0B4B    ORIYA VOWEL SIGN O
U+0B4C    ORIYA VOWEL SIGN AU
U+0B57    ORIYA AU LENGTH MARK
U+0BBE    TAMIL VOWEL SIGN AA
U+0BBF    TAMIL VOWEL SIGN I
U+0BC1    TAMIL VOWEL SIGN U
U+0BC2    TAMIL VOWEL SIGN UU
U+0BC6    TAMIL VOWEL SIGN E
U+0BC7    TAMIL VOWEL SIGN EE
U+0BC8    TAMIL VOWEL SIGN AI
U+0BCA    TAMIL VOWEL SIGN O
U+0BCB    TAMIL VOWEL SIGN OO
U+0BCC    TAMIL VOWEL SIGN AU
U+0BD7    TAMIL AU LENGTH MARK
U+0C01    TELUGU SIGN CANDRABINDU
U+0C02    TELUGU SIGN ANUSVARA
U+0C03    TELUGU SIGN VISARGA
U+0C41    TELUGU VOWEL SIGN U
U+0C42    TELUGU VOWEL SIGN UU
U+0C43    TELUGU VOWEL SIGN VOCALIC R
U+0C44    TELUGU VOWEL SIGN VOCALIC RR
U+0C82    KANNADA SIGN ANUSVARA
U+0C83    KANNADA SIGN VISARGA
U+0CBE    KANNADA VOWEL SIGN AA
U+0CC0    KANNADA VOWEL SIGN II
U+0CC1    KANNADA VOWEL SIGN U
U+0CC2    KANNADA VOWEL SIGN UU
U+0CC3    KANNADA VOWEL SIGN VOCALIC R
U+0CC4    KANNADA VOWEL SIGN VOCALIC RR
U+0CC7    KANNADA VOWEL SIGN EE
U+0CC8    KANNADA VOWEL SIGN AI
U+0CCA    KANNADA VOWEL SIGN O
U+0CCB    KANNADA VOWEL SIGN OO
U+0CD5    KANNADA LENGTH MARK
U+0CD6    KANNADA AI LENGTH MARK
U+0D02    MALAYALAM SIGN ANUSVARA
U+0D03    MALAYALAM SIGN VISARGA
U+0D3E    MALAYALAM VOWEL SIGN AA
U+0D3F    MALAYALAM VOWEL SIGN I
U+0D40    MALAYALAM VOWEL SIGN II
U+0D46    MALAYALAM VOWEL SIGN E
U+0D47    MALAYALAM VOWEL SIGN EE
U+0D48    MALAYALAM VOWEL SIGN AI
U+0D4A    MALAYALAM VOWEL SIGN O
U+0D4B    MALAYALAM VOWEL SIGN OO
U+0D4C    MALAYALAM VOWEL SIGN AU
U+0D57    MALAYALAM AU LENGTH MARK
U+0D82    SINHALA SIGN ANUSVARAYA
U+0D83    SINHALA SIGN VISARGAYA
U+0DCF    SINHALA VOWEL SIGN AELA-PILLA
U+0DD0    SINHALA VOWEL SIGN KETTI AEDA-PILLA
U+0DD1    SINHALA VOWEL SIGN DIGA AEDA-PILLA
U+0DD8    SINHALA VOWEL SIGN GAETTA-PILLA
U+0DD9    SINHALA VOWEL SIGN KOMBUVA
U+0DDA    SINHALA VOWEL SIGN DIGA KOMBUVA
U+0DDB    SINHALA VOWEL SIGN KOMBU DEKA
U+0DDC    SINHALA VOWEL SIGN KOMBUVA HAA AELA-PILLA
U+0DDD    SINHALA VOWEL SIGN KOMBUVA HAA DIGA AELA-PILLA
U+0DDE    SINHALA VOWEL SIGN KOMBUVA HAA GAYANUKITTA
U+0DDF    SINHALA VOWEL SIGN GAYANUKITTA
U+0DF2    SINHALA VOWEL SIGN DIGA GAETTA-PILLA
U+0DF3    SINHALA VOWEL SIGN DIGA GAYANUKITTA
U+0F3E    TIBETAN SIGN YAR TSHES
U+0F3F    TIBETAN SIGN MAR TSHES
U+0F7F    TIBETAN SIGN RNAM BCAD
U+102B    MYANMAR VOWEL SIGN TALL AA
U+102C    MYANMAR VOWEL SIGN AA
U+1031    MYANMAR VOWEL SIGN E
U+1038    MYANMAR SIGN VISARGA
U+103B    MYANMAR CONSONANT SIGN MEDIAL YA
U+103C    MYANMAR CONSONANT SIGN MEDIAL RA
U+1056    MYANMAR VOWEL SIGN VOCALIC R
U+1057    MYANMAR VOWEL SIGN VOCALIC RR
U+1062    MYANMAR VOWEL SIGN SGAW KAREN EU
U+1063    MYANMAR TONE MARK SGAW KAREN HATHI
U+1064    MYANMAR TONE MARK SGAW KAREN KE PHO
U+1067    MYANMAR VOWEL SIGN WESTERN PWO KAREN EU
U+1068    MYANMAR VOWEL SIGN WESTERN PWO KAREN UE
U+1069    MYANMAR SIGN WESTERN PWO KAREN TONE-1
U+106A    MYANMAR SIGN WESTERN PWO KAREN TONE-2
U+106B    MYANMAR SIGN WESTERN PWO KAREN TONE-3
U+106C    MYANMAR SIGN WESTERN PWO KAREN TONE-4
U+106D    MYANMAR SIGN WESTERN PWO KAREN TONE-5
U+1083    MYANMAR VOWEL SIGN SHAN AA
U+1084    MYANMAR VOWEL SIGN SHAN E
U+1087    MYANMAR SIGN SHAN TONE-2
U+1088    MYANMAR SIGN SHAN TONE-3
U+1089    MYANMAR SIGN SHAN TONE-5
U+108A    MYANMAR SIGN SHAN TONE-6
U+108B    MYANMAR SIGN SHAN COUNCIL TONE-2
U+108C    MYANMAR SIGN SHAN COUNCIL TONE-3
U+108F    MYANMAR SIGN RUMAI PALAUNG TONE-5
U+109A    MYANMAR SIGN KHAMTI TONE-1
U+109B    MYANMAR SIGN KHAMTI TONE-3
U+109C    MYANMAR VOWEL SIGN AITON A
U+17B6    KHMER VOWEL SIGN AA
U+17BE    KHMER VOWEL SIGN OE
U+17BF    KHMER VOWEL SIGN YA
U+17C0    KHMER VOWEL SIGN IE
U+17C1    KHMER VOWEL SIGN E
U+17C2    KHMER VOWEL SIGN AE
U+17C3    KHMER VOWEL SIGN AI
U+17C4    KHMER VOWEL SIGN OO
U+17C5    KHMER VOWEL SIGN AU
U+17C7    KHMER SIGN REAHMUK
U+17C8    KHMER SIGN YUUKALEAPINTU
U+1923    LIMBU VOWEL SIGN EE
U+1924    LIMBU VOWEL SIGN AI
U+1925    LIMBU VOWEL SIGN OO
U+1926    LIMBU VOWEL SIGN AU
U+1929    LIMBU SUBJOINED LETTER YA
U+192A    LIMBU SUBJOINED LETTER RA
U+192B    LIMBU SUBJOINED LETTER WA
U+1930    LIMBU SMALL LETTER KA
U+1931    LIMBU SMALL LETTER NGA
U+1933    LIMBU SMALL LETTER TA
U+1934    LIMBU SMALL LETTER NA
U+1935    LIMBU SMALL LETTER PA
U+1936    LIMBU SMALL LETTER MA
U+1937    LIMBU SMALL LETTER RA
U+1938    LIMBU SMALL LETTER LA
U+1A19    BUGINESE VOWEL SIGN E
U+1A1A    BUGINESE VOWEL SIGN O
U+1A55    TAI THAM CONSONANT SIGN MEDIAL RA
U+1A57    TAI THAM CONSONANT SIGN LA TANG LAI
U+1A61    TAI THAM VOWEL SIGN A
U+1A63    TAI THAM VOWEL SIGN AA
U+1A64    TAI THAM VOWEL SIGN TALL AA
U+1A6D    TAI THAM VOWEL SIGN OY
U+1A6E    TAI THAM VOWEL SIGN E
U+1A6F    TAI THAM VOWEL SIGN AE
U+1A70    TAI THAM VOWEL SIGN OO
U+1A71    TAI THAM VOWEL SIGN AI
U+1A72    TAI THAM VOWEL SIGN THAM AI
U+1B04    BALINESE SIGN BISAH
U+1B35    BALINESE VOWEL SIGN TEDUNG
U+1B3B    BALINESE VOWEL SIGN RA REPA TEDUNG
U+1B3D    BALINESE VOWEL SIGN LA LENGA TEDUNG
U+1B3E    BALINESE VOWEL SIGN TALING
U+1B3F    BALINESE VOWEL SIGN TALING REPA
U+1B40    BALINESE VOWEL SIGN TALING TEDUNG
U+1B41    BALINESE VOWEL SIGN TALING REPA TEDUNG
U+1B43    BALINESE VOWEL SIGN PEPET TEDUNG
U+1B44    BALINESE ADEG ADEG
U+1B82    SUNDANESE SIGN PANGWISAD
U+1BA1    SUNDANESE CONSONANT SIGN PAMINGKAL
U+1BA6    SUNDANESE VOWEL SIGN PANAELAENG
U+1BA7    SUNDANESE VOWEL SIGN PANOLONG
U+1BAA    SUNDANESE SIGN PAMAAEH
U+1BE7    BATAK VOWEL SIGN E
U+1BEA    BATAK VOWEL SIGN I
U+1BEB    BATAK VOWEL SIGN KARO I
U+1BEC    BATAK VOWEL SIGN O
U+1BEE    BATAK VOWEL SIGN U
U+1BF2    BATAK PANGOLAT
U+1BF3    BATAK PANONGONAN
U+1C24    LEPCHA SUBJOINED LETTER YA
U+1C25    LEPCHA SUBJOINED LETTER RA
U+1C26    LEPCHA VOWEL SIGN AA
U+1C27    LEPCHA VOWEL SIGN I
U+1C28    LEPCHA VOWEL SIGN O
U+1C29    LEPCHA VOWEL SIGN OO
U+1C2A    LEPCHA VOWEL SIGN U
U+1C2B    LEPCHA VOWEL SIGN UU
U+1C34    LEPCHA CONSONANT SIGN NYIN-DO
U+1C35    LEPCHA CONSONANT SIGN KANG
U+1CE1    VEDIC TONE ATHARVAVEDIC INDEPENDENT SVARITA
U+1CF2    VEDIC SIGN ARDHAVISARGA
U+1CF3    VEDIC SIGN ROTATED ARDHAVISARGA
U+302E    HANGUL SINGLE DOT TONE MARK
U+302F    HANGUL DOUBLE DOT TONE MARK
U+A823    SYLOTI NAGRI VOWEL SIGN A
U+A824    SYLOTI NAGRI VOWEL SIGN I
U+A827    SYLOTI NAGRI VOWEL SIGN OO
U+A880    SAURASHTRA SIGN ANUSVARA
U+A881    SAURASHTRA SIGN VISARGA
U+A8B4    SAURASHTRA CONSONANT SIGN HAARU
U+A8B5    SAURASHTRA VOWEL SIGN AA
U+A8B6    SAURASHTRA VOWEL SIGN I
U+A8B7    SAURASHTRA VOWEL SIGN II
U+A8B8    SAURASHTRA VOWEL SIGN U
U+A8B9    SAURASHTRA VOWEL SIGN UU
U+A8BA    SAURASHTRA VOWEL SIGN VOCALIC R
U+A8BB    SAURASHTRA VOWEL SIGN VOCALIC RR
U+A8BC    SAURASHTRA VOWEL SIGN VOCALIC L
U+A8BD    SAURASHTRA VOWEL SIGN VOCALIC LL
U+A8BE    SAURASHTRA VOWEL SIGN E
U+A8BF    SAURASHTRA VOWEL SIGN EE
U+A8C0    SAURASHTRA VOWEL SIGN AI
U+A8C1    SAURASHTRA VOWEL SIGN O
U+A8C2    SAURASHTRA VOWEL SIGN OO
U+A8C3    SAURASHTRA VOWEL SIGN AU
U+A952    REJANG CONSONANT SIGN H
U+A953    REJANG VIRAMA
U+A983    JAVANESE SIGN WIGNYAN
U+A9B4    JAVANESE VOWEL SIGN TARUNG
U+A9B5    JAVANESE VOWEL SIGN TOLONG
U+A9BA    JAVANESE VOWEL SIGN TALING
U+A9BB    JAVANESE VOWEL SIGN DIRGA MURE
U+A9BD    JAVANESE CONSONANT SIGN KERET
U+A9BE    JAVANESE CONSONANT SIGN PENGKAL
U+A9BF    JAVANESE CONSONANT SIGN CAKRA
U+A9C0    JAVANESE PANGKON
U+AA2F    CHAM VOWEL SIGN O
U+AA30    CHAM VOWEL SIGN AI
U+AA33    CHAM CONSONANT SIGN YA
U+AA34    CHAM CONSONANT SIGN RA
U+AA4D    CHAM CONSONANT SIGN FINAL H
U+AA7B    MYANMAR SIGN PAO KAREN TONE
U+AA7D    MYANMAR SIGN TAI LAING TONE-5
U+AAEB    MEETEI MAYEK VOWEL SIGN II
U+AAEE    MEETEI MAYEK VOWEL SIGN AU
U+AAEF    MEETEI MAYEK VOWEL SIGN AAU
U+AAF5    MEETEI MAYEK VOWEL SIGN VISARGA
U+ABE3    MEETEI MAYEK VOWEL SIGN ONAP
U+ABE4    MEETEI MAYEK VOWEL SIGN INAP
U+ABE6    MEETEI MAYEK VOWEL SIGN YENAP
U+ABE7    MEETEI MAYEK VOWEL SIGN SOUNAP
U+ABE9    MEETEI MAYEK VOWEL SIGN CHEINAP
U+ABEA    MEETEI MAYEK VOWEL SIGN NUNG
U+ABEC    MEETEI MAYEK LUM IYEK
U+11000    BRAHMI SIGN CANDRABINDU
U+11002    BRAHMI SIGN VISARGA
U+11082    KAITHI SIGN VISARGA
U+110B0    KAITHI VOWEL SIGN AA
U+110B1    KAITHI VOWEL SIGN I
U+110B2    KAITHI VOWEL SIGN II
U+110B7    KAITHI VOWEL SIGN O
U+110B8    KAITHI VOWEL SIGN AU
U+1112C    CHAKMA VOWEL SIGN E
U+11182    SHARADA SIGN VISARGA
U+111B3    SHARADA VOWEL SIGN AA
U+111B4    SHARADA VOWEL SIGN I
U+111B5    SHARADA VOWEL SIGN II
U+111BF    SHARADA VOWEL SIGN AU
U+111C0    SHARADA SIGN VIRAMA
U+1122C    KHOJKI VOWEL SIGN AA
U+1122D    KHOJKI VOWEL SIGN I
U+1122E    KHOJKI VOWEL SIGN II
U+11232    KHOJKI VOWEL SIGN O
U+11233    KHOJKI VOWEL SIGN AU
U+11235    KHOJKI SIGN VIRAMA
U+112E0    KHUDAWADI VOWEL SIGN AA
U+112E1    KHUDAWADI VOWEL SIGN I
U+112E2    KHUDAWADI VOWEL SIGN II
U+11302    GRANTHA SIGN ANUSVARA
U+11303    GRANTHA SIGN VISARGA
U+1133E    GRANTHA VOWEL SIGN AA
U+1133F    GRANTHA VOWEL SIGN I
U+11341    GRANTHA VOWEL SIGN U
U+11342    GRANTHA VOWEL SIGN UU
U+11343    GRANTHA VOWEL SIGN VOCALIC R
U+11344    GRANTHA VOWEL SIGN VOCALIC RR
U+11347    GRANTHA VOWEL SIGN EE
U+11348    GRANTHA VOWEL SIGN AI
U+1134B    GRANTHA VOWEL SIGN OO
U+1134C    GRANTHA VOWEL SIGN AU
U+1134D    GRANTHA SIGN VIRAMA
U+11357    GRANTHA AU LENGTH MARK
U+11362    GRANTHA VOWEL SIGN VOCALIC L
U+11363    GRANTHA VOWEL SIGN VOCALIC LL
U+114B0    TIRHUTA VOWEL SIGN AA
U+114B1    TIRHUTA VOWEL SIGN I
U+114B2    TIRHUTA VOWEL SIGN II
U+114B9    TIRHUTA VOWEL SIGN E
U+114BB    TIRHUTA VOWEL SIGN AI
U+114BC    TIRHUTA VOWEL SIGN O
U+114BD    TIRHUTA VOWEL SIGN SHORT O
U+114BE    TIRHUTA VOWEL SIGN AU
U+114C1    TIRHUTA SIGN VISARGA
U+115AF    SIDDHAM VOWEL SIGN AA
U+115B0    SIDDHAM VOWEL SIGN I
U+115B1    SIDDHAM VOWEL SIGN II
U+115B8    SIDDHAM VOWEL SIGN E
U+115B9    SIDDHAM VOWEL SIGN AI
U+115BA    SIDDHAM VOWEL SIGN O
U+115BB    SIDDHAM VOWEL SIGN AU
U+115BE    SIDDHAM SIGN VISARGA
U+11630    MODI VOWEL SIGN AA
U+11631    MODI VOWEL SIGN I
U+11632    MODI VOWEL SIGN II
U+1163B    MODI VOWEL SIGN O
U+1163C    MODI VOWEL SIGN AU
U+1163E    MODI SIGN VISARGA
U+116AC    TAKRI SIGN VISARGA
U+116AE    TAKRI VOWEL SIGN I
U+116AF    TAKRI VOWEL SIGN II
U+116B6    TAKRI SIGN VIRAMA
U+11720    AHOM VOWEL SIGN A
U+11721    AHOM VOWEL SIGN AA
U+11726    AHOM VOWEL SIGN E
U+16F51    MIAO SIGN ASPIRATION
U+16F52    MIAO SIGN REFORMED VOICING
U+16F53    MIAO SIGN REFORMED ASPIRATION
U+16F54    MIAO VOWEL SIGN A
U+16F55    MIAO VOWEL SIGN AA
U+16F56    MIAO VOWEL SIGN AHH
U+16F57    MIAO VOWEL SIGN AN
U+16F58    MIAO VOWEL SIGN ANG
U+16F59    MIAO VOWEL SIGN O
U+16F5A    MIAO VOWEL SIGN OO
U+16F5B    MIAO VOWEL SIGN WO
U+16F5C    MIAO VOWEL SIGN W
U+16F5D    MIAO VOWEL SIGN E
U+16F5E    MIAO VOWEL SIGN EN
U+16F5F    MIAO VOWEL SIGN ENG
U+16F60    MIAO VOWEL SIGN OEY
U+16F61    MIAO VOWEL SIGN I
U+16F62    MIAO VOWEL SIGN IA
U+16F63    MIAO VOWEL SIGN IAN
U+16F64    MIAO VOWEL SIGN IANG
U+16F65    MIAO VOWEL SIGN IO
U+16F66    MIAO VOWEL SIGN IE
U+16F67    MIAO VOWEL SIGN II
U+16F68    MIAO VOWEL SIGN IU
U+16F69    MIAO VOWEL SIGN ING
U+16F6A    MIAO VOWEL SIGN U
U+16F6B    MIAO VOWEL SIGN UA
U+16F6C    MIAO VOWEL SIGN UAN
U+16F6D    MIAO VOWEL SIGN UANG
U+16F6E    MIAO VOWEL SIGN UU
U+16F6F    MIAO VOWEL SIGN UEI
U+16F70    MIAO VOWEL SIGN UNG
U+16F71    MIAO VOWEL SIGN Y
U+16F72    MIAO VOWEL SIGN YI
U+16F73    MIAO VOWEL SIGN AE
U+16F74    MIAO VOWEL SIGN AEE
U+16F75    MIAO VOWEL SIGN ERR
U+16F76    MIAO VOWEL SIGN ROUNDED ERR
U+16F77    MIAO VOWEL SIGN ER
U+16F78    MIAO VOWEL SIGN ROUNDED ER
U+16F79    MIAO VOWEL SIGN AI
U+16F7A    MIAO VOWEL SIGN EI
U+16F7B    MIAO VOWEL SIGN AU
U+16F7C    MIAO VOWEL SIGN OU
U+16F7D    MIAO VOWEL SIGN N
U+16F7E    MIAO VOWEL SIGN NG
U+1D165    MUSICAL SYMBOL COMBINING STEM
U+1D166    MUSICAL SYMBOL COMBINING SPRECHGESANG STEM
U+1D16D    MUSICAL SYMBOL COMBINING AUGMENTATION DOT
U+1D16E    MUSICAL SYMBOL COMBINING FLAG-1
U+1D16F    MUSICAL SYMBOL COMBINING FLAG-2
U+1D170    MUSICAL SYMBOL COMBINING FLAG-3
U+1D171    MUSICAL SYMBOL COMBINING FLAG-4
U+1D172    MUSICAL SYMBOL COMBINING FLAG-5
*/
};

}

#endif // UNICODE_MARK_SPACING_COMBINING_HPP_INCLUDED
