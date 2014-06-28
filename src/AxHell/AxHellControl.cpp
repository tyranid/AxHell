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
#include "stdafx.h"
#include "AxHellControl.h"
#include <strsafe.h>
#include <stdlib.h>
#include "ByteStream.h"

#import <msxml3.dll>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "wininet.lib")

// Public key for verifying installer signature
unsigned char publicKey[148] = {
	0x06, 0x02, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x52, 0x53, 0x41, 0x31, 0x00, 0x04, 0x00, 0x00,
	0x01, 0x00, 0x01, 0x00, 0x37, 0x30, 0xAD, 0x26, 0x79, 0x7C, 0x88, 0xFD, 0xFC, 0x9E, 0x3C, 0x70,
	0x93, 0x8F, 0x4E, 0x74, 0x11, 0x52, 0xF7, 0x96, 0xF2, 0x87, 0xEC, 0x7F, 0x60, 0xC3, 0x65, 0x6F,
	0x09, 0xDC, 0xBE, 0xCD, 0xD3, 0xC3, 0xFE, 0xB2, 0xB3, 0x28, 0x31, 0xE0, 0x9F, 0x24, 0x9E, 0x91,
	0xF5, 0x13, 0x97, 0x1C, 0x70, 0x6B, 0x95, 0x91, 0x4C, 0x22, 0x53, 0xF2, 0x79, 0x8F, 0xE8, 0x7E,
	0x6C, 0x53, 0x21, 0xA7, 0x83, 0xBD, 0xDD, 0x42, 0x89, 0xC6, 0xF5, 0xA3, 0x9D, 0x29, 0xC1, 0xE3,
	0x6D, 0xA3, 0x6D, 0xD7, 0x3B, 0x24, 0x2F, 0x52, 0xE8, 0xA9, 0xD7, 0xAF, 0xC6, 0xE5, 0xB0, 0x4B,
	0xC2, 0x1A, 0xF3, 0x05, 0xB8, 0xD2, 0xC1, 0xBC, 0x89, 0x81, 0x7B, 0x42, 0x35, 0x28, 0xD3, 0x81,
	0x95, 0xFE, 0x8E, 0xDB, 0x33, 0x17, 0x52, 0x26, 0x1B, 0xBC, 0x76, 0xF2, 0xE7, 0xBE, 0x6D, 0x45,
	0xF3, 0xA5, 0x09, 0x9E
};

// Define two different hash values for possible installers
unsigned char sha1Hash[][SHA1_HASH_SIZE] = {
	{ 0xb2, 0x6e, 0x44, 0x76, 0x4e, 0x3e, 0xd0, 0x24, 0x82, 0x03, 
	  0x2a, 0x98, 0x7c, 0x9d, 0xb3, 0x15, 0x3c, 0xbf, 0xf0, 0xab },
	{ 0xa0, 0x19, 0x00, 0xd9, 0xfa, 0x30, 0xae, 0x53, 0x9b, 0x89, 
	  0x03, 0x78, 0x93, 0xbb, 0xa4, 0x8d, 0x1c, 0x59, 0xe0, 0xc3 },
};

// List of allowed utilities
LPCWSTR allowedUtilities[] = {
	L"notepad",
	L"mspaint"
};

////////////////////////////////////////////////////////////////////
// IsValidUtility 
// Checks a utility name is valid
//
// Parameters:
// lpName - Name of the utility to check
//
// Return:
// true if a valid utility, false otherwise
////////////////////////////////////////////////////////////////////
static bool IsValidUtility(LPCWSTR lpName)
{
	if(lpName)
	{
		for(size_t i = 0; i < _countof(allowedUtilities); i++)
		{
			if(_wcsicmp(allowedUtilities[i], lpName) == 0)
			{
				return true;
			}
		}
	}

	return false;
}


