#include    "System/main.h"
#include    "System/Application.h"
#include    <assimp/version.h>
#include    <iostream>

int wmain(int argc, wchar_t** argv, wchar_t** evnp)
{

#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//defined(DEBUG) || defined(_DEBUG)

    std::cout << "assimp version ( major,minor,reveision) "
        << aiGetVersionMajor() << ","
        << aiGetVersionMinor() << ","
        << aiGetVersionRevision() << std::endl;

    // �A�v���P�[�V�������s
    Application app(SCREEN_WIDTH, SCREEN_HEIGHT);
    app.Run();

    return 0;
}