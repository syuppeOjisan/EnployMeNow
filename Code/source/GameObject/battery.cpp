#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "System/input.h"
#include "Scene/Base/scene.h"
#include "Component/modelRenderer.h"
#include "Component/shader.h"
#include "GameObject/battery.h"
#include "GameObject/bullet.h"
//#include "shadow.h"


using namespace DirectX::SimpleMath;

void Battery::Init()
{

	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\battery1.obj");

//	AddComponent<Shadow>()->SetSize(3.0f);


	m_Child = AddChild<GameObject>();
	m_Child->AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	m_Child->AddComponent<ModelRenderer>()->Load("asset\\model\\battery2.obj");

}




void Battery::Update()
{
	m_MoveTime += 1.0f / 60.0f;

	m_Position.z += cosf(m_MoveTime * 1.0f) * 0.1f;


	Vector3 rotation = m_Child->GetRotation();
	rotation.y += 0.02f;
	m_Child->SetRotation(rotation);



	//’e”­ŽË
	m_BulletTime += 1.0f / 60.0f;

	if (m_BulletTime > 1.0f)
	{
		m_BulletTime -= 1.0f;

		Vector3 forward = m_Child->GetForward();

		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(2);
		bullet->SetPosition(m_Position + Vector3(0.0f, 1.5f, 0.0f));
		bullet->SetVelocity(forward * -0.5f);
	}
}


