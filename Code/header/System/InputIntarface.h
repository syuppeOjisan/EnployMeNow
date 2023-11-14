#pragma once

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
	/// �}�E�X���W�擾
	/// </summary>
	/// <param name="_x">�}�E�XX���W</param>
	/// <param name="_y">�}�E�XY���W</param>
	virtual void GetMousePosition(int& _x, int& _y) = 0;

	/// <summary>
	/// �}�E�X�̈ړ��������擾
	/// </summary>
	/// <param name="_x">�}�E�XX�ړ�����</param>
	/// <param name="_y">�}�E�XY�ړ�����</param>
	virtual void GetMouseMove(int& _x, int& _y) = 0;

	/// <summary>
	/// �R���g���[���[�X�e�B�b�N�X���擾
	/// </summary>
	/// <param name="_x">�X��X��</param>
	/// <param name="_y">�X��Y��</param>
	virtual void GetPadStick(float& _x, float& _y) = 0;

};

// �����̃��\�b�h�͉��ʃN���X�Ɉˑ����Ă���ASOLID�����I�ɗǂ��Ȃ��B���ǎd���Ȃ����ȁH