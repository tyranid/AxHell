// AxHellControl.h : Declaration of the CAxHellControl
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "MyIObjectSafetyImpl.h"
#include "AxHell_i.h"
#include "_IAxHellControlEvents_CP.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;



// CAxHellControl
class ATL_NO_VTABLE CAxHellControl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IAxHellControl, &IID_IAxHellControl, &LIBID_AxHellLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CAxHellControl>,
	public IOleControlImpl<CAxHellControl>,
	public IOleObjectImpl<CAxHellControl>,
	public IOleInPlaceActiveObjectImpl<CAxHellControl>,
	public IViewObjectExImpl<CAxHellControl>,
	public IOleInPlaceObjectWindowlessImpl<CAxHellControl>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CAxHellControl>,
	public CProxy_IAxHellControlEvents<CAxHellControl>,
	public IObjectWithSiteImpl<CAxHellControl>,
	public IServiceProviderImpl<CAxHellControl>,
	public IPersistStorageImpl<CAxHellControl>,
	public ISpecifyPropertyPagesImpl<CAxHellControl>,
	public IPersistPropertyBagImpl<CAxHellControl>,
	public IQuickActivateImpl<CAxHellControl>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CAxHellControl>,
#endif
	public IProvideClassInfo2Impl<&CLSID_AxHellControl, &__uuidof(_IAxHellControlEvents), &LIBID_AxHellLib>,
	public IPropertyNotifySinkCP<CAxHellControl>,
	public CComCoClass<CAxHellControl, &CLSID_AxHellControl>,
	public CComControl<CAxHellControl>,
	public MyIObjectSafetyImpl<CAxHellControl, INTERFACE_USES_SECURITY_MANAGER | INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:


	CAxHellControl()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_AXHELLCONTROL)


DECLARE_NOT_AGGREGATABLE(CAxHellControl)

BEGIN_COM_MAP(CAxHellControl)
	COM_INTERFACE_ENTRY(IAxHellControl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IServiceProvider)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_PROP_MAP(CAxHellControl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CAxHellControl)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(__uuidof(_IAxHellControlEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CAxHellControl)
	CHAIN_MSG_MAP(CComControl<CAxHellControl>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

BEGIN_CATEGORY_MAP(CAxHellControl)
	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
END_CATEGORY_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* const arr[] =
		{
			&IID_IAxHellControl,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IAxHellControl
public:
	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		RECT& rc = *(RECT*)di.prcBounds;
		// Set Clip region to the rectangle specified by di.prcBounds
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("AxHellControl");
#ifndef _WIN32_WCE
		TextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			pszText,
			lstrlen(pszText));
#else
		ExtTextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			ETO_OPAQUE,
			NULL,
			pszText,
			ATL::lstrlen(pszText),
			NULL);
#endif

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		return S_OK;
	}

	STDMETHOD(_InternalQueryService)(REFGUID /*guidService*/, REFIID /*riid*/, void** /*ppvObject*/)
	{
		return E_NOTIMPL;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT QuerySite(REFIID riid, void** ppv);
	HRESULT QueryForSiteServiceProvider(IServiceProvider** pServiceProv);
	HRESULT QuerySiteForService(REFGUID serviceGuid, REFIID riid, void** ppv);

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		DEBUG_PRINTF(L"Deleting\n");
	}

	STDMETHOD(RunUtility)(VARIANT varName);
	STDMETHOD(ReverseString)(BSTR bstrString, BSTR* bstrRet);
	STDMETHOD(CreateRandomString)(ULONG ulSeed, BSTR* bstrRet);
	STDMETHOD(TransformXSLT)(BSTR bstrXML, BSTR bstrXSLT, BSTR* bstrResult);
	STDMETHOD(CreateObject)(BSTR bstrCLSID, VARIANT* ppObject);
	STDMETHOD(GetHint)(BSTR bstrFuncName, BSTR* pbstrHint);
	STDMETHOD(RunInstaller)(BSTR url, BSTR signature, VARIANT_BOOL* bRet);
	STDMETHOD(RunInstallerEx)(BSTR url, VARIANT_BOOL* bRet);
	STDMETHOD(CreateByteStream)(ULONG ulSize, IDispatch** ppDispatch);
	STDMETHOD(RunInstallerExEx)(BSTR url);
};

OBJECT_ENTRY_AUTO(__uuidof(AxHellControl), CAxHellControl)
