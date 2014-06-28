

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Oct 04 16:09:32 2010
 */
/* Compiler settings for AxHell.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __AxHell_i_h__
#define __AxHell_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAxHellControl_FWD_DEFINED__
#define __IAxHellControl_FWD_DEFINED__
typedef interface IAxHellControl IAxHellControl;
#endif 	/* __IAxHellControl_FWD_DEFINED__ */


#ifndef __IByteStream_FWD_DEFINED__
#define __IByteStream_FWD_DEFINED__
typedef interface IByteStream IByteStream;
#endif 	/* __IByteStream_FWD_DEFINED__ */


#ifndef __IByteStreamInit_FWD_DEFINED__
#define __IByteStreamInit_FWD_DEFINED__
typedef interface IByteStreamInit IByteStreamInit;
#endif 	/* __IByteStreamInit_FWD_DEFINED__ */


#ifndef ___IAxHellControlEvents_FWD_DEFINED__
#define ___IAxHellControlEvents_FWD_DEFINED__
typedef interface _IAxHellControlEvents _IAxHellControlEvents;
#endif 	/* ___IAxHellControlEvents_FWD_DEFINED__ */


#ifndef __AxHellControl_FWD_DEFINED__
#define __AxHellControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class AxHellControl AxHellControl;
#else
typedef struct AxHellControl AxHellControl;
#endif /* __cplusplus */

#endif 	/* __AxHellControl_FWD_DEFINED__ */


#ifndef __ByteStream_FWD_DEFINED__
#define __ByteStream_FWD_DEFINED__

#ifdef __cplusplus
typedef class ByteStream ByteStream;
#else
typedef struct ByteStream ByteStream;
#endif /* __cplusplus */

#endif 	/* __ByteStream_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAxHellControl_INTERFACE_DEFINED__
#define __IAxHellControl_INTERFACE_DEFINED__