////////////////////////////////////////////////////////////////////
// RunUtility
// Run a utility, only allow a permitted set of applications
//
// Parameters:
// varName - Variant holding the string for the utility
//
// Return:
// S_OK = Success
// E_FAIL = Generic error
// E_ACCESSDENIED - Not a permitted utility
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::RunUtility(VARIANT varName)
{
	HRESULT hr = E_FAIL;
	variant_t var(varName);

	try
	{
		if(IsValidUtility((bstr_t)var))
		{
			if(CCommonUtils::RunProcess((bstr_t)var, FALSE))
			{
				hr = S_OK;
			}
			else
			{
				hr = E_FAIL;
			}
		}
		else
		{
			throw _com_error(E_ACCESSDENIED);
		}
	}
	catch(_com_error& err)
	{
		hr = err.Error();
	}

	return SET_HRESULT(hr);
}

////////////////////////////////////////////////////////////////////
// ReverseString
// Reverses a string buffer and returns it to the caller
//
// Parameters:
// bstrString - The string to reverse
// bstrRet - The BSTR pointer to return the reversed string
//
// Return:
// S_OK = Success
// E_INVALIDARG - Invalid argument
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::ReverseString(BSTR bstrString, BSTR* bstrRet)
{
	WCHAR buf[512] = {0};
	UINT  i = 0;
	UINT  len;
	HRESULT hr = E_FAIL;

	len = ::SysStringLen(bstrString);
	
	if(len < sizeof(buf))
	{
		while(len > 0)
		{
			buf[i] = bstrString[len-1];
			i++;
			len--;
		}

		if(bstrRet != NULL)
		{
			*bstrRet = ::SysAllocString(buf);
		}

		hr = S_OK;
	}
	else
	{
		hr = E_INVALIDARG;
	}

	return SET_HRESULT(hr);
}

////////////////////////////////////////////////////////////////////
// CreateRandomString
// Creates a random string and returns it to the caller
//
// Parameters:
// bstrRet - The BSTR pointer to return the random string
//
// Return:
// S_OK - Success
// E_INVALIDARG - Invalid argument
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::CreateRandomString(ULONG ulSeed, BSTR* bstrRet)
{
	WCHAR buf[512];	
	HRESULT hr = E_FAIL;
	ULONG len;

	try
	{
		if(bstrRet == NULL)
		{
			throw _com_error(E_INVALIDARG);
		}
		
		if(ulSeed != 0)
		{
			srand(ulSeed);
		}

		len = 1 + (ULONG) (_countof(buf) * ((double)rand() / (double)(RAND_MAX+1)));
		
		memset(buf, 0, sizeof(buf));
		for(ULONG i = 0; i < len; i++)
		{
			buf[i] = (WCHAR)rand();
		}
		
		*bstrRet = ::SysAllocString(buf);
		if(*bstrRet == NULL)
		{
			throw _com_error(E_OUTOFMEMORY);
		}

		hr = S_OK;
	}
	catch(_com_error& err)
	{
		hr = err.Error();
	}

	return SET_HRESULT(hr);
}

////////////////////////////////////////////////////////////////////
// TransformXSLT
// Do an XSLT transform on some supplied XML and XSLT data
//
// Parameters:
// bstrXML - The XML to transform
// bstrXSLT - The XSLT to use to transform
// bstrResult - The BSTR pointer to return the transformed data in
//
// Return:
// S_OK - Success
// E_INVALIDARG - Invalid argument
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::TransformXSLT(BSTR bstrXML, BSTR bstrXSLT, BSTR* bstrResult)
{
	MSXML2::IXMLDOMDocumentPtr xmlDocument;
	MSXML2::IXMLDOMDocumentPtr xsltDocument; 
	HRESULT hr = E_FAIL;
	bstr_t  ret;

	try
	{
		if(!bstrXML || !bstrXSLT || !bstrResult)
		{
			throw _com_error(E_INVALIDARG);
		}

		hr = xmlDocument.CreateInstance(__uuidof(MSXML2::DOMDocument30));
		if(FAILED(hr))
		{
			throw _com_error(hr);
		}

		// Prevent external files being loaded
		xmlDocument->resolveExternals = VARIANT_FALSE;
		xmlDocument->async = VARIANT_FALSE;

		if(xmlDocument->loadXML(bstrXML) != VARIANT_TRUE)
		{
			throw _com_error(hr);
		}

		hr = xsltDocument.CreateInstance(__uuidof(MSXML2::DOMDocument30));
		if(FAILED(hr))
		{
			throw _com_error(hr);
		}

		xsltDocument->resolveExternals = VARIANT_FALSE;
		xsltDocument->async = VARIANT_FALSE;

		if(xsltDocument->loadXML(bstrXSLT) != VARIANT_TRUE)
		{
			throw _com_error(E_INVALIDARG);
		}

		ret = xmlDocument->transformNode(xsltDocument);

		*bstrResult = ret.Detach();
	}
	catch(_com_error& err)
	{
		DEBUG_PRINTF(L"Error %ls\n", err.ErrorMessage());
		hr = err.Error();
	}

	return SET_HRESULT(hr);
}

