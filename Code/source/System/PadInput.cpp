#include "System/PadInput.h"

bool PadInput::Init(void)
{
	// �����o�ϐ�������
	m_NowPadState = {};
	m_OldPadState = {};
	return true;
}

void PadInput::Update(void)
{
	// 1�t���[���O�̓��͏�Ԃ�]�L���ĕۑ�
	m_OldPadState = m_NowPadState;

	// �ŐV�̓��͏�Ԃ��擾
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
	// �X�e�B�b�N�����擾���A-1.0����1.0�͈̔͂Ɏ��߂�
	DirectX::SimpleMath::Vector2 leftTmp = {};
	leftTmp.x = static_cast<float>(m_NowPadState.sThumbLX / SHORT_RANGE);
	leftTmp.y = static_cast<float>(m_NowPadState.sThumbLY / SHORT_RANGE);

	DirectX::SimpleMath::Vector2 rightTmp = {};
	rightTmp.x = static_cast<float>(m_NowPadState.sThumbRX / SHORT_RANGE);
	rightTmp.y = static_cast<float>(m_NowPadState.sThumbRY / SHORT_RANGE);

	// �X�e�B�b�N����]�L
	_leftStick = leftTmp;
	_rightStick = rightTmp;
}