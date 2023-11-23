#include "GameObject/Base/CharacterBase.h"

void CharacterBase::Update()
{		
	// ベロシティを加算する
	// * m_Timeをすることでスローモーションとかできる？	
	SetPosition(GetPosition() + (m_Velocity * m_Time));
	m_Velocity = { 0 }; // ベロシティを０に


	// アニメーションさせる
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
	// 指定されたアニメーションがロードされているかをチェック
	if (!m_pModel->CheckIsAnimation(_nextAnimation))
	{
		return false;
	}

	// 変更するアニメーションを設定
	if (m_IsAnimBlendOver && (m_NowAnimation != _nextAnimation))
	{
		m_PrevAnimation = m_NowAnimation;	// 実行していたアニメーションを前回のものに
		m_NowAnimation = _nextAnimation;	// 新しく指定されたアニメーションと前回のものをブレンド
		m_IsAnimBlendOver = false;			// ここからブレンドを開始してほしいのでフラグと変数初期化
		m_BlendRate = 0.0f;

		m_PrevAnimationFrame = m_NowAnimationFrame;
		m_NowAnimationFrame = 0;
	}

	return true;
}

bool CharacterBase::SetNextAnimation(int _animID)
{
	// 指定されたアニメーションがロードされているかをチェック
	if (!m_pModel->CheckIsAnimation(_animID))
	{
		return false;
	}

	// 変更するアニメーションを設定
	if (m_IsAnimBlendOver && (m_NowAnimationID != _animID))
	{
		m_PrevAnimationID = m_NowAnimationID;	// 実行していたアニメーションを前回のものとして保存
		m_NowAnimationID = _animID;				// 新しく指定されたアニメーションと前回のものをブレンド

		m_IsAnimBlendOver = false;				// ここからブレンドを開始してほしいのでフラグと変数初期化
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