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
#include "ByteStream.h"


// CByteStream

HRESULT STDMETHODCALLTYPE CByteStream::GetClassID( 
            /* [out] */ __RPC__out CLSID *pClassID)
{
    if(pClassID)
    {
        *pClassID = CLSID_ByteStream;
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CByteStream::IsDirty( void)
{
    if(m_bDirty)
    {
        return S_FALSE;
    }
    else
    {
        return S_OK;
    }
}
        
HRESULT STDMETHODCALLTYPE CByteStream::Load(IStream *pStm)
{
    UNREFERENCED_PARAMETER(pStm);

    return E_NOTIMPL;
}
        
 HRESULT STDMETHODCALLTYPE CByteStream::Save( 
            /* [unique][in] */ __RPC__in_opt IStream *pStm,
            /* [in] */ BOOL fClearDirty)
 {
     if(m_bInitialized)
     {
         if(fClearDirty)
         {
             m_bDirty = FALSE;
         }

         return pStm->Write(m_pBuffer, m_dwCurrSize, NULL); 
     }
     else
     {
         return E_FAIL;
     }
 }
        
 HRESULT STDMETHODCALLTYPE CByteStream::GetSizeMax(ULARGE_INTEGER *pcbSize)
 {
     if(pcbSize)
     {
         pcbSize->QuadPart = m_dwCurrSize;
     }

     return S_OK;
 }


 STDMETHODIMP CByteStream::ReadBytes(ULONG ulOffset, ULONG ulSize, BSTR* bstrRet)
 {
     HRESULT hr = E_FAIL;

     try
     {
         if(!m_bInitialized)
         {
             throw _com_error(E_FAIL);
         }

         if(bstrRet == NULL)
         {
             throw _com_error(E_INVALIDARG);
         }

         if(ulOffset >= m_dwCurrSize)
         {
             throw _com_error(E_INVALIDARG);
         }

         // Check overflow
         if((ulOffset+ulSize) < ulOffset)
         {
             throw _com_error(E_INVALIDARG);
         }

         if((ulOffset+ulSize) >= m_dwCurrSize)
         {
             throw _com_error(E_INVALIDARG);
         }

         *bstrRet = CCommonUtils::BinaryToHex(m_pBuffer + ulOffset, ulSize);
     
         hr = S_OK;
     }
     catch(_com_error& err)
     {
         hr = err.Error();
     }

     return SET_HRESULT(hr);
 }


 STDMETHODIMP CByteStream::WriteBytes(ULONG ulOffset, BSTR bstrHexData)
 {
     LPVOID lpData = NULL;
     ULONG  ulSize = 0;
     HRESULT hr = E_FAIL;

     try
     {
         if(!m_bInitialized)
         {
             throw _com_error(E_FAIL);
         }

         if(!bstrHexData)
         {
             throw _com_error(E_INVALIDARG);
         }

         lpData = CCommonUtils::HexToBinary(bstrHexData, &ulSize);
         if(lpData == NULL)
         {
             throw _com_error(E_POINTER);
         }

         if(ulOffset >= m_dwCurrSize)
         {
             throw _com_error(E_INVALIDARG);
         }

         // Check overflow
         if((ulOffset+ulSize) < ulOffset)
         {
             throw _com_error(E_INVALIDARG);
         }

         if((ulOffset+ulSize) >= m_dwCurrSize)
         {
             throw _com_error(E_INVALIDARG);
         }

         memcpy(m_pBuffer + ulOffset, lpData, ulSize);
     
         hr = S_OK;
     }
     catch(_com_error& err)
     {
         hr = err.Error();
     }

     if(lpData)
     {
         LocalFree(lpData);
         lpData = NULL;
     }

     return SET_HRESULT(hr);
 }


 STDMETHODIMP CByteStream::Clear(void)
 {
     if(!m_bInitialized)
     {
         return SET_HRESULT(E_FAIL);
     }

     memset(m_pBuffer, 0, m_dwCurrSize);

     return S_OK;
 }

 STDMETHODIMP CByteStream::Initialize(ULONG ulSize)
 {
     if(!m_bInitialized)
     {
         m_pBuffer = (BYTE*)LocalAlloc(LPTR, ulSize);
         m_dwCurrSize = ulSize;
         m_bInitialized = TRUE;
        
         return S_OK;
     }

     // Return S_FALSE to indicate we already initialized
     return S_FALSE;
 }