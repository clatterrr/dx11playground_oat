#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <wrl/client.h>
#include <string>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <directxcolors.h>
using namespace DirectX;
using namespace Microsoft::WRL;

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	void InitResource(ID3D11Device* t_device, ID3D11DeviceContext* t_context);
public:
	 ComPtr<ID3D11Texture2D> ShadowRTTex;
	 ComPtr<ID3D11RenderTargetView> ShadowDepthRTV;

	 ComPtr<ID3D11Texture2D> ShadowDSTex;
	 ComPtr<ID3D11DepthStencilView> ShadowDepthDSV;
	 ComPtr<ID3D11ShaderResourceView> ShadowDepthSRV;
	 D3D11_TEXTURE2D_DESC descDepth = {};
	 ComPtr<ID3D11Resource> ShadowResource;
	 ComPtr<ID3D11Texture2D> pTexSource;
	
};

#endif


