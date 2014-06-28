////////////////////////////////////////////////////////////////////////////////
// AxHell - A simple Exploitable ActiveX Control for RE/VR 
// Copyright (C) 2010 James Forshaw

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CommonUtils.h"
#include <strsafe.h>

#pragma comment(lib, "crypt32.lib")

void CCommonUtils::DebugPrintf(LPCWSTR fmt, ...)
{
    WCHAR buf[1024];
    va_list va;

    va_start(va, fmt);
    vswprintf_s(buf, fmt, va);
    OutputDebugString(buf);
    va_end(va);
}

const GUID GUID_CUSTOM_CONFIRMOBJECTSAFETY =
    {0x10200490,0xfa38,0x11d0,{0xac,0x0e,0x00,0xa0,0xc9,0xf,0xff,0xc0}};

// This take from MSDN and probably isn't safe :)
HRESULT CCommonUtils::CreateObject(IInternetHostSecurityManager *psecman, 
    DWORD dwSafetyEnabled, BOOL fWillLoad,
    CLSID clsid, REFIID riid, void **ppv)
{
    HRESULT hr;
 
    DEBUG_PRINTF(L"%p 0x%08X\n", psecman, dwSafetyEnabled);
    
    if(ppv)
    {
        *ppv = NULL;
    }

    // If we have safety enabled and the security manager is available
    if(dwSafetyEnabled && psecman)
    {
        // Ask security manager if we can create objects.
        DWORD dwPolicy;
        if (FAILED(hr = psecman->ProcessUrlAction(
             URLACTION_ACTIVEX_RUN, (BYTE *)&dwPolicy, sizeof(dwPolicy), 
             (BYTE *)&clsid, sizeof(clsid), 0, 0)))
        {
            return hr;
        }
    
        if (URLPOLICY_ALLOW != dwPolicy)
            return E_ACCESSDENIED;
    }

    if (FAILED(hr = CoCreateInstance(clsid, NULL, CLSCTX_SERVER, riid, ppv)))
        return hr;

    // Must go to CLEANUP after this point on failure to free *ppv.
    
    if(dwSafetyEnabled && psecman)
    {
        // Query the security manager to see if this object is safe to use.
        DWORD dwPolicy, *pdwPolicy, cbPolicy;
        
        CONFIRMSAFETY csafe;
        csafe.pUnk = *(IUnknown **)ppv;
        csafe.clsid = clsid;
        csafe.dwFlags = (fWillLoad ? CONFIRMSAFETYACTION_LOADOBJECT : 0);
        
        if (FAILED(hr = psecman->QueryCustomPolicy(
            GUID_CUSTOM_CONFIRMOBJECTSAFETY, (BYTE **)&pdwPolicy, 
            &cbPolicy, (BYTE *)&csafe, sizeof(csafe), 0)))
        {
            goto CLEANUP;
        }

        dwPolicy = URLPOLICY_DISALLOW;
        if (NULL != pdwPolicy)
        {
            if (sizeof(DWORD) <= cbPolicy)
                dwPolicy = *pdwPolicy;
            CoTaskMemFree(pdwPolicy);
        }
    
        if (URLPOLICY_ALLOW != dwPolicy)
        {
            hr = E_ACCESSDENIED;
            goto CLEANUP;
        }
    }

    return NOERROR;

CLEANUP:
    (*(IUnknown **)ppv)->Release();
    *ppv = NULL;

    return hr;
}

LPWSTR CCommonUtils::ErrorToString(HRESULT hr)
{
    LPWSTR lpRet = NULL;

    if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, hr, 0, (LPWSTR)&lpRet, 8*1024, NULL) == 0)
    {
        lpRet = NULL;
    }

    return lpRet;
}

HRESULT CCommonUtils::SetErrorCode(HRESULT hr)
{
    if(FAILED(hr))
    {
        CComPtr<ICreateErrorInfo> createErrorInfo;

        if(SUCCEEDED(CreateErrorInfo(&createErrorInfo)))
        {
            CComPtr<IErrorInfo> errorInfo;

            createErrorInfo->SetDescription(ErrorToString(hr));
            createErrorInfo->SetGUID(GUID_NULL);
            createErrorInfo->SetHelpContext(0);
            createErrorInfo->SetHelpFile(NULL);
            createErrorInfo->SetSource(NULL);

            errorInfo = createErrorInfo;
            SetErrorInfo(0, errorInfo);
        }
    }

    return hr;
}

bool CCommonUtils::IsValidFileName(LPCWSTR lpString)
{
    UNREFERENCED_PARAMETER(lpString);
    return false;
}

