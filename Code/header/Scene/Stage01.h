#pragma once

#include "Base/scene.h"

class Stage01 : public Scene
{
private:
	class Transition* m_Transition{};
	GameObject* m_BGMPLayer;
	bool m_Goal = false;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
};

