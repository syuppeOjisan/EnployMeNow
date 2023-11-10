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
#include "GameObject/PlayerCamera.h"
#include "GameObject/Item_X.h"
#include "GameObject/Item_Y.h"
#include "GameObject/Item_Z.h"

#include <iostream>

using namespace DirectX::SimpleMath;

void Player::Init()
{
	CharacterBase::Init();

	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	m_pModel = AddComponent<AnimationModel>();
	
	m_pModel->Load("asset\\model\\Player_Tpose.fbx");							// animation ok
	m_pModel->LoadAnimation("asset\\model\\Player_Idle.fbx", "Idle");
	m_pModel->LoadAnimation("asset\\model\\Player_Walk.fbx", "Run");
	m_pModel->LoadAnimation("asset\\model\\Player_Punching.fbx", "Punching");

	AddComponent<Shadow>()->SetSize(1.5f);

	// �T�C�Y����
	m_Scale = Vector3(0.02f, 0.02f, 0.02f);

	// �����蔻��ݒ�
	this->m_AABBCollision = SetAABB(this->m_Position, this->m_Scale.x, this->m_Scale.y, this->m_Scale.z);

	// ���[�V�����ϐ�������
	m_NowAnimation = "Idle";
	m_PrevAnimation = "Idle";
}

void Player::Update()
{
	Vector3 oldPosition = m_Position;

	Scene* nowscene = Manager::GetScene();
	Camera* cameraobj = nowscene->GetGameObject<PlayerCamera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	// ���E�ɉ�]
	if (Input::GetKeyPress('A'))
	{
		m_Position -= XAxis * 0.4f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Position += XAxis * 0.3f;
	}

	// �O���x�N�g�����擾
	Vector3 forward = m_pCamera->GetCameraFrontVec();
	forward.Normalize();

	forward.x *= 0.3;
	forward.y = 0;
	forward.z *= 0.3;

	// �O��ړ�
	if (Input::GetKeyPress('W'))
	{
		SetVelocity(forward);
		SetNextAnimation("Run");
	}
	else if (Input::GetKeyPress('S'))
	{
		SetVelocity(-forward);
		SetNextAnimation("Run");
	}
	else if (Input::GetKeyPress('P'))
	{
		SetNextAnimation("Punching");
	}
	else
	{
		SetNextAnimation("Idle");
	}

	//�W�����v
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		m_Velocity.y = 0.2f;
	}


	// TODO:�e�����Ă�������Ɍ���
	// �ꉞ�ł���
	//�e����
	if (Input::GetKeyTrigger('K'))
	{
		forward.y += 0.2f;
		forward.Normalize();
		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(2);
		bullet->SetPosition(m_Position + Vector3(0.0f,2.0f,0.0f));
		bullet->SetVelocity(forward);
	}


	//�d��
	m_Velocity.y -= 0.015f;

	//��R
	m_Velocity.y -= m_Velocity.y * 0.01f;


	//�ڒn
	float groundHeight = 0.0f;
	if (GetPosition().y <= groundHeight)
	{
		m_isLanding = true;
	}
	else
	{
		m_isLanding = false;
	}


	// �����蔻��X�V
	this->m_AABBCollision = SetAABB(this->m_Position, 1, 1, 1);

	// ���݃V�[�����擾
	Scene* scene = Manager::GetScene();

	// TODO:�����蔻������������ɕς���
	// ���߂ăQ�[���V�[�����Ŕ�����s������

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
					m_isLanding = true;
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
				else
				{
					groundHeight = position.y + scale.y * 2.0f;
				}
			}
			else
			{
				m_isLanding = false;
			}
		}
	}


	// �ʒu���O�ȉ��Ȃ�n�ʈʒu�ɃZ�b�g����
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	////TODO:�u�����h���[�g���������ł��ĂȂ�����^�C�~���O��������
	////�u�����h���[�g���̂����f���̕��Ɏ������Ă���������
	//if (Input::GetKeyPress('W'))
	//{
	//	m_Model->SetNextAnimation("Run");
	//	m_BlendRate += 0.1f;

	//}
	//else if (Input::GetKeyPress('S'))
	//{
	//	m_Model->SetNextAnimation("Run");
	//	m_BlendRate += 0.1f;
	//}
	//else if (Input::GetKeyPress('P'))
	//{
	//	//TODO: �p���`���͎�̐�ɓ����蔻���t���ĉ����悤�ɂ���
	//	m_Model->SetNextAnimation("Punching");
	//	m_BlendRate += 0.1f;
	//}
	//else if(m_Model->GetIsAnimBlend())
	//{
	//	m_Model->SetNextAnimation("Idle");
	//	m_Model->SetBlendSpeed(0.1f);
	//	m_BlendRate += 0.1f;
	//}
	//m_Frame++;

	CharacterBase::Update();
}


void Player::PreDraw()
{
	m_pModel->Update(m_PrevAnimation, m_PrevAnimationFrame, m_NowAnimation, m_NowAnimationFrame, m_BlendRate);
}

void Player::SetCamera(PlayerCamera* _camera)
{
	this->m_pCamera = _camera;
}