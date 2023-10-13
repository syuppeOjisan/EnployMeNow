#pragma once

#include "Base/gameObject.h"

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

class Player : public GameObject
{
	// TODO:SE��A�z�z��ŊǗ�������
private:
	DirectX::SimpleMath::Vector3				m_Velocity{};
	class	AnimationModel*						m_Model;
	class	PlayerCamera*						m_pCamera;
	BULLET_MODE									m_BulletMode;	// ���ݑ������̒e�̃��[�h
	BULLET_KIND									m_BulletEquip;	// �������̒e
	std::unordered_map<std::string, Audio*>		m_SEList;		// SE�̃��X�g
	float	m_BlendRate;
	int		m_Frame;
	bool	m_isLanding;	// ��x�����W�����v�ł��Ȃ��悤�ɂ���
	bool	m_isSEplay_X;
	bool	m_isSEplay_Y;
	bool	m_isSEplay_Z;


public:
	void Init() override;
	void Update() override;
	void PreDraw() override;

	void SetCamera(PlayerCamera* _camera);

	BULLET_MODE GetBulletMode();	// ���̒e�̃��[�h���擾
	BULLET_KIND GetBulletKind() { return m_BulletEquip; }
};