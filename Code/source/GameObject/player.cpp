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

	// �V�F�[�_�[�̓ǂݍ���
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	m_pModel = AddComponent<AnimationModel>();
	
	// ���[�V�����ǂݍ���
	m_pModel->Load("asset\\model\\Player_Tpose.fbx");
	//m_pModel->LoadAnimation("asset\\model\\Player_Idle.fbx", "Idle");
	//m_pModel->LoadAnimation("asset\\model\\Player_Walk.fbx", "Walk");
	//m_pModel->LoadAnimation("asset\\model\\Player_Punching.fbx", "Punching");

	m_pModel->LoadAnimation("asset\\model\\Player_Idle.fbx", ANIMATION_ID_IDLE);
	m_pModel->LoadAnimation("asset\\model\\Player_Walk.fbx", ANIMATION_ID_WALK);
	m_pModel->LoadAnimation("asset\\model\\Player_WalkBack.fbx", ANIMATION_ID_WALKBACK);
	m_pModel->LoadAnimation("asset\\model\\Player_Run.fbx", ANIMATION_ID_RUN);
	m_pModel->LoadAnimation("asset\\model\\Player_Punching.fbx", ANIMATION_ID_PUNCHING);

	AddComponent<Shadow>()->SetSize(1.5f);

	// �T�C�Y����
	m_Scale = Vector3(0.02f, 0.02f, 0.02f);

	// �����蔻��ݒ�
	this->m_AABBCollision = SetAABB(this->m_Position, this->m_Scale.x, this->m_Scale.y, this->m_Scale.z);

	// ���[�V�����ϐ�������
	m_NowAnimation = "Idle";
	m_PrevAnimation = "Idle";

	m_NowAnimationSpeed = 1;
	m_PreAnimationSpeed = 1;

	m_NowAnimationID = ANIMATION_ID_IDLE;
	m_PrevAnimationID = ANIMATION_ID_IDLE;

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

	// �C���^�[�t�F�[�X���X�V���Ďg�p
	if (m_pInput)
	{
		m_pInput->Update();
		Vector2 leftStick = {};
		Vector2 rightStick = {};
		m_pInput->GetDeviceMovement(leftStick, rightStick);

		// �J�����̑O�����x�N�g��
		Vector3 CamForward = m_pCamera->GetCameraFrontVec();

		// Z���̈ړ��������쐬
		Vector3 forward{};
		forward = CamForward;
		forward.x *= leftStick.y;
		forward.z *= leftStick.y;

		// X���̈ړ��������쐬
		Vector3 right{};
		right.z = -CamForward.x;
		right.x = CamForward.z;

		right.x *= leftStick.x;
		right.z *= leftStick.x;

		// �����2�̎��̈ړ�����������
		forward += right;
		//forward.Normalize();
		//forward.x *= 0.1;
		forward.y = 0;
		//forward.z *= 0.1;

		std::cout << "forward:" << std::endl;
		std::cout << "X:" << forward.x << std::endl;
		std::cout << "Y:" << forward.y << std::endl;
		std::cout << "Z:" << forward.z << std::endl;
		std::cout << "right:" << std::endl;
		std::cout << "X:" << right.x << std::endl;
		std::cout << "Y:" << right.y << std::endl;
		std::cout << "Z:" << right.z << std::endl;


		//if (rightStick.y > 0.1f)
		{
			SetVelocity(forward);

			//std::cout << "forward:" << std::endl;
			//std::cout << "X:" << forward.x << std::endl;
			//std::cout << "Y:" << forward.y << std::endl;
			//std::cout << "Z:" << forward.z << std::endl;

			// �J�����̑O�����x�N�g�����擾
			Vector3 cameraFront = m_pCamera->GetCameraFrontVec();

			//abs(forward.x) > abs(forward.z) ? m_NowAnimationSpeed = abs(forward.x) : m_NowAnimationSpeed = abs(forward.z);

			
			// �v���C���[���J�����������Ă�����Ɍ�����
			if (abs(forward.x) > 0.5f || abs(forward.z) > 0.5f)
			{
				SetNextAnimation(ANIMATION_ID_RUN);
				SetRotateToVector(m_pCamera->GetCameraFrontVec());
			}
			else if (abs(forward.x) < 0.5f && abs(forward.x) != 0.0f || abs(forward.z) < 0.5f && abs(forward.z) != 0.0f)
			{
				SetNextAnimation(ANIMATION_ID_WALK);
				m_NowAnimationSpeed *= 2;
				SetRotateToVector(m_pCamera->GetCameraFrontVec());

			}
			else
			{
				SetNextAnimation(ANIMATION_ID_IDLE);
			}
		}


		//// �O��ړ�
		//if (m_pInput->GetPressed('W'))
		//{
		//	if (m_pInput->GetPressed(VK_SHIFT))
		//	{
		//		SetVelocity(forward * 2);
		//		SetNextAnimation(ANIMATION_ID_RUN);
		//	}
		//	else
		//	{
		//		SetVelocity(forward);
		//		
		//		// �J�����̑O�����x�N�g�����擾
		//		Vector3 cameraFront = m_pCamera->GetCameraFrontVec();

		//		// �v���C���[���J�����������Ă�����Ɍ�����
		//		SetRotateToVector(cameraFront);


		//		SetNextAnimation(ANIMATION_ID_WALK);
		//	}
		//}
		//else if (m_pInput->GetPressed('S'))
		//{
		//	SetVelocity(-forward);
		//	SetNextAnimation(ANIMATION_ID_WALKBACK);
		//}
		//else if (m_pInput->GetPressed('P'))
		//{
		//	SetNextAnimation(ANIMATION_ID_PUNCHING);
		//}
		//else
		//{
		//	SetNextAnimation(ANIMATION_ID_IDLE);
		//}

		//�W�����v
		if (m_pInput->GetPressed(VK_SPACE))
		{
			SetRotation(Vector3(0, DEGREE_TO_RADIAN(90), 0));
		}
	}


	// TODO:�e�����Ă�������Ɍ���
	// �ꉞ�ł���
	//�e����
	//if (Input::GetKeyTrigger('K'))
	//{
	//	forward.y += 0.2f;
	//	forward.Normalize();
	//	Scene* scene = Manager::GetScene();
	//	Bullet* bullet = scene->AddGameObject<Bullet>(2);
	//	bullet->SetPosition(m_Position + Vector3(0.0f,2.0f,0.0f));
	//	bullet->SetVelocity(forward);
	//}


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

	CharacterBase::Update();
}


void Player::PreDraw()
{
	m_pModel->Update(m_PrevAnimationID, m_PrevAnimationFrame, m_NowAnimationID, m_NowAnimationFrame, m_BlendRate);
}

void Player::SetCamera(PlayerCamera* _camera)
{
	this->m_pCamera = _camera;
}

//void Player::SetRotateToVector(Vector3 _vector)
//{
//	// �x�N�g���������Ă���p�x�����߂�
//	float rotateRadianDestination = atan2f(_vector.x, _vector.z);
//	rotateRadianDestination = FixRadian(rotateRadianDestination);	// ���W�A���␳
//
//	// ���݂̉�]�p�Ƃ̊p�x�������߂�
//	float rotateRadian = rotateRadianDestination - GetRotation().y;
//	rotateRadian = FixRadian(rotateRadian);	// ���W�A���␳
//
//	// �p�x������
//	Vector3 pRotate = GetRotation();
//	pRotate.y += rotateRadian;
//	pRotate.y = FixRadian(pRotate.y);
//
//	// ��]�p�K�p
//	SetRotation(Vector3(0, pRotate.y, 0));
//}
