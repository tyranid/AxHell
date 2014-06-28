// ByteStream.h : Declaration of the CByteStream

#pragma once
#include "resource.h"       // main symbols



#include "AxHell_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CByteStream

class ATL_NO_VTABLE CByteStream :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CByteStream, &CLSID_ByteStream>,
	public IDispatchImpl<IByteStream, &IID_IByteStream, &LIBID_AxHellLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStream,
	public IByteStreamInit
{
public:
	CByteStream() : m_pBuffer(NULL), m_bInitialized(FALSE), m_dwCurrSize(0), m_bDirty(FALSE)
	{
	}

	~CByteStream()
	{
		if(m_bInitialized)
		{
			LocalFree(m_pBuffer);
			m_pBuffer = NULL;
			m_bInitialized = FALSE;
		}
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BYTESTREAM)


BEGIN_COM_MAP(CByteStream)
	COM_INTERFACE_ENTRY(IByteStream)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IByteStreamInit)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	BYTE* m_pBuffer;
	BOOL  m_bInitialized;
	DWORD m_dwCurrSize;
	BOOL  m_bDirty;

public:

	virtual HRESULT STDMETHODCALLTYPE GetClassID( 
            /* [out] */ __RPC__out CLSID *pClassID);

	virtual HRESULT STDMETHODCALLTYPE IsDirty( void);
        
    virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [unique][in] */ __RPC__in_opt IStream *pStm);
        
    virtual HRESULT STDMETHODCALLTYPE Save( 
            /* [unique][in] */ __RPC__in_opt IStream *pStm,
            /* [in] */ BOOL fClearDirty);
        
    virtual HRESULT STDMETHODCALLTYPE GetSizeMax( 
            /* [out] */ __RPC__out ULARGE_INTEGER *pcbSize);
        

	STDMETHOD(ReadBytes)(ULONG ulOffset, ULONG ulSize, BSTR* bstrRet);
	STDMETHOD(WriteBytes)(ULONG ulOffset, BSTR bstrHexData);
	STDMETHOD(Clear)(void);

	STDMETHOD(Initialize)(ULONG ulSize);
};

OBJECT_ENTRY_AUTO(__uuidof(ByteStream), CByteStream)
