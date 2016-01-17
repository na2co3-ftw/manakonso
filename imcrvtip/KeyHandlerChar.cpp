﻿
#include "immanatip.h"
#include "TextService.h"

HRESULT CTextService::_HandleChar(TfEditCookie ec, ITfContext *pContext, WPARAM wParam, WCHAR ch, WCHAR chO)
{
	ROMAN_KANA_CONV rkc;
	HRESULT ret = S_OK;

	if(showentry)
	{
		_HandleCharShift(ec, pContext);
	}

	if((okuriidx != 0) && (okuriidx + 1 == cursoridx))
	{
		if(chO != L'\0')
		{
			kana.replace(okuriidx, 1, 1, chO);	//送りローマ字
		}
	}

	switch(inputmode)
	{
	case im_hiragana:
		{
			//ローマ字仮名変換 待機処理
			rkc.hacm[0] = ch;
			rkc.hacm[1] = L'\0';
			ret = _ConvRomanKana(&rkc);
			switch(ret)
			{
			case S_OK:	//一致
			case E_PENDING:	//途中まで一致
				if(rkc.hacm[0] != L'\0' && rkc.wait)	//待機
				{
					ch = L'\0';
					switch(inputmode)
					{
					case im_hiragana:
						roman.append(rkc.yula);
						break;
					default:
						break;
					}
				}
				break;
			default:
				break;
			}

			//ローマ字仮名変換
			std::wstring roman_conv = roman;
			if(ch != L'\0')
			{
				roman_conv.push_back(ch);
			}
			wcsncpy_s(rkc.hacm, roman_conv.c_str(), _TRUNCATE);
			ret = _ConvRomanKana(&rkc);

			if(ret == E_ABORT && wParam == VK_PACKET && ch != TKB_NEXT_PAGE && ch != TKB_PREV_PAGE)
			{
				rkc.hacm[0] = rkc.yula[0] = ch;
				rkc.hacm[1] = rkc.yula[1] = L'\0';
				rkc.soku = FALSE;
				rkc.wait = FALSE;
				ret = S_OK;
			}

			switch(ret)
			{
			case S_OK:	//一致
				if(rkc.hacm[0] != L'\0' && rkc.wait)	//待機
				{
					_HandleCharShift(ec, pContext);

					switch(inputmode)
					{
					case im_hiragana:
						roman.assign(rkc.yula);
						break;
					default:
						break;
					}

					_Update(ec, pContext);
				}
				else
				{
					std::wstring kana_ins;
					switch(inputmode)
					{
					case im_hiragana:
						kana_ins = rkc.yula;
						break;
					default:
						break;
					}

					if(!kana_ins.empty())
					{
						kana.insert(cursoridx, kana_ins);
						if(okuriidx != 0 && cursoridx <= okuriidx)
						{
							okuriidx += kana_ins.size();
						}
						cursoridx += kana_ins.size();
					}

					roman.clear();

					if(inputkey)
					{
						_HandleCharShift(ec, pContext);
						if(cx_begincvokuri && !hintmode &&
							!kana.empty() && okuriidx != 0 && !rkc.soku && !rkc.wait)
						{
							cursoridx = kana.size();
							showentry = TRUE;
							_StartConv(ec, pContext);
							_Update(ec, pContext);
							break;
						}

						if(cx_dynamiccomp || cx_dyncompmulti)
						{
							_DynamicComp(ec, pContext);

							if(rkc.soku)
							{
								roman.push_back(ch);
								_Update(ec, pContext);
							}
						}
						else
						{
							if(rkc.soku)
							{
								roman.push_back(ch);
							}
							_Update(ec, pContext);
						}
					}
					else
					{
						if(rkc.soku)
						{
							_HandleCharShift(ec, pContext);
							roman.push_back(ch);
							_Update(ec, pContext);
						}
						else
						{
							if(pContext != NULL)
							{
								cursoridx = kana.size();
								_Update(ec, pContext, TRUE);
							}
							_HandleCharReturn(ec, pContext);
						}
					}
				}
				break;

			case E_PENDING:	//途中まで一致
				_HandleCharShift(ec, pContext);

				if(rkc.hacm[0] != L'\0' && rkc.wait)	//待機
				{
					switch(inputmode)
					{
					case im_hiragana:
						roman.assign(rkc.yula);
						break;
					default:
						break;
					}
				}
				else
				{
					roman.push_back(ch);
				}

				if(pContext != NULL)
				{
					_Update(ec, pContext);
				}
				_Update(ec, pContext);
				break;

			case E_ABORT:	//不一致
				_HandleCharShift(ec, pContext);
				_Update(ec, pContext);
				break;

			default:
				break;
			}
			break;
		}
		break;

	default:
		break;
	}

	return ret;
}

HRESULT CTextService::_HandleCharReturn(TfEditCookie ec, ITfContext *pContext, BOOL back)
{
	_EndCompletionList(ec, pContext);

	//terminate composition
	cursoridx = kana.size();
	_Update(ec, pContext, TRUE, back);
	_TerminateComposition(ec, pContext);
	_ResetStatus();

	return S_OK;
}

HRESULT CTextService::_HandleCharShift(TfEditCookie ec, ITfContext *pContext)
{
	if(showentry || (!inputkey && !kana.empty() && roman.empty()))
	{
		_EndCompletionList(ec, pContext);

		//leave composition
		cursoridx = kana.size();
		_Update(ec, pContext, TRUE);
		if(pContext != NULL)
		{
			ITfRange *pRange;
			if(_IsComposing() && _pComposition->GetRange(&pRange) == S_OK)
			{
				pRange->Collapse(ec, TF_ANCHOR_END);
				_pComposition->ShiftStart(ec, pRange);
				SafeRelease(&pRange);
			}
		}
		_ResetStatus();
	}

	return S_OK;
}
