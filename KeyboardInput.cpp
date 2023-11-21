#include "System/KeyboardInput.h"

bool KeyboardInput::Init(void)
{
	// メンバ変数を初期化
	memset(m_nowKeyState, 0, KEY_COUNT);
	memset(m_oldKeyState, 0, KEY_COUNT);
	return true;
}

void KeyboardInput::Update(void)
{

	// 1フレーム前の入力状態を転記して保存
	memcpy(m_oldKeyState, m_nowKeyState, KEY_COUNT);
	
	// キー状態を更新
	for (int i = 0; i < KEY_COUNT; i++)
	{
		if (GetAsyncKeyState(i) & 0x8000) 
		{
			m_nowKeyState[i] = true;
		}
		else
		{
			m_nowKeyState[i] = false;
		}
	}
}

bool KeyboardInput::GetPressed(int _key)
{
	// 引数で指定されたキーの押下状態を返す
	return m_nowKeyState[_key];
}

bool KeyboardInput::GetTregger(int _key)
{
	// 引数で指定されたキーのトリガー押下状態を返す
	// 前回フレームで押されていたら押下判定にはならない
	return ((m_nowKeyState[_key] && !m_oldKeyState[_key]));
}

void KeyboardInput::GetDeviceMovement(DirectX::SimpleMath::Vector2& _wasdMove, DirectX::SimpleMath::Vector2& _arrowMove)
{
	// WASD移動
	{
		if (GetPressed('W'))
		{
			_wasdMove.y = 1.0f;
		}
		else if (GetPressed('S'))
		{
			_wasdMove.y = -1.0f;
		}
		else
		{
			_wasdMove.y = 0;
		}

		if (GetPressed('D'))
		{
			_wasdMove.x = 1.0f;
		}
		else if (GetPressed('A'))
		{
			_wasdMove.x = -1.0f;
		}
		else
		{
			_wasdMove.x = 0;
		}
	}

	// 矢印キー移動
	{
		if (GetPressed(VK_UP))
		{
			_arrowMove.y = 1.0f;
		}
		else if (GetPressed(VK_DOWN))
		{
			_arrowMove.y = -1.0f;
		}
		else
		{
			_arrowMove.y = 0.0f;
		}

		if (GetPressed(VK_RIGHT))
		{
			_arrowMove.x = 1.0f;
		}
		else if (GetPressed(VK_LEFT))
		{
			_arrowMove.x -= 1.0f;
		}
		else
		{
			_arrowMove.x = 0.0f;
		}
	}

}
