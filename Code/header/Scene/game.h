#pragma once

#include "Base/scene.h"

class Game : public Scene
{
private:
	class Transition*	m_Transition{};
	class Audio*		m_BGM{};
	bool m_Goal = false;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;

};