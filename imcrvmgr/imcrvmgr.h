﻿
#ifndef IMCRVMGR_H
#define IMCRVMGR_H

#include "common.h"
#include "parseskkdic.h"

typedef struct {
	SKKDIC userdic;
	USEROKURI userokuri;
	KEYORDER complements;
	KEYORDER accompaniments;
} USERDATA;

// ConfigMgr
void CreateConfigPath();
void UpdateConfigPath();
void CreateIpcName();
void LoadConfig();
BOOL IsFileModified(LPCWSTR path, FILETIME *ft);

// ConvNum
std::wstring ConvNum(const std::wstring &key, const std::wstring &candidate);

// SearchCharacter
std::wstring SearchUnicode(const std::wstring &searchkey);
std::wstring SearchJISX0213(const std::wstring &searchkey);
std::wstring SearchJISX0208(const std::wstring &searchkey);
std::wstring SearchCharacterCode(const std::wstring &searchkey);

// SearchDictionary
void SearchDictionary(const std::wstring &searchkey, const std::wstring &okuri, SKKDICCANDIDATES &sc);
std::wstring SearchSKKDic(const std::wstring &searchkey, const std::wstring &okuri);
void MakeSKKDicPos();
std::wstring ConvertKey(const std::wstring &searchkey, const std::wstring &okuri);
std::wstring ConvertCandidate(const std::wstring &searchkey, const std::wstring &candidate, const std::wstring &okuri);

// SearchUserDictionary
std::wstring SearchUserDic(const std::wstring &searchkey, const std::wstring &okuri);
void SearchComplement(const std::wstring &searchkey, SKKDICCANDIDATES &sc);
void SearchComplementSearchCandidate(SKKDICCANDIDATES &sc, int max);
void AddUserDic(WCHAR command, const std::wstring &searchkey, const std::wstring &candidate, const std::wstring &annotation, const std::wstring &okuri);
void DelUserDic(WCHAR command, const std::wstring &searchkey, const std::wstring &candidate);
BOOL LoadSKKUserDic();
void SaveSKKUserDic(void *p);
void StartSaveSKKUserDic(BOOL bThread);
void BackUpSKKUserDic();

// Server
void SrvProc(WCHAR command, const std::wstring &argument, std::wstring &result);
unsigned int __stdcall SrvThread(void *p);
HANDLE SrvStart();

#define BACKUP_GENS		3

extern CRITICAL_SECTION csUserDataSave;
extern BOOL bUserDicChg;
extern FILETIME ftConfig, ftSKKDic;
#ifdef _DEBUG
extern HWND hwndEdit;
extern HFONT hFont;
#endif
extern HINSTANCE hInst;
extern HANDLE hMutex;
extern HANDLE hThreadSrv;
extern BOOL bSrvThreadExit;

extern SKKDIC userdic;
extern USEROKURI userokuri;
extern KEYORDER complements;
extern KEYORDER accompaniments;

extern LPCWSTR TextServiceDesc;
extern LPCWSTR DictionaryManagerClass;

// ファイルパス
extern WCHAR pathconfigxml[MAX_PATH];	//設定
extern WCHAR pathuserdic[MAX_PATH];		//ユーザー辞書
extern WCHAR pathuserbak[MAX_PATH];		//ユーザー辞書バックアッププレフィックス
extern WCHAR pathskkdic[MAX_PATH];		//取込SKK辞書

extern WCHAR krnlobjsddl[MAX_KRNLOBJNAME];	//SDDL
extern WCHAR mgrpipename[MAX_KRNLOBJNAME];	//名前付きパイプ
extern WCHAR mgrmutexname[MAX_KRNLOBJNAME];	//ミューテックス

extern BOOL precedeokuri;	//送り仮名が一致した候補を優先する

#endif //IMCRVMGR_H
