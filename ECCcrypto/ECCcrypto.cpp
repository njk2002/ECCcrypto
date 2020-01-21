// ECCcrypto.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "ECCcrypto.h"
#include <iostream>
#include <windows.h>
#include <string>
#include "sodium.h"
#include "CEclass.h"
#include <fstream>
#include <time.h>
#include "zxcvbn\zxcvbn.h"

#pragma comment(lib,"libsodium.lib")
//这是为了将来调用那个dll
//下面主要是自动生成的内容
//注意看第26-32行，160行左右，260行往后
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
INT_PTR CALLBACK    SKTextDEnc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PKTextDEnc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PKFileDEnc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SKFileDEnc(HWND, UINT, WPARAM, LPARAM);
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
            //在此添加你在菜单中加入的选项的消息处理程序
            //照葫芦画瓢即可，注意(IDD_ENC)和Enc的位置要自定义
            case ID_32771:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SKTEXTENC), hWnd, SKTextDEnc);
                break;
            case ID_32773:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_TEST), hWnd, Test);
                break;
            case ID_32774:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_PKTEXTENC), hWnd, PKTextDEnc);
                break;
            case ID_32776:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SKFILEENC), hWnd, SKFileDEnc);
                break;
            case ID_32777:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_PKFILEENC), hWnd, PKFileDEnc);
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
// “Chacha20”框的消息处理程序。
INT_PTR CALLBACK SKTextDEnc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
            char* b64 = bin2base64(key, crypto_secretbox_KEYBYTES);
            wchar_t* display = chartowchar(b64);
            SetDlgItemText(hDlg, IDC_EDIT1, display);
            delete[] b64;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            clock_t start2 = clock();
            wchar_t key1[45];
            GetDlgItemText(hDlg, IDC_EDIT1, key1, 45);

            char* key2 = wchartochar(key1);
            unsigned char* key = base642bin(key2);
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t* text = new wchar_t[204800];
            GetDlgItemText(hDlg, IDC_EDIT2, text, 204800);
            //从较大的文本框中获取文字，给了200kb的缓冲区
            char* text3 = itgcrypto(text, key);
            //调用自定义的集成加密方法
            wchar_t* display = chartowchar((char*)text3);
            SetDlgItemText(hDlg, IDC_EDIT2, display);
            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] key2;
            delete[] key;
            delete[] text;
            delete[] text3;
            delete[] display;
            clock_t start = clock();
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON4)
        {
            clock_t start2 = clock();
            wchar_t key1[45];
            GetDlgItemText(hDlg, IDC_EDIT1, key1, 45);

            char* key2 = wchartochar(key1);
            unsigned char* key = base642bin(key2);
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t* text = new wchar_t[317200+16+24+24];
            GetDlgItemText(hDlg, IDC_EDIT2, text, 317200 + 16 + 24 + 24);
            unsigned char* text4 = itgdecrypto(text, key);
            size_t len = sLEN(text4,24);
            //获得原文的长度
            unsigned char* text5 = new unsigned char[len + 1];
            text5[len] = '\0';
            memcpy_s(text5, len, text4 + 24, len);
            //把解密结果不属于原文的部分噶掉
            wchar_t* display = chartowchar((char*)text5);
            SetDlgItemText(hDlg, IDC_EDIT2, display);
            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] key2;
            delete[] key;
            delete[] text;
            delete[] text4;
            delete[] text5;
            delete[] display;
            clock_t start = clock();
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



