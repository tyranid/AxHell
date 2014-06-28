// dllmain.h : Declaration of module class.

class CAxHellModule : public ATL::CAtlDllModuleT< CAxHellModule >
{
public :
	DECLARE_LIBID(LIBID_AxHellLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_AXHELL, "{E2731C4C-CEA8-4FBC-9691-F17B5780BA2C}")
};

extern class CAxHellModule _AtlModule;
