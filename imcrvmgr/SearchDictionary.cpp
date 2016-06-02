
#include "utf8.h"
#include "imcrvmgr.h"

//エントリの行頭位置
typedef std::vector<long> POS;
POS skkdicpos_a; //送りありエントリ
POS skkdicpos_n; //送りなしエントリ

void SearchDictionary(const std::wstring &searchkey, const std::wstring &okuri, SKKDICCANDIDATES &sc)
{
	std::wstring candidate;
	std::wregex re;
	std::wstring fmt;

	//ユーザー辞書
	candidate += SearchUserDic(searchkey, okuri);

	//SKK辞書
	candidate += SearchSKKDic(searchkey, okuri);

	if (okuri.empty()) {
		//Unicodeコードポイント
		candidate += SearchUnicode(searchkey);

		//JIS X 0213 面区点番号
		candidate += SearchJISX0213(searchkey);

		//JIS X 0218 区点番号
		candidate += SearchJISX0208(searchkey);

		if (searchkey.size() > 1 && searchkey[0] == L'?')
		{
			//文字コード表記変換 (ASCII, JIS X 0201(8bit), JIS X 0213 / Unicode)
			candidate += SearchCharacterCode(searchkey.substr(1));
		}
	}

	re.assign(L"/\n/");
	fmt.assign(L"/");
	candidate = std::regex_replace(candidate, re, fmt);

	re.assign(L"[\\x00-\\x19]");
	fmt.assign(L"");
	candidate = std::regex_replace(candidate, re, fmt);

	ParseSKKDicCandiate(candidate, sc);

	//重複候補を削除
	if(sc.size() > 1)
	{
		FORWARD_ITERATION_I(sc_itrf, sc)
		{
			for(auto sc_itrb = sc_itrf + 1; sc_itrb != sc.end(); )
			{
				if(sc_itrf->first == sc_itrb->first)
				{
					sc_itrb = sc.erase(sc_itrb);
				}
				else
				{
					++sc_itrb;
				}
			}
		}
	}
}

std::wstring SearchSKKDic(const std::wstring &searchkey, const std::wstring &okuri)
{
	FILE *fp;
	std::wstring candidate, wsbuf, kbuf, cbuf;
	WCHAR wbuf[READBUFSIZE];
	PWCHAR pwb;
	long pos, left, mid, right;
	size_t cidx;

	_wfopen_s(&fp, pathskkdic, RB);
	if(fp == nullptr)
	{
		return candidate;
	}

	left = 0;
	if(okuri.empty())
	{
		right = (long)skkdicpos_n.size() - 1;
	}
	else
	{
		right = (long)skkdicpos_a.size() - 1;
	}

	while(left <= right)
	{
		mid = left + (right - left) / 2;
		if(okuri.empty())
		{
			pos = skkdicpos_n[mid];
		}
		else
		{
			pos = skkdicpos_a[mid];
		}
		fseek(fp, pos, SEEK_SET);

		wsbuf.clear();
		kbuf.clear();
		cbuf.clear();

		while((pwb = fgetws(wbuf, _countof(wbuf), fp)) != nullptr)
		{
			wsbuf += wbuf;

			if(!wsbuf.empty() && wsbuf.back() == L'\n')
			{
				break;
			}
		}

		if(pwb == nullptr)
		{
			break;
		}

		if((cidx = wsbuf.find_last_of(L'/')) != std::wstring::npos)
		{
			wsbuf.erase(cidx + 1);
			wsbuf.push_back(L'\n');
		}

		if((cidx = wsbuf.find_first_of(L'\x20')) != std::wstring::npos)
		{
			kbuf = wsbuf.substr(0, cidx);
			if((cidx = wsbuf.find_first_of(L'/', cidx)) != std::wstring::npos)
			{
				cbuf = wsbuf.substr(cidx);
			}
		}

		int cmpkey = searchkey.compare(kbuf);
		if(cmpkey == 0)
		{
			candidate = cbuf;
			break;
		}
		else if(cmpkey > 0)
		{
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}
	}

	fclose(fp);

	return candidate;
}

void MakeSKKDicPos()
{
	FILE *fp;
	WCHAR wbuf[READBUFSIZE];
	PWCHAR pwb, pwn;
	long pos;
	int okuri = -1;

	skkdicpos_a.clear();
	skkdicpos_a.shrink_to_fit();
	skkdicpos_n.clear();
	skkdicpos_n.shrink_to_fit();

	_wfopen_s(&fp, pathskkdic, RB);
	if(fp == nullptr)
	{
		return;
	}

	fseek(fp, 2, SEEK_SET); //skip BOM
	pos = ftell(fp);

	while(true)
	{
		while((pwb = fgetws(wbuf, _countof(wbuf), fp)) != nullptr)
		{
			if((pwn = wcschr(wbuf, L'\n')) != nullptr)
			{
				if((pwn != wbuf) && (*(pwn - 1) == L'\r'))
				{
					*(pwn - 1) = L'\n';
					*pwn = L'\0';
				}
				break;
			}
		}

		if(pwb == nullptr)
		{
			break;
		}

		if(wcscmp(EntriesAri, wbuf) == 0)
		{
			okuri = 1;
		}
		else if(wcscmp(EntriesNasi, wbuf) == 0)
		{
			okuri = 0;
		}
		else
		{
			switch(okuri)
			{
			case 1:
				skkdicpos_a.push_back(pos);
				break;
			case 0:
				skkdicpos_n.push_back(pos);
				break;
			default:
				break;
			}
		}

		pos = ftell(fp);
	}

	fclose(fp);

	std::reverse(skkdicpos_a.begin(), skkdicpos_a.end());
}

std::wstring ConvertKey(const std::wstring &searchkey, const std::wstring &okuri)
{
	std::wstring ret;
	std::wregex re(L"[0-9]+");
	std::wstring fmt(L"#");

	//xtu/xtsuで「っ」を送り仮名にしたとき送りローマ字を「t」に変換する
	if (okuri == L"っ" && searchkey.back() == L'x') {
		return searchkey.substr(0, searchkey.size() - 1) + L"t";
	}

	//文字コード表記変換のとき見出し語変換しない
	if (searchkey.size() > 1 && searchkey[0] == L'?')
	{
		return std::wstring(L"");
	}

	//数値変換
	if (std::regex_search(searchkey, re)) {
		ret = std::regex_replace(searchkey, re, fmt);
	}

	ret = std::regex_replace(ret, std::wregex(L"[\\x00-\\x19]"), std::wstring(L""));

	return ret;
}

std::wstring ConvertCandidate(const std::wstring &searchkey, const std::wstring &candidate, const std::wstring &okuri)
{
	std::wstring ret;
	std::wstring candidate_tmp = candidate;
	std::wregex rxnum(L"[0-9]+");
	std::wregex rxint(L"#[0-9]");

	//xtu/xtsuで「っ」を送り仮名にしたとき送りローマ字「t」を有効にする
	if (okuri == L"っ" && searchkey.back() == L'x') {
		return candidate;
	}

	//数値変換
	if (std::regex_search(searchkey, rxnum) && std::regex_search(candidate_tmp, rxint)) {
		ret = ConvNum(searchkey, candidate_tmp);
		candidate_tmp = ret;
	}

	//concat
	if (candidate_tmp.size() > 2 &&
		candidate_tmp[0] == L'(' && candidate_tmp[candidate_tmp.size() - 1] == L')') {
		ret = ParseConcat(candidate_tmp);
	}

	ret = std::regex_replace(ret, std::wregex(L"[\\x00-\\x19]"), std::wstring(L""));

	return ret;
}
