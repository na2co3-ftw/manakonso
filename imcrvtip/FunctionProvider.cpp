﻿
#include "immanatip.h"
#include "TextService.h"

HRESULT CTextService::GetType(GUID *pguid)
{
	if(pguid == NULL)
	{
		return E_INVALIDARG;
	}

	*pguid = c_clsidTextService;

	return S_OK;
}

HRESULT CTextService::GetDescription(BSTR *pbstrDesc)
{
	BSTR bstrDesc;

	if(pbstrDesc == NULL)
	{
		return E_INVALIDARG;
	}

	*pbstrDesc = NULL;

	bstrDesc = SysAllocString(TextServiceDesc);

	if(bstrDesc == NULL)
	{
		return E_OUTOFMEMORY;
	}

	*pbstrDesc = bstrDesc;

	return S_OK;
}

HRESULT CTextService::GetFunction(REFGUID rguid, REFIID riid, IUnknown **ppunk)
{
	if(ppunk == NULL)
	{
		return E_INVALIDARG;
	}

	*ppunk = NULL;

	//This value can be GUID_NULL.
	if(!IsEqualGUID(rguid, GUID_NULL) && !IsEqualGUID(rguid, c_clsidTextService))
	{
		return E_INVALIDARG;
	}

	if(IsEqualIID(riid, IID_ITfFnConfigure))
	{
		*ppunk = (ITfFnConfigure *)this;
	}
	else if(IsEqualIID(riid, IID_ITfFnShowHelp))
	{
		*ppunk = (ITfFnShowHelp *)this;
	}
	else if(IsEqualIID(riid, IID_ITfFnGetPreferredTouchKeyboardLayout))
	{
		*ppunk = (ITfFnGetPreferredTouchKeyboardLayout *)this;
	}
	else
	{
		return E_INVALIDARG;
	}

	if(*ppunk)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

HRESULT CTextService::GetDisplayName(BSTR *pbstrName)
{
	BSTR bstrName;

	if(pbstrName == NULL)
	{
		return E_INVALIDARG;
	}

	*pbstrName = NULL;

	bstrName = SysAllocString(L"設定");

	if(bstrName == NULL)
	{
		return E_OUTOFMEMORY;
	}

	*pbstrName = bstrName;

	return S_OK;
}

HRESULT CTextService::Show(HWND hwndParent, LANGID langid, REFGUID rguidProfile)
{
	if(IsEqualGUID(rguidProfile, c_guidProfile))
	{
		_StartConfigure();
	}
	else
	{
		return E_INVALIDARG;
	}

	return S_OK;
}

HRESULT CTextService::Show(HWND hwndParent)
{
	_StartConfigure();
	return S_OK;
}

HRESULT CTextService::GetLayout(TKBLayoutType *pTKBLayoutType, WORD *pwPreferredLayoutId)
{
	if(pTKBLayoutType != NULL && pwPreferredLayoutId != NULL)
	{
		*pTKBLayoutType = TKBLT_OPTIMIZED;
		*pwPreferredLayoutId = TKBL_OPT_JAPANESE_ABC;
	}
	else
	{
		return E_INVALIDARG;
	}

	return S_OK;
}

BOOL CTextService::_InitFunctionProvider()
{
	HRESULT hr = E_FAIL;

	ITfSourceSingle *pSourceSingle;
	if(_pThreadMgr->QueryInterface(IID_PPV_ARGS(&pSourceSingle)) == S_OK)
	{
		hr = pSourceSingle->AdviseSingleSink(_ClientId, IID_IUNK_ARGS((ITfFunctionProvider *)this));
		SafeRelease(&pSourceSingle);
	}

	return (hr == S_OK);
}

void CTextService::_UninitFunctionProvider()
{
	ITfSourceSingle *pSourceSingle;
	if(_pThreadMgr->QueryInterface(IID_PPV_ARGS(&pSourceSingle)) == S_OK)
	{
		pSourceSingle->UnadviseSingleSink(_ClientId, IID_ITfFunctionProvider);
		SafeRelease(&pSourceSingle);
	}
}
