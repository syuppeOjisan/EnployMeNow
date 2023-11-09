
#include "System/main.h"
#include "System/input.h"

const int Input::KEYCOUNT;
BYTE Input::m_OldKeyState[KEYCOUNT];
BYTE Input::m_KeyState[KEYCOUNT];

POINT Input::m_WindowCenter;
POINT Input::m_NowMousePos;
POINT Input::m_OldMousePos;
bool  Input::m_isMouseMove;

bool Input::m_CursorTrap;

HWND Input::m_WindowInfo;


void Input::Init(HWND hWnd)
{
	m_CursorTrap = true;

	m_WindowInfo = hWnd;

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

	// �N���C�A���g�E�B���h�E�̃T�C�Y���擾
	RECT rc;
	GetClientRect(m_WindowInfo, &rc);

	// �N���C�A���g�̈���W���X�N���[���̈���W�ɕϊ�
	POINT pt = { rc.left, rc.top };
	POINT pt2 = { rc.right, rc.bottom };
	ClientToScreen(m_WindowInfo, &pt);
	ClientToScreen(m_WindowInfo, &pt2);
	SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);

	
	m_WindowCenter.x = rc.left + (rc.right - rc.left) / 2;
	m_WindowCenter.y = rc.top + (rc.bottom - rc.top) / 2;

}

void Input::Uninit()
{


}

void Input::Update()
{
	memcpy( m_OldKeyState, m_KeyState, 256 );	// 1�t���[���O�̃L�[��Ԃ�]�L���ĕۑ�
	GetKeyboardState( m_KeyState );				// �V�����L�[��Ԃ��擾

	//m_OldMousePos = m_NowMousePos;	// �P�t���[���O�̃}�E�X�ʒu��ۑ�
	//GetCursorPos(&m_NowMousePos);	// ���݂̃}�E�X�ʒu���擾/�ۑ�

	// �J�[�\�����E�B���h�E����o�Ȃ��悤�ɂ���
	if (m_CursorTrap)
	{
		// �N���C�A���g�E�B���h�E�̃T�C�Y���擾
		RECT rc;
		GetClientRect(m_WindowInfo, &rc);

		// �N���C�A���g�̈���W���X�N���[���̈���W�ɕϊ�
		POINT pt = { rc.left, rc.top };
		POINT pt2 = { rc.right, rc.bottom };
		ClientToScreen(m_WindowInfo, &pt);
		ClientToScreen(m_WindowInfo, &pt2);
		SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);

		POINT center;
		center.x = rc.left + (rc.right - rc.left) / 2;
		center.y = rc.top + (rc.bottom - rc.top) / 2;

		//�@�}�E�X�̈ړ��͈͂𐧌�
		if (m_NowMousePos.x > rc.right || m_NowMousePos.x < rc.left ||
			m_NowMousePos.y < rc.top || m_NowMousePos.y > rc.bottom)
		{
			SetCursorPos(m_WindowCenter.x, m_WindowCenter.y);
		}
	}

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::GetKeyReleased(BYTE KeyCode)
{
	return (!(m_KeyState[KeyCode] & 0x80) && (m_OldKeyState[KeyCode] & 0x80));
}

POINT Input::GetNowMoucePos()
{
	return m_NowMousePos;
}

POINT Input::GetNowMoucePosClient()
{
	POINT cliantMousePos = m_NowMousePos;
	ScreenToClient(m_WindowInfo, &cliantMousePos);

	return cliantMousePos;
}

void Input::SetCursorTrap(bool trap)
{
	m_CursorTrap = trap;
}

DirectX::SimpleMath::Vector2 Input::GetMouseMove()
{

	DirectX::SimpleMath::Vector2 mouseMove;
	mouseMove.x = m_NowMousePos.x;
	mouseMove.y = m_NowMousePos.y;

	return mouseMove;
}