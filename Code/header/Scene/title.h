#pragma once
#include <unordered_map>

#include "Base/scene.h"

class Title : public Scene
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;

private:
	class Transition* m_Transition{};
};