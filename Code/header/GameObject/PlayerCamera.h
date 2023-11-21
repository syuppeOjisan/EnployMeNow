#pragma once

#include "camera.h"
#include "GameObject/Base/CharacterBase.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class PlayerCamera : public Camera
{
public:
	//-----------------------
	// ������
	//-----------------------
	// �R���X�g���N�^
	PlayerCamera(CharacterBase*_pTarget);
	PlayerCamera() {};

	// �f�X�g���N�^
	~PlayerCamera(void) {m_Position = Vector3(0.0f, 10.0f, -50.0f); m_Target = Vector3(0.0f, 0.0f, 0.0f);};
	
	void Init()override;	// ����������
	void Update()override;	// �X�V����

	//-----------------------
	// �A�N�Z�T
	//-----------------------

	// �^�[�Q�b�g�ݒ�
	void SetTarget(CharacterBase* _pTarget);

	// �J�����̑O�����P�ʃx�N�g�����擾
	Vector3 GetCameraFrontVec();

	float getNowSensi(void) { return m_ViewControlSensitivity; }
	void SetSensitivity(float _sensi) { m_ViewControlSensitivity = _sensi; }

	float* GetShake(void) { return &shake; }
	float* GetShakeWidth(void) { return &shakeWidth; }
	float* GetShakeSpeed(void) { return &shakeSpeed; }
	bool* GetShakeDO(void) { return &shakeDO; }

	bool* GetCamMove(void) { return &camMove; }

	//-----------------------
	// �����o�ϐ�
	//-----------------------
private:
	// �Ǐ]����L�����N�^
	CharacterBase* m_pTargetObject;

	// �v���C���[�̌������x�N�g��
	Vector3 m_BackVec;
	// �O��t���[���ł̃J�����ʒu
	Vector3 m_LastCamPos;
	// ���_���슴�x
	float m_ViewControlSensitivity;

	float shake = 0.0f;
	float shakeWidth = 0.2f;
	float shakeSpeed = 0.1f;
	bool shakeDO = false;

	bool camMove = true;
};