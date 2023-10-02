#pragma once

#include "Base/gameObject.h"

class Battery : public GameObject
{

private:

	GameObject* m_Child{};
	float m_MoveTime{};
	float m_BulletTime{};

public:
	void Init() override;
	void Update() override;

};