#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "System/collision.h"
#include "System/input.h"
#include "Component/modelRenderer.h"
#include "Component/animationModel.h"
#include "Component/shader.h"
#include "Component/audio.h"
#include "Component/shadow.h"
#include "GameObject/player.h"
#include "GameObject/cylinder.h"
#include "GameObject/box.h"
#include "GameObject/bullet.h"
#include "GameObject/goal.h"
#include "GameObject/camera.h"


using namespace DirectX::SimpleMath;

void Player::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso"); //20230909-02
	m_Model = AddComponent<AnimationModel>();
	
	m_Model->Load("asset\\model\\Player_Tpose.fbx");							// animation ok
	m_Model->LoadAnimation("asset\\model\\Player_Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Player_Walk.fbx", "Run");

//	m_Model->Load("asset\\model\\Akai2.fbx");									// animation ok
//	m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Idle");
//	m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Run");

//	m_Model->Load("data\\model\\Walking\\Walking2.fbx");						// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Idle");
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Run");

//	m_Model->Load("data\\model\\Walking\\Walking.fbx");							// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Idle");
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Run");

//	m_Model->Load("data\\model\\Walking\\Walking.dae");							// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Idle");		// animation ok
//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Run");			// animation ok
	

	AddComponent<Shadow>()->SetSize(1.5f);

	m_SE = AddComponent<Audio>();
	m_SE->Load("asset\\audio\\wan.wav");

	m_Scale = Vector3(0.02f, 0.02f, 0.02f);
}

void Player::Update()
{
	Vector3 oldPosition = m_Position;

	Scene* nowscene = Manager::GetScene();
	Camera* cameraobj = nowscene->GetGameObject<Camera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	// ���E�ɉ�]
	if (Input::GetKeyPress('A'))
	{
		m_Position -= XAxis * 0.05f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Position += XAxis * 0.05f;
	}

	// �O���x�N�g�����擾
//	Vector3 forward = GetForward();
	Vector3 forward = ZAxis;

	if (Input::GetKeyPress('W'))
	{
		m_Position += forward * 0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position -= forward * 0.1f;
	}

	//�W�����v
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		m_Velocity.y = 0.35f;
	}

	// �e�̃��[�h�ύX
	if (Input::GetKeyTrigger('1'))
	{
		m_bulletMode = SIZE_UP;
	}

	if (Input::GetKeyTrigger('2'))
	{
		m_bulletMode = SIZE_DOWN;
	}

	//�e����
	if (Input::GetKeyTrigger('K'))
	{
		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(2);
		bullet->SetPosition(m_Position + Vector3(0.0f, 1.0f, 0.0f));
		bullet->SetVelocity(forward * 0.5f);
		bullet->SetBulletMode(m_bulletMode);

		m_SE->Play();
	}

	// �}�E�X�Ńv���C���[�̎��_�ړ�
	float sensitivity = 0.01f;
	Vector2 mousePos = Input::GetMouseMove();
	m_Rotation.y += mousePos.x * sensitivity;

	// �s�N�Z���P�ʂ���x���@�ɕϊ����� Pitch �𐧖�
	float degreesPerPixel = 1.0f; // ���̒l�A�K�؂Ȓl�ɒ���
	m_Rotation.x += mousePos.y * sensitivity * degreesPerPixel;

	// �㉺�����̐����K�p
	m_Rotation.x = max(-90.0f, min(90.0f, m_Rotation.x));


	//�d��
	m_Velocity.y -= 0.015f;

	//��R
	m_Velocity.y -= m_Velocity.y * 0.01f;

	//�ړ�
	m_Position += m_Velocity;

	//�ڒn
	float groundHeight = 0.0f;

	// ���݃V�[�����擾
	Scene* scene = Manager::GetScene();

	// cylinder�Ƃ̓����蔻��
	{
		std::vector<Cylinder*> cylinderlist = scene->GetGameObjects<Cylinder>();

		for (const auto& cylobj : cylinderlist) {
			Vector3 intersectiopoint{};
			// �v���C����BS�쐬
			BoundingSphere playerbs{};

			playerbs.center = m_Position;
			playerbs.radius = fabs(m_Scale.x / 2.0f);

			//�@�~���̓����蔻��f�[�^�쐬 
			Vector3 position = cylobj->GetPosition();
			Vector3 scale = cylobj->GetScale();

			BoundingCylinder boundingcylinder{};
			boundingcylinder.bottom = position;
			boundingcylinder.bottom.y = 0.0f;

			boundingcylinder.top = position;
			boundingcylinder.top.y = position.y + scale.y - 0.5f;

			boundingcylinder.radius = scale.x;
			bool sts = CollisionSphereCylinder(playerbs, boundingcylinder);
			if (sts) {
				if (m_Position.y < position.y + scale.y - 0.5f)
				{
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
				else
				{
					groundHeight = position.y + scale.y;
				}
				break;
			}
		}
	}

	// Box�Ƃ̓����蔻��
	{
		std::vector<Box*> boxlist = scene->GetGameObjects<Box>();

		for (const auto& boxobj : boxlist) {
			// BOX�̓����蔻��f�[�^�쐬 
			Vector3 position = boxobj->GetPosition();
			Vector3 scale = boxobj->GetScale();

			if (position.x - scale.x - 0.5f < m_Position.x && m_Position.x < position.x + scale.x + 0.5f &&
				position.z - scale.z - 0.5f < m_Position.z && m_Position.z < position.z + scale.z + 0.5f)
			{
				if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
				{
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
				else
				{
					groundHeight = position.y + scale.y * 2.0f;
				}
			}
		}
	}

	// �ʒu���O�ȉ��Ȃ�n�ʈʒu�ɃZ�b�g����
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	// �S�[���Ƃ̓����蔻��
	{
		Goal* goal = scene->GetGameObject<Goal>();

		if (goal)
		{
			Vector3 position = goal->GetPosition();
			Vector3 scale = goal->GetScale();

			AABB aabbBox;
			AABB aabbPlayer;

			aabbBox = SetAABB(position, fabs(2.0f * scale.x), fabs(2.0f * scale.y), fabs(2.0f * scale.z));
			aabbPlayer = SetAABB(m_Position, fabs(1.0f * m_Scale.x), fabs(1.0f * m_Scale.y), fabs(1.0f * m_Scale.z));

			bool sts = CollisionAABB(aabbBox, aabbPlayer);

			if (sts)
			{
				goal->SetDestroy();
			}
		}
	}


	if (Input::GetKeyPress('W'))
	{
		m_BlendRate += 0.1f;
		m_Frame++;

	}
	else if (Input::GetKeyPress('S'))
	{
		m_BlendRate += 0.1f;
		m_Frame--;
	}
	else
	{
		m_BlendRate -= 0.1f;
		m_Frame++;
	}

	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;

	m_Frame++;

}


void Player::PreDraw()
{
	m_Model->Update("Idle", m_Frame, "Run", m_Frame, m_BlendRate);
}

BULLET_MODE Player::GetBulletMode()
{
	return m_bulletMode;
}

