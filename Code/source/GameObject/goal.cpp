#include "GameObject/Goal.h"
#include "Component/modelRenderer.h"
#include "Component/shader.h"
#include "Component/shadow.h"

void Goal::Init()
{

	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\goal.obj");


	AddComponent<Shadow>()->SetSize(1.5f);

}
