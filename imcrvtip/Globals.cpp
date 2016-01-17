
#include <initguid.h>

#include "configxml.h"
#include "immanatip.h"

HINSTANCE g_hInst;

LPCWSTR TextServiceDesc = TEXTSERVICE_DESC;
LPCWSTR LangbarItemDesc = L"ver. " TEXTSERVICE_VER;
LPCWSTR CandidateWindowClass = TEXTSERVICE_NAME L"CandidateWindow";
LPCWSTR InputModeWindowClass = TEXTSERVICE_NAME L"InputModeWindow";

#ifndef _DEBUG

// {3CCDA3BE-05AA-4493-8DBE-32404E4E1EB9}
const GUID c_clsidTextService =
{ 0x3ccda3be, 0x5aa, 0x4493, { 0x8d, 0xbe, 0x32, 0x40, 0x4e, 0x4e, 0x1e, 0xb9 } };

// {92F2D7A2-8A42-4E5B-B4A5-CE86C72CDC08}
const GUID c_guidProfile =
{ 0x92f2d7a2, 0x8a42, 0x4e5b, { 0xb4, 0xa5, 0xce, 0x86, 0xc7, 0x2c, 0xdc, 0x8 } };

// {D688469C-17BE-4DD2-AB83-F34EF9081515}
const GUID c_guidPreservedKeyOn =
{ 0xd688469c, 0x17be, 0x4dd2, { 0xab, 0x83, 0xf3, 0x4e, 0xf9, 0x8, 0x15, 0x15 } };

// {DB0BB088-D38D-4AA1-AF68-0D704082AA32}
const GUID c_guidPreservedKeyOff =
{ 0xdb0bb088, 0xd38d, 0x4aa1, { 0xaf, 0x68, 0xd, 0x70, 0x40, 0x82, 0xaa, 0x32 } };

// {A18E8D81-C613-447A-B769-40ACB039F744}
const GUID c_guidLangBarItemButton =
{ 0xa18e8d81, 0xc613, 0x447a, { 0xb7, 0x69, 0x40, 0xac, 0xb0, 0x39, 0xf7, 0x44 } };

// {C19A92CE-4E92-4BE0-A470-F65EC5FC771D}
const GUID c_guidCandidateListUIElement =
{ 0xc19a92ce, 0x4e92, 0x4be0, { 0xa4, 0x70, 0xf6, 0x5e, 0xc5, 0xfc, 0x77, 0x1d } };

// {97CDCEA6-DFA1-4504-ADEA-5B66F34B0775}
const GUID c_guidDisplayAttributeInputMark =
{ 0x97cdcea6, 0xdfa1, 0x4504, { 0xad, 0xea, 0x5b, 0x66, 0xf3, 0x4b, 0x7, 0x75 } };

// {D8B86B68-5885-425C-84D0-BBDC04FC3924}
const GUID c_guidDisplayAttributeInputText =
{ 0xd8b86b68, 0x5885, 0x425c, { 0x84, 0xd0, 0xbb, 0xdc, 0x4, 0xfc, 0x39, 0x24 } };

// {A9B57FEB-C462-4B12-92C0-7AFE82A49808}
const GUID c_guidDisplayAttributeInputOkuri =
{ 0xa9b57feb, 0xc462, 0x4b12, { 0x92, 0xc0, 0x7a, 0xfe, 0x82, 0xa4, 0x98, 0x8 } };

// {C6C01724-BE6B-48DE-982B-BE7E64390689}
const GUID c_guidDisplayAttributeConvMark =
{ 0xc6c01724, 0xbe6b, 0x48de, { 0x98, 0x2b, 0xbe, 0x7e, 0x64, 0x39, 0x6, 0x89 } };

// {77807CA9-A995-4D2F-A00B-51398FFBAC03}
const GUID c_guidDisplayAttributeConvText =
{ 0x77807ca9, 0xa995, 0x4d2f, { 0xa0, 0xb, 0x51, 0x39, 0x8f, 0xfb, 0xac, 0x3 } };

