﻿
#include "configxml.h"
#include "immanacnf.h"
#include "convtable.h"
#include "resource.h"

#define CONFKANALEN 0x100

WCHAR conv_point[CONV_POINT_NUM][3][2];
std::vector<ROMAN_KANA_CONV> roman_kana_conv;
TF_PRESERVEDKEY preservedkey[PRESERVEDKEY_NUM][MAX_PRESERVEDKEY];
static const struct {
	int id;
	LPCWSTR section;
} preservedkeySectionInfo[PRESERVEDKEY_NUM] = {
	{IDC_LIST_PRSRVKEY_ON, SectionPreservedKeyON},
	{IDC_LIST_PRSRVKEY_OFF, SectionPreservedKeyOFF},
};

void LoadCheckButton(HWND hDlg, int nIDDlgItem, LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault)
{
	std::wstring strxmlval;

	ReadValue(pathconfigxml, lpAppName, lpKeyName, strxmlval, lpDefault);
	CheckDlgButton(hDlg, nIDDlgItem, (_wtoi(strxmlval.c_str()) == TRUE ? BST_CHECKED : BST_UNCHECKED));
}

void SaveCheckButton(HWND hDlg, int nIDDlgItem, LPCWSTR lpKeyName)
{
	WCHAR num[2];

	num[0] = L'0' + IsDlgButtonChecked(hDlg, nIDDlgItem);
	num[1] = L'\0';
	WriterKey(pXmlWriter, lpKeyName, num);
}

void LoadKeyMap(HWND hDlg, int nIDDlgItem, LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault)
{
	std::wstring strxmlval;
	LPCWSTR lpDefVal = L"\uFFFF";

	ReadValue(pathconfigxml, lpAppName, lpKeyName, strxmlval, lpDefVal);
	if(strxmlval == lpDefVal) strxmlval = lpDefault;
	SetDlgItemTextW(hDlg, nIDDlgItem, strxmlval.c_str());
}

void SaveKeyMap(HWND hDlg, int nIDDlgItem, LPCWSTR lpKeyName)
{
	WCHAR keyre[KEYRELEN];

	GetDlgItemTextW(hDlg, nIDDlgItem, keyre, _countof(keyre));
	WriterKey(pXmlWriter, lpKeyName, keyre);
}

void SetConfigPreservedKeyONOFF(int onoff, const APPDATAXMLLIST &list)
{
	if(onoff != 0 && onoff != 1)
	{
		return;
	}

	ZeroMemory(preservedkey[onoff], sizeof(preservedkey[onoff]));

	if(list.size() != 0)
	{
		int i = 0;
		FORWARD_ITERATION_I(l_itr, list)
		{
			if(i >= MAX_PRESERVEDKEY)
			{
				break;
			}

			FORWARD_ITERATION_I(r_itr, *l_itr)
			{
				if(r_itr->first == AttributeVKey)
				{
					preservedkey[onoff][i].uVKey = wcstoul(r_itr->second.c_str(), NULL, 0);
				}
				else if(r_itr->first == AttributeMKey)
				{
					preservedkey[onoff][i].uModifiers = wcstoul(r_itr->second.c_str(), NULL, 0);
					if(preservedkey[onoff][i].uModifiers == 0)
					{
						preservedkey[onoff][i].uModifiers = TF_MOD_IGNORE_ALL_MODIFIER;
					}
				}
			}

			i++;
		}
	}
	else
	{
		preservedkey[onoff][0].uVKey = VK_OEM_3/*0xC0*/;
		preservedkey[onoff][0].uModifiers = TF_MOD_ALT;
		preservedkey[onoff][1].uVKey = VK_KANJI/*0x19*/;
		preservedkey[onoff][1].uModifiers = TF_MOD_IGNORE_ALL_MODIFIER;
		preservedkey[onoff][2].uVKey = VK_OEM_AUTO/*0xF3*/;
		preservedkey[onoff][2].uModifiers = TF_MOD_IGNORE_ALL_MODIFIER;
		preservedkey[onoff][3].uVKey = VK_OEM_ENLW/*0xF4*/;
		preservedkey[onoff][3].uModifiers = TF_MOD_IGNORE_ALL_MODIFIER;
	}
}

