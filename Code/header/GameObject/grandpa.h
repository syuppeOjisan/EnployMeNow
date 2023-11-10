#pragma once
#include "Base/CharacterBase.h"

class grandpa : public CharacterBase
{
public:
	void Init() override;
	void Update() override;
	void PreDraw() override;
};

