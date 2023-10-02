#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "Component/modelRenderer.h"
#include "Component/shader.h"
#include "GameObject/box.h"
//#include "shadow.h"
//#include "input.h"
//#include "scene.h"

void Box::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\box.obj");

//	AddComponent<Shadow>()->SetSize(8.0f);
}
