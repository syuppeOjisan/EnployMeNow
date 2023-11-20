#include <iostream>
#include "GameObject/PlayerCamera.h"
#include "../../header/System/input.h"

PlayerCamera::PlayerCamera(CharacterBase*_pTarget)
{
	// �^�[�Q�b�g���w��
	m_pTargetObject = _pTarget;

	// �����_�ƃJ�����ʒu���w��i���̂Q�������l���ƃo�O��j
	m_Position = Vector3(0.0f, 10.0f, -50.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);

	Init();
}

void PlayerCamera::Init()
{
	// �O��t���[���ł̃J�����ʒu������
	m_LastCamPos = Vector3(0, 0, 0);

	// �����_�ƃJ�����ʒu���w��i���̂Q�������l���ƃo�O��j
	m_Position = Vector3(0.0f, 10.0f, -50.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);

	// ���_�ړ��}�E�X���x����
	m_MouseSensitivity = 0.3f;
}

void PlayerCamera::Update()
{
	if (m_pTargetObject)
	{
		// �J�����ʒu��^��납��20�x��]������
		//Matrix mxRotOffset = Matrix::CreateRotationY(DirectX::XMConvertToRadians(20.0f));
		//m_BackVec = Vector3::Transform(m_BackVec, mxRotOffset);

		// ��ō�����I�t�Z�b�g�t���̌������x�N�g���Ƀ}�E�X�̈ړ���������
		// �J�������}�E�X�ňړ�������
		if (m_pInput)
		{
			int mouseX, mouseY;
			m_pInput->GetMouseMove(mouseX, mouseY);
			Matrix mxMouseRotationY = Matrix::CreateRotationY(DirectX::XMConvertToRadians(mouseX * m_MouseSensitivity));
			Matrix mxMouseRotationX = Matrix::CreateRotationX(DirectX::XMConvertToRadians(mouseY * m_MouseSensitivity));
			m_BackVec = Vector3::Transform(m_BackVec, mxMouseRotationY);
			m_BackVec = Vector3::Transform(m_BackVec, mxMouseRotationX);
			
		}


		// TODO:�J�����ɂ������蔻���t������
		// TODO:�J�������猩�����ʂ��v���C���[�̐��ʂƂ���
		// �v���C���[���ړ��L�[��������J�����������Ă�������ɉ�]��������

		// �쐬�����������x�N�g����ɃJ������ݒu
		Vector3 CamPos;
		Vector3 TargetPos = m_pTargetObject->GetPosition();
		float CamDistance = 13.0f; // �L�����ƃJ�����̋���
		CamPos.x = TargetPos.x + m_BackVec.x * CamDistance;
		CamPos.y = TargetPos.y + m_BackVec.y * CamDistance + 5.0f; // �����ォ�猩��
		CamPos.z = TargetPos.z + m_BackVec.z * CamDistance;

		// �J�����ʒu��ݒ�
		this->SetPosition(CamPos);
		m_LastCamPos = CamPos;	// �ʒu��ۑ�
		
		// �J�����̒����_��ݒ�
		Vector3 CamFocus;
		static bool shakePuls = true;
		CamFocus.x = TargetPos.x;
		CamFocus.y = TargetPos.y + shake;
		CamFocus.z = TargetPos.z;
		this->m_Target = CamFocus;

		// �������[�V�����̂Ƃ��i���܂̓v���C���[�Ɉˑ������R�[�h�Ȃ̂Ō�Œ����j
		if (shakeDO && m_pTargetObject->GetNextAnimationID() == 1)
		{
			if (shakePuls)
			{
				shake += shakeSpeed;
				if (shake > shakeWidth)
				{
					shakePuls = false;
				}
			}
			else if (!shakePuls)
			{
				shake -= shakeSpeed;
				if (shake < -shakeWidth)
				{
					shakePuls = true;
				}
			}
		}
		else
		{
			shake = 0.0f;
		}

	}
	else
	{
		// �����^�[�Q�b�g��null�Ȃ烍�O�ɏo��
		std::cout << "PlayerCamera.cpp - Update()" << std::endl;
		std::cout << "m_pTarget��null�ł�" << std::endl;
	}
}

void PlayerCamera::SetTarget(CharacterBase* _pTarget)
{
	// �^�[�Q�b�g�w��
	m_pTargetObject = _pTarget;
	m_BackVec = -m_pTargetObject->GetForward();
}

Vector3 PlayerCamera::GetCameraFrontVec()
{
	Vector3 camPos = this->GetPosition();
	Vector3 playerPos = m_pTargetObject->GetPosition();

	Vector3 camFrontVec = playerPos - camPos;
	camFrontVec.Normalize();

	return camFrontVec;
}
