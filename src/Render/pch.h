#ifndef RENDER_PCH_H_INCLUDED
#define RENDER_PCH_H_INCLUDED
// == Win Includes ==
#include <Windows.h>
#include <wrl.h>
#include <commctrl.h>
#include <comdef.h>

// == D3D12 Includes ==
#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <dxgi1_4.h>
#include <dxgidebug.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#include <DirectXColors.h>

#include "d3dx12.h"

// == D3D11 / D2D Includes ==
#include <d3d11on12.h>
#include <d2d1_1.h>
#include <d2d1.h>
#include <dwrite.h>

// == STD Includes ==
#include <iostream>
#include <string>
#include <sstream>

// == Comments ==
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Comctl32.lib")

#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "D2d1.lib")

// == Typedefs ==
#undef byte

// == Macros ==
#define NOMINMAX

#define PRINT_DEBUG(DebugMsg) { WOStringStream os_; os_ << (DebugMsg) << "\n"; OutputDebugStringW(os_.str().c_str());}
#define PRINT_COM_ERROR(msg, result) { _com_error err(result); LPCTSTR errMsg = err.ErrorMessage(); PRINT_DEBUG(msg); PRINT_DEBUG(errMsg);}
#define ThrowIfFailed(hresult, msg) { if (FAILED(hresult)) { PRINT_COM_ERROR(msg, hresult) throw; }}

#define RESOURCE_STATE_COPY_DEST RenderContext::GetMSAACount() & NONE ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_RESOLVE_DEST
#define RESOURCE_STATE_COPY_SOURCE RenderContext::GetMSAACount() & NONE ? D3D12_RESOURCE_STATE_COPY_SOURCE : D3D12_RESOURCE_STATE_RESOLVE_SOURCE

#endif