////////////////////////////////////////////////////////////////////
// QuerySite
// Internal function to query the site object for an interface
//
// Parameters:
// riid - The IID to query for
// ppv - Pointer to an object to put queried object
//
// Return:
// S_OK - Success
// E_INVALIDARG - Invalid argument
// E_* - Other errors
////////////////////////////////////////////////////////////////////
HRESULT CAxHellControl::QuerySite(REFIID riid, void** ppv)
{
	HRESULT hr = E_INVALIDARG;

	if(ppv)
	{
		*ppv = NULL;

		// IObjectWithSite
		if(m_spClientSite)
		{
			hr = m_spClientSite->QueryInterface(riid, ppv);
		}

		// IOleClientSite
		if(FAILED(hr) && m_spUnkSite)
		{
			hr = m_spUnkSite->QueryInterface(riid, ppv);
		}
	}

	return hr;
}

////////////////////////////////////////////////////////////////////
// QuerySiteForServiceProvider
// Internal function to query the service provide for the site
//
// Parameters:
// pServiceProv - Pointer to store the service provider object
//
// Return:
// S_OK - Success
// E_INVALIDARG - Invalid argument
// E_* - Other errors
////////////////////////////////////////////////////////////////////
HRESULT CAxHellControl::QueryForSiteServiceProvider(IServiceProvider** pServiceProv)
{
	CComPtr<IServiceProvider> serviceProv;
	HRESULT hr = E_INVALIDARG;

	try
	{
		if(pServiceProv == NULL)
		{
			throw _com_error(E_INVALIDARG);
		}

		*pServiceProv = NULL;

		serviceProv = m_spClientSite;
		if(serviceProv == NULL)
		{
			serviceProv = m_spUnkSite;
		}

		if(serviceProv == NULL)
		{
			throw _com_error(E_FAIL);
		}
		else
		{
			hr = S_OK;
		}

		*pServiceProv = serviceProv.Detach();
	}
	catch(_com_error& err)
	{
		hr = err.Error();
	}

	return hr;
}

////////////////////////////////////////////////////////////////////
// QuerySiteForService
// Internal function to query the service provide for the site
//
// Parameters:
// serviceGuid - The service GUID
// riid - The IID to query for
// ppv - A pointer to store the resulting interface
//
// Return:
// S_OK - Success
// E_INVALIDARG - Invalid argument
// E_* - Other errors
////////////////////////////////////////////////////////////////////
HRESULT CAxHellControl::QuerySiteForService(REFGUID serviceGuid, REFIID riid, void** ppv)
{
	DEBUG_PRINTF(L"OLE %p, Object %p\n", m_spClientSite, m_spUnkSite);
	CComPtr<IServiceProvider> serviceProv;
	HRESULT hr = E_INVALIDARG;

	try
	{
		hr = QueryForSiteServiceProvider(&serviceProv);
		if(FAILED(hr))
		{
			throw _com_error(hr);
		}

		hr = serviceProv->QueryService(serviceGuid, riid, ppv);
	}
	catch(_com_error& err)
	{
		hr = err.Error();
	}

	return hr;
}

