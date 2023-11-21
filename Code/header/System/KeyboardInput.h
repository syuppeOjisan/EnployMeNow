#pragma once
#include <Windows.h>
#include "InputIntarface.h"

/// <summary>
/// �L�[�{�[�h���͊Ǘ��N���X
/// </summary>
class KeyboardInput : public InputIntarface
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
	/// �ړ��L�[���͒l
	/// </summary>
	/// <param name="_leftStick">WASD�L�[���͒l</param>
	/// <param name="_rightStick">���L�[���͒l</param>
	void GetDeviceMovement(DirectX::SimpleMath::Vector2& _wasdMove, DirectX::SimpleMath::Vector2& _arrowMove) override;

private:
	// �L�[�̐����`
	static const int KEY_COUNT = 256;

	bool m_nowKeyState[KEY_COUNT];	// ���݂̃L�[���͏��
	bool m_oldKeyState[KEY_COUNT];	// �P�t���[���O�̓��͏��
};

