#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include "Base/gameObject.h"

class Polygon2D : public GameObject
{
private:

//	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
//	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
//	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

//	ID3D11Buffer* m_VertexBuffer{};
//	ID3D11ShaderResourceView* m_Texture{};

//	ID3D11VertexShader* m_VertexShader{};
//	ID3D11PixelShader* m_PixelShader{};
//	ID3D11InputLayout* m_VertexLayout{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};