#include <io.h>
#include "System/main.h"
#include "System/Application.h"
#include "System/renderer.h"
#include "imgui_impl_dx11.h"


using namespace DirectX::SimpleMath;

D3D_FEATURE_LEVEL       Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           Renderer::m_Device{};
ID3D11DeviceContext*    Renderer::m_DeviceContext{};
IDXGISwapChain*         Renderer::m_SwapChain{};
ID3D11RenderTargetView* Renderer::m_RenderTargetView{};
ID3D11DepthStencilView* Renderer::m_DepthStencilView{};

ID3D11Buffer*			Renderer::m_WorldBuffer{};
ID3D11Buffer*			Renderer::m_ViewBuffer{};
ID3D11Buffer*			Renderer::m_ProjectionBuffer{};
ID3D11Buffer*			Renderer::m_MaterialBuffer{};
ID3D11Buffer*			Renderer::m_LightBuffer{};


ID3D11DepthStencilState* Renderer::m_DepthStateEnable{};
ID3D11DepthStencilState* Renderer::m_DepthStateDisable{};


ID3D11BlendState*		Renderer::m_BlendState{};
ID3D11BlendState*		Renderer::m_BlendStateATC{};

Application*			Renderer::m_Application;


void Renderer::Init(Application* ap)
{
	HRESULT hr = S_OK;

	// 持ち主オブジェクトのアドレスをセット
	m_Application = ap;

	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_Application->GetWidth();
	swapChainDesc.BufferDesc.Height = m_Application->GetHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_Application->GetWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain( NULL,
										D3D_DRIVER_TYPE_HARDWARE,
										NULL,
										0,
										NULL,
										0,
										D3D11_SDK_VERSION,
										&swapChainDesc,
										&m_SwapChain,
										&m_Device,
										&m_FeatureLevel,
										&m_DeviceContext );






	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget{};
	m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&renderTarget );
	m_Device->CreateRenderTargetView( renderTarget, NULL, &m_RenderTargetView );
	renderTarget->Release();


	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)m_Application->GetWidth();
	viewport.Height = (FLOAT)m_Application->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports( 1, &viewport );



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; 
	rasterizerDesc.CullMode = D3D11_CULL_BACK; 
//	rasterizerDesc.CullMode = D3D11_CULL_NONE;
//	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE; 

	ID3D11RasterizerState *rs;
	m_Device->CreateRasterizerState( &rasterizerDesc, &rs );

	m_DeviceContext->RSSetState( rs );




	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Device->CreateBlendState( &blendDesc, &m_BlendState );

	blendDesc.AlphaToCoverageEnable = TRUE;
	m_Device->CreateBlendState( &blendDesc, &m_BlendStateATC );

	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff );





	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateEnable );//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateDisable );//深度無効ステート

	m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	m_Device->CreateSamplerState( &samplerDesc, &samplerState );

	m_DeviceContext->PSSetSamplers( 0, 1, &samplerState );



	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(Matrix);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_WorldBuffer );
	m_DeviceContext->VSSetConstantBuffers( 0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_ViewBuffer );
	m_DeviceContext->VSSetConstantBuffers( 1, 1, &m_ViewBuffer );

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_ProjectionBuffer );
	m_DeviceContext->VSSetConstantBuffers( 2, 1, &m_ProjectionBuffer );


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_MaterialBuffer );
	m_DeviceContext->VSSetConstantBuffers( 3, 1, &m_MaterialBuffer );
	m_DeviceContext->PSSetConstantBuffers( 3, 1, &m_MaterialBuffer );


	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_LightBuffer );
	m_DeviceContext->VSSetConstantBuffers( 4, 1, &m_LightBuffer );
	m_DeviceContext->PSSetConstantBuffers( 4, 1, &m_LightBuffer );





	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = Vector4(0.5f, -1.0f, 0.8f, 0.0f);
	light.Direction.Normalize();
	light.Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	light.Diffuse = Color(1.5f, 1.5f, 1.5f, 1.0f);
	SetLight(light);



	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// imgui初期化
	if (!ImGui_ImplDX11_Init(m_Device, m_DeviceContext))
	{
		assert(0 && "ImGui初期化失敗");
	}
}



