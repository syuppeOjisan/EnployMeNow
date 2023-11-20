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
	/// �}�E�X���W�擾
	/// </summary>
	/// <param name="_x">�}�E�XX���W</param>
	/// <param name="_y">�}�E�XY���W</param>
	void GetMousePosition(int& _x, int& _y) override;

	/// <summary>
	/// �}�E�X�̈ړ��������擾
	/// </summary>
	/// <param name="_x">�}�E�XX�ړ�����</param>
	/// <param name="_y">�}�E�XY�ړ�����</param>
	void GetMouseMove(int& _x, int& _y) override;

	// ���̃��\�b�h�͂��̃N���X�ł͎g�p���Ȃ�
	void GetPadStick(DirectX::SimpleMath::Vector2& _left, DirectX::SimpleMath::Vector2& _right) override {};

private:
	POINT m_nowMousePosition;	// �}�E�X�ʒu - �ŐV
	POINT m_oldMousePosition;	// �}�E�X�ʒu - �P�t���[���O
};