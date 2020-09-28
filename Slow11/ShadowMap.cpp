#include "ShadowMap.h"

ShadowMap::ShadowMap()
{

	
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::InitResource(ID3D11Device* t_device, ID3D11DeviceContext* t_context)
{
	descDepth.Width = 800;
	descDepth.Height = 800;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.CPUAccessFlags = 0;
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	t_device->CreateTexture2D(&descDepth, nullptr, ShadowRTTex.GetAddressOf());

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = descDepth.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	t_device->CreateRenderTargetView(ShadowRTTex.Get(), &rtvDesc, ShadowDepthRTV.GetAddressOf());

	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//	descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
//	descDepth.Format = DXGI_FORMAT_R32_FLOAT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL |  D3D11_BIND_SHADER_RESOURCE;
	t_device->CreateTexture2D(&descDepth, nullptr, &ShadowDSTex);

	D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
	descView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descView.Flags = 0;
	descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descView.Texture2D.MipSlice = 0;
	t_device->CreateDepthStencilView(ShadowDSTex.Get(), &descView, ShadowDepthDSV.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	t_device->CreateShaderResourceView(ShadowDSTex.Get(), &srvDesc, ShadowDepthSRV.GetAddressOf());

}
