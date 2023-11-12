#pragma once

#include "Base/CharacterBase.h"

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
	void Update() override;
	void PreDraw() override;

	void SetCamera(PlayerCamera* _camera);	// �v���C���[�ɃJ��������o�^

private:
	class	PlayerCamera* m_pCamera;	// �v���C���[�p�J����
	bool	m_isLanding;				// ��x�����W�����v�ł��Ȃ��悤�ɂ���

	enum
	{
		ANIMATION_ID_IDLE = 0,
		ANIMATION_ID_WALK,
		ANIMATION_ID_PUNCHING,
	};
};