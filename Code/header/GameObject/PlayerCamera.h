#pragma once

#include "camera.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class PlayerCamera : public Camera
{
public:
	//-----------------------
	// ������
	//-----------------------
	// �R���X�g���N�^
	PlayerCamera(GameObject *_pTarget);	
	PlayerCamera() {};

	// �f�X�g���N�^
	~PlayerCamera(void) {m_Position = Vector3(0.0f, 10.0f, -50.0f); m_Target = Vector3(0.0f, 0.0f, 0.0f);};
	
	void Init()override;	// ����������
	void Update()override;	// �X�V����

	//-----------------------
	// �A�N�Z�T
	//-----------------------

	// �^�[�Q�b�g�ݒ�
	void SetTarget(GameObject* _pTarget);

	// �J�����̑O�����P�ʃx�N�g�����擾
	Vector3 GetCameraFrontVec();

	//-----------------------
	// �����o�ϐ�
	//-----------------------
private:
	// �Ǐ]����Q�[���I�u�W�F�N�g
	GameObject* m_pTargetObject;

	// �v���C���[�̌������x�N�g��
	Vector3 m_BackVec;
	// �O��t���[���ł̃J�����ʒu
	Vector3 m_LastCamPos;
	// �}�E�X���x
	float m_MouseSensitivity;
};