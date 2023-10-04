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
}

void PlayerCamera::Update()
{
	if (m_pTargetObject)
	{
		// ����L�����̑O�����x�N�g�����擾
		Vector3 forwordVec = m_pTargetObject->GetForward();
		// �O�����x�N�g���𔽓]�����A�������x�N�g���ɂ���
		Vector3 backVec;
		backVec.x = -forwordVec.x;
		backVec.y = -forwordVec.y;
		backVec.z = -forwordVec.z;

		// �J�����ʒu��^��납��20�x��]������
		Matrix mxRotOffset = Matrix::CreateRotationY(DirectX::XMConvertToRadians(20.0f));
		backVec = Vector3::Transform(backVec, mxRotOffset);

		// TODO:�Ȃ񂩂��������ɃJ�����������Ȃ����玡��
		// ��ō�����I�t�Z�b�g�t���̌������x�N�g���Ƀ}�E�X�̈ړ���������
		// �J�������}�E�X�ňړ�������
		Matrix mxMouseRotationY = Matrix::CreateRotationY(DirectX::XMConvertToRadians(Input::GetMouseMove().x));
		Matrix mxMouseRotationZ = Matrix::CreateRotationY(DirectX::XMConvertToRadians(Input::GetMouseMove().y));
		backVec = Vector3::Transform(backVec, mxMouseRotationY);
		backVec = Vector3::Transform(backVec, mxMouseRotationZ);

		// �쐬�����������x�N�g����ɃJ������ݒu
		Vector3 CamPos;
		Vector3 TargetPos = m_pTargetObject->GetPosition();
		float CamDistance = 10.0f; // �L�����ƃJ�����̋���
		CamPos.x = TargetPos.x + backVec.x * CamDistance;
		CamPos.y = TargetPos.y + backVec.y * CamDistance + 5.0f;
		CamPos.z = TargetPos.z + backVec.z * CamDistance;

		// �ɂ₩�J��������
		//float blendFactor = 0.97f; // ���ω��̏d��
		//CamPos.x = m_LastCamPos.x * blendFactor + CamPos.x * (1.0f - blendFactor);
		//CamPos.y = m_LastCamPos.y * blendFactor + CamPos.y * (1.0f - blendFactor);
		//CamPos.z = m_LastCamPos.z * blendFactor + CamPos.z * (1.0f - blendFactor);

		// �J�����ʒu��ݒ�
		this->SetPosition(CamPos);
		m_LastCamPos = CamPos;	// �ʒu��ۑ�
		
		// �J�����̒����_��ݒ�
		Vector3 CamFocus;
		CamFocus.x = TargetPos.x + forwordVec.x;
		CamFocus.y = TargetPos.y + forwordVec.y;
		CamFocus.z = TargetPos.z + forwordVec.z;
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
}
