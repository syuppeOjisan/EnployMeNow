#include "System/PadInput.h"

bool PadInput::Init(void)
{
	// メンバ変数初期化
	m_NowPadState = {};
	m_OldPadState = {};
	return true;
}

void PadInput::Update(void)
{
	// 1フレーム前の入力状態を転記して保存
	m_OldPadState = m_NowPadState;

	// 最新の入力状態を取得
	XINPUT_STATE inputStateTmp = {};
	XInputGetState(0, &inputStateTmp);
	m_NowPadState = inputStateTmp.Gamepad;
}

bool PadInput::GetPressed(int _key)
{
	return (m_NowPadState.wButtons & _key);
}

bool PadInput::GetTregger(int _key)
{
	return ((m_NowPadState.wButtons & _key) && (!(m_OldPadState.wButtons & _key)));
}

void PadInput::GetDeviceMovement(DirectX::SimpleMath::Vector2& _leftStick, DirectX::SimpleMath::Vector2& _rightStick)
{
	// スティック情報を取得し、-1.0から1.0の範囲に収める
	DirectX::SimpleMath::Vector2 leftTmp = {};
	leftTmp.x = static_cast<float>(m_NowPadState.sThumbLX / SHORT_RANGE);
	leftTmp.y = static_cast<float>(m_NowPadState.sThumbLY / SHORT_RANGE);

	DirectX::SimpleMath::Vector2 rightTmp = {};
	rightTmp.x = static_cast<float>(m_NowPadState.sThumbRX / SHORT_RANGE);
	rightTmp.y = static_cast<float>(m_NowPadState.sThumbRY / SHORT_RANGE);

	// スティック情報を転記
	_leftStick = leftTmp;
	_rightStick = rightTmp;
}