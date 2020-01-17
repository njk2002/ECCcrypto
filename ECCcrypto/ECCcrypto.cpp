// ECCcrypto.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "ECCcrypto.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <sodium.h>
#include "CEclass.h"
#include <fstream>

#pragma comment(lib,"libsodium.lib")
#pragma once
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Enc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Dec(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Test(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ECCCRYPTO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ECCCRYPTO));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ECCCRYPTO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ECCCRYPTO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_32771:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ENC), hWnd, Enc);
                break;
            case ID_32772:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DEC), hWnd, Dec);
                break;
            case ID_32773:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_TEST), hWnd, Test);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
// “加密”框的消息处理程序。
INT_PTR CALLBACK Enc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1 || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON2)
        {    //genkey
            unsigned char key[crypto_secretbox_KEYBYTES];
            crypto_secretbox_keygen(key);

            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
// “解密”框的消息处理程序。
INT_PTR CALLBACK Dec(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1 || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON2)
        {
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
// “测试”框的消息处理程序。
INT_PTR CALLBACK Test(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1 || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON2)
        {
            /*
            
            char* text1 = bin2base64(key,sizeof key);
            wchar_t* text = CharToWchar_t(text1);
            SetDlgItemText(hDlg, IDC_EDIT2, text);
            int b = strlen(text1);
            unsigned char nonce[crypto_secretbox_NONCEBYTES];
            unsigned char* bin = base642bin(text1);
#define MESSAGE ((const unsigned char *) "tessdfsadssat")
#define MESSAGE_LEN 13
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)
            unsigned char ciphertext[CIPHERTEXT_LEN];
            unsigned char ciphertext1[CIPHERTEXT_LEN];
            randombytes_buf(nonce, sizeof nonce);
            crypto_secretbox_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce, key);
            crypto_secretbox_easy(ciphertext1, MESSAGE, MESSAGE_LEN, nonce, bin);
            unsigned char decrypted[MESSAGE_LEN];
            unsigned char decrypted1[MESSAGE_LEN];
            crypto_secretbox_open_easy(decrypted, ciphertext, CIPHERTEXT_LEN, nonce, key);
            crypto_secretbox_open_easy(decrypted1, ciphertext1, CIPHERTEXT_LEN, nonce, bin);
            int a = sodium_memcmp(decrypted1, decrypted1, MESSAGE_LEN);
            */
            unsigned char key[crypto_secretbox_KEYBYTES];
            crypto_secretbox_keygen(key);
            wchar_t text[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text, 2048);
            char* text1 = intergratedcrypto(text,key);

            //wchar_t* display = CharToWchar_t((char*)decrypted1);
            
            //SetDlgItemText(hDlg, IDC_EDIT1, display);
            //
            
            
            
            
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