void Renderer::Uninit()
{

	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();


	m_DeviceContext->ClearState();
	m_RenderTargetView->Release();
	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->Release();

	ImGui_ImplDX11_Shutdown();
}




void Renderer::Begin()
{
	float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, clearColor );
	m_DeviceContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void Renderer::End()
{
	ImGui::Render();
	m_SwapChain->Present( 1, 0 );
}




void Renderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );
	else
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateDisable, NULL );

}



void Renderer::SetATCEnable( bool Enable )
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetWorldViewProjection2D()
{
	Matrix world;
	world = Matrix::Identity;			// 単位行列にする
	world = world.Transpose();			// 転置
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

	Matrix view;
	view = Matrix::Identity;
	view = view.Transpose();
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

	Matrix projection;
/*
// update 右手系＝＝＝＞左手系  （DIRECTXTKのメソッドは右手系だった） 20230511 update by tomoki.suzuki　
	projection = projection.CreateOrthographicOffCenter(
		static_cast<float>(m_Application->GetWidth() * -0.5f),			// ビューボリュームの最小Ｘ
		static_cast<float>(m_Application->GetWidth() * 0.5f),			// ビューボリュームの最大Ｘ
		static_cast<float>(m_Application->GetHeight() * 0.5f),			// ビューボリュームの最小Ｙ
		static_cast<float>(m_Application->GetHeight() * -0.5f),			// ビューボリュームの最大Ｙ
		0.0f,
		1000.0f);
*/

/*
	// update 右手系＝＝＝＞左手系  （DIRECTXTKのメソッドは右手系だった） 20230511 update by tomoki.suzuki　
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		static_cast<float>(m_Application->GetWidth() * -0.5f),			// ビューボリュームの最小Ｘ
		static_cast<float>(m_Application->GetWidth() * 0.5f),			// ビューボリュームの最大Ｘ
		static_cast<float>(m_Application->GetHeight() * 0.5f),			// ビューボリュームの最小Ｙ
		static_cast<float>(m_Application->GetHeight() * -0.5f),			// ビューボリュームの最大Ｙ
		0.0f,
		1000.0f);
*/

	// 2D描画を左上原点にする  (20230512 update by tomoki.suzuki　
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(m_Application->GetWidth()),					// ビューボリュームの最小Ｘ
		static_cast<float>(m_Application->GetHeight()),					// ビューボリュームの最小Ｙ
		0.0f,															// ビューボリュームの最大Ｙ
		0.0f,
		1.0f);


	projection = projection.Transpose();

	m_DeviceContext->UpdateSubresource( m_ProjectionBuffer, 0, NULL, &projection, 0, 0 );
}


void Renderer::SetWorldMatrix( Matrix* WorldMatrix )
{
	Matrix world;
	world = WorldMatrix->Transpose();					// 転置

	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetViewMatrix(Matrix* ViewMatrix )
{
	Matrix view;
	view = ViewMatrix->Transpose();						// 転置

	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

void Renderer::SetProjectionMatrix( Matrix* ProjectionMatrix )
{
	Matrix projection;
	projection = ProjectionMatrix->Transpose();

	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}



void Renderer::SetMaterial( MATERIAL Material )
{
	m_DeviceContext->UpdateSubresource( m_MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetLight( LIGHT Light )
{
	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}





void Renderer::CreateVertexShader( ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName )
{

	FILE* file;
	long int fsize;

	fopen_s(&file,FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader( ID3D11PixelShader** PixelShader, const char* FileName )
{
	FILE* file;
	long int fsize;

	fopen_s(&file, FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}