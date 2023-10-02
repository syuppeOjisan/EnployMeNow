#pragma once

#include "Base/gameObject.h"
#include "Component/animationModel.h"

// �e�̃��[�h
enum BULLET_MODE
{
	SIZE_UP,
	SIZE_DOWN,
	NONE,
};

class Player : public GameObject
{

private:

	DirectX::SimpleMath::Vector3		m_Velocity{};
	class	Audio*						m_SE{};
	class	AnimationModel*				m_Model;
	BULLET_MODE							m_bulletMode;	// ���ݑ������̒e�̃��[�h
	float	m_BlendRate;
	int		m_Frame;
	bool	m_isLanding;	// ��x�����W�����v�ł��Ȃ��悤�ɂ���


public:
	void Init() override;
	void Update() override;
	void PreDraw() override;

	BULLET_MODE GetBulletMode();	// ���̒e�̃��[�h���擾
};