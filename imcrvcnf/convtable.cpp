
#include "convtable.h"

// ローマ字仮名変換表

const ROMAN_KANA_CONV roman_kana_conv_default[ROMAN_KANA_TBL_DEF_NUM] =
{
	{L"a",  L"아"}, {L"i",  L"이"}, {L"u",  L"우"}, {L"e",  L"에"}, {L"o",  L"오"},
	{L"pa", L"빠"}, {L"pi", L"삐"}, {L"pu", L"뿌"}, {L"pe", L"뻬"}, {L"po", L"뽀"},
	{L"ba", L"바"}, {L"bi", L"비"}, {L"bu", L"부"}, {L"be", L"베"}, {L"bo", L"보"},
	{L"va", L"바"}, {L"vi", L"비"}, {L"vu", L"부"}, {L"ve", L"베"}, {L"vo", L"보"},
	{L"fa", L"파"}, {L"fi", L"피"}, {L"fu", L"푸"}, {L"fe", L"페"}, {L"fo", L"포"},
	{L"ma", L"마"}, {L"mi", L"미"}, {L"mu", L"무"}, {L"me", L"메"}, {L"mo", L"모"},
	{L"ta", L"타"}, {L"ti", L"티"}, {L"tu", L"투"}, {L"te", L"테"}, {L"to", L"토"},
	{L"da", L"다"}, {L"di", L"디"}, {L"du", L"두"}, {L"de", L"데"}, {L"do", L"도"},
	{L"tsa", L"짜"}, {L"tsi", L"찌"}, {L"tsu", L"쭈"}, {L"tse", L"쩨"}, {L"tso", L"쪼"},
	{L"za", L"자"}, {L"zi", L"지"}, {L"zu", L"주"}, {L"ze", L"제"}, {L"zo", L"조"},
	{L"sa", L"사"}, {L"si", L"시"}, {L"su", L"수"}, {L"se", L"세"}, {L"so", L"소"},
	{L"swa", L"따"}, {L"swi", L"띠"},                  {L"swe", L"떼"}, {L"swo", L"또"},
	{L"txa", L"차"}, {L"txi", L"치"}, {L"txu", L"추"}, {L"txe", L"체"}, {L"txo", L"초"},
	{L"ja", L"까"}, {L"ji", L"끼"}, {L"ju", L"꾸"}, {L"je", L"께"}, {L"jo", L"꼬"},
	{L"xa", L"싸"}, {L"xi", L"씨"}, {L"xu", L"쑤"}, {L"xe", L"쎄"}, {L"xo", L"쏘"},
	{L"na", L"나"}, {L"ni", L"니"}, {L"nu", L"누"}, {L"ne", L"네"}, {L"no", L"노"},
	{L"la", L"라"}, {L"li", L"리"}, {L"lu", L"루"}, {L"le", L"레"}, {L"lo", L"로"},
	{L"ka", L"카"}, {L"ki", L"키"}, {L"ku", L"쿠"}, {L"ke", L"케"}, {L"ko", L"코"},
	{L"ga", L"가"}, {L"gi", L"기"}, {L"gu", L"구"}, {L"ge", L"게"}, {L"go", L"고"},
	{L"ha", L"하"}, {L"hi", L"히"}, {L"hu", L"후"}, {L"he", L"헤"}, {L"ho", L"호"},
	{L"wa", L"와"},	{L"wi", L"의"},                 {L"we", L"웨"}, {L"wo", L"워"},

	{L"wu", L"우"},
	{L"swu", L"수"},

	{L"ya", L"야"}, {L"yu", L"위"}, {L"ye", L"예"}, {L"yo", L"요"},
	{L"pya", L"뺘"}, {L"pyu", L"쀠"}, {L"pye", L"뼤"}, {L"pyo", L"뾰"},
	{L"bya", L"뱌"}, {L"byu", L"뷔"}, {L"bye", L"볘"}, {L"byo", L"뵤"},
	{L"vya", L"뱌"}, {L"vyu", L"뷔"}, {L"vye", L"볘"}, {L"vyo", L"뵤"},
	{L"fya", L"퍄"}, {L"fyu", L"퓌"}, {L"fye", L"폐"}, {L"fyo", L"표"},
	{L"mya", L"먀"}, {L"myu", L"뮈"}, {L"mye", L"몌"}, {L"myo", L"묘"},
	{L"tya", L"탸"}, {L"tyu", L"튀"}, {L"tye", L"톄"}, {L"tyo", L"툐"},
	{L"dya", L"댜"}, {L"dyu", L"뒤"}, {L"dye", L"뎨"}, {L"dyo", L"됴"},
	{L"tsya", L"쨔"}, {L"tsyu", L"쮜"}, {L"tsye", L"쪠"}, {L"tsyo", L"쬬"},
	{L"zya", L"쟈"}, {L"zyu", L"쥐"}, {L"zye", L"졔"}, {L"zyo", L"죠"},
	{L"sya", L"샤"}, {L"syu", L"쉬"}, {L"sye", L"셰"}, {L"syo", L"쇼"},
	{L"txya", L"챠"}, {L"txyu", L"취"}, {L"txye", L"쳬"}, {L"txyo", L"쵸"},
	{L"jya", L"꺄"}, {L"jyu", L"뀌"}, {L"jye", L"꼐"}, {L"jyo", L"꾜"},
	{L"xya", L"쌰"}, {L"xyu", L"쒸"}, {L"xye", L"쎼"}, {L"xyo", L"쑈"},
	{L"nya", L"냐"}, {L"nyu", L"뉘"}, {L"nye", L"녜"}, {L"nyo", L"뇨"},
	{L"lya", L"랴"}, {L"lyu", L"뤼"}, {L"lye", L"례"}, {L"lyo", L"료"},
	{L"kya", L"캬"}, {L"kyu", L"퀴"}, {L"kye", L"켸"}, {L"kyo", L"쿄"},
	{L"gya", L"갸"}, {L"gyu", L"귀"}, {L"gye", L"계"}, {L"gyo", L"교"},
	{L"hya", L"햐"}, {L"hyu", L"휘"}, {L"hye", L"혜"}, {L"hyo", L"효"},

	{L"n", L"은"},
	{L"nn", L"은"},
	{L"k",  L"읏"},
	{L"kk", L"읏"},

	//数字
	{L"0", L"0"}, {L"1", L"1"},
	{L"2", L"2"}, {L"3", L"3"},
	{L"4", L"4"}, {L"5", L"5"},
	{L"6", L"6"}, {L"7", L"7"},
	{L"8", L"8"}, {L"9", L"9"},

	//記号
	{L"\x20", L"\x20"},
	{L"!", L"！"},		//full-width
	{L"\"", L"\""},
	{L"#", L"#"},
	{L"$", L"$"},
	{L"%", L"%"},
	{L"&", L"&"},
	{L"'", L"'"},
	{L"(", L"("},
	{L")", L")"},
	{L"*", L"*"},
	{L"+", L"+"},
	{L",", L"、"},		//full-width, half width katakana
	{L"-", L"ー"},		//full-width, half width katakana
	{L".", L"。"},		//full-width, half width katakana
	{L"/", L"/"},
	//   [0-9]   //
	{L":", L":"},
	{L";", L";"},
	{L"<", L"<"},
	{L"=", L"="},
	{L">", L">"},
	{L"?", L"？"},		//full-width
	{L"@", L"@"},
	//   [A-Z]   //
	{L"[", L"「"},		//full-width, half width katakana
	{L"\\", L"\\"},
	{L"]", L"」"},		//full-width, half width katakana
	{L"^", L"^"},
	{L"_", L"_"},
	{L"`", L"`"},
	//   [a-z]   //
	{L"{", L"{"},
	{L"|", L"|"},
	{L"}", L"}"},
	{L"~", L"~"},

	{L"",L""}
};
