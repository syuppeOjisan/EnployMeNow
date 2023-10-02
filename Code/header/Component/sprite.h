#pragma once

#include	"../System/renderer.h"
#include	"Base/component.h"
#include	"../System/utftosjisconv.h"

class Sprite : public Component
{

private:
	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};
//	ID3D11Resource*				m_TextureResource{};

	MATERIAL					m_Material{};

	bool						m_isActive;

public:

	using Component::Component;

	void Init(int x, int y, int Width, int Height, const char* TextureName);
	void Uninit() override;
	void Draw() override;

	void SetMaterial(MATERIAL Material) { m_Material = Material; }

	void SetActive(bool _active) { m_isActive = _active; }

};