/* interface IAxHellControl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IAxHellControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("088D2CF7-D853-4893-8B8C-7941BD8F7781")
    IAxHellControl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetHint( 
            /* [in] */ BSTR bstrFuncName,
            /* [retval][out] */ BSTR *pbstrHint) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RunUtility( 
            /* [in] */ VARIANT varName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReverseString( 
            /* [in] */ BSTR bstrString,
            /* [retval][out] */ BSTR *bstrRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateRandomString( 
            /* [in] */ ULONG ulSeed,
            /* [retval][out] */ BSTR *bstrRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TransformXSLT( 
            /* [in] */ BSTR bstrXML,
            /* [in] */ BSTR bstrXSLT,
            /* [retval][out] */ BSTR *bstrResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateObject( 
            /* [in] */ BSTR bstrCLSID,
            /* [retval][out] */ VARIANT *ppObject) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RunInstaller( 
            /* [in] */ BSTR url,
            /* [in] */ BSTR signature,
            /* [retval][out] */ VARIANT_BOOL *bRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RunInstallerEx( 
            /* [in] */ BSTR url,
            /* [retval][out] */ VARIANT_BOOL *bRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateByteStream( 
            /* [in] */ ULONG ulSize,
            /* [retval][out] */ IDispatch **ppDispatch) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RunInstallerExEx( 
            /* [in] */ BSTR url) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAxHellControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAxHellControl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAxHellControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAxHellControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAxHellControl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAxHellControl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAxHellControl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAxHellControl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetHint )( 
            IAxHellControl * This,
            /* [in] */ BSTR bstrFuncName,
            /* [retval][out] */ BSTR *pbstrHint);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RunUtility )( 
            IAxHellControl * This,
            /* [in] */ VARIANT varName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReverseString )( 
            IAxHellControl * This,
            /* [in] */ BSTR bstrString,
            /* [retval][out] */ BSTR *bstrRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateRandomString )( 
            IAxHellControl * This,
            /* [in] */ ULONG ulSeed,
            /* [retval][out] */ BSTR *bstrRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TransformXSLT )( 
            IAxHellControl * This,
            /* [in] */ BSTR bstrXML,
            /* [in] */ BSTR bstrXSLT,
            /* [retval][out] */ BSTR *bstrResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateObject )( 
            IAxHellControl * This,
            /* [in] */ BSTR bstrCLSID,
            /* [retval][out] */ VARIANT *ppObject);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RunInstaller )( 
            IAxHellControl * This,
            /* [in] */ BSTR url,
            /* [in] */ BSTR signature,
            /* [retval][out] */ VARIANT_BOOL *bRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RunInstallerEx )( 
            IAxHellControl * This,
            /* [in] */ BSTR url,
            /* [retval][out] */ VARIANT_BOOL *bRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateByteStream )( 
            IAxHellControl * This,
            /* [in] */ ULONG ulSize,
            /* [retval][out] */ IDispatch **ppDispatch);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RunInstallerExEx )( 
            IAxHellControl * This,
            /* [in] */ BSTR url);
        
        END_INTERFACE
    } IAxHellControlVtbl;

    interface IAxHellControl
    {
        CONST_VTBL struct IAxHellControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAxHellControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAxHellControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAxHellControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAxHellControl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAxHellControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAxHellControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAxHellControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAxHellControl_GetHint(This,bstrFuncName,pbstrHint)	\
    ( (This)->lpVtbl -> GetHint(This,bstrFuncName,pbstrHint) ) 

#define IAxHellControl_RunUtility(This,varName)	\
    ( (This)->lpVtbl -> RunUtility(This,varName) ) 

#define IAxHellControl_ReverseString(This,bstrString,bstrRet)	\
    ( (This)->lpVtbl -> ReverseString(This,bstrString,bstrRet) ) 

#define IAxHellControl_CreateRandomString(This,ulSeed,bstrRet)	\
    ( (This)->lpVtbl -> CreateRandomString(This,ulSeed,bstrRet) ) 

#define IAxHellControl_TransformXSLT(This,bstrXML,bstrXSLT,bstrResult)	\
    ( (This)->lpVtbl -> TransformXSLT(This,bstrXML,bstrXSLT,bstrResult) ) 

#define IAxHellControl_CreateObject(This,bstrCLSID,ppObject)	\
    ( (This)->lpVtbl -> CreateObject(This,bstrCLSID,ppObject) ) 

#define IAxHellControl_RunInstaller(This,url,signature,bRet)	\
    ( (This)->lpVtbl -> RunInstaller(This,url,signature,bRet) ) 

#define IAxHellControl_RunInstallerEx(This,url,bRet)	\
    ( (This)->lpVtbl -> RunInstallerEx(This,url,bRet) ) 

#define IAxHellControl_CreateByteStream(This,ulSize,ppDispatch)	\
    ( (This)->lpVtbl -> CreateByteStream(This,ulSize,ppDispatch) ) 

#define IAxHellControl_RunInstallerExEx(This,url)	\
    ( (This)->lpVtbl -> RunInstallerExEx(This,url) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAxHellControl_INTERFACE_DEFINED__ */


#ifndef __IByteStream_INTERFACE_DEFINED__
#define __IByteStream_INTERFACE_DEFINED__

/* interface IByteStream */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IByteStream;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("33D7950F-D773-4F6D-983B-853EAB320986")
    IByteStream : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadBytes( 
            /* [in] */ ULONG ulOffset,
            /* [in] */ ULONG ulSize,
            /* [retval][out] */ BSTR *bstrRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteBytes( 
            /* [in] */ ULONG ulOffset,
            /* [in] */ BSTR bstrHexData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IByteStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IByteStream * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IByteStream * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IByteStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IByteStream * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IByteStream * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IByteStream * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IByteStream * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadBytes )( 
            IByteStream * This,
            /* [in] */ ULONG ulOffset,
            /* [in] */ ULONG ulSize,
            /* [retval][out] */ BSTR *bstrRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteBytes )( 
            IByteStream * This,
            /* [in] */ ULONG ulOffset,
            /* [in] */ BSTR bstrHexData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IByteStream * This);
        
        END_INTERFACE
    } IByteStreamVtbl;

    interface IByteStream
    {
        CONST_VTBL struct IByteStreamVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IByteStream_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IByteStream_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IByteStream_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IByteStream_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IByteStream_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IByteStream_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IByteStream_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IByteStream_ReadBytes(This,ulOffset,ulSize,bstrRet)	\
    ( (This)->lpVtbl -> ReadBytes(This,ulOffset,ulSize,bstrRet) ) 

#define IByteStream_WriteBytes(This,ulOffset,bstrHexData)	\
    ( (This)->lpVtbl -> WriteBytes(This,ulOffset,bstrHexData) ) 

#define IByteStream_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IByteStream_INTERFACE_DEFINED__ */


#ifndef __IByteStreamInit_INTERFACE_DEFINED__
#define __IByteStreamInit_INTERFACE_DEFINED__

/* interface IByteStreamInit */
/* [unique][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_IByteStreamInit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8F9B5604-D0E4-4B2A-BB71-1D6958BB9F41")
    IByteStreamInit : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( 
            ULONG ulSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IByteStreamInitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IByteStreamInit * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IByteStreamInit * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IByteStreamInit * This);
        
        HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IByteStreamInit * This,
            ULONG ulSize);
        
        END_INTERFACE
    } IByteStreamInitVtbl;

    interface IByteStreamInit
    {
        CONST_VTBL struct IByteStreamInitVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IByteStreamInit_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IByteStreamInit_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IByteStreamInit_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IByteStreamInit_Initialize(This,ulSize)	\
    ( (This)->lpVtbl -> Initialize(This,ulSize) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IByteStreamInit_INTERFACE_DEFINED__ */



#ifndef __AxHellLib_LIBRARY_DEFINED__
#define __AxHellLib_LIBRARY_DEFINED__

/* library AxHellLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_AxHellLib;

#ifndef ___IAxHellControlEvents_DISPINTERFACE_DEFINED__
#define ___IAxHellControlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IAxHellControlEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IAxHellControlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1D82C6A4-031A-427C-B620-04AA08AFA624")
    _IAxHellControlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IAxHellControlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IAxHellControlEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IAxHellControlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IAxHellControlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IAxHellControlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IAxHellControlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IAxHellControlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IAxHellControlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IAxHellControlEventsVtbl;

    interface _IAxHellControlEvents
    {
        CONST_VTBL struct _IAxHellControlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IAxHellControlEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IAxHellControlEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IAxHellControlEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IAxHellControlEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IAxHellControlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IAxHellControlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IAxHellControlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IAxHellControlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_AxHellControl;

#ifdef __cplusplus

class DECLSPEC_UUID("3AE51322-4D0E-4AAD-9F67-FD3925EE5FE1")
AxHellControl;
#endif

EXTERN_C const CLSID CLSID_ByteStream;

#ifdef __cplusplus

class DECLSPEC_UUID("35ECD2DB-3A98-4E79-969E-0FE2C28CC623")
ByteStream;
#endif
#endif /* __AxHellLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


