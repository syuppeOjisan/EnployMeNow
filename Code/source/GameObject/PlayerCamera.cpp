#include <iostream>
#include "GameObject/PlayerCamera.h"

PlayerCamera::PlayerCamera(GameObject *_pTarget)
{
	// �^�[�Q�b�g���w��
	m_pTarget = _pTarget;

	Init();
}

void PlayerCamera::Init()
{
	// �O��t���[���ł̃J�����ʒu������
	m_LastCamPos = Vector3(0, 0, 0);
}

void PlayerCamera::Update()
{
	if (m_pTarget)
	{
		// ����L�����̑O�����x�N�g�����擾
		Vector3 forwordVec = m_pTarget->GetForward();
		// �O�����x�N�g���𔽓]�����A�������x�N�g���ɂ���
		Vector3 backVec;
		backVec.x = -forwordVec.x;
		backVec.y = -forwordVec.y;
		backVec.z = -forwordVec.z;

		// �J�����ʒu��^��납��20�x��]������
		Matrix mxRotOffset = Matrix::CreateRotationY(DirectX::XMConvertToRadians(20.0f));
		backVec = Vector3::Transform(backVec, mxRotOffset);

		// �쐬�����������x�N�g����ɃJ������ݒu
		Vector3 CamPos;
		Vector3 TargetPos = m_pTarget->GetPosition();
		float CamDistance = 3.0f; // �L�����ƃJ�����̋���
		CamPos.x = TargetPos.x + backVec.x * CamDistance;
		CamPos.y = TargetPos.y + backVec.y * CamDistance + 1.0f;
		CamPos.z = TargetPos.z + backVec.z * CamDistance;

		// �ɂ₩�J��������
		float blendFactor = 0.95f; // ���ω��̏d��
		CamPos.x = m_LastCamPos.x * blendFactor + CamPos.x * (1.0f - blendFactor);
		CamPos.y = m_LastCamPos.y * blendFactor + CamPos.y * (1.0f - blendFactor);
		CamPos.z = m_LastCamPos.z * blendFactor + CamPos.z * (1.0f - blendFactor);

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
	m_pTarget = _pTarget;
}
