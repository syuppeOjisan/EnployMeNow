#include "System/MouseInput.h"

bool MouseInput::Init(void)
{
	//�@�����o�ϐ�������
	m_nowMousePosition = { 0 };
	m_oldMousePosition = { 0 };
	return true;
} 

void MouseInput::Update(void)
{
	// ���݂̃}�E�X�ʒu��ۑ�
	m_oldMousePosition = m_nowMousePosition;

	// ���݂̃}�E�X�ʒu�擾
	GetCursorPos(&m_nowMousePosition);
}

bool MouseInput::GetPressed(int _key)
{
	return false;
}

bool MouseInput::GetTregger(int _key)
{
	return false;
}

void MouseInput::GetMousePosition(int& _x, int& _y)
{
	// �}�E�X�̌��݈ʒu���擾
	POINT point = { 0 };
	GetCursorPos(&point);
	// �����Ŏw�肳���A�h���X�ɕԂ�
	_x = point.x;
	_y = point.y;
}

void MouseInput::GetMouseMove(int& _x, int& _y)
{
	// �}�E�X�̈ړ��x����������o���ĕԂ�
	_x = m_nowMousePosition.x - m_oldMousePosition.x;
	_y = m_nowMousePosition.y - m_oldMousePosition.y;
}
