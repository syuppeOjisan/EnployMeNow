#pragma once

#include "Base/scene.h"

class Game : public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void HitCheck() override;

private:
	class Transition* m_Transition{};
	class Audio* m_BGM{};
	bool m_Goal = false;
};