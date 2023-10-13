#pragma once
#include "base/gameObject.h"
#include "Component/Shader.h"

class ItemBase : public GameObject
{
public:
	void Init()override { AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso"); }
	void Update()override;
};

