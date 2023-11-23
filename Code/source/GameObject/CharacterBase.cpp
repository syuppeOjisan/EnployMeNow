#include "GameObject/Base/CharacterBase.h"

void CharacterBase::Update()
{		
	// �x���V�e�B�����Z����
	// * m_Time�����邱�ƂŃX���[���[�V�����Ƃ��ł���H	
	SetPosition(GetPosition() + (m_Velocity * m_Time));
	m_Velocity = { 0 }; // �x���V�e�B���O��


	// �A�j���[�V����������
	if (!m_IsAnimBlendOver)
	{
		m_BlendRate += 0.1f * m_Time;
	}
	m_NowAnimationFrame += m_NowAnimationSpeed * m_Time;
	m_PrevAnimationFrame += m_PrevAnimationFrame * m_Time;

	if (m_BlendRate > 1.0f)
	{
		m_IsAnimBlendOver = true;
		m_BlendRate = 1.0f;
	}
}

bool CharacterBase::SetNextAnimation(const char* _nextAnimation)
{
	// �w�肳�ꂽ�A�j���[�V���������[�h����Ă��邩���`�F�b�N
	if (!m_pModel->CheckIsAnimation(_nextAnimation))
	{
		return false;
	}

	// �ύX����A�j���[�V������ݒ�
	if (m_IsAnimBlendOver && (m_NowAnimation != _nextAnimation))
	{
		m_PrevAnimation = m_NowAnimation;	// ���s���Ă����A�j���[�V������O��̂��̂�
		m_NowAnimation = _nextAnimation;	// �V�����w�肳�ꂽ�A�j���[�V�����ƑO��̂��̂��u�����h
		m_IsAnimBlendOver = false;			// ��������u�����h���J�n���Ăق����̂Ńt���O�ƕϐ�������
		m_BlendRate = 0.0f;

		m_PrevAnimationFrame = m_NowAnimationFrame;
		m_NowAnimationFrame = 0;
	}

	return true;
}

bool CharacterBase::SetNextAnimation(int _animID)
{
	// �w�肳�ꂽ�A�j���[�V���������[�h����Ă��邩���`�F�b�N
	if (!m_pModel->CheckIsAnimation(_animID))
	{
		return false;
	}

	// �ύX����A�j���[�V������ݒ�
	if (m_IsAnimBlendOver && (m_NowAnimationID != _animID))
	{
		m_PrevAnimationID = m_NowAnimationID;	// ���s���Ă����A�j���[�V������O��̂��̂Ƃ��ĕۑ�
		m_NowAnimationID = _animID;				// �V�����w�肳�ꂽ�A�j���[�V�����ƑO��̂��̂��u�����h

		m_IsAnimBlendOver = false;				// ��������u�����h���J�n���Ăق����̂Ńt���O�ƕϐ�������
		m_BlendRate = 0.0f;
		m_PrevAnimationFrame = m_NowAnimationFrame;
		m_NowAnimationFrame = 0;

		m_PreAnimationSpeed = m_NowAnimationSpeed;
	}

	return true;
}


void CharacterBase::CalcHitStop(float _startFrame, float _endFrame, float difference)
{
	
	if (m_NowAnimationFrame < _startFrame)
	{
		// do nothing
	}
	else if (m_NowAnimationFrame > _startFrame && m_NowAnimationFrame < _endFrame)
	{
		m_Time = difference;
	}
	else
	{
		m_Time = 1.0f;
	}

}