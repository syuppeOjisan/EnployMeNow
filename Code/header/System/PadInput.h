#pragma once
#include <Windows.h>
#include <Xinput.h>
#pragma comment (lib, "xinput.lib")

#include "InputIntarface.h"

/// <summary>
/// �Q�[���p�b�h���͊Ǘ��N���X
/// </summary>
class PadInput : public InputIntarface
{
public:
	bool Init(void) override;
	void Update(void) override;

	/// <summary>
	/// ������Ԏ擾
	/// </summary>
	/// <param name="_key">�L�[�R�[�h</param>
	/// <returns>�������</returns>
	bool GetPressed(int _key) override;

	/// <summary>
	/// ������Ԏ擾 - �g���K�[
	/// </summary>
	/// <param name="_key">�L�[�R�[�h</param>
	/// <returns>�������</returns>
	bool GetTregger(int _key) override;

	/// <summary>
	/// �X�e�B�b�N�̓|����x
	/// </summary>
	/// <param name="_leftStick">�X�e�B�b�NX��</param>
	/// <param name="_rightStick">�X�e�B�b�NY��</param>
	void GetDeviceMovement(DirectX::SimpleMath::Vector2& _leftStick, DirectX::SimpleMath::Vector2& _rightStick) override;

private:
	XINPUT_GAMEPAD	m_NowPadState;		// �R���g���[���[���͏�� - �ŐV
	XINPUT_GAMEPAD	m_OldPadState;		// �R���g���[���[���͏�� - �P�t���[���O

	bool			m_isVib;			// �o�C�u���[�V���������ǂ���
	int				m_VibCount;			// �o�C�u���[�V�����̌o�ߎ���
	int				m_VibCountLimit;	// �o�C�u���[�V��������

	// SHORT�^�̍ő�l
	const float SHORT_RANGE = 32768.0f;
};

