#include "bacon.h"

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

WSADATA wsaData;
SOCKET winSock;
HWND hWnd;
WNDCLASSEX wc;

struct sockaddr_in sockAddr;

// Change these
int port = 4444;
char *ip = "127.0.0.1";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int testing(void) {
    WSAStartup(MAKEWORD(2,2), &wsaData);
    winSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = inet_addr(ip);

    WSAConnect(winSock, (SOCKADDR*)&sockAddr, sizeof(sockAddr), NULL, NULL, NULL, NULL);

    // Create a hidden window
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "HiddenWindow";
    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(0, "HiddenWindow", NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);

    if (hWnd == NULL) {
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up
    closesocket(winSock);
    WSACleanup();
   
    return 0;
}