// {2A28DD42-F0C5-4197-BACD-0B1E98120EE2}
const GUID c_guidDisplayAttributeConvOkuri =
{ 0x2a28dd42, 0xf0c5, 0x4197, { 0xba, 0xcd, 0xb, 0x1e, 0x98, 0x12, 0xe, 0xe2 } };

// {332D7DB3-7FCB-479E-AD44-7D16F4F3ACD3}
const GUID c_guidDisplayAttributeConvAnnot =
{ 0x332d7db3, 0x7fcb, 0x479e, { 0xad, 0x44, 0x7d, 0x16, 0xf4, 0xf3, 0xac, 0xd3 } };

#else

// {4D97960C-1D59-4466-BEFE-4C1328D2550D}
const GUID c_clsidTextService =
{0x4d97960c, 0x1d59, 0x4466, {0xbe, 0xfe, 0x4c, 0x13, 0x28, 0xd2, 0x55, 0x0d}};

// {820E9894-024B-4bd1-98AF-3942B772CFF1}
const GUID c_guidProfile =
{0x820e9894, 0x024b, 0x4bd1, {0x98, 0xaf, 0x39, 0x42, 0xb7, 0x72, 0xcf, 0xf1}};

// {4D10F34B-5C4E-4121-8795-C480110A644B}
const GUID c_guidPreservedKeyOn =
{0x4d10f34b, 0x5c4e, 0x4121, {0x87, 0x95, 0xc4, 0x80, 0x11, 0x0a, 0x64, 0x4b}};

// {6B6743B3-2D6F-4BB9-9D74-7007928C1E94}
const GUID c_guidPreservedKeyOff =
{0x6b6743b3, 0x2d6f, 0x4bb9, {0x9d, 0x74, 0x70, 0x07, 0x92, 0x8c, 0x1e, 0x94}};

// {F4BF0D3C-D4CE-456f-837E-FE6712C6A8C3}
const GUID c_guidLangBarItemButton =
{0xf4bf0d3c, 0xd4ce, 0x456f, {0x83, 0x7e, 0xfe, 0x67, 0x12, 0xc6, 0xa8, 0xc3}};

// {25A6388F-D3CB-4866-A2C3-94E00970BF45}
const GUID c_guidCandidateListUIElement =
{0x25a6388f, 0xd3cb, 0x4866, {0xa2, 0xc3, 0x94, 0xe0, 0x09, 0x70, 0xbf, 0x45}};

// {CB22C53A-AD57-485A-A6CF-20390A0D5098}
const GUID c_guidDisplayAttributeInputMark =
{0xcb22c53a, 0xad57, 0x485a, {0xa6, 0xcf, 0x20, 0x39, 0x0a, 0x0d, 0x50, 0x98}};

// {6F99E3F1-36AC-4015-B334-211CFFCB3262}
const GUID c_guidDisplayAttributeInputText =
{0x6f99e3f1, 0x36ac, 0x4015, {0xb3, 0x34, 0x21, 0x1c, 0xff, 0xcb, 0x32, 0x62}};

// {D2176C6C-8758-40C6-9612-5832FA315879}
const GUID c_guidDisplayAttributeInputOkuri =
{0xd2176c6c, 0x8758, 0x40c6, {0x96, 0x12, 0x58, 0x32, 0xfa, 0x31, 0x58, 0x79}};

// {B564E740-166D-45B1-AF44-7CCC7F75A807}
const GUID c_guidDisplayAttributeConvMark =
{0xb564e740, 0x166d, 0x45b1, {0xaf, 0x44, 0x7c, 0xcc, 0x7f, 0x75, 0xa8, 0x07}};

// {6877D302-1C51-4ba4-9329-2F80B5E3A4E7}
const GUID c_guidDisplayAttributeConvText =
{0x6877d302, 0x1c51, 0x4ba4, {0x93, 0x29, 0x2f, 0x80, 0xb5, 0xe3, 0xa4, 0xe7}};

