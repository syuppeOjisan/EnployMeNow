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
	class	PlayerCamera* m_pCamera;


	BULLET_MODE									m_BulletMode;	// ���ݑ������̒e�̃��[�h
	BULLET_KIND									m_BulletEquip;	// �������̒e
	float	m_BlendRate;
	int		m_Frame;
	char	m_NextAnim;		// ���ɑJ�ڂ���A�j���[�V����
	bool	m_isLanding;	// ��x�����W�����v�ł��Ȃ��悤�ɂ���
};