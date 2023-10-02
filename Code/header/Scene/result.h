#pragma once

#include "Base/scene.h"

class Result : public Scene
{
private:
	class Transition* m_Transition{};


public:
	void Init() override;
	void Update() override;
};