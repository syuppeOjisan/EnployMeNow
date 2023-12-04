#pragma once

#include "Base/CharacterBase.h"

#include "System/KeyboardInput.h"
#include "GameObject/PlayerCamera.h"
#include "Component/animationModel.h"
#include "Component/audio.h"

// ���e�̃��[�h
enum BULLET_MODE
{
	SIZE_UP,
	SIZE_DOWN,
};

// ���e�̎��
enum BULLET_KIND
{
	CHANGE_X_AXSIS,
	CHANGE_Y_AXSIS,
	CHANGE_Z_AXSIS,
	NONE,
};

class Player : public CharacterBase
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void PreDraw() override;

	/// <summary>
	/// �v���C���[�ɓ��̓C���^�[�t�F�[�X���Z�b�g
	/// </summary>
	/// <param name="_input">���̓C���^�[�t�F�[�X</param>
	void SetInput(InputIntarface* _input) { m_pInput = _input; m_pInput->Init(); }

	void SetCamera(PlayerCamera* _camera);	// �v���C���[�ɃJ��������o�^

	///// <summary>
	///// �v���C���[�̌������x�N�g���̕��Ɍ�����
	///// </summary>
	///// <param name="_vector">�����Ăق��������x�N�g��</param>
	//void SetRotateToVector(Vector3 _vector);

private:
	class	PlayerCamera* m_pCamera;	// �v���C���[�p�J����
	bool	m_isLanding;				// ��x�����W�����v�ł��Ȃ��悤�ɂ���

	// ���[�V����ID
	enum
	{
		ANIMATION_ID_IDLE = 0,
		ANIMATION_ID_WALK,
		ANIMATION_ID_WALKBACK,
		ANIMATION_ID_RUN,
		ANIMATION_ID_PUNCHING,
		ANIMATION_ID_PUNCHI_CHARGE,
		ANIMATION_ID_JUMP,
	};
};