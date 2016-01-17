﻿
#include "configxml.h"
#include "immanacnf.h"

LPCWSTR TextServiceDesc = TEXTSERVICE_DESC;

IXmlWriter *pXmlWriter;
IStream *pXmlFileStream;

WCHAR cnfmutexname[MAX_KRNLOBJNAME];	//ミューテックス

// ファイルパス
WCHAR pathconfigxml[MAX_PATH];	//設定
WCHAR pathskkdic[MAX_PATH];		//取込SKK辞書
WCHAR pathskkidx[MAX_PATH];		//取込SKK辞書インデックス

void CreateConfigPath()
{
	PWSTR appdatafolder = NULL;

	ZeroMemory(pathconfigxml, sizeof(pathconfigxml));
	ZeroMemory(pathskkdic, sizeof(pathskkdic));
	ZeroMemory(pathskkidx, sizeof(pathskkidx));

	if(SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DONT_VERIFY, NULL, &appdatafolder) == S_OK)
	{
		WCHAR appdir[MAX_PATH];

		_snwprintf_s(appdir, _TRUNCATE, L"%s\\%s", appdatafolder, TextServiceDesc);

		CoTaskMemFree(appdatafolder);

		CreateDirectoryW(appdir, NULL);
		SetCurrentDirectoryW(appdir);

		_snwprintf_s(pathconfigxml, _TRUNCATE, L"%s\\%s", appdir, fnconfigxml);
		_snwprintf_s(pathskkdic, _TRUNCATE, L"%s\\%s", appdir, fnskkdic);
		_snwprintf_s(pathskkidx, _TRUNCATE, L"%s\\%s", appdir, fnskkidx);
	}

	ZeroMemory(cnfmutexname, sizeof(cnfmutexname));

	LPWSTR pszUserUUID = NULL;

	if(GetUserUUID(&pszUserUUID))
	{
		_snwprintf_s(cnfmutexname, _TRUNCATE, L"%s%s", IMmanaCNFMUTEX, pszUserUUID);

		LocalFree(pszUserUUID);
	}
}

BOOL SetFileDacl(LPCWSTR path)
{
	BOOL bRet = FALSE;
	WCHAR sddl[MAX_KRNLOBJNAME] = {L'\0'};
	PSECURITY_DESCRIPTOR psd = NULL;
	LPWSTR pszUserSid;

	if(GetUserSid(&pszUserSid))
	{
		// SDDL_ALL_APP_PACKAGES / SDDL_RESTRICTED_CODE / SDDL_LOCAL_SYSTEM / SDDL_BUILTIN_ADMINISTRATORS / User SID
		_snwprintf_s(sddl, _TRUNCATE, L"D:%s(A;;FR;;;RC)(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;%s)",
			(IsWindowsVersion62OrLater() ? L"(A;;FR;;;AC)" : L""), pszUserSid);

		LocalFree(pszUserSid);
	}

	if(ConvertStringSecurityDescriptorToSecurityDescriptorW(sddl, SDDL_REVISION_1, &psd, NULL))
	{
		if(SetFileSecurityW(path, DACL_SECURITY_INFORMATION, psd))
		{
			bRet = TRUE;
		}
		LocalFree(psd);
	}

	return bRet;
}

int GetScaledSizeX(HWND hwnd, int size)
{
	HDC hdc = GetDC(hwnd);
	int dpi = GetDeviceCaps(hdc, LOGPIXELSY);
	ReleaseDC(hwnd, hdc);
	return MulDiv(size, dpi, 96);
}

void DrawSelectColor(HWND hDlg, int id, COLORREF col)
{
	RECT rect;
	HDC hdc;
	HWND hwnd;

	hwnd = GetDlgItem(hDlg, id);
	hdc = GetDC(hwnd);
	SelectObject(hdc, GetStockObject(BLACK_PEN));
	SetDCBrushColor(hdc, col);
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	GetClientRect(hwnd, &rect);
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	ReleaseDC(hwnd, hdc);
}
