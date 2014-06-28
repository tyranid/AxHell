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

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Shlwapi.lib")

BOOL ExportKey(LPCTSTR lpFileName, HCRYPTKEY hKey, DWORD dwBlobType)
{
	BOOL bRet = FALSE;
	DWORD dwLength = 0;
	LPBYTE lpData = NULL;
	FILE* fp = NULL;

	if(!CryptExportKey(hKey, NULL, dwBlobType, 0, NULL, &dwLength))
	{
		goto endFunction;
	}

	lpData = (LPBYTE)malloc(dwLength);
	if(!CryptExportKey(hKey, NULL, dwBlobType, 0, lpData, &dwLength))
	{
		goto endFunction;
	}

	if(_tfopen_s(&fp, lpFileName, _T("wb")) != 0)
	{
		goto endFunction;
	}

	(void)fwrite(lpData, 1, dwLength, fp);

	bRet = TRUE;

endFunction:

	if(lpData)
	{
		free(lpData);
	}

	if(fp)
	{
		fclose(fp);
	}

	return bRet;
}

LPVOID ReadFile(LPCTSTR lpFileName, DWORD& dwLength)
{
	FILE* fp = NULL;
	LPVOID lpRet = NULL;

	if(_tfopen_s(&fp, lpFileName, _T("rb")) != 0)
	{
		goto endFunction;
	}

	fseek(fp, 0, SEEK_END);

	dwLength = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	
	lpRet = malloc(dwLength);

	fread(lpRet, 1, dwLength, fp);

endFunction:

	if(fp)
	{
		fclose(fp);
	}

	return lpRet;
}