void LoadConfigPreservedKey()
{
	APPDATAXMLLIST list;

	//for compatibility
	HRESULT hr = ReadList(pathconfigxml, SectionPreservedKey, list);

	if(hr == S_OK && list.size() != 0)
	{
		for(int k = 0; k < PRESERVEDKEY_NUM; k++)
		{
			SetConfigPreservedKeyONOFF(k, list);
		}
	}
	else
	{
		for(int k = 0; k < PRESERVEDKEY_NUM; k++)
		{
			list.clear();
			hr = ReadList(pathconfigxml, preservedkeySectionInfo[k].section, list);
			SetConfigPreservedKeyONOFF(k, list);
		}
	}
}

void LoadPreservedKey(HWND hwnd)
{
	LVITEMW item;
	WCHAR num[8];

	LoadConfigPreservedKey();

	for(int k = 0; k < PRESERVEDKEY_NUM; k++)
	{
		HWND hWndListView = GetDlgItem(hwnd, preservedkeySectionInfo[k].id);

		for(int i = 0; i < MAX_PRESERVEDKEY; i++)
		{
			if(preservedkey[k][i].uVKey == 0 &&
				preservedkey[k][i].uModifiers == 0)
			{
				break;
			}

			item.mask = LVIF_TEXT;
			_snwprintf_s(num, _TRUNCATE, L"0x%02X", preservedkey[k][i].uVKey);
			item.pszText = num;
			item.iItem = i;
			item.iSubItem = 0;
			ListView_InsertItem(hWndListView, &item);
			_snwprintf_s(num, _TRUNCATE, L"%d", preservedkey[k][i].uModifiers & TF_MOD_ALT ? 1 : 0);
			item.pszText = num;
			item.iItem = i;
			item.iSubItem = 1;
			ListView_SetItem(hWndListView, &item);
			_snwprintf_s(num, _TRUNCATE, L"%d", preservedkey[k][i].uModifiers & TF_MOD_CONTROL ? 1 : 0);
			item.pszText = num;
			item.iItem = i;
			item.iSubItem = 2;
			ListView_SetItem(hWndListView, &item);
			_snwprintf_s(num, _TRUNCATE, L"%d", preservedkey[k][i].uModifiers & TF_MOD_SHIFT ? 1 : 0);
			item.pszText = num;
			item.iItem = i;
			item.iSubItem = 3;
			ListView_SetItem(hWndListView, &item);
		}
	}
}

