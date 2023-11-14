#include "System/KeyboardInput.h"

bool KeyboardInput::Init(void)
{
	// �����o�ϐ���������
	memset(m_nowKeyState, 0, KEY_COUNT);
	memset(m_oldKeyState, 0, KEY_COUNT);
	return true;
}

void KeyboardInput::Update(void)
{

	// 1�t���[���O�̃L�[��Ԃ�]�L���ĕۑ�
	memcpy(m_oldKeyState, m_nowKeyState, KEY_COUNT);
	
	// �L�[��Ԃ��X�V
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
	// �����Ŏw�肳�ꂽ�L�[�̉�����Ԃ�Ԃ�
	return m_nowKeyState[_key];
}

bool KeyboardInput::GetTregger(int _key)
{
	// �����Ŏw�肳�ꂽ�L�[�̃g���K�[������Ԃ�Ԃ�
	// �O��t���[���ŉ�����Ă����牟������ɂ͂Ȃ�Ȃ�
	return ((m_nowKeyState[_key] && !m_oldKeyState[_key]));
}
