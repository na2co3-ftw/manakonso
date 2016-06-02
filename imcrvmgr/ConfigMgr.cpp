
#include "configxml.h"
#include "utf8.h"
#include "imcrvmgr.h"

LPCWSTR TextServiceDesc = TEXTSERVICE_DESC;
LPCWSTR DictionaryManagerClass = TEXTSERVICE_NAME L"DictionaryManager";

// ファイルパス
WCHAR pathconfigxml[MAX_PATH];	//設定
WCHAR pathuserdic[MAX_PATH];	//ユーザー辞書
WCHAR pathuserbak[MAX_PATH];	//ユーザー辞書バックアッププレフィックス
WCHAR pathskkdic[MAX_PATH];		//取込SKK辞書

WCHAR krnlobjsddl[MAX_KRNLOBJNAME];		//SDDL
WCHAR mgrpipename[MAX_KRNLOBJNAME];		//名前付きパイプ
WCHAR mgrmutexname[MAX_KRNLOBJNAME];	//ミューテックス

BOOL precedeokuri = FALSE;	//送り仮名が一致した候補を優先する

void CreateConfigPath()
{
	PWSTR knownfolderpath = nullptr;

	ZeroMemory(pathconfigxml, sizeof(pathconfigxml));
	ZeroMemory(pathuserdic, sizeof(pathuserdic));
	ZeroMemory(pathuserbak, sizeof(pathuserbak));
	ZeroMemory(pathskkdic, sizeof(pathskkdic));

	if(SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DONT_VERIFY, nullptr, &knownfolderpath) == S_OK)
	{
		WCHAR appdir[MAX_PATH];

		_snwprintf_s(appdir, _TRUNCATE, L"%s\\%s", knownfolderpath, TextServiceDesc);

		CoTaskMemFree(knownfolderpath);

		CreateDirectoryW(appdir, nullptr);
		SetCurrentDirectoryW(appdir);

		_snwprintf_s(pathconfigxml, _TRUNCATE, L"%s\\%s", appdir, fnconfigxml);
		_snwprintf_s(pathuserdic, _TRUNCATE, L"%s\\%s", appdir, fnuserdic);
		_snwprintf_s(pathuserbak, _TRUNCATE, L"%s\\%s", appdir, fnuserbak);
		_snwprintf_s(pathskkdic, _TRUNCATE, L"%s\\%s", appdir, fnskkdic);
	}
}

void UpdateConfigPath()
{
	PWSTR knownfolderpath = nullptr;

	//%AppData%\\CorvusSKK\\config.xml
	//%AppData%\\CorvusSKK\\skkdict.txt
	if(SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DONT_VERIFY, nullptr, &knownfolderpath) == S_OK)
	{
		_snwprintf_s(pathconfigxml, _TRUNCATE, L"%s\\%s\\%s", knownfolderpath, TextServiceDesc, fnconfigxml);
		_snwprintf_s(pathskkdic, _TRUNCATE, L"%s\\%s\\%s", knownfolderpath, TextServiceDesc, fnskkdic);

		CoTaskMemFree(knownfolderpath);
	}

	if(GetFileAttributesW(pathconfigxml) == INVALID_FILE_ATTRIBUTES)
	{
#ifdef _DEBUG
		//<module directory>\\config.xml
		if(GetModuleFileNameW(hInst, pathconfigxml, _countof(pathconfigxml)) != 0)
		{
			WCHAR *pdir = wcsrchr(pathconfigxml, L'\\');
			if(pdir != nullptr)
			{
				*(pdir + 1) = L'\0';
				wcsncat_s(pathconfigxml, fnconfigxml, _TRUNCATE);
			}
		}
#else
		//%SystemRoot%\\IME\\IMCRVSKK\\config.xml
		if(SHGetKnownFolderPath(FOLDERID_Windows, KF_FLAG_DONT_VERIFY, nullptr, &knownfolderpath) == S_OK)
		{
			_snwprintf_s(pathconfigxml, _TRUNCATE, L"%s\\%s\\%s\\%s", knownfolderpath, L"IME", TEXTSERVICE_DIR, fnconfigxml);

			CoTaskMemFree(knownfolderpath);
		}
#endif
	}

	if(GetFileAttributesW(pathskkdic) == INVALID_FILE_ATTRIBUTES)
	{
#ifdef _DEBUG
		//<module directory>\\skkdict.txt
		if(GetModuleFileNameW(hInst, pathskkdic, _countof(pathskkdic)) != 0)
		{
			WCHAR *pdir = wcsrchr(pathskkdic, L'\\');
			if(pdir != nullptr)
			{
				*(pdir + 1) = L'\0';
				wcsncat_s(pathskkdic, fnskkdic, _TRUNCATE);
			}
		}
#else
		//%SystemRoot%\\IME\\IMCRVSKK\\skkdict.txt
		if(SHGetKnownFolderPath(FOLDERID_Windows, KF_FLAG_DONT_VERIFY, nullptr, &knownfolderpath) == S_OK)
		{
			_snwprintf_s(pathskkdic, _TRUNCATE, L"%s\\%s\\%s\\%s", knownfolderpath, L"IME", TEXTSERVICE_DIR, fnskkdic);

			CoTaskMemFree(knownfolderpath);
		}
#endif
	}
}

