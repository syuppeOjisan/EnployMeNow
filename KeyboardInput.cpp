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

	// 1フレーム前のキー状態を転記して保存
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
