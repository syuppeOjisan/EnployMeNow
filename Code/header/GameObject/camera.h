#pragma once

#include	<SimpleMath.h>
#include "Base/gameObject.h"

class Camera : public GameObject{
private:

	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	DirectX::SimpleMath::Matrix GetViewMatrix() 
	{ 
		return m_ViewMatrix;
	}
};