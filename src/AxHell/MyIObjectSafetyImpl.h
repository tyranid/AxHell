#pragma once

#include <atlwin.h>
#include <objsafe.h>
#include <urlmon.h>

// Implement IObjectSafety which supports all the safety options, as well as implementing getting
// the current hosting document and URL if available
template <class T, DWORD dwSupportedSafety>
class ATL_NO_VTABLE MyIObjectSafetyImpl : 
	public IObjectSafety
{
protected:
	virtual BOOL FAccessAllowed()
	{
		return TRUE;
	}

	HRESULT GetHostedDocument(IHTMLDocument2** pDoc)
	{
		// Declarations
		HRESULT	                          hr = E_FAIL;
		CComPtr<IServiceProvider>         spSrvProv;
		CComPtr<IInternetSecurityManager> spInetSecMgr;
		CComPtr<IWebBrowser2>             spWebBrowser;
		CComPtr<IDispatch>				  spDocument;

		try
		{
			T* pT = (T*)this;

			if(pDoc == NULL)
			{
				throw _com_error(E_INVALIDARG);
			}

			*pDoc = NULL;

			hr = pT->QuerySiteForService(SID_SWebBrowserApp, IID_IWebBrowser2, (void **)&spWebBrowser);
			if(SUCCEEDED(hr))
			{
				// Query the web browser object for the activation URL
				hr = spWebBrowser->get_Document(&spDocument);
				if(FAILED(hr))
				{
					throw _com_error(hr);
				}

				hr = spDocument->QueryInterface(IID_PPV_ARGS(pDocument));
			}
			else
			{
				// Local declarations
				CComPtr<IHTMLDocument2>		spDoc;
				CComPtr<IOleContainer>		spContainer;
				CComPtr<IOleClientSite>		spClientSite;
		
				// Reinitialize
				spSrvProv = NULL;
				spClientSite = NULL;

				hr = pT->QuerySite(IID_IOleClientSite, (void**)&spClientSite);
				if(FAILED(hr))
				{
					throw _com_error(hr);
				}
					
				if(spClientSite == NULL)
				{
					throw _com_error(E_FAIL);
				}

				hr = spClientSite->GetContainer(&spContainer);
				if (FAILED(hr))
				{
					throw _com_error(hr);
				}

				hr = spContainer->QueryInterface(IID_PPV_ARGS(pDocument));
				if (FAILED(hr))
				{
					throw _com_error(hr);
				}
			}
		}
		catch(_com_error& err)
		{
			hr = err.Error();
		}

		return hr;
	}

	HRESULT GetDocumentURLFromSite()
	{
		// Declarations
		HRESULT	                          hr = E_FAIL;
		CComPtr<IServiceProvider>         spSrvProv;
		CComPtr<IInternetSecurityManager> spInetSecMgr;
		CComPtr<IWebBrowser2>             spWebBrowser;
		CComBSTR						  bstrURL;

		try
		{
			T* pT = (T*)this;

			hr = pT->QuerySiteForService(SID_SWebBrowserApp, IID_IWebBrowser2, (void **)&spWebBrowser);
			if(SUCCEEDED(hr))
			{
				// Query the web browser object for the activation URL
				hr = spWebBrowser->get_LocationURL(&bstrURL);
			}
			else
			{
				// Local declarations
				CComPtr<IHTMLDocument2>		spDoc;
				CComPtr<IOleContainer>		spContainer;
				CComPtr<IOleClientSite>		spClientSite;
		
				// Reinitialize
				spSrvProv = NULL;
				spClientSite = NULL;

				hr = pT->QuerySite(IID_IOleClientSite, (void**)&spClientSite);
				if(FAILED(hr))
				{
					throw _com_error(hr);
				}
					
				if(spClientSite == NULL)
				{
					throw _com_error(E_FAIL);
				}

				hr = spClientSite->GetContainer(&spContainer);
				if (FAILED(hr))
				{
					throw _com_error(hr);
				}

				hr = spContainer->QueryInterface(IID_IHTMLDocument2, (void **)&spDoc);
				if (FAILED(hr))
				{
					throw _com_error(hr);
				}

				hr = spDoc->get_URL(&bstrURL);
				if (FAILED(hr))
				{
					throw _com_error(hr);
				}
			}
			
			if(_countof(m_strDocumentURL) < wcslen(bstrURL))
			{
				throw _com_error(E_INVALIDARG);				
			}

			wcscpy(m_strDocumentURL, bstrURL);

			// Query the site for its associated security manager
			hr = pT->QuerySiteForService(SID_SInternetSecurityManager, IID_IInternetSecurityManager, (void **)&spInetSecMgr);
			if (FAILED(hr))
			{
				throw _com_error(hr);
			}

			// Query the security manager for the zone the activation URL belongs to
			hr = spInetSecMgr->MapUrlToZone(bstrURL, &m_dwZone, 0);
			if (FAILED(hr))
			{
				throw _com_error(hr);
			}
		}
		catch(_com_error& err)
		{
			hr = err.Error();
		}

		return hr;
	}


public:
	MyIObjectSafetyImpl() : m_dwCurrentSafety(0), m_dwZone(0)
	{
		memset(m_strDocumentURL, 0, sizeof(m_strDocumentURL));
	}

	STDMETHOD(GetInterfaceSafetyOptions)(
		_In_ REFIID riid, 
		_Out_ DWORD *pdwSupportedOptions, 
		_Out_ DWORD *pdwEnabledOptions)
	{
		ATLTRACE(atlTraceControls,2,_T("IObjectSafetyImpl::GetInterfaceSafetyOptions\n"));
		T* pT = static_cast<T*>(this);
		if (pdwSupportedOptions == NULL || pdwEnabledOptions == NULL)
			return E_POINTER;

		HRESULT hr;
		IUnknown* pUnk;
		// Check if we support this interface
		hr = pT->GetUnknown()->QueryInterface(riid, (void**)&pUnk);
		if (SUCCEEDED(hr))
		{
			// We support this interface so set the safety options accordingly
			pUnk->Release();	// Release the interface we just acquired
			*pdwSupportedOptions = dwSupportedSafety;
			
			if (FAccessAllowed())
			{
				*pdwEnabledOptions   = m_dwCurrentSafety;
			}
			else 
			{
				*pdwEnabledOptions   = 0;
			}
		}
		else
		{
			// We don't support this interface
			*pdwSupportedOptions = 0;
			*pdwEnabledOptions   = 0;
		}
		return hr;
	}
	STDMETHOD(SetInterfaceSafetyOptions)(
		_In_ REFIID riid, 
		_In_ DWORD dwOptionSetMask, 
		_In_ DWORD dwEnabledOptions)
	{
		ATLTRACE(atlTraceControls,2,_T("IObjectSafetyImpl::SetInterfaceSafetyOptions\n"));
		T* pT = static_cast<T*>(this);
		IUnknown* pUnk;

		// Check if we support the interface and return E_NOINTEFACE if we don't
		if (FAILED(pT->GetUnknown()->QueryInterface(riid, (void**)&pUnk)))
			return E_NOINTERFACE;
		pUnk->Release();	// Release the interface we just acquired

		// If we are asked to set options we don't support then fail
		if (dwOptionSetMask & ~dwSupportedSafety)
			return E_FAIL;

		DWORD dwNewSafety = (m_dwCurrentSafety & ~dwOptionSetMask) | (dwOptionSetMask & dwEnabledOptions);
			
		if (m_dwCurrentSafety == dwNewSafety)
		{
			return S_OK;
		}
		
		if (!FAccessAllowed())
		{
			return E_ACCESSDENIED;
		}
		// Set the safety options we have been asked to
		m_dwCurrentSafety = dwNewSafety;	
		
		return S_OK;
	}

	LPCWSTR GetDocumentURL()
	{
		if(m_strDocumentURL[0] == 0)
		{
			GetDocumentURLFromSite();
		}

		return m_strDocumentURL;
	}

	DWORD m_dwZone;
	WCHAR m_strDocumentURL[1024];
	DWORD m_dwCurrentSafety;
};
