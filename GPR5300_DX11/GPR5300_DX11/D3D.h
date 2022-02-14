#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <xnamath.h>

#include "Definitions.h"
#include "Window.h"

class D3D
{
public:
	bool Initialize(HINSTANCE hInstance, Window& window);

	void Finalize();

	void BeginRender() 
	{ 
		mD3DevCon->ClearRenderTargetView(mD3RenderTargetView, D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));
		mD3DevCon->ClearDepthStencilView(mD3DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f, 0);
		mD3DevCon->OMSetDepthStencilState(mD3DepthStencilState, 1);
	}

	void EndRender() 
	{ 
		mDxSwapChain->Present(0, 0);
	}

	ID3D11Device* GetDevice() { return mD3Device; }
	
	ID3D11DeviceContext* GetDevCon() { return mD3DevCon; }

	static bool CreateTextureAndSampler(LPCWSTR path, ID3D11Device* device,
		ID3D11ShaderResourceView** outTexture, ID3D11SamplerState** outSampler);

	template<typename T>
	static bool CreateConstantBuffer(ID3D11Device* d3Device, ID3D11Buffer** constantBuffer) 
	{
		HRESULT hr;
		bool success = false;

		D3D11_BUFFER_DESC constBufDesc = {};
		constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufDesc.ByteWidth = sizeof(T);
		constBufDesc.CPUAccessFlags = 0;
		constBufDesc.MiscFlags = 0;
		constBufDesc.Usage = D3D11_USAGE_DEFAULT;

		hr = d3Device->CreateBuffer(&constBufDesc, nullptr, constantBuffer);
		success = CheckDxError(hr, "Failed to create constant buffer!");

		return success;
	}

private:
	bool CreateRasterizerStates();

	IDXGISwapChain*		 mDxSwapChain			= nullptr;
	ID3D11Device*		 mD3Device				= nullptr;
	ID3D11DeviceContext* mD3DevCon				= nullptr;
	ID3D11RenderTargetView* mD3RenderTargetView = nullptr;

	ID3D11Texture2D*	 mD3DepthStencilTexture = nullptr;
	ID3D11DepthStencilView* mD3DepthStencilView = nullptr;
	ID3D11DepthStencilState* mD3DepthStencilState = nullptr;

	ID3D11RasterizerState* mD3SolidRasterizerState = nullptr;
	ID3D11RasterizerState* mD3WireframeRasterizerState = nullptr;
};

