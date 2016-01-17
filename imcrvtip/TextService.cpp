﻿
#include "immanatip.h"
#include "TextService.h"
#include "CandidateList.h"
#include "CandidateWindow.h"
#include "InputModeWindow.h"

CTextService::CTextService()
{
	DllAddRef();

	_cRef = 1;

	_pThreadMgr = NULL;
	_ClientId = TF_CLIENTID_NULL;
	_dwThreadMgrEventSinkCookie = TF_INVALID_COOKIE;
	_dwThreadFocusSinkCookie = TF_INVALID_COOKIE;
	_dwCompartmentEventSinkOpenCloseCookie = TF_INVALID_COOKIE;
	_dwCompartmentEventSinkInputmodeConversionCookie = TF_INVALID_COOKIE;
	_pTextEditSinkContext = NULL;
	_dwTextEditSinkCookie = TF_INVALID_COOKIE;
	_pComposition = NULL;
	_pLangBarItem = NULL;
	_pLangBarItemI = NULL;
	_pCandidateList = NULL;
	_pInputModeWindow = NULL;

	hFont = NULL;
	_pD2DFactory = NULL;
	_pD2DDCRT = NULL;
	for(int i = 0; i < DISPLAY_COLOR_NUM; i++)
	{
		_pD2DBrush[i] = NULL;
	}
	_drawtext_option = D2D1_DRAW_TEXT_OPTIONS_NONE;
	_pDWFactory = NULL;
	_pDWTF = NULL;

	_dwActiveFlags = 0;
	_ImmersiveMode = FALSE;
	_UILessMode = FALSE;
	_ShowInputMode = FALSE;

	hPipe = INVALID_HANDLE_VALUE;

	inputmode = im_default;

	_ResetStatus();

	_CreateConfigPath();
}

CTextService::~CTextService()
{
	DllRelease();
}

STDAPI CTextService::QueryInterface(REFIID riid, void **ppvObj)
{
	if(ppvObj == NULL)
	{
		return E_INVALIDARG;
	}

	*ppvObj = NULL;

	if(IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfTextInputProcessor))
	{
		*ppvObj = (ITfTextInputProcessor *)this;
	}
	else if(IsEqualIID(riid, IID_ITfTextInputProcessorEx))
	{
		*ppvObj = (ITfTextInputProcessorEx *)this;
	}
	else if(IsEqualIID(riid, IID_ITfThreadMgrEventSink))
	{
		*ppvObj = (ITfThreadMgrEventSink *)this;
	}
	else if(IsEqualIID(riid, IID_ITfThreadFocusSink))
	{
		*ppvObj = (ITfThreadFocusSink *)this;
	}
	else if(IsEqualIID(riid, IID_ITfCompartmentEventSink))
	{
		*ppvObj = (ITfCompartmentEventSink *)this;
	}
	else if(IsEqualIID(riid, IID_ITfTextEditSink))
	{
		*ppvObj = (ITfTextEditSink *)this;
	}
	else if(IsEqualIID(riid, IID_ITfKeyEventSink))
	{
		*ppvObj = (ITfKeyEventSink *)this;
	}
	else if(IsEqualIID(riid, IID_ITfCompositionSink))
	{
		*ppvObj = (ITfKeyEventSink *)this;
	}
	else if(IsEqualIID(riid, IID_ITfDisplayAttributeProvider))
	{
		*ppvObj = (ITfDisplayAttributeProvider *)this;
	}
	else if(IsEqualIID(riid, IID_ITfFunctionProvider))
	{
		*ppvObj = (ITfFunctionProvider *)this;
	}
	else if(IsEqualIID(riid, IID_ITfFnConfigure))
	{
		*ppvObj = (ITfFnConfigure *)this;
	}
	else if(IsEqualIID(riid, IID_ITfFnShowHelp))
	{
		*ppvObj = (ITfFnShowHelp *)this;
	}
	else if(IsEqualIID(riid, IID_ITfFnGetPreferredTouchKeyboardLayout))
	{
		*ppvObj = (ITfFnGetPreferredTouchKeyboardLayout *)this;
	}

	if(*ppvObj)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDAPI_(ULONG) CTextService::AddRef()
{
	return ++_cRef;
}

STDAPI_(ULONG) CTextService::Release()
{
	if(--_cRef == 0)
	{
		delete this;
		return 0;
	}

	return _cRef;
}

STDAPI CTextService::Activate(ITfThreadMgr *ptim, TfClientId tid)
{
	return ActivateEx(ptim, tid, 0);
}

STDAPI CTextService::ActivateEx(ITfThreadMgr *ptim, TfClientId tid, DWORD dwFlags)
{
	_pThreadMgr = ptim;
	_pThreadMgr->AddRef();
	_ClientId = tid;

	CCandidateWindow::_InitClass();
	CInputModeWindow::_InitClass();

	if (!_IsKeyboardOpen())
	{
		_KeyboardSetDefaultMode();
	}

	if(!_InitThreadMgrEventSink())
	{
		goto exit;
	}

	if(!_InitThreadFocusSink())
	{
		goto exit;
	}

	if(!_InitCompartmentEventSink())
	{
		goto exit;
	}

	ITfDocumentMgr *pDocumentMgr;
	if((_pThreadMgr->GetFocus(&pDocumentMgr) == S_OK) && (pDocumentMgr != NULL))
	{
		_InitTextEditSink(pDocumentMgr);
		SafeRelease(&pDocumentMgr);
	}

	if(!_InitLanguageBar())
	{
		goto exit;
	}

	if(!_InitKeyEventSink())
	{
		goto exit;
	}

	if(!_InitDisplayAttributeGuidAtom())
	{
		goto exit;
	}

	if(!_InitFunctionProvider())
	{
		goto exit;
	}

	_KeyboardOpenCloseChanged(FALSE);

	return S_OK;

exit:
	Deactivate();
	return E_FAIL;
}

STDAPI CTextService::Deactivate()
{
	if(_pThreadMgr == NULL)
	{
		return S_OK;
	}

	_SaveUserDic();

	_EndCandidateList();

	_EndInputModeWindow();

	_UninitFunctionProvider();

	_UninitPreservedKey(0);
	_UninitPreservedKey(1);

	_UninitKeyEventSink();

	_UninitLanguageBar();

	_InitTextEditSink(NULL);

	_UninitCompartmentEventSink();

	_UninitThreadFocusSink();

	_UninitThreadMgrEventSink();

	_UninitFont();

	CCandidateWindow::_UninitClass();
	CInputModeWindow::_UninitClass();

	SafeRelease(&_pThreadMgr);

	_ClientId = TF_CLIENTID_NULL;

	return S_OK;
}
