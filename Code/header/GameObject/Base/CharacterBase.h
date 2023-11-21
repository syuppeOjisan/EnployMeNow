#pragma once

#include "gameObject.h"
#include "System/collision.h"

#include "Component/animationModel.h"

using namespace DirectX::SimpleMath;

// �L�����N�^�[�i�G�����֌W�Ȃ��j�̊��N���X�ł�
class CharacterBase : public GameObject
{
public:
	virtual void Init() override { m_pModel = nullptr; m_Time = 1.0f; }
	virtual void Uninit() override {}
	virtual void Update() override;
	virtual void Draw() override {}
	virtual void PreDraw() override {}

	// �x���V�e�B���Z�b�g
	void SetVelocity(Vector3 _velocity) { m_Velocity = _velocity; }
	void SetVelocity(float _x, float _y, float _z) { m_Velocity.x = _x; m_Velocity.y = _y; m_Velocity.z = _z;}

	Vector3 GetVelocity(void) { return m_Velocity; }

	// �����蔻������擾(AABB)
	AABB GetAABBCollision() { return m_AABBCollision; }
	// �����蔻������擾(��)
	BoundingSphere GetSphereCollision() { return m_SphereCollision; }

	// �������Ԃ��Z�b�g
	void SetIsHit(bool _isHit) { m_IsHit = _isHit; }
	// �����蔻���Ԃ��Q�b�g
	bool* GetIsHit(void) { return &m_IsHit; }

	// ���f�������Q�b�g
	AnimationModel* GetModel(void) { return m_pModel; }

	// ���ԊǗ��ϐ����Z�b�g
	void SetTime(float _time) { m_Time = _time; }
	// ���ԊǗ��ϐ����Q�b�g
	float GetTime(void) { return m_Time; }

	// ���[�V�����ύX - ������w��
	bool SetNextAnimation(const char* _nextAnimation);
	// ���[�V�����ύX - ID�w��
	bool SetNextAnimation(int _animID);

	// ���݂̃��[�V�����擾 - ������
	const char* GetNowAnimName(void) { return m_NowAnimation; }
	// �O��̃��[�V�������擾 - ������
	const char* GetPrevAnimName(void) { return m_PrevAnimation; }

	// ���݂̃��[�V�����擾 - ID
	int GetNextAnimationID(void) { return m_NowAnimationID; }
	// �O��̃��[�V�����擾 - ID
	int GetPrevAnimationID(void) { return m_PrevAnimationID; }

	// ���݂̃��[�V�����t���[�����擾
	int GetNowAnimationFrame(void) { return m_NowAnimationFrame; }
	// �O��̃��[�V�����t���[�����擾
	int GetPrevAnimationFrame(void) { return m_PrevAnimationFrame; }

	// �A�j���[�V�����u�����h���I��������ǂ������擾
	bool* GetIsAnimBlendOver(void) { return &m_IsAnimBlendOver; }

	/// <summary>
	/// �q�b�g�X�g�b�v�̌v�Z���s���܂�
	/// </summary>
	/// <param name="_startFrame"> �q�b�g�X�g�b�v���J�n����t���[��</param>
	/// <param name="_endFrame"> �q�b�g�X�g�b�v���I������t���[��</param>
	/// <param name="difference"> �q�b�g�X�g�b�v���̑��x��</param>
	void CalcHitStop(float _startFrame, float _endFrame, float difference);

protected:
	Vector3				m_Velocity{};				// �x���V�e�B

	AnimationModel*		m_pModel;					// ���f�����
	float				m_Time;						// ���ԊǗ��ϐ�
	float				m_BlendRate;				// ���[�V�����u�����h���[�g
	float				m_NowAnimationFrame = 0;	// �Đ����̃A�j���[�V�����t���[��
	float				m_PrevAnimationFrame = 0;	// �O�Đ����Ă����A�j���[�V�����t���[��
	float				m_NowAnimationSpeed = 1.0f;	// �Đ����̃A�j���[�V�������x
	float				m_PreAnimationSpeed = 1.0f;	// �O�Đ����Ă����A�j���[�V�������x
	const char*			m_NowAnimation;				// ���̃��[�V������
	const char*			m_PrevAnimation;			// �O��̃��[�V������
	int					m_NowAnimationID;			// ���̃��[�V����ID
	int					m_PrevAnimationID;			// �O��̃��[�V����ID

	bool				m_IsHit;					// ��������
	bool				m_IsAnimBlendOver;			// �A�j���[�V�����u�����h���I��������ǂ���


	// �����蔻����
	AABB			m_AABBCollision{};		// AABB�R���W�������
	BoundingSphere	m_SphereCollision{};	// �ۃR���W�������
};