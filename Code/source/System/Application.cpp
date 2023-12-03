//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <chrono>
#include <thread>
#include "System/Application.h"
#include "System/fpscontrol.h"
#include "System/renderer.h"
#include "System/manager.h"
#include "System/Input.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
const auto ClassName = TEXT("2023 framework ひな型");     //!< ウィンドウクラス名.
const auto WindowName = TEXT("2023 framework ひな型");    //!< ウィンドウ名.

///////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
Application::Application(uint32_t width, uint32_t height)
: m_hInst   (nullptr)
, m_hWnd    (nullptr)
, m_Width   (width)
, m_Height  (height)
{ /* DO_NOTHING */ }

//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
Application::~Application()
{ /* DO_NOTHING */ }

//-----------------------------------------------------------------------------
//      実行します.
//-----------------------------------------------------------------------------
void Application::Run()
{
    if (InitApp())
    { 
        MainLoop(); 
    }

    TermApp();
}

//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
bool Application::InitApp()
{
    // ウィンドウの初期化.
    if (!InitWnd())
    { 
        return false; 
    }

    // 正常終了.
    return true;
}

//-----------------------------------------------------------------------------
//      終了処理です.
//-----------------------------------------------------------------------------
void Application::TermApp()
{
    // ウィンドウの終了処理.
    TermWnd();
}

//-----------------------------------------------------------------------------
//      ウィンドウの初期化処理です.
//-----------------------------------------------------------------------------
bool Application::InitWnd()
{
    // インスタンスハンドルを取得.
    auto hInst = GetModuleHandle(nullptr);
    if (hInst == nullptr)
    { 
        return false; 
    }

    // ウィンドウの設定.
    WNDCLASSEX wc = {};
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WndProc;
    wc.hIcon            = LoadIcon(hInst, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(hInst, IDC_ARROW);
    wc.hbrBackground    = GetSysColorBrush(COLOR_BACKGROUND);
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = ClassName;
    wc.hIconSm          = LoadIcon(hInst, IDI_APPLICATION);

    // ウィンドウの登録.
    if (!RegisterClassEx(&wc))
    { return false; }

    // インスタンスハンドル設定.
    m_hInst = hInst;

    // ウィンドウのサイズを設定.
    RECT rc = {};
    rc.right  = static_cast<LONG>(m_Width);
    rc.bottom = static_cast<LONG>(m_Height);

    // ウィンドウサイズを調整.
    auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    AdjustWindowRect(&rc, style, FALSE);

    // ウィンドウを生成.
    m_hWnd = CreateWindowEx(
        0,
        ClassName,
        WindowName,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right  - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_hInst,
        nullptr);

    if (m_hWnd == nullptr)
    { return false; }
 
    // ウィンドウを表示.
    ShowWindow(m_hWnd, SW_SHOWNORMAL);

    // ウィンドウを更新.
    UpdateWindow(m_hWnd);

    // ウィンドウにフォーカスを設定.
    SetFocus(m_hWnd);

    // 正常終了.
    return true;
}

//-----------------------------------------------------------------------------
//      ウィンドウの終了処理です.
//-----------------------------------------------------------------------------
void Application::TermWnd()
{
    // ウィンドウの登録を解除.
    if (m_hInst != nullptr)
    { UnregisterClass(ClassName, m_hInst); }

    m_hInst = nullptr;
    m_hWnd  = nullptr;
}

//-----------------------------------------------------------------------------
//      メインループです.
//-----------------------------------------------------------------------------
void Application::MainLoop()
{
    MSG msg = {};

    // FPS調整クラス
    FPS fpsrate(60);

    // ゲームの初期処理
    Manager::Init(this, m_hWnd);


    while(WM_QUIT != msg.message && Manager::GetLoopFlag())
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else 
        {
            uint64_t delta_time = 0;

            // デルタタイムを計算
            delta_time = fpsrate.CalcDelta();
            Manager::Update(delta_time);        // ゲーム更新
            Manager::Draw(delta_time);		    // ゲーム描画

            // 規定時間まで時間調整
            fpsrate.Wait();
        }
    }

    // ゲームの終了処理
    Manager::Uninit();

}


// Forward declare message handler from imgui_impl_win32.cpp
// 前方定義
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

//-----------------------------------------------------------------------------
//      ウィンドウプロシージャです.
//-----------------------------------------------------------------------------
LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wp, lp))
        return true;

    switch(msg)
    {
        case WM_DESTROY:
            { PostQuitMessage(0); }
            break;

        case WM_MOUSEMOVE:
        { Input::SetisMouceMove(true); }
        break;

        default:
        { Input::SetisMouceMove(false); }
            break;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}
