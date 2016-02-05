
#include "parseskkdic.h"
#include "configxml.h"
#include "utf8.h"
#include "imcrvmgr.h"

void SearchDictionary(const std::wstring &searchkey, const std::wstring &okuri, SKKDICCANDIDATES &sc)
{
	std::wstring candidate;
	std::wregex re;
	std::wstring fmt;

	//ユーザー辞書
	candidate += SearchUserDic(searchkey, okuri);

	//SKK辞書
	candidate += SearchSKKDic(searchkey);

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

std::wstring SearchSKKDic(const std::wstring &searchkey)
{
	FILE *fpdic, *fpidx;
	std::wstring key, candidate, wsbuf;
	WCHAR wbuf[DICBUFSIZE];
	long pos, left, mid, right;
	int cmpkey;
	size_t pidx;

	_wfopen_s(&fpidx, pathskkidx, RB);
	if(fpidx == NULL)
	{
		return candidate;
	}
	_wfopen_s(&fpdic, pathskkdic, RB);
	if(fpdic == NULL)
	{
		fclose(fpidx);
		return candidate;
	}

	key = searchkey + L"\x20";

	left = 0;
	fseek(fpidx, 0, SEEK_END);
	right = (ftell(fpidx) / sizeof(pos)) - 1;

	while(left <= right)
	{
		mid = (left + right) / 2;
		fseek(fpidx, mid * sizeof(pos), SEEK_SET);
		fread(&pos, sizeof(pos), 1, fpidx);

		fseek(fpdic, pos, SEEK_SET);
		memset(wbuf, 0, sizeof(wbuf));

		fgetws(wbuf, _countof(wbuf), fpdic);
		wsbuf = wbuf;

		cmpkey = wcsncmp(key.c_str(), wsbuf.c_str(), key.size());
		if(cmpkey == 0)
		{
			if((pidx = wsbuf.find_last_of(L'/')) != std::string::npos)
			{
				wsbuf.erase(pidx);
				wsbuf.append(L"/\n");
			}

			if((pidx = wsbuf.find_first_of(L'\x20')) != std::string::npos)
			{
				if((pidx = wsbuf.find_first_of(L'/', pidx)) != std::string::npos)
				{
					candidate = wsbuf.substr(pidx);
				}
			}
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

	fclose(fpdic);
	fclose(fpidx);

	return candidate;
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

	return ret;
}
