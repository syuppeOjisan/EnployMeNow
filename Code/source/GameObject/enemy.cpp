#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "System/input.h"
#include "Scene/Base/scene.h"
#include "Component/modelRenderer.h"
#include "GameObject/enemy.h"
#include "Component/shader.h"
//#include "shadow.h"


void Enemy::Init()
{

	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");

//	AddComponent<Shadow>()->SetSize(2.0f);

}