void CreateIpcName()
{
	ZeroMemory(krnlobjsddl, sizeof(krnlobjsddl));
	ZeroMemory(mgrpipename, sizeof(mgrpipename));
	ZeroMemory(mgrmutexname, sizeof(mgrmutexname));

	LPWSTR pszUserSid = nullptr;

	if(GetUserSid(&pszUserSid))
	{
		// SDDL_ALL_APP_PACKAGES / SDDL_RESTRICTED_CODE / SDDL_LOCAL_SYSTEM / SDDL_BUILTIN_ADMINISTRATORS / User SID
		_snwprintf_s(krnlobjsddl, _TRUNCATE, L"D:%s(A;;GA;;;RC)(A;;GA;;;SY)(A;;GA;;;BA)(A;;GA;;;%s)",
			(IsWindowsVersion62OrLater() ? L"(A;;GA;;;AC)" : L""), pszUserSid);

		// (SDDL_MANDATORY_LABEL, SDDL_NO_WRITE_UP, SDDL_ML_LOW)
		wcsncat_s(krnlobjsddl, L"S:(ML;;NW;;;LW)", _TRUNCATE);

		LocalFree(pszUserSid);
	}

	LPWSTR pszUserUUID = nullptr;

	if(GetUserUUID(&pszUserUUID))
	{
		_snwprintf_s(mgrpipename, _TRUNCATE, L"%s%s", IMCRVMGRPIPE, pszUserUUID);
		_snwprintf_s(mgrmutexname, _TRUNCATE, L"%s%s", IMCRVMGRMUTEX, pszUserUUID);

		LocalFree(pszUserUUID);
	}
}

void LoadConfig()
{
	std::wstring strxmlval;

	ReadValue(pathconfigxml, SectionBehavior, ValuePrecedeOkuri, strxmlval);
	precedeokuri = _wtoi(strxmlval.c_str());
	if(precedeokuri != TRUE && precedeokuri != FALSE)
	{
		precedeokuri = FALSE;
	}
}

BOOL IsFileModified(LPCWSTR path, FILETIME *ft)
{
	BOOL ret = FALSE;
	HANDLE hFile;
	FILETIME ftn;

	if(path != nullptr && ft != nullptr)
	{
		hFile = CreateFileW(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			if(GetFileTime(hFile, nullptr, nullptr, &ftn))
			{
				if(((ULARGE_INTEGER *)ft)->QuadPart != ((ULARGE_INTEGER *)&ftn)->QuadPart)
				{
					*ft = ftn;
					ret = TRUE;
				}
			}
			CloseHandle(hFile);
		}
	}

	return ret;
}