void SavePreservedKey(HWND hwnd)
{
	APPDATAXMLLIST list;
	APPDATAXMLROW row;
	APPDATAXMLATTR attr;
	WCHAR key[8];

	for(int k = 0; k < PRESERVEDKEY_NUM; k++)
	{
		list.clear();

		HWND hWndListView = GetDlgItem(hwnd, preservedkeySectionInfo[k].id);
		int count = ListView_GetItemCount(hWndListView);

		for(int i = 0; i < count && i < MAX_PRESERVEDKEY; i++)
		{
			ListView_GetItemText(hWndListView, i, 0, key, _countof(key));
			preservedkey[k][i].uVKey = wcstoul(key, NULL, 0);
			preservedkey[k][i].uModifiers = 0;
			ListView_GetItemText(hWndListView, i, 1, key, _countof(key));
			if(key[0] == L'1')
			{
				preservedkey[k][i].uModifiers |= TF_MOD_ALT;
			}
			ListView_GetItemText(hWndListView, i, 2, key, _countof(key));
			if(key[0] == L'1')
			{
				preservedkey[k][i].uModifiers |= TF_MOD_CONTROL;
			}
			ListView_GetItemText(hWndListView, i, 3, key, _countof(key));
			if(key[0] == L'1')
			{
				preservedkey[k][i].uModifiers |= TF_MOD_SHIFT;
			}
		}
		if(count < MAX_PRESERVEDKEY)
		{
			preservedkey[k][count].uVKey = 0;
			preservedkey[k][count].uModifiers = 0;
		}

		WriterStartSection(pXmlWriter, preservedkeySectionInfo[k].section);	//Start of SectionPreservedKey(ON|OFF)

		for(int i = 0; i < MAX_PRESERVEDKEY; i++)
		{
			if(preservedkey[k][i].uVKey == 0 &&
				preservedkey[k][i].uModifiers == 0)
			{
				break;
			}

			attr.first = AttributeVKey;
			_snwprintf_s(key, _TRUNCATE, L"0x%02X", preservedkey[k][i].uVKey);
			attr.second = key;
			row.push_back(attr);

			attr.first = AttributeMKey;
			_snwprintf_s(key, _TRUNCATE, L"%X", preservedkey[k][i].uModifiers);
			attr.second = key;
			row.push_back(attr);

			list.push_back(row);
			row.clear();
		}

		WriterList(pXmlWriter, list);

		WriterEndSection(pXmlWriter);	//End of SectionPreservedKey(ON|OFF)
	}
}

void LoadConfigConvPoint()
{
	APPDATAXMLLIST list;

	ZeroMemory(conv_point, sizeof(conv_point));

	HRESULT hr = ReadList(pathconfigxml, SectionConvPoint, list);

	if(hr == S_OK && list.size() != 0)
	{
		int i = 0;
		FORWARD_ITERATION_I(l_itr, list)
		{
			if(i >= CONV_POINT_NUM)
			{
				break;
			}

			FORWARD_ITERATION_I(r_itr, *l_itr)
			{
				if(r_itr->first == AttributeCPStart)
				{
					conv_point[i][0][0] = r_itr->second.c_str()[0];
				}
				else if(r_itr->first == AttributeCPAlter)
				{
					conv_point[i][1][0] = r_itr->second.c_str()[0];
				}
				else if(r_itr->first == AttributeCPOkuri)
				{
					conv_point[i][2][0] = r_itr->second.c_str()[0];
				}
			}

			i++;
		}
	}
	else if(hr != S_OK)
	{
		for(int i = 0; i < 26; i++)
		{
			conv_point[i][0][0] = L'A' + (WCHAR)i;
			conv_point[i][1][0] = L'a' + (WCHAR)i;
			conv_point[i][2][0] = L'a' + (WCHAR)i;
		}
	}
}

void LoadConvPoint(HWND hwnd)
{
	LVITEMW item;

	LoadConfigConvPoint();

	HWND hWndListView = GetDlgItem(hwnd, IDC_LIST_CONVPOINT);

	for(int i = 0; i < CONV_POINT_NUM; i++)
	{
		if(conv_point[i][0][0] == L'\0' &&
			conv_point[i][1][0] == L'\0' &&
			conv_point[i][2][0] == L'\0')
		{
			break;
		}

		item.mask = LVIF_TEXT;
		item.pszText = conv_point[i][0];
		item.iItem = i;
		item.iSubItem = 0;
		ListView_InsertItem(hWndListView, &item);
		item.pszText = conv_point[i][1];
		item.iItem = i;
		item.iSubItem = 1;
		ListView_SetItem(hWndListView, &item);
		item.pszText = conv_point[i][2];
		item.iItem = i;
		item.iSubItem = 2;
		ListView_SetItem(hWndListView, &item);
	}
}

