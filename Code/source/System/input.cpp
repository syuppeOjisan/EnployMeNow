
#include "System/main.h"
#include "System/input.h"

const int Input::KEYCOUNT;

BYTE Input::m_OldKeyState[KEYCOUNT];
BYTE Input::m_KeyState[KEYCOUNT];

UNIVERSAL_INPUT_STATE Input::m_NowInputState;
UNIVERSAL_INPUT_STATE Input::m_OldInputState;

POINT Input::m_WindowCenter;
POINT Input::m_NowMousePos;
POINT Input::m_OldMousePos;
bool  Input::m_isMouseMove;

bool Input::m_CursorTrap;

HWND Input::m_WindowInfo;


void Input::Init(HWND hWnd)
{

	m_WindowInfo = hWnd;

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

	m_NowInputState = { false };
	m_OldInputState = { false };

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

	m_OldMousePos = m_NowMousePos;	// �P�t���[���O�̃}�E�X�ʒu��ۑ�  
	GetCursorPos(&m_NowMousePos);	// ���݂̃}�E�X�ʒu���擾/�ۑ�

	UpdateUnivInput();	// ���j�o�[�T�����͏�Ԃ��X�V

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

void Input::UpdateUnivInput()
{
	// �O��̃t���[���̓��͏�Ԃ�ۑ�
	m_OldInputState = m_NowInputState;

	// ���͏�Ԃ��X�V
	{
		// �O�i
		if (Input::GetKeyPress('W'))
		{
			m_NowInputState.Forward = true;
		}
		else
		{
			m_NowInputState.Forward = false;
		}

		// ��i
		if (Input::GetKeyPress('S'))
		{
			m_NowInputState.BackWord = true;
		}
		else
		{
			m_NowInputState.BackWord = false;
		}

		// ��
		if (Input::GetKeyPress('A'))
		{
			m_NowInputState.Left = true;
		}
		else
		{
			m_NowInputState.Left = false;
		}

		// �E
		if (Input::GetKeyPress('D'))
		{
			m_NowInputState.Right = true;
		}
		else
		{
			m_NowInputState.Right = false;
		}

		// �W�����v
		if (Input::GetKeyPress(VK_SPACE))
		{
			m_NowInputState.Jump = true;
		}
		else
		{
			m_NowInputState.Jump = false;
		}

		// �p���`
		if (Input::GetKeyPress('P'))
		{
			m_NowInputState.Punching = true;
		}
		else
		{
			m_NowInputState.Punching = false;
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

bool Input::GetUniversulPress(UNIVERSAL_INPUT_ID _inputID)
{
	switch (_inputID)
	{
	case FORWARD:
		return m_NowInputState.Forward;
		break;

	case BACKWORD:
		return m_NowInputState.BackWord;
		break;

	case RIGHT:
		return m_NowInputState.Right;
		break;

	case LEFT:
		return m_NowInputState.Left;
		break;

	case JUMP:
		return m_NowInputState.Jump;
		break;

	case PUNCHING:
		return m_NowInputState.Punching;
		break;

	default:
		return false;
		break;
	}
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
	mouseMove.x = m_NowMousePos.x - m_OldMousePos.x;
	mouseMove.y = m_NowMousePos.y - m_OldMousePos.y;

	return mouseMove;
}