#pragma once

#include "Base/gameObject.h"

class Score : public GameObject
{

private:
	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};

	int m_Count;

public:
	void Init();
	void Uninit();
	void Draw();
	void AddCount(int Count) { m_Count += Count; }
};