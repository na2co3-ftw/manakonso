
#include "configxml.h"
#include "imcrvcnf.h"
#include "convtable.h"
#include "resource.h"

INT_PTR CALLBACK DlgProcKana(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWndListView;
	LV_COLUMNW lvc;
	LVITEMW item;
	int index, count;
	ROMAN_KANA_CONV rkc;
	ROMAN_KANA_CONV rkcBak;
	WCHAR soku[2];
	NMLISTVIEW *pListView;
	OPENFILENAMEW ofn;
	WCHAR path[MAX_PATH];

	switch(message)
	{
	case WM_INITDIALOG:
		hWndListView = GetDlgItem(hDlg, IDC_LIST_KANATBL);
		ListView_SetExtendedListViewStyle(hWndListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_CENTER;

		lvc.iSubItem = 0;
		lvc.cx = GetScaledSizeX(hDlg, 60);
		lvc.pszText = L"幻字";
		ListView_InsertColumn(hWndListView, 0, &lvc);
		lvc.iSubItem = 1;
		lvc.cx = GetScaledSizeX(hDlg, 60);
		lvc.pszText = L"京字";
		ListView_InsertColumn(hWndListView, 1, &lvc);
		lvc.iSubItem = 4;
		lvc.cx = GetScaledSizeX(hDlg, 30);
		lvc.pszText = L"…";
		ListView_InsertColumn(hWndListView, 2, &lvc);

		SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_R, L"");
		SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_H, L"");
		CheckDlgButton(hDlg, IDC_CHECKBOX_KANATBL_SOKU, BST_UNCHECKED);
		CheckDlgButton(hDlg, IDC_CHECKBOX_KANATBL_WAIT, BST_UNCHECKED);

		LoadKana(hDlg);

		return TRUE;

	case WM_COMMAND:
		hWndListView = GetDlgItem(hDlg, IDC_LIST_KANATBL);
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_LOADKANA:
			path[0] = L'\0';
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = path;
			ofn.nMaxFile = _countof(path);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrTitle = L"Load Kana Table File";
			ofn.lpstrFilter = L"*.txt\0*.txt\0" L"*.*\0*.*\0\0";
			if(GetOpenFileNameW(&ofn))
			{
				LoadKanaTxt(hDlg, ofn.lpstrFile);
				PropSheet_Changed(GetParent(hDlg), hDlg);
			}
			break;

		case IDC_BUTTON_SAVEKANA:
			path[0] = L'\0';
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = path;
			ofn.nMaxFile = _countof(path);
			ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
			ofn.lpstrTitle = L"Save Kana Table File";
			ofn.lpstrFilter = L"*.txt\0*.txt\0" L"*.*\0*.*\0\0";
			ofn.lpstrDefExt = L"txt";
			if(GetSaveFileNameW(&ofn))
			{
				SaveKanaTxt(hDlg, ofn.lpstrFile);
				MessageBoxW(hDlg, L"完了しました。", TextServiceDesc, MB_OK | MB_ICONINFORMATION);
			}
			break;

		case IDC_BUTTON_KANATBL_W:
			index = ListView_GetNextItem(hWndListView, -1, LVNI_SELECTED);
			count = ListView_GetItemCount(hWndListView);
			if(index >= 0)
			{
				PropSheet_Changed(GetParent(hDlg), hDlg);

				GetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_R, rkc.hacm, _countof(rkc.hacm));
				GetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_H, rkc.yula, _countof(rkc.yula));;
				IsDlgButtonChecked(hDlg, IDC_CHECKBOX_KANATBL_SOKU) == BST_CHECKED ? rkc.soku = TRUE : rkc.soku = FALSE;
				IsDlgButtonChecked(hDlg, IDC_CHECKBOX_KANATBL_WAIT) == BST_CHECKED ? rkc.wait = TRUE : rkc.wait = FALSE;
				SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_R, rkc.hacm);
				SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_H, rkc.yula);
				ListView_SetItemText(hWndListView, index, 0, rkc.hacm);
				ListView_SetItemText(hWndListView, index, 1, rkc.yula);
				soku[1] = L'\0';
				soku[0] = L'0' + (rkc.soku ? 1 : 0) + (rkc.wait ? 2 : 0);
				ListView_SetItemText(hWndListView, index, 2, soku);
			}
			else if(count < ROMAN_KANA_TBL_MAX)
			{
				PropSheet_Changed(GetParent(hDlg), hDlg);

				GetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_R, rkc.hacm, _countof(rkc.hacm));
				GetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_H, rkc.yula, _countof(rkc.yula));
				IsDlgButtonChecked(hDlg, IDC_CHECKBOX_KANATBL_SOKU) == BST_CHECKED ? rkc.soku = TRUE : rkc.soku = FALSE;
				IsDlgButtonChecked(hDlg, IDC_CHECKBOX_KANATBL_WAIT) == BST_CHECKED ? rkc.wait = TRUE : rkc.wait = FALSE;
				SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_R, rkc.hacm);
				SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_H, rkc.yula);
				item.mask = LVIF_TEXT;
				item.pszText = rkc.hacm;
				item.iItem = count;
				item.iSubItem = 0;
				ListView_InsertItem(hWndListView, &item);
				item.pszText = rkc.yula;
				item.iItem = count;
				item.iSubItem = 1;
				ListView_SetItem(hWndListView, &item);
				soku[1] = L'\0';
				soku[0] = L'0' + (rkc.soku ? 1 : 0) + (rkc.wait ? 2 : 0);
				item.pszText = soku;
				item.iItem = count;
				item.iSubItem = 4;
				ListView_SetItem(hWndListView, &item);
			}
			return TRUE;

		case IDC_BUTTON_KANATBL_D:
			index = ListView_GetNextItem(hWndListView, -1, LVNI_SELECTED);
			if(index != -1)
			{
				PropSheet_Changed(GetParent(hDlg), hDlg);

				ListView_DeleteItem(hWndListView, index);
			}
			return TRUE;

		case IDC_BUTTON_KANATBL_UP:
			index = ListView_GetNextItem(hWndListView, -1, LVNI_SELECTED);
			if(index > 0)
			{
				PropSheet_Changed(GetParent(hDlg), hDlg);

				ListView_GetItemText(hWndListView, index - 1, 0, rkcBak.hacm, _countof(rkcBak.hacm));
				ListView_GetItemText(hWndListView, index - 1, 1, rkcBak.yula, _countof(rkcBak.yula));
				ListView_GetItemText(hWndListView, index - 1, 2, soku, _countof(soku));
				((soku[0] - L'0') & 0x1) ? rkcBak.soku = TRUE : rkcBak.soku = FALSE;
				((soku[0] - L'0') & 0x2) ? rkcBak.wait = TRUE : rkcBak.wait = FALSE;
				ListView_GetItemText(hWndListView, index, 0, rkc.hacm, _countof(rkc.hacm));
				ListView_GetItemText(hWndListView, index, 1, rkc.yula, _countof(rkc.yula));
				ListView_GetItemText(hWndListView, index, 2, soku, _countof(soku));
				((soku[0] - L'0') & 0x1) ? rkc.soku = TRUE : rkc.soku = FALSE;
				((soku[0] - L'0') & 0x2) ? rkc.wait = TRUE : rkc.wait = FALSE;
				ListView_SetItemText(hWndListView, index - 1, 0, rkc.hacm);
				ListView_SetItemText(hWndListView, index - 1, 1, rkc.yula);;
				soku[1] = L'\0';
				soku[0] = L'0' + (rkc.soku ? 1 : 0) + (rkc.wait ? 2 : 0);
				ListView_SetItemText(hWndListView, index - 1, 2, soku);
				ListView_SetItemText(hWndListView, index, 0, rkcBak.hacm);
				ListView_SetItemText(hWndListView, index, 1, rkcBak.yula);
				soku[1] = L'\0';
				soku[0] = L'0' + (rkcBak.soku ? 1 : 0) + (rkcBak.wait ? 2 : 0);
				ListView_SetItemText(hWndListView, index, 2, soku);
				ListView_SetItemState(hWndListView, index - 1, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
			}
			return TRUE;

		case IDC_BUTTON_KANATBL_DOWN:
			index = ListView_GetNextItem(hWndListView, -1, LVNI_SELECTED);
			count = ListView_GetItemCount(hWndListView);
			if(index >= 0 && index < count - 1)
			{
				PropSheet_Changed(GetParent(hDlg), hDlg);

				ListView_GetItemText(hWndListView, index + 1, 0, rkcBak.hacm, _countof(rkcBak.hacm));
				ListView_GetItemText(hWndListView, index + 1, 1, rkcBak.yula, _countof(rkcBak.yula));
				ListView_GetItemText(hWndListView, index + 1, 2, soku, _countof(soku));
				((soku[0] - L'0') & 0x1) ? rkcBak.soku = TRUE : rkcBak.soku = FALSE;
				((soku[0] - L'0') & 0x2) ? rkcBak.wait = TRUE : rkcBak.wait = FALSE;
				ListView_GetItemText(hWndListView, index, 0, rkc.hacm, _countof(rkc.hacm));
				ListView_GetItemText(hWndListView, index, 1, rkc.yula, _countof(rkc.yula));
				ListView_GetItemText(hWndListView, index, 2, soku, _countof(soku));
				((soku[0] - L'0') & 0x1) ? rkc.soku = TRUE : rkc.soku = FALSE;
				((soku[0] - L'0') & 0x2) ? rkc.wait = TRUE : rkc.wait = FALSE;
				ListView_SetItemText(hWndListView, index + 1, 0, rkc.hacm);
				ListView_SetItemText(hWndListView, index + 1, 1, rkc.yula);
				soku[1] = L'\0';
				soku[0] = L'0' + (rkc.soku ? 1 : 0) + (rkc.wait ? 2 : 0);
				ListView_SetItemText(hWndListView, index + 1, 2, soku);
				ListView_SetItemText(hWndListView, index, 0, rkcBak.hacm);
				ListView_SetItemText(hWndListView, index, 1, rkcBak.yula);
				soku[1] = L'\0';
				soku[0] = L'0' + (rkcBak.soku ? 1 : 0) + (rkcBak.wait ? 2 : 0);
				ListView_SetItemText(hWndListView, index, 2, soku);
				ListView_SetItemState(hWndListView, index + 1, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
			}
			return TRUE;

		default:
			break;
		}
		break;

	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case LVN_ITEMCHANGED:
			pListView = (NMLISTVIEW*)((LPNMHDR)lParam);
			if(pListView->uChanged & LVIF_STATE)
			{
				hWndListView = ((LPNMHDR)lParam)->hwndFrom;
				index = ListView_GetNextItem(hWndListView, -1, LVNI_SELECTED);
				if(index == -1)
				{
					SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_R, L"");
					SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_H, L"");
					CheckDlgButton(hDlg, IDC_CHECKBOX_KANATBL_SOKU, BST_UNCHECKED);
					CheckDlgButton(hDlg, IDC_CHECKBOX_KANATBL_WAIT, BST_UNCHECKED);
				}
				else
				{
					ListView_GetItemText(hWndListView, index, 0, rkc.hacm, _countof(rkc.hacm));
					ListView_GetItemText(hWndListView, index, 1, rkc.yula, _countof(rkc.hacm));
					ListView_GetItemText(hWndListView, index, 2, soku, _countof(soku));
					((soku[0] - L'0') & 0x1) ? rkc.soku = TRUE : rkc.soku = FALSE;
					((soku[0] - L'0') & 0x2) ? rkc.wait = TRUE : rkc.wait = FALSE;
					SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_R, rkc.hacm);
					SetDlgItemTextW(hDlg, IDC_EDIT_KANATBL_H, rkc.yula);
					CheckDlgButton(hDlg, IDC_CHECKBOX_KANATBL_SOKU, (rkc.soku ? BST_CHECKED : BST_UNCHECKED));
					CheckDlgButton(hDlg, IDC_CHECKBOX_KANATBL_WAIT, (rkc.wait ? BST_CHECKED : BST_UNCHECKED));
				}
				return TRUE;
			}
			break;

		case PSN_APPLY:
			SaveKana(hDlg);

			WriterEndElement(pXmlWriter);	//TagRoot

			WriterNewLine(pXmlWriter);

			WriterFinal(&pXmlWriter, &pXmlFileStream);
			
			return TRUE;

		default:
			break;
		}
		break;
	}

	return FALSE;
}