BOOL SignFile(LPCTSTR lpFileName, LPCTSTR lpKey)
{
	LPVOID lpKeyData = NULL;
	DWORD  dwKeySize = 0;
	LPVOID lpFileData = NULL;
	DWORD  dwFileSize = 0;
	BOOL bRet = FALSE;
	HCRYPTPROV hProv = NULL;
	HCRYPTKEY  hKey = NULL;
	HCRYPTHASH hHash = NULL;
	LPVOID lpSignature = NULL;
	DWORD  dwSigSize = 0;
	LPTSTR lpBase64 = 0;
	DWORD  dwBase64Size = 0;

	lpKeyData = ReadFile(lpKey, dwKeySize);
	if(!lpKeyData)
	{
		goto endFunction;
	}

	lpFileData = ReadFile(lpFileName, dwFileSize);
	if(!lpFileData)
	{
		goto endFunction;
	}

	if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
	{
		goto endFunction;
	}

	if(!CryptImportKey(hProv, (LPBYTE)lpKeyData, dwKeySize, NULL, 0, &hKey))
	{
		goto endFunction;
	}

	if(!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) 
	{
		goto endFunction;
	}

	if(!CryptHashData(hHash, (LPBYTE)lpFileData, dwFileSize, 0)) 
	{
		goto endFunction;
	}

	if(!CryptSignHash(hHash, AT_SIGNATURE, NULL, 0, NULL, &dwSigSize))
	{
		goto endFunction;
	}

	lpSignature = malloc(dwSigSize);

	if(!CryptSignHash(hHash, AT_SIGNATURE, NULL, 0, (LPBYTE)lpSignature, &dwSigSize))
	{
		goto endFunction;
	}

	if(!CryptBinaryToString((LPBYTE)lpSignature, dwSigSize, CRYPT_STRING_BASE64, NULL, &dwBase64Size))
	{
		goto endFunction;
	}

	lpBase64 = (LPTSTR) malloc(dwBase64Size*sizeof(TCHAR));

	if(!CryptBinaryToString((LPBYTE)lpSignature, dwSigSize, CRYPT_STRING_BASE64, lpBase64, &dwBase64Size))
	{
		goto endFunction;
	}

	printf("%ls\n", lpBase64);

endFunction:

	if(lpBase64)
	{
		free(lpBase64);
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

BOOL VerifyFile(LPCTSTR lpFileName, LPCTSTR lpKey, LPCTSTR lpBase64)
{
	LPVOID lpKeyData = NULL;
	DWORD  dwKeySize = 0;
	LPVOID lpFileData = NULL;
	DWORD  dwFileSize = 0;
	BOOL bRet = FALSE;
	HCRYPTPROV hProv = NULL;
	HCRYPTKEY  hKey = NULL;
	HCRYPTHASH hHash = NULL;
	LPVOID lpSignature = NULL;
	DWORD  dwSigSize = 0;

	lpKeyData = ReadFile(lpKey, dwKeySize);
	if(!lpKeyData)
	{
		goto endFunction;
	}

	lpFileData = ReadFile(lpFileName, dwFileSize);
	if(!lpFileData)
	{
		goto endFunction;
	}

	if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
	{
		goto endFunction;
	}

	if(!CryptImportKey(hProv, (LPBYTE)lpKeyData, dwKeySize, NULL, 0, &hKey))
	{
		goto endFunction;
	}

	if(!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) 
	{
		goto endFunction;
	}

	if(!CryptHashData(hHash, (LPBYTE)lpFileData, dwFileSize, 0)) 
	{
		goto endFunction;
	}

	if(!CryptStringToBinary(lpBase64, 0, CRYPT_STRING_BASE64, NULL, &dwSigSize, NULL, NULL))
	{
		goto endFunction;
	}

	lpSignature = malloc(dwSigSize);
	if(!CryptStringToBinary(lpBase64, 0, CRYPT_STRING_BASE64, (LPBYTE)lpSignature, &dwSigSize, NULL, NULL))
	{
		goto endFunction;
	}

	if(!CryptVerifySignature(hHash, (LPBYTE)lpSignature, dwSigSize, hKey, NULL, 0))
	{
		goto endFunction;
	}

	printf("Signature Matched\n");

	bRet = TRUE;

endFunction:

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


BOOL GenerateNewKey(LPCTSTR lpFileName)
{
	HCRYPTPROV hProv = NULL;
	HCRYPTKEY  hKey = NULL;
	BOOL bRet = FALSE;
	TCHAR fileName[MAX_PATH];

	if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
	{
		goto endFunction;
	}

	if(!CryptGenKey(hProv, AT_SIGNATURE, CRYPT_EXPORTABLE, &hKey))
	{
		goto endFunction;
	}

	if(PathCombine(fileName, NULL, lpFileName) == NULL)
	{
		goto endFunction;
	}

	if(!PathRenameExtension(fileName, _T(".key")))
	{
		goto endFunction;
	}

	if(!ExportKey(fileName, hKey, PRIVATEKEYBLOB))
	{
		goto endFunction;
	}

	if(!PathRenameExtension(fileName, _T(".pub")))
	{
		goto endFunction;
	}

	if(!ExportKey(fileName, hKey, PUBLICKEYBLOB))
	{
		goto endFunction;
	}

	bRet = TRUE;
	
endFunction:

	if(hKey)
	{
		CryptDestroyKey(hKey);
	}

	if(hProv)
	{
		CryptReleaseContext(hProv, 0);
	}

	return bRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2)
	{
		printf("Usage: FileSigner command [options]\n");
		return 1;
	}

	if(_tcscmp(argv[1], _T("gen")) == 0)
	{
		// Generate public/private key, output to blobs
		if(argc > 2)
		{
			GenerateNewKey(argv[2]);
		}
	}
	else if(_tcscmp(argv[1], _T("sign")) == 0)
	{
		// Sign a file using a specified private key and output the hash
		if(argc > 3)
		{
			SignFile(argv[2], argv[3]);
		}
	}
	else if(_tcscmp(argv[1], _T("verify")) == 0)
	{
		if(argc > 4)
		{
			VerifyFile(argv[2], argv[3], argv[4]);
		}
	}

	return 0;
}