////////////////////////////////////////////////////////////////////
// CreateObject
// Function to create a new ActiveX object
//
// Parameters:
// bstrCLSID - The classid string of the control
// ppObject - Variant to store the object if successful
//
// Return:
// S_OK - Success
// E_* - Other errors
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::CreateObject(BSTR bstrCLSID, VARIANT* ppObject)
{
	CLSID clsid = CLSID_NULL;
	HRESULT hr = E_FAIL;
	CComPtr<IInternetHostSecurityManager> secman;
	IDispatch* pDisp = NULL;

	try
	{
		if(ppObject == NULL)
		{
			throw _com_error(E_INVALIDARG);
		}

		VariantInit(ppObject);

		hr = CLSIDFromString(bstrCLSID, &clsid);
		if(FAILED(hr))
		{
			hr = CLSIDFromProgID(bstrCLSID, &clsid);
		}

		if(FAILED(hr))
		{
			DEBUG_PRINTF(L"Error: 0x%08X\n", hr);
			throw _com_error(hr);
		}

		hr = QuerySiteForService(SID_SInternetHostSecurityManager, IID_PPV_ARGS(&secman));
		if(FAILED(hr))
		{
			DEBUG_PRINTF(L"Couldnt get service 0x%08X\n", hr);
			throw _com_error(hr);
		}

		DEBUG_PRINTF(L"Creating Object\n");
		// Create object honouring safety flags if available
		hr = CCommonUtils::CreateObject(secman, m_dwCurrentSafety, FALSE, clsid, IID_PPV_ARGS(&pDisp));
		if(FAILED(hr))
		{
			throw _com_error(hr);
		}
		DEBUG_PRINTF(L"%p\n", pDisp);

		V_VT(ppObject) = VT_DISPATCH;
		V_DISPATCH(ppObject) = pDisp;
		pDisp = NULL;

		DEBUG_PRINTF(L"CreateObject 0x%08X\n", hr);
	}
	catch(_com_error& err)
	{
		hr = err.Error();
	}

	if(pDisp)
	{
		pDisp->Release();
		pDisp = NULL;
	}

	return SET_HRESULT(hr);
}

// Hint structure
struct HintData
{
	LPCWSTR lpName;
	LPCWSTR lpHint;
};

// List of hints
HintData hints[] = {
	{ L"GetHint", L"What do you think?" },
	{ L"RunUtility", L"Do object's strike twice?" },
	{ L"CreateRandomString", L"Not everything is as random as it seems" },
	{ L"ReverseString", L"Don't forget to return to the beginning" },
	{ L"TransformXSLT", L"Feature rich, safety poor" },
	{ L"CreateObject", L"Knowing is half the battle" },
	{ L"RunInstaller", L"Sometimes different questions lead to the right answer" },
	{ L"RunInstallerEx", L"A real brute to defeat" },
	{ L"RunInstallerExEx", L"Меѓународен домените?" },
	{ L"CreateByteStream", L"A surfeit of resources can take you everywhere" },
};

////////////////////////////////////////////////////////////////////
// GetHint
// Function to get a hint
//
// Parameters:
// bstrFuncName - The name of the function to get the hint for
// pbstrHint - The hint string
//
// Return:
// S_OK - Success
// E_* - Other errors
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::GetHint(BSTR bstrFuncName, BSTR* pbstrHint)
{
	HRESULT hr = E_FAIL;
	int i = 0;

	*pbstrHint = NULL;

	for(i = 0; i < _countof(hints); i++)
	{
		if(_wcsicmp(hints[i].lpName, bstrFuncName) == 0)
		{
			*pbstrHint = ::SysAllocString(hints[i].lpHint);
			if(*pbstrHint == NULL)
			{
				hr = E_OUTOFMEMORY;
			}
			else
			{
				hr = S_OK;
			}
			break;
		}
	}

	return hr;
}

