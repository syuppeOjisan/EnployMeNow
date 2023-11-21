#pragma once
#include <Windows.h>
#include "InputIntarface.h"

/// <summary>
/// �}�E�X���͊Ǘ��N���X
/// </summary>
class MouseInput : public InputIntarface
{
public:
	bool Init(void) override;
	void Update(void) override;

	/// <summary>
	/// ������Ԏ擾
	/// </summary>
	/// <param name="_key">�L�[�R�[�h</param>
	/// <returns> �������</returns>
	bool GetPressed(int _key) override;

	/// <summary>
	/// ������Ԏ擾 - �g���K�[
	/// </summary>
	/// <param name="_key">�L�[�R�[�h</param>
	/// <returns>�@�������</returns>
	bool GetTregger(int _key) override;

	/// <summary>
	/// �}�E�X�̈ړ��������擾
	/// </summary>
	/// <param name="_mouseMove">�}�E�X���W</param>
	/// <param name="_mousePosition">�}�E�X�ړ�����</param>
	void GetDeviceMovement(DirectX::SimpleMath::Vector2& _mouseMove, DirectX::SimpleMath::Vector2& _mousePosition) override;

private:
	POINT m_nowMousePosition;	// �}�E�X�ʒu - �ŐV
	POINT m_oldMousePosition;	// �}�E�X�ʒu - �P�t���[���O
};