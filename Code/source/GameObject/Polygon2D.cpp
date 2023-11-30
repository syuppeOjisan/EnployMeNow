//#include <WICTextureLoader.h>
#include "Component/sprite.h"
#include "Component/shader.h"
#include "GameObject/polygon2D.h"
//#include "renderer.h"

using namespace DirectX::SimpleMath;

void Polygon2D::Init()
{
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(0, 0, 200, 200, "asset\\texture\\grass.jpg");

	
/*
	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3(200.0f, 0.0f, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(0.0f, 200.0f, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3(200.0f, 200.0f, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		L"asset/texture/grass.jpg",
		nullptr,
		&m_Texture);

	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader/unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader/unlitTexturePS.cso");

*/


}


void Polygon2D::Uninit()
{
	for (auto& component : m_Component) 
	{
		component->Uninit();
	}
	m_Component.clear();
}

void Polygon2D::Update()
{
}

void Polygon2D::Draw()
{

	for (auto& cmpt : m_Component) {
		cmpt->Draw();
	}


/*
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
*/

}