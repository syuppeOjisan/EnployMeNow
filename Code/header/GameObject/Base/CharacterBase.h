#pragma once

#include "gameObject.h"
#include "System/collision.h"

#include "Component/animationModel.h"

using namespace DirectX::SimpleMath;

// �L�����N�^�[�i�G�����֌W�Ȃ��j�̊��N���X�ł�
class CharacterBase : public GameObject
{
public:
	virtual void Init() { m_Model = nullptr; }
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void PreDraw() {}

	// �����蔻������擾(AABB)
	AABB GetAABBCollision() { return m_AABBCollision; }
	// �����蔻������擾(��)
	BoundingSphere GetSphereCollision() { return m_SphereCollision; }

	// �������Ԃ��Z�b�g
	void SetIsHit(bool _isHit) { m_IsHit = _isHit; }
	// �����蔻���Ԃ��Q�b�g
	bool* GetIsHit(void) { return &m_IsHit; }
	// ���f�������Q�b�g
	AnimationModel* GetModel(void) { return m_Model; }

protected:
	Vector3				m_Velocity{};	// �x���V�e�B
	AnimationModel*		m_Model;		// ���f�����
	float				m_BlendRate;	// ���[�V�����u�����h���[�g
	int					m_Frame;		// �t���[��
	bool				m_IsHit;		// ��������

	// �����蔻����
	AABB			m_AABBCollision{};		// AABB�R���W�������
	BoundingSphere	m_SphereCollision{};	// �ۃR���W�������
};