bool CCommonUtils::RunProcess(LPWSTR bstrName, BOOL bWait)
{
    STARTUPINFO startInfo = {0};
    PROCESS_INFORMATION procInfo = {0};
    bool bRet = false;

    try
    {
        if(bstrName)
        {
            if(!CreateProcess(NULL, bstrName, NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo))
            {
                throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
            }
            else
            {
                if(bWait)
                {
                    WaitForSingleObject(procInfo.hProcess, INFINITE);
                }

                bRet = true;
                CloseHandle(procInfo.hProcess);
                CloseHandle(procInfo.hThread);
            }
        }
    }
    catch(_com_error& err)
    {
        DEBUG_PRINTF(L"Error 0x%08X\n", err.Error());
        bRet = false;
    }

    return bRet;
}


bool CCommonUtils::CalcFileHash(ALG_ID hashType, LPCTSTR lpFileName, BYTE* lpHash, DWORD dwHashSize)
{
    LPVOID lpData;
    DWORD  dwLen;
    bool bRet = false;

    lpData = ReadFileData(lpFileName, dwLen);
    if(lpData)
    {
        bRet = CalcHash(hashType, (LPBYTE)lpData, dwLen, lpHash, dwHashSize);
        free(lpData);
    }

    return bRet;
}

bool CCommonUtils::CalcHash(ALG_ID hashType, const BYTE* lpData, DWORD dwLen, BYTE* lpHash, DWORD dwHashSize)
{
    HCRYPTPROV hProv = NULL;
    HCRYPTHASH hHash = NULL;
    bool bRet = false;

    if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
    {
        goto endFunction;
    }

    if(!CryptCreateHash(hProv, hashType, 0, 0, &hHash)) 
    {
        goto endFunction;
    }

    if(!CryptHashData(hHash, lpData, dwLen, 0)) 
    {
        goto endFunction;
    }

    if(!CryptGetHashParam(hHash, HP_HASHVAL, lpHash, &dwHashSize, 0))
    {
        goto endFunction;
    }

    bRet = true;

endFunction:

    if(hHash) 
    {
        CryptDestroyHash(hHash);
    }

    if(hProv) 
    {
        CryptReleaseContext(hProv, 0);
    }

    return bRet;
}

LPVOID CCommonUtils::ReadFileData(LPCTSTR lpFileName, DWORD& dwLength)
{
    FILE* fp = NULL;
    LPVOID lpRet = NULL;

    try
    {
        if(_tfopen_s(&fp, lpFileName, _T("rb")) != 0)
        {
            throw _com_error(E_FAIL);
        }

        if(fseek(fp, 0, SEEK_END) != 0)
        {
            throw _com_error(E_FAIL);
        }

        dwLength = ftell(fp);

        if(fseek(fp, 0, SEEK_SET) != 0)
        {
            throw _com_error(E_FAIL);
        }
    
        lpRet = malloc(dwLength);
        if(lpRet == NULL)
        {
            throw _com_error(E_OUTOFMEMORY);
        }

        if(fread(lpRet, 1, dwLength, fp) != dwLength)
        {
            throw _com_error(E_FAIL);
        }
    }
    catch(_com_error& err)
    {
        DEBUG_PRINTF(L"Error in reading file 0x%08X\n", err.Error());
        if(lpRet)
        {
            free(lpRet);
            lpRet = NULL;
        }
    }

    if(fp)
    {
        fclose(fp);
        fp = NULL;
    }

    return lpRet;
}

bool CCommonUtils::VerifyFile(LPCTSTR lpFileName, LPVOID lpKeyData, DWORD dwKeySize, LPCTSTR lpBase64)
{
    LPVOID lpFileData = NULL;
    DWORD  dwFileSize = 0;
    bool bRet = false;
    HCRYPTPROV hProv = NULL;
    HCRYPTKEY  hKey = NULL;
    HCRYPTHASH hHash = NULL;
    LPVOID lpSignature = NULL;
    DWORD  dwSigSize = 0;

    try
    {
        lpFileData = ReadFileData(lpFileName, dwFileSize);
        if(!lpFileData)
        {
            throw _com_error(E_FAIL);
        }

        if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
        {
            throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
        }

        if(!CryptImportKey(hProv, (LPBYTE)lpKeyData, dwKeySize, NULL, 0, &hKey))
        {
            throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
        }

        if(!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) 
        {
            throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
        }

        if(!CryptHashData(hHash, (LPBYTE)lpFileData, dwFileSize, 0)) 
        {
            throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
        }

        if(!CryptStringToBinary(lpBase64, 0, CRYPT_STRING_BASE64, NULL, &dwSigSize, NULL, NULL))
        {
            throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
        }

        lpSignature = malloc(dwSigSize);
        if(!CryptStringToBinary(lpBase64, 0, CRYPT_STRING_BASE64, (LPBYTE)lpSignature, &dwSigSize, NULL, NULL))
        {
            throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
        }

        if(!CryptVerifySignature(hHash, (LPBYTE)lpSignature, dwSigSize, hKey, NULL, 0))
        {
            throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
        }

        bRet = true;
    }
    catch(_com_error& err)
    {
        DEBUG_PRINTF(L"Error in verifying file 0x%08X\n", err.Error());
    }

    if(hHash)
    {
        CryptDestroyHash(hHash);
    }

    if(hProv)
    {
        CryptReleaseContext(hProv, 0);
    }

    if(lpSignature)
    {
        free(lpSignature);
    }

    if(lpFileData)
    {
        free(lpFileData);
    }

    if(lpKeyData)
    {
        free(lpKeyData);
    }

    return bRet;
}