void SaveConvPoint(HWND hwnd)
{
	APPDATAXMLLIST list;
	APPDATAXMLROW row;
	APPDATAXMLATTR attr;
	WCHAR key[2];

	HWND hWndListView = GetDlgItem(hwnd, IDC_LIST_CONVPOINT);
	int count = ListView_GetItemCount(hWndListView);

	for(int i = 0; i < count && i < CONV_POINT_NUM; i++)
	{
		ListView_GetItemText(hWndListView, i, 0, key, _countof(key));
		wcsncpy_s(conv_point[i][0], key, _TRUNCATE);
		ListView_GetItemText(hWndListView, i, 1, key, _countof(key));
		wcsncpy_s(conv_point[i][1], key, _TRUNCATE);
		ListView_GetItemText(hWndListView, i, 2, key, _countof(key));
		wcsncpy_s(conv_point[i][2], key, _TRUNCATE);
	}
	if(count < CONV_POINT_NUM)
	{
		conv_point[count][0][0] = L'\0';
		conv_point[count][1][0] = L'\0';
		conv_point[count][2][0] = L'\0';
	}

	WriterStartSection(pXmlWriter, SectionConvPoint);	//Start of SectionConvPoint

	for(int i = 0; i < CONV_POINT_NUM; i++)
	{
		if(conv_point[i][0][0] == L'\0' &&
			conv_point[i][1][0] == L'\0' &&
			conv_point[i][2][0] == L'\0')
		{
			break;
		}

		attr.first = AttributeCPStart;
		attr.second = conv_point[i][0];
		row.push_back(attr);

		attr.first = AttributeCPAlter;
		attr.second = conv_point[i][1];
		row.push_back(attr);

		attr.first = AttributeCPOkuri;
		attr.second = conv_point[i][2];
		row.push_back(attr);

		list.push_back(row);
		row.clear();
	}

	WriterList(pXmlWriter, list);

	WriterEndSection(pXmlWriter);	//End of SectionConvPoint
}

void LoadConfigKana()
{
	APPDATAXMLLIST list;
	ROMAN_KANA_CONV rkc;

	roman_kana_conv.clear();
	roman_kana_conv.shrink_to_fit();

	HRESULT hr = ReadList(pathconfigxml, SectionKana, list);

	if(hr == S_OK && list.size() != 0)
	{
		int i = 0;
		FORWARD_ITERATION_I(l_itr, list)
		{
			if(i >= ROMAN_KANA_TBL_MAX)
			{
				break;
			}

			ZeroMemory(&rkc, sizeof(rkc));

			FORWARD_ITERATION_I(r_itr, *l_itr)
			{
				WCHAR *pszb = NULL;
				size_t blen = 0;

				if(r_itr->first == AttributeRoman)
				{
					pszb = rkc.hacm;
					blen = _countof(rkc.hacm);
				}
				else if(r_itr->first == AttributeHiragana)
				{
					pszb = rkc.yula;
					blen = _countof(rkc.yula);
				}
				else if(r_itr->first == AttributeSpOp)
				{
					rkc.soku = (_wtoi(r_itr->second.c_str()) & 0x1) ? TRUE : FALSE;
					rkc.wait = (_wtoi(r_itr->second.c_str()) & 0x2) ? TRUE : FALSE;
				}

				if(pszb != NULL)
				{
					wcsncpy_s(pszb, blen, r_itr->second.c_str(), _TRUNCATE);
				}
			}

			roman_kana_conv.push_back(rkc);
			i++;
		}
	}
	else if(hr != S_OK)
	{
		for(int i = 0; i < ROMAN_KANA_TBL_DEF_NUM; i++)
		{
			if(roman_kana_conv_default[i].hacm[0] == L'\0')
			{
				break;
			}
			roman_kana_conv.push_back(roman_kana_conv_default[i]);
		}
	}
}

