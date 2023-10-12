#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
//#include "input.h"
#include "Scene/Base/scene.h"
#include "Component/modelRenderer.h"
#include "Component/shader.h"
#include "GameObject/sky.h"
#include "GameObject/PlayerCamera.h"



using namespace DirectX::SimpleMath;

void Sky::Init()
{
	m_Scale = Vector3(100.0f, 100.0f, 100.0f);


	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\sky.obj");

}


void Sky::Update()
{

	Scene* scene = Manager::GetScene();
	//TODO:GetGameObject<PlayerCamera>()�ł̓J�����̕ύX�ɏ_���Ȃ��̂Œ����H�@�K�v���ǂ����͂킩��Ȃ�
	Camera* camera = scene->GetGameObject<PlayerCamera>();

	Vector3 cameraPosition = camera->GetPosition();
	m_Position = cameraPosition;

}

