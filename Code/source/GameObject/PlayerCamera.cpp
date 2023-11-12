#include <iostream>
#include "GameObject/PlayerCamera.h"
#include "../../header/System/input.h"

PlayerCamera::PlayerCamera(GameObject *_pTarget)
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
		if (Input::GetisMouceMove())
		{
			Matrix mxMouseRotationY = Matrix::CreateRotationY(DirectX::XMConvertToRadians(Input::GetMouseMove().x * m_MouseSensitivity));
			Matrix mxMouseRotationX = Matrix::CreateRotationX(DirectX::XMConvertToRadians(Input::GetMouseMove().y * m_MouseSensitivity));
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
		CamPos.y = TargetPos.y + m_BackVec.y * CamDistance + 5.0f;
		CamPos.z = TargetPos.z + m_BackVec.z * CamDistance;

		// �J�����ʒu��ݒ�
		this->SetPosition(CamPos);
		m_LastCamPos = CamPos;	// �ʒu��ۑ�
		
		// �J�����̒����_��ݒ�
		Vector3 CamFocus;
		CamFocus.x = TargetPos.x;
		CamFocus.y = TargetPos.y;
		CamFocus.z = TargetPos.z;
		this->m_Target = CamFocus;
	}
	else
	{
		// �����^�[�Q�b�g��null�Ȃ烍�O�ɏo��
		std::cout << "PlayerCamera.cpp - Update()" << std::endl;
		std::cout << "m_pTarget��null�ł�" << std::endl;
	}
}

void PlayerCamera::SetTarget(GameObject* _pTarget)
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