void LoadKana(HWND hwnd)
{
	LVITEMW item;
	WCHAR soku[2];

	LoadConfigKana();

	HWND hWndListView = GetDlgItem(hwnd, IDC_LIST_KANATBL);
	int count = (int)roman_kana_conv.size();

	for(int i = 0; i < count; i++)
	{
		item.mask = LVIF_TEXT;
		item.pszText = roman_kana_conv[i].hacm;
		item.iItem = i;
		item.iSubItem = 0;
		ListView_InsertItem(hWndListView, &item);
		item.pszText = roman_kana_conv[i].yula;
		item.iItem = i;
		item.iSubItem = 1;
		ListView_SetItem(hWndListView, &item);
		soku[0] = L'0' + (roman_kana_conv[i].soku ? 1 : 0) + (roman_kana_conv[i].wait ? 2 : 0);
		soku[1] = L'\0';
		item.pszText = soku;
		item.iItem = i;
		item.iSubItem = 2;
		ListView_SetItem(hWndListView, &item);
	}
}

void SaveKana(HWND hwnd)
{
	APPDATAXMLLIST list;
	APPDATAXMLROW row;
	APPDATAXMLATTR attr;
	ROMAN_KANA_CONV rkc;
	WCHAR soku[2];

	roman_kana_conv.clear();
	roman_kana_conv.shrink_to_fit();

	HWND hWndListView = GetDlgItem(hwnd, IDC_LIST_KANATBL);
	int count = ListView_GetItemCount(hWndListView);

	for(int i = 0; i < count && i < ROMAN_KANA_TBL_MAX; i++)
	{
		ListView_GetItemText(hWndListView, i, 0, rkc.hacm, _countof(rkc.hacm));
		ListView_GetItemText(hWndListView, i, 1, rkc.yula, _countof(rkc.yula));
		ListView_GetItemText(hWndListView, i, 2, soku, _countof(soku));
		((soku[0] - L'0') & 0x1) != 0 ? rkc.soku = TRUE : rkc.soku = FALSE;
		((soku[0] - L'0') & 0x2) != 0 ? rkc.wait = TRUE : rkc.wait = FALSE;

		roman_kana_conv.push_back(rkc);
	}

	WriterStartSection(pXmlWriter, SectionKana);	//Start of SectionKana

	for(int i = 0; i < count; i++)
	{
		attr.first = AttributeRoman;
		attr.second = roman_kana_conv[i].hacm;
		row.push_back(attr);

		attr.first = AttributeHiragana;
		attr.second = roman_kana_conv[i].yula;
		row.push_back(attr);

		attr.first = AttributeSpOp;
		soku[0] = L'0' + (roman_kana_conv[i].soku ? 1 : 0) + (roman_kana_conv[i].wait ? 2 : 0);
		soku[1] = L'\0';
		attr.second = soku;
		row.push_back(attr);

		list.push_back(row);
		row.clear();
	}

	WriterList(pXmlWriter, list);

	WriterEndSection(pXmlWriter);	//End of SectionKana
}

void LoadConfigKanaTxt(LPCWSTR path)
{
	FILE *fp;
	wchar_t b[CONFKANALEN];
	const wchar_t seps[] = L"\t\n\0";
	size_t sidx, eidx;
	ROMAN_KANA_CONV rkc;
	WCHAR soku[2];

	roman_kana_conv.clear();
	roman_kana_conv.shrink_to_fit();

	_wfopen_s(&fp, path, RccsUTF8);
	if(fp == NULL)
	{
		return;
	}

	ZeroMemory(b, sizeof(b));

	while(fgetws(b, CONFKANALEN, fp) != NULL)
	{
		if(roman_kana_conv.size() >= ROMAN_KANA_TBL_MAX)
		{
			break;
		}

		ZeroMemory(&rkc, sizeof(rkc));

		sidx = 0;
		eidx = wcscspn(&b[sidx], seps);

		for(int i = 0; i <= 2; i++)
		{
			if(sidx + eidx >= _countof(b))
			{
				break;
			}
			b[sidx + eidx] = L'\0';

			switch(i)
			{
			case 0:
				_snwprintf_s(rkc.hacm, _TRUNCATE, L"%s", &b[sidx]);
				break;
			case 1:
				_snwprintf_s(rkc.yula, _TRUNCATE, L"%s", &b[sidx]);
				break;
			case 2:
				_snwprintf_s(soku, _TRUNCATE, L"%s", &b[sidx]);
				rkc.soku = (_wtoi(soku) & 0x1) ? TRUE : FALSE;
				rkc.wait = (_wtoi(soku) & 0x2) ? TRUE : FALSE;
				break;
			default:
				break;
			}

			sidx += eidx + 1;
			if(sidx >= _countof(b))
			{
				break;
			}
			eidx = wcscspn(&b[sidx], seps);
		}

		ZeroMemory(b, sizeof(b));

		if(rkc.hacm[0] == L'\0' &&
			rkc.yula[0] == L'\0')
		{
			continue;
		}

		roman_kana_conv.push_back(rkc);
	}

	fclose(fp);
}