INT_PTR CALLBACK    PKTextDEnc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PKFileDEnc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SKFileDEnc(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK PKTextDEnc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
            char* b64 = bin2base64(key, crypto_secretbox_KEYBYTES);
            wchar_t* display = chartowchar(b64);
            SetDlgItemText(hDlg, IDC_EDIT1, display);
            delete[] b64;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            wchar_t key1[45];
            GetDlgItemText(hDlg, IDC_EDIT1, key1, 45);

            char* key2 = wchartochar(key1);
            unsigned char* key = base642bin(key2);
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t text[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text, 2048);
            //从较大的文本框中获取文字，给了2kb的缓冲区
            char* text3 = itgcrypto(text, key);
            //调用自定义的集成加密方法
            wchar_t* display = chartowchar((char*)text3);
            SetDlgItemText(hDlg, IDC_EDIT2, display);
            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] key2;
            delete[] key;
            delete[] text3;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON4)
        {
            wchar_t key1[45];
            GetDlgItemText(hDlg, IDC_EDIT1, key1, 45);

            char* key2 = wchartochar(key1);
            unsigned char* key = base642bin(key2);
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t text[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text, 2048);
            unsigned char* text4 = itgdecrypto(text, key);
            size_t len = sLEN(text4,24);
            //获得原文的长度
            unsigned char* text5 = new unsigned char[len + 1];
            text5[len] = '\0';
            memcpy_s(text5, len, text4 + 24, len);
            //把解密结果不属于原文的部分噶掉
            wchar_t* display = chartowchar((char*)text5);
            SetDlgItemText(hDlg, IDC_EDIT2, display);
            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] key2;
            delete[] key;
            delete[] text4;
            delete[] text5;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK PKFileDEnc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
            char* b64 = bin2base64(key, crypto_secretbox_KEYBYTES);
            wchar_t* display = chartowchar(b64);
            SetDlgItemText(hDlg, IDC_EDIT1, display);
            delete[] b64;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            wchar_t key1[45];
            GetDlgItemText(hDlg, IDC_EDIT1, key1, 45);

            char* key2 = wchartochar(key1);
            unsigned char* key = base642bin(key2);
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t text[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text, 2048);
            //从较大的文本框中获取文字，给了2kb的缓冲区
            char* text3 = itgcrypto(text, key);
            //调用自定义的集成加密方法
            wchar_t* display = chartowchar((char*)text3);
            SetDlgItemText(hDlg, IDC_EDIT2, display);
            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] key2;
            delete[] key;
            delete[] text3;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON4)
        {
            wchar_t key1[45];
            GetDlgItemText(hDlg, IDC_EDIT1, key1, 45);

            char* key2 = wchartochar(key1);
            unsigned char* key = base642bin(key2);
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t text[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text, 2048);
            unsigned char* text4 = itgdecrypto(text, key);
            size_t len = sLEN(text4,24);
            //获得原文的长度
            unsigned char* text5 = new unsigned char[len + 1];
            text5[len] = '\0';
            memcpy_s(text5, len, text4 + 24, len);
            //把解密结果不属于原文的部分噶掉
            wchar_t* display = chartowchar((char*)text5);
            SetDlgItemText(hDlg, IDC_EDIT2, display);
            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] key2;
            delete[] key;
            delete[] text4;
            delete[] text5;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK SKFileDEnc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
            char* b64 = bin2base64(key, crypto_secretbox_KEYBYTES);
            wchar_t* display = chartowchar(b64);
            SetDlgItemText(hDlg, IDC_EDIT1, display);
            delete[] b64;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            wchar_t key1[45];
            GetDlgItemText(hDlg, IDC_EDIT1, key1, 45);

            char* key2 = wchartochar(key1);
            unsigned char* key = base642bin(key2);
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t text[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text, 2048);
            //从较大的文本框中获取文字，给了2kb的缓冲区
            char* text3 = itgcrypto(text, key);
            //调用自定义的集成加密方法
            wchar_t* display = chartowchar((char*)text3);
            SetDlgItemText(hDlg, IDC_EDIT2, display);
            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] key2;
            delete[] key;
            delete[] text3;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON4)
        {
            wchar_t key1[45];
            GetDlgItemText(hDlg, IDC_EDIT1, key1, 45);

            char* key2 = wchartochar(key1);
            unsigned char* key = base642bin(key2);
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t text[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text, 2048);
            unsigned char* text4 = itgdecrypto(text, key);
            size_t len = sLEN(text4,24);
            //获得原文的长度
            unsigned char* text5 = new unsigned char[len + 1];
            text5[len] = '\0';
            memcpy_s(text5, len, text4 + 24, len);
            //把解密结果不属于原文的部分噶掉
            wchar_t* display = chartowchar((char*)text5);
            SetDlgItemText(hDlg, IDC_EDIT2, display);
            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] key2;
            delete[] key;
            delete[] text4;
            delete[] text5;
            delete[] display;
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
// “测试”框的消息处理程序。
//这个窗口对应菜单“文件>test”,主要用于测试不成熟功能
INT_PTR CALLBACK Test(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1 || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            wchar_t perpubkey[45];
            GetDlgItemText(hDlg, IDC_EDIT3, perpubkey, 45);
            char* pubkey2 = wchartochar(perpubkey);
            unsigned char* pubkey = base642bin(pubkey2);
            unsigned char ephpublickey[crypto_box_PUBLICKEYBYTES];
            unsigned char ephsecretkey[crypto_box_SECRETKEYBYTES];
            crypto_box_keypair(ephpublickey, ephsecretkey);
            unsigned char nonce[crypto_box_NONCEBYTES];
            randombytes_buf(nonce, sizeof nonce);
            
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t text14[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text14, 2048);
            //从较大的文本框中获取文字，给了2kb的缓冲区
            unsigned char key[crypto_secretbox_KEYBYTES];
            crypto_secretbox_keygen(key);
            char* text16 = wchartochar(text14);
            const char* text = text16;
            size_t len = strlen(text);
            char* text2 = new char[len + 5];
            const char* text4 = ".enc";
            memcpy_s(text2, len + 5, text, len);
            memcpy_s(text2 + len, len + 5, text4, 5);
            long len2 = getFileSize6(text);
            unsigned char ciphertext[32 + 16];
            crypto_box_easy(ciphertext, key, 32, nonce,
                pubkey, ephsecretkey);

            char len4[24];
            sprintf_s(len4, "%ld", len2);

            std::ifstream op;
            std::ofstream wr;
            op.open(text, std::ios::binary);
            wr.open(text2, std::ios::binary);
            wr.write((const char*)ephpublickey, 32);
            wr.write((const char*)nonce, 24);
            wr.write((const char*)ciphertext, 48);
            wr.write((const char*)len4, 24);


            int i = 0;
            if (10240 <= len2) {
                char* text6 = new char[10240];
                unsigned char* text8 = new unsigned char[10256];
                for (i = 0; i * 10240 + 10240 <= len2; i++) {
                    
                    op.read(text6, 10240);
                    crypto_secretbox_easy(text8, (unsigned char*)text6, 10240, nonce, key);
                    sodium_increment(nonce, crypto_box_NONCEBYTES);
                    wr.write((const char*)text8, 10256);
                    
                }
                delete[] text8;
                delete[] text6;
                int test = len2 % 10240;
                char* text10 = new char[len2 % 10240];
                op.read(text10, len2 % 10240);
                unsigned char* text12 = new unsigned char[len2 % 10240 + 16];
                crypto_secretbox_easy(text12, (unsigned char*)text10, len2 % 10240, nonce, key);
                wr.write((const char*)text12, (len2 % 10240) + 16);
                op.close();
                wr.close();
                delete[] text12;
                delete[] text10;

            }
            else {
                char* text6 = new char[len2 % 10240];
                op.read(text6, len2 % 10240);
                unsigned char* text8 = new unsigned char[len2 % 10240 + 16];
                crypto_secretbox_easy(text8, (unsigned char*)text6, len2 % 10240, nonce, key);
                wr.write((const char*)text8, (len2 % 10240) + 16);
                op.close();
                wr.close();
                delete[] text8;
                delete[] text6;
            }

            delete[] text2, pubkey2, pubkey, text16;
            
            //调用自定义的集成加密方法
            //wchar_t* display = chartowchar((char*)text3);
            //SetDlgItemText(hDlg, IDC_EDIT2, display);
            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON2)
        {
            unsigned char perpublickey[crypto_box_PUBLICKEYBYTES];
            unsigned char persecretkey[crypto_box_SECRETKEYBYTES];
            crypto_box_keypair(perpublickey, persecretkey);
            char* b641 = bin2base64(persecretkey, crypto_secretbox_KEYBYTES);
            char* b642 = bin2base64(perpublickey, crypto_secretbox_KEYBYTES);
            wchar_t* display1 = chartowchar(b641);
            wchar_t* display2 = chartowchar(b642);
            SetDlgItemText(hDlg, IDC_EDIT1, display1);
            SetDlgItemText(hDlg, IDC_EDIT3, display2);
            //这是用来测试哪个地方不能接受中文的，发现就是wchar和char互转的位置
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON4)
        {
            wchar_t prikey1[45];
            GetDlgItemText(hDlg, IDC_EDIT1, prikey1, 45);
            char* prikey2 = wchartochar(prikey1);
            unsigned char* prikey = base642bin(prikey2);
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t text13[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text13, 2048);
            char* text15 = wchartochar(text13);
            
            
            const char* text1 = text15;
            size_t len5 = strlen(text1);
            char* text3 = new char[len5-4];
            memcpy_s(text3, len5-4, text1, len5-4);
            text3[len5-3] = '\0';
            std::ifstream op1;
            std::ofstream wr1;
            op1.open(text1, std::ios::binary);
            wr1.open(text3, std::ios::binary);
            char len1[24];
            unsigned char ephpublickey1[crypto_box_PUBLICKEYBYTES];
            unsigned char nonce1[crypto_box_NONCEBYTES];
            unsigned char key1[crypto_secretbox_KEYBYTES];
            unsigned char keycip[48];
            op1.read((char*)ephpublickey1, 32);
            op1.read((char*)nonce1, 24);
            op1.read((char*)keycip, 48);
            op1.read((char*)len1, 24);
            size_t len3 = sLEN((unsigned char*)len1, 24);
            crypto_box_open_easy(key1, keycip, 48, nonce1, ephpublickey1, prikey);
            int i1 = 0;
            if (10240 <= len3) {
                char* text5 = new char[10256];
                unsigned char* text7 = new unsigned char[10240];
                for (i1 = 0; i1 * 10240 + 10240 <= len3; i1++) {

                    op1.read(text5, 10256);

                    crypto_secretbox_open_easy(text7, (unsigned char*)text5, 10256, nonce1, key1);
                    sodium_increment(nonce1, crypto_box_NONCEBYTES);
                    wr1.write((const char*)text7, 10240);

                }
                delete[] text5, text7;
                char* text9 = new char[len3 % 10240 + 16];
                op1.read(text9, len3 % 10240 + 16);
                unsigned char* text11 = new unsigned char[len3 % 10240];
                crypto_secretbox_open_easy(text11, (unsigned char*)text9, len3 % 10240 + 16, nonce1, key1);
                wr1.write((const char*)text11, len3 % 10240);
                delete[] text9, text11;
            }
            else {
                char* text5 = new char[len3 % 10240 + 16];
                op1.read(text5, len3 % 10240 + 16);
                unsigned char* text7 = new unsigned char[len3 % 10240];
                crypto_secretbox_open_easy(text7, (unsigned char*)text5, len3 % 10240 + 16, nonce1, key1);
                wr1.write((const char*)text7, len3 % 10240);
                delete[] text5, text7;
            }


            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] text15;
            delete[] prikey2, prikey;

            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}