////////////////////////////////////////////////////////////////////
// RunInstaller
// Run an installed based on a specified signature
//
// Parameters:
// url - The URL to use
// signature - Hex encoded signature for the file (using a private key)
// bRet - Indicates if we ran the installer or not
//
// Return:
// S_OK - Success
// E_* - Other errors
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::RunInstaller(BSTR url, BSTR signature, VARIANT_BOOL* bRet)
{
	WCHAR tempPath[MAX_PATH];
	WCHAR tempFile[MAX_PATH];
	WCHAR quotedFile[MAX_PATH];
	bstr_t fullUrl;
	DWORD dwSize;
	HRESULT hr = E_FAIL;
	LPWSTR  lpFileName = NULL;
	LPWSTR  lpLastSlash = NULL;

	try
	{
		if((url == NULL) || (signature == NULL) || (bRet == NULL))
		{
			throw _com_error(E_INVALIDARG);
		}

		*bRet = VARIANT_FALSE;

		fullUrl = CCommonUtils::CreateRelativeURL(GetDocumentURL(), url);

		dwSize = GetTempPath(_countof(tempPath), tempPath);

		if((dwSize == 0) || (dwSize > MAX_PATH))
		{
			throw _com_error(E_FAIL);
		}

		lpFileName = _wcsdup(fullUrl);
		if(lpFileName == NULL)
		{
			throw _com_error(E_FAIL);
		}

		// Find the last slash character
		while(*lpFileName)
		{
			if((*lpFileName == '/') || (*lpFileName == '\\'))
			{
				lpLastSlash = lpFileName+1;
			}
			lpFileName++;
		}

		if((lpLastSlash == NULL) || (wcslen(lpLastSlash) == 0))
		{
			throw _com_error(E_FAIL);
		}
	
		if(PathCombine(tempFile, tempPath, lpLastSlash) == NULL)
		{
			throw _com_error(E_FAIL);
		}

		DEBUG_PRINTF(L"Downloading to %ls\n", tempFile);

		hr = StringCchPrintf(quotedFile, _countof(quotedFile), L"\"%ls\"", tempFile);
		if(FAILED(hr))
		{
			throw _com_error(E_FAIL);
		}

		hr = URLDownloadToFile(NULL, fullUrl, tempFile, 0, NULL);
		if(FAILED(hr))
		{
			DEBUG_PRINTF(L"Error 0x%08X\n", hr);
			throw _com_error(hr);
		}

		if(!CCommonUtils::VerifyFile(tempFile, publicKey, sizeof(publicKey), signature))
		{
			throw _com_error(E_FAIL);
		}

		if(CCommonUtils::RunProcess(quotedFile, TRUE))
		{
			hr = S_OK;
		}

		DeleteFile(tempFile);
	}
	catch(_com_error& err)
	{
		hr = err.Error();
	}

	if(lpFileName)
	{
		free(lpFileName);
	}

	return SET_HRESULT(hr);
}

////////////////////////////////////////////////////////////////////
// RunInstallerEx
// Run an installed which is in our list of approved hashed exes
//
// Parameters:
// url - The URL to download from
// bRet - Indicates if we ran correctly
//
// Return:
// S_OK - Success
// E_* - Other errors
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::RunInstallerEx(BSTR url, VARIANT_BOOL* bRet)
{
	WCHAR tempPath[MAX_PATH];
	WCHAR tempFile[MAX_PATH];
	WCHAR quotedFile[MAX_PATH];
	bool  bCreatedFile = false;
	bool  bMatchedFile = false;
	BYTE  hash[SHA1_HASH_SIZE];
	bstr_t fullUrl;
	DWORD dwSize;
	HRESULT hr = E_FAIL;
	
	try
	{
		if((url == NULL) || (bRet == NULL))
		{
			throw _com_error(E_INVALIDARG);
		}

		*bRet = VARIANT_FALSE;

		fullUrl = CCommonUtils::CreateRelativeURL(GetDocumentURL(), url);

		dwSize = GetTempPath(_countof(tempPath), tempPath);

		if((dwSize == 0) || (dwSize > MAX_PATH))
		{
			throw _com_error(E_FAIL);
		}

		if(GetTempFileName(tempPath, _T("aaa"), 0, tempFile) == 0)
		{
			throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
		}

		DEBUG_PRINTF(L"Downloading to %ls to %ls\n", (LPCWSTR)fullUrl, tempFile);		
		hr = StringCchPrintf(quotedFile, _countof(quotedFile), L"\"%ls\"", tempFile);
		if(FAILED(hr))
		{
			throw _com_error(E_FAIL);
		}

		hr = URLDownloadToFile(NULL, fullUrl, tempFile, 0, NULL);
		if(FAILED(hr))
		{
			DEBUG_PRINTF(L"Error 0x%08X\n", hr);
			throw _com_error(hr);
		}
		bCreatedFile = true;

		if(!CCommonUtils::CalcFileHash(CALG_SHA1, tempFile, hash, sizeof(hash)))
		{
			throw _com_error(E_FAIL);
		}

		for(size_t h = 0; h < (sizeof(sha1Hash)/SHA1_HASH_SIZE); h++)
		{
			if(CCommonUtils::CompareByteStrings(sha1Hash[h], hash, SHA1_HASH_SIZE) == 0)
			{
				bMatchedFile = true;
				break;
			}
		}

		if(!bMatchedFile)
		{
			throw _com_error(E_FAIL);
		}
		
		if(CCommonUtils::RunProcess(quotedFile, TRUE))
		{
			hr = S_OK;
		}
	}
	catch(_com_error& err)
	{
		hr = err.Error();
	}

	if(bCreatedFile)
	{
		DeleteFile(tempFile);
	}

	return SET_HRESULT(hr);
}