int CCommonUtils::CompareByteStrings(const BYTE* str1, const BYTE* str2, size_t len)
{
    for(size_t i = 0; i < len; i++)
    {
        if(str1[i] != str2[i])
        {
            return (int)str2[i] - (int)str1[i];
        }

        if(str1[i] == 0)
        {
            break;
        }
    }

    return 0;
}

bstr_t CCommonUtils::CreateRelativeURL(bstr_t baseURL, bstr_t relURL)
{
    WCHAR ret[INTERNET_MAX_URL_LENGTH];
    DWORD dwLen = INTERNET_MAX_URL_LENGTH;
    HRESULT hr = E_FAIL;

    if(PathIsURL(baseURL))
    {
        hr = UrlCombine(baseURL, relURL, ret, &dwLen, 0);
        if(FAILED(hr))
        {
            throw _com_error(hr);
        }
    }
    else
    {
        hr = StringCchCopyW(ret, sizeof(ret)/sizeof(WCHAR), relURL);
        if(FAILED(hr))
        {
            throw _com_error(hr);
        }
    }

    return ret;
}

int HexToInt(WCHAR ch)
{
    if((ch >= '0') && (ch <= '9'))
    {
        return ch - '0';
    }
    else if((ch >= 'A') && (ch <= 'Z'))
    {
        return (ch - 'A') + 10;
    }
    else if((ch >= 'a') && (ch <= 'z'))
    {
        return (ch - 'a') + 10;
    }

    return -1;
}

WCHAR IntToHex(int val)
{
    val &= 0xF;
    if((val >= 0) && (val < 10))
    {
        return (WCHAR)('0' + val);
    }
    else if((val >= 10) && (val < 16))
    {
        return (WCHAR)('A' + val - 10);
    }
    else
    {
        // Shouldn't reach here
        return L'0';
    }
}

LPVOID CCommonUtils::HexToBinary(BSTR bstrHex, ULONG* pulSize)
{
    LPBYTE lpRet = NULL;
    UINT  len = ::SysStringLen(bstrHex);
    UINT  pos = 0;

    if(len & 1)
    {
        // Invalid length, must be a multiple of 2 characters
        throw _com_error(E_INVALIDARG);
    }

    lpRet = static_cast<LPBYTE>(LocalAlloc(LPTR, len / 2));
    if(lpRet == NULL)
    {
        throw _com_error(E_FAIL);
    }

    for(pos = 0; pos < len; pos += 2)
    {
        int unib = HexToInt(bstrHex[pos]);
        int lnib = HexToInt(bstrHex[pos+1]);

        if((unib < 0) || (lnib < 0))
        {
            break;
        }

        lpRet[pos/2] = static_cast<BYTE>((unib << 4) | (lnib));
    }

    if(pos == len)
    {
        *pulSize = len / 2;
    }
    else
    {
        (void)LocalFree(lpRet);
        lpRet = NULL;
        throw _com_error(E_INVALIDARG);
    }

    return lpRet;
}


BSTR CCommonUtils::BinaryToHex(LPVOID lpData, ULONG ulSize)
{
    LPWSTR lpString = NULL;
    BSTR   bstrRet = NULL;
    LPBYTE lpBytes = (LPBYTE)lpData;
    DWORD dwSize = 0;

    dwSize = ulSize*2*sizeof(WCHAR)+sizeof(WCHAR);
    // Alloc enough for two chars per byte plus NUL
    lpString = (LPWSTR)LocalAlloc(LPTR, dwSize);
    if(!lpString)
    {
        throw _com_error(E_OUTOFMEMORY);
    }

    memset(lpString, 0, dwSize);
    for(UINT pos = 0; pos < ulSize; pos++)
    {
        lpString[pos*2] = IntToHex(lpBytes[pos] >> 4);
        lpString[pos*2+1] = IntToHex(lpBytes[pos] & 0xF);
    }

    bstrRet = ::SysAllocString(lpString);
    LocalFree(lpString);

    return bstrRet;
}

bstr_t CCommonUtils::GetUrlHostName(LPCSTR lpURL)
{
    bstr_t lpRet = L"";
    URL_COMPONENTSA urlComponents = {0};
    
    urlComponents.dwStructSize = sizeof(urlComponents);
    urlComponents.lpszHostName = (LPSTR)malloc(strlen(lpURL)+1);
    urlComponents.dwHostNameLength = strlen(lpURL)+1;
    if(InternetCrackUrlA(lpURL, 0, 0, &urlComponents))
    {
        lpRet = urlComponents.lpszHostName;		
    }

    free(urlComponents.lpszHostName);	

    return lpRet;
}
