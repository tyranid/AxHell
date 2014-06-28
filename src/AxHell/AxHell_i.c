

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IAxHellControl,0x088D2CF7,0xD853,0x4893,0x8B,0x8C,0x79,0x41,0xBD,0x8F,0x77,0x81);


MIDL_DEFINE_GUID(IID, IID_IByteStream,0x33D7950F,0xD773,0x4F6D,0x98,0x3B,0x85,0x3E,0xAB,0x32,0x09,0x86);


MIDL_DEFINE_GUID(IID, IID_IByteStreamInit,0x8F9B5604,0xD0E4,0x4B2A,0xBB,0x71,0x1D,0x69,0x58,0xBB,0x9F,0x41);


MIDL_DEFINE_GUID(IID, LIBID_AxHellLib,0x2D452F16,0xC5E5,0x45DA,0xAC,0x5C,0x7E,0x98,0x2F,0xB9,0x80,0xEC);


MIDL_DEFINE_GUID(IID, DIID__IAxHellControlEvents,0x1D82C6A4,0x031A,0x427C,0xB6,0x20,0x04,0xAA,0x08,0xAF,0xA6,0x24);


MIDL_DEFINE_GUID(CLSID, CLSID_AxHellControl,0x3AE51322,0x4D0E,0x4AAD,0x9F,0x67,0xFD,0x39,0x25,0xEE,0x5F,0xE1);


MIDL_DEFINE_GUID(CLSID, CLSID_ByteStream,0x35ECD2DB,0x3A98,0x4E79,0x96,0x9E,0x0F,0xE2,0xC2,0x8C,0xC6,0x23);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



