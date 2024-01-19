// Main.cpp

#include "framework.h"
#include "resource.h"
#include <Windows.h>
#include "App.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance); // Mark hPrevInstance as unreferenced
    UNREFERENCED_PARAMETER(lpCmdLine);     // Mark lpCmdLine as unreferenced

    App myApp(hInstance, nCmdShow);
    return myApp.Run();
}