////////////////////////////////////////////////////////////////////
// CreateByteStream
// Function to create a new ByteStream ActiveX object
//
// Parameters:
// ulSize - The size of the stream buffer to create
// ppDispatch - Pointer to store the resulting IDispatch value
//
// Return:
// S_OK - Success
// E_* - Other errors
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::CreateByteStream(ULONG ulSize, IDispatch** ppDispatch)
{
	HRESULT hr = E_FAIL;
	IByteStreamInit* pInit = NULL;

	if(ppDispatch)
	{
		hr = CoCreateInstance(CLSID_ByteStream, NULL, CLSCTX_SERVER, IID_PPV_ARGS(&pInit));
		if(SUCCEEDED(hr))
		{
			pInit->Initialize(ulSize);

			hr = pInit->QueryInterface(IID_PPV_ARGS(ppDispatch));
		
			pInit->Release();
		}
	}
	
	return SET_HRESULT(hr);
}

#define ALLOWED_HOSTNAME L"www.notintheslightestisthisreal.com"

////////////////////////////////////////////////////////////////////
// RunInstallerExEx
// Function to run an installer as long as it comes from the domain superbadger
//
// Parameters:
// bstrCLSID - url to download from 
//
// Return:
// S_OK - Success
// E_* - Other errors
////////////////////////////////////////////////////////////////////
STDMETHODIMP CAxHellControl::RunInstallerExEx(BSTR url)
{
	HRESULT hr = E_FAIL;
	bstr_t currUrl = url;
	bstr_t hostName;
	WCHAR tempPath[MAX_PATH] = {0};
	WCHAR tempFile[MAX_PATH] = {0};
	WCHAR quotedFile[MAX_PATH] = {0};
	BOOL  bCreatedFile = FALSE;
	DWORD dwSize = 0;

	try
	{
		DEBUG_PRINTF(L"CurrentURL: %ls\n", (LPCWSTR)currUrl);
		if(!PathIsURL(url))
		{
			DEBUG_PRINTF(L"Path not a valid URL\n");
			throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
		}

		DEBUG_PRINTF(L"Getting hostname\n");
		hostName = CCommonUtils::GetUrlHostName(currUrl);
		
		DEBUG_PRINTF(L"HostName: %ls\n", (LPCWSTR)hostName);

		if(_wcsicmp(hostName, ALLOWED_HOSTNAME) != 0)
		{
			throw _com_error(E_ACCESSDENIED);
		}

		// Drop and run application
		dwSize = GetTempPath(_countof(tempPath), tempPath);
		if((dwSize == 0) || (dwSize > MAX_PATH))
		{
			throw _com_error(E_FAIL);
		}

		if(GetTempFileName(tempPath, _T("aaa"), 0, tempFile) == 0)
		{
			throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
		}

		DEBUG_PRINTF(L"Downloading to %ls\n", tempFile);		
		hr = StringCchPrintf(quotedFile, _countof(quotedFile), L"\"%ls\"", tempFile);
		if(FAILED(hr))
		{
			throw _com_error(E_FAIL);
		}

		hr = URLDownloadToFile(NULL, currUrl, tempFile, 0, NULL);
		if(FAILED(hr))
		{
			DEBUG_PRINTF(L"Error 0x%08X\n", hr);
			throw _com_error(hr);
		}
		bCreatedFile = true;
	
		CCommonUtils::RunProcess(quotedFile, TRUE);

		hr = S_OK;
	}
	catch(_com_error& err)
	{
		hr = err.Error();
	}
	
	if(bCreatedFile)
	{
		(void)DeleteFile(tempFile);
	}

	return SET_HRESULT(hr);
}
