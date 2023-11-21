#pragma once

#include <SimpleMath.h>

/// <summary>
/// ���͏��Ǘ��p�C���^�[�t�F�[�X
/// </summary>
class InputIntarface
{
public:
	virtual ~InputIntarface() {};	// ���z�f�X�g���N�^

	virtual bool Init(void) = 0;
	virtual void Update(void) = 0;
	
	/// <summary>
	/// ������Ԏ擾
	/// </summary>
	/// <param name="_key">�L�[�R�[�h</param>
	/// <returns> �������</returns>
	virtual bool GetPressed(int _key) = 0;

	/// <summary>
	/// ������Ԏ擾 - �g���K�[
	/// </summary>
	/// <param name="_key">�L�[�R�[�h</param>
	/// <returns>�@�������</returns>
	virtual bool GetTregger(int _key) = 0;

	/// <summary>
	/// �f�o�C�X�̈ړ��x�������擾
	/// </summary>
	/// <param name="_leftStickOrMousemove">���X�e�B�b�N�X�� OR �}�E�X�ړ�����</param>
	/// <param name="_rightStick">�E�X�e�B�b�N�X��</param>
	virtual void GetDeviceMovement(DirectX::SimpleMath::Vector2& _leftStickOrMousemove, DirectX::SimpleMath::Vector2& _rightStick) = 0;
};

// ���ʃN���X�͂����̃��\�b�h���ׂĂ��g�p����킯�ł͂Ȃ��̂�SOLID�����I�ɔ���