// {F99304F1-9F91-439E-8446-6FE0F8A98EDC}
const GUID c_guidDisplayAttributeConvOkuri =
{0xf99304f1, 0x9f91, 0x439e, {0x84, 0x46, 0x6f, 0xe0, 0xf8, 0xa9, 0x8e, 0xdc}};

// {C6040719-6FF3-4b92-A589-36E93BFD53EC}
const GUID c_guidDisplayAttributeConvAnnot =
{0xc6040719, 0x6ff3, 0x4b92, {0xa5, 0x89, 0x36, 0xe9, 0x3b, 0xfd, 0x53, 0xec}};

#endif

LPCWSTR markNo = L":";
LPCWSTR markAnnotation = L";";
LPCWSTR markCandEnd = L"\u3000";
LPCWSTR markCursor = L"|";
LPCWSTR markReg = L"登録";
LPCWSTR markRegL = L"[";
LPCWSTR markRegR = L"]";
LPCWSTR markRegKeyEnd = L"：";
LPCWSTR markSP = L"\x20";
LPCWSTR markNBSP = L"\u00A0";
LPCWSTR markMidashi = L"▽";
LPCWSTR markHenkan = L"▼";
LPCWSTR markOkuri = L"*";

const DISPLAYATTRIBUTE_INFO c_gdDisplayAttributeInfo[DISPLAYATTRIBUTE_INFO_NUM] =
{
	{
		ValueDisplayAttrInputMark, c_guidDisplayAttributeInputMark,
		c_daDisplayAttributeSeries[0], c_daDisplayAttributeInputMark
	},
	{
		ValueDisplayAttrInputText, c_guidDisplayAttributeInputText,
		c_daDisplayAttributeSeries[1], c_daDisplayAttributeInputText
	},
	{
		ValueDisplayAttrInputOkuri, c_guidDisplayAttributeInputOkuri,
		c_daDisplayAttributeSeries[2], c_daDisplayAttributeInputOkuri
	},
	{
		ValueDisplayAttrConvMark, c_guidDisplayAttributeConvMark,
		c_daDisplayAttributeSeries[3], c_daDisplayAttributeConvMark
	},
	{
		ValueDisplayAttrConvText, c_guidDisplayAttributeConvText,
		c_daDisplayAttributeSeries[4], c_daDisplayAttributeConvText
	},
	{
		ValueDisplayAttrConvOkuri, c_guidDisplayAttributeConvOkuri,
		c_daDisplayAttributeSeries[5], c_daDisplayAttributeConvOkuri
	},
	{
		ValueDisplayAttrConvAnnot, c_guidDisplayAttributeConvAnnot,
		c_daDisplayAttributeSeries[6], c_daDisplayAttributeConvAnnot
	}
};

// added in Windows 8 SDK
#ifndef _WIN32_WINNT_WIN8

const GUID GUID_TFCAT_TIPCAP_IMMERSIVESUPPORT =
{0x13A016DF, 0x560B, 0x46CD, {0x94, 0x7A, 0x4C, 0x3A, 0xF1, 0xE0, 0xE3, 0x5D}};

const GUID GUID_TFCAT_TIPCAP_SYSTRAYSUPPORT =
{0x25504FB4, 0x7BAB, 0x4BC1, {0x9C, 0x69, 0xCF, 0x81, 0x89, 0x0F, 0x0E, 0xF5}};

const GUID GUID_LBI_INPUTMODE =
{0x2C77A81E, 0x41CC, 0x4178, {0xA3, 0xA7, 0x5F, 0x8A, 0x98, 0x75, 0x68, 0xE6}};

const IID IID_ITfFnGetPreferredTouchKeyboardLayout =
{0x5F309A41, 0x590A, 0x4ACC, {0xA9, 0x7F, 0xD8, 0xEF, 0xFF, 0x13, 0xFD, 0xFC}};

#endif //_WIN32_WINNT_WIN8