void LoadKanaTxt(HWND hwnd, LPCWSTR path)
{
	LVITEMW item;
	WCHAR soku[2];

	LoadConfigKanaTxt(path);

	HWND hWndListView = GetDlgItem(hwnd, IDC_LIST_KANATBL);
	ListView_DeleteAllItems(hWndListView);
	int count = (int)roman_kana_conv.size();

	for(int i = 0; i < count; i++)
	{
		item.mask = LVIF_TEXT;
		item.pszText = roman_kana_conv[i].hacm;
		item.iItem = i;
		item.iSubItem = 0;
		ListView_InsertItem(hWndListView, &item);
		item.pszText = roman_kana_conv[i].yula;
		item.iItem = i;
		item.iSubItem = 1;
		ListView_SetItem(hWndListView, &item);
		soku[0] = L'0' + (roman_kana_conv[i].soku ? 1 : 0) + (roman_kana_conv[i].wait ? 2 : 0);
		soku[1] = L'\0';
		item.pszText = soku;
		item.iItem = i;
		item.iSubItem = 2;
		ListView_SetItem(hWndListView, &item);
	}
}

void SaveKanaTxt(HWND hwnd, LPCWSTR path)
{
	ROMAN_KANA_CONV rkc;
	WCHAR soku[2];
	FILE *fp;

	roman_kana_conv.clear();
	roman_kana_conv.shrink_to_fit();

	HWND hWndListView = GetDlgItem(hwnd, IDC_LIST_KANATBL);
	int count = ListView_GetItemCount(hWndListView);

	for(int i = 0; i < count && i < ROMAN_KANA_TBL_MAX; i++)
	{
		ListView_GetItemText(hWndListView, i, 0, rkc.hacm, _countof(rkc.hacm));
		ListView_GetItemText(hWndListView, i, 1, rkc.yula, _countof(rkc.yula));
		ListView_GetItemText(hWndListView, i, 2, soku, _countof(soku));
		((soku[0] - L'0') & 1) ? rkc.soku = TRUE : rkc.soku = FALSE;
		((soku[0] - L'0') & 2) ? rkc.wait = TRUE : rkc.wait = FALSE;

		roman_kana_conv.push_back(rkc);
	}

	_wfopen_s(&fp, path, WccsUTF8);
	if(fp != NULL)
	{
		count = (int)roman_kana_conv.size();

		for(int i = 0; i < count; i++)
		{
			if(roman_kana_conv[i].hacm[0] == L'\0' &&
					roman_kana_conv[i].yula[0] == L'\0')
			{
					continue;
			}
			fwprintf(fp, L"%s\t%s\t%d\n",
								roman_kana_conv[i].hacm,
								roman_kana_conv[i].yula,
								roman_kana_conv[i].soku | (roman_kana_conv[i].wait << 1));
		}

		fclose(fp);
	}
}
