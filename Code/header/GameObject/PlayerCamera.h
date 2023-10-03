#pragma once

// TODO:�v���C���[�J�����̈ʒu�ƒ����_���ǂ���������l�ɂȂ�̂Ńo�O���Ď��s�ł��Ȃ����玡��

#include "camera.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class PlayerCamera : public Camera
{
public:
	PlayerCamera(GameObject *_pTarget);
	PlayerCamera() {};
	~PlayerCamera(void) {};
	
	void Init()override;
	void Update()override;

	// �^�[�Q�b�g�ݒ�
	void SetTarget(GameObject* _pTarget);

private:
	// �Ǐ]����Q�[���I�u�W�F�N�g
	GameObject* m_pTarget;

	// �O��t���[���ł̃J�����ʒu
	Vector3 m_LastCamPos;
};