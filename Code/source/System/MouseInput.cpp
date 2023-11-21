#include "System/MouseInput.h"

#include <iostream>

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

void MouseInput::GetDeviceMovement(DirectX::SimpleMath::Vector2& _mousePosition, DirectX::SimpleMath::Vector2& _mouseMove)
{
	// �}�E�X�̈ړ��x����������o���ĕԂ�
	_mouseMove.x = m_nowMousePosition.x - m_oldMousePosition.x;
	_mouseMove.y = m_nowMousePosition.y - m_oldMousePosition.y;
	_mouseMove.Normalize();

	std::cout << "_mouseMove:" << std::endl;
	std::cout << "X:" << _mouseMove.x << std::endl;
	std::cout << "Y:" << _mouseMove.y << std::endl;

	// �}�E�X�̈ʒu��Ԃ�
	_mousePosition.x = m_nowMousePosition.x;
	_mousePosition.y = m_nowMousePosition.y;
}