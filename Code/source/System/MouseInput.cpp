#include "System/MouseInput.h"

#include <iostream>

bool MouseInput::Init(void)
{
	//　メンバ変数初期化
	m_nowMousePosition = { 0 };
	m_oldMousePosition = { 0 };
	return true;
} 

void MouseInput::Update(void)
{
	// 現在のマウス位置を保存
	m_oldMousePosition = m_nowMousePosition;

	// 現在のマウス位置取得
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
	// マウスの移動度合いを割り出して返す
	_mouseMove.x = m_nowMousePosition.x - m_oldMousePosition.x;
	_mouseMove.y = m_nowMousePosition.y - m_oldMousePosition.y;
	_mouseMove.Normalize();

	std::cout << "_mouseMove:" << std::endl;
	std::cout << "X:" << _mouseMove.x << std::endl;
	std::cout << "Y:" << _mouseMove.y << std::endl;

	// マウスの位置を返す
	_mousePosition.x = m_nowMousePosition.x;
	_mousePosition.y = m_nowMousePosition.y;
}