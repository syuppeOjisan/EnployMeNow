#include "System/MouseInput.h"

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

void MouseInput::GetMousePosition(int& _x, int& _y)
{
	// マウスの現在位置を取得
	POINT point = { 0 };
	GetCursorPos(&point);
	// 引数で指定さたアドレスに返す
	_x = point.x;
	_y = point.y;
}

void MouseInput::GetMouseMove(int& _x, int& _y)
{
	// マウスの移動度合いを割り出して返す
	_x = m_nowMousePosition.x - m_oldMousePosition.x;
	_y = m_nowMousePosition.y - m_oldMousePosition.y;
}
