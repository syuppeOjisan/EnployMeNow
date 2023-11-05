#include "Gameobject/grandpa.h"

#include "Component/Shader.h"
#include "Scene/Base/scene.h"
#include "GameObject/player.h"
#include "System/manager.h"
#include "System/collision.h"
using namespace DirectX::SimpleMath;

void grandpa::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	m_Model = AddComponent<AnimationModel>();

	m_Model->Load("asset\\model\\Grandpa_Tpose.fbx");							// animation ok
	m_Model->LoadAnimation("asset\\model\\Grandpa_Idel.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Grandpa_Die.fbx", "Die");
	m_Model->SetFirstAnimation("Idle");

	// �T�C�Y����
	m_Scale = Vector3(0.02f, 0.02f, 0.02f);
}

void grandpa::Update()
{
	// ���݃V�[�����擾
	Scene* scene = Manager::GetScene();
	// �v���C���[�Ƃ̓����蔻��
	{
		Player* pPlayer = scene->GetGameObject<Player>();

		if (pPlayer)
		{
			Vector3 position = pPlayer->GetPosition();
			Vector3 scale = pPlayer->GetScale();

			AABB aabbBox;
			AABB aabbPlayer;

			aabbBox = SetAABB(position, fabs(scale.x), fabs(scale.y), fabs(scale.z));
			aabbPlayer = SetAABB(m_Position, fabs(m_Scale.x), fabs(m_Scale.y), fabs(m_Scale.z));

			bool sts = CollisionAABB(aabbBox, aabbPlayer);

			if (sts)
			{
				m_Model->SetNextAnimation("Die");
			}
		}
	}
	m_Frame++;
}

void grandpa::PreDraw()
{
	m_Model->Update(m_Frame, m_Frame, m_BlendRate);
}