#pragma once

#include <atlbase.h>
#include <atlctl.h>
#include <comdef.h>
#include <shlguid.h>
#include <wininet.h>
#include <WinCrypt.h>

using namespace ATL;

#ifdef _ENABLE_DEBUG_OUTPUT
#define DEBUG_PRINTF(x, ...) CCommonUtils::DebugPrintf(L"%hs: " x, __FUNCTION__, ## __VA_ARGS__)
#else
#define DEBUG_PRINTF(x, ...) ((void)0)
#endif

#define SHA1_HASH_SIZE (20)

class CCommonUtils
{
	CCommonUtils(void) {}
	~CCommonUtils(void) {}

public:

	static void DebugPrintf(LPCWSTR fmt, ...);
	static bstr_t CreateRelativeURL(bstr_t baseURL, bstr_t relURL);
	static int CompareByteStrings(const BYTE* str1, const BYTE* str2, size_t len);
	//static CComBSTR GetStringFromVariant(CComVariant& var);
	static HRESULT CreateObject(IInternetHostSecurityManager *psecman, DWORD dwSafetyEnabled, BOOL fWillLoad,
						CLSID clsid, REFIID riid, void **ppv);
	static HRESULT SetErrorCode(HRESULT hr);
	static LPWSTR  ErrorToString(HRESULT hr);
	// Verify a signature against a static public key
	static bool VerifyFileSignature(LPCWSTR lpFileName, LPCWSTR lpSignature);
	static bool IsValidFileName(LPCWSTR lpString);
	static bool CalcFileHash(ALG_ID hashType, LPCTSTR lpFileName, BYTE* lpHash, DWORD dwHashSize);
	static bool CalcHash(ALG_ID hashType, const BYTE* lpData, DWORD dwLen, BYTE* lpHash, DWORD dwHashSize);
	static LPVOID ReadFileData(LPCTSTR lpFileName, DWORD& dwLength);
	static bool VerifyFile(LPCTSTR lpFileName, LPVOID lpKeyData, DWORD dwKeySize, LPCTSTR lpBase64);
	static bool RunProcess(LPWSTR bstrName, BOOL bWait);
	// Convert a hex string to a binary blob
	static LPVOID HexToBinary(BSTR bstrHex, ULONG* pulSize);
	// Convert a binary blob to a hex string
	static BSTR BinaryToHex(LPVOID lpData, ULONG ulSize);
	static bstr_t GetUrlHostName(LPCSTR lpURL);
};

#define SET_HRESULT(hr) CCommonUtils::SetErrorCode(hr)

