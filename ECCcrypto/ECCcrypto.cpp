// ECCcrypto.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "ECCcrypto.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <string.h>
#include "sodium.h"
#include "SmallTools.h"
#include "EncAndDec.h"
#include <fstream>
#include "zxcvbn.h"
#include <Commdlg.h>
#include "resource.h"
#include "base58.h"
#include "blake2.h"
#include "blake2-impl.h"

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
int count = 0;
unsigned char* prikey;
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PKTextDEnc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PKFileDEnc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Test(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    GenKey(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    sodium_init();
    prikey = (unsigned char*)sodium_malloc(32);
    int g = sodium_mprotect_noaccess(prikey);
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
            case ID_32773:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_TEST), hWnd, Test);
                break;
            case ID_32774:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_PKTEXTENC), hWnd, PKTextDEnc);
                break;
            case ID_32777:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_PKFILEENC), hWnd, PKFileDEnc);
                break;
            case ID_32778:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_GENKEY), hWnd, GenKey);
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
            LOGFONT logfont;
            ZeroMemory(&logfont, sizeof(LOGFONT));
            logfont.lfCharSet = GB2312_CHARSET;
            logfont.lfHeight = -20;
            HFONT hFont = CreateFontIndirect(&logfont);
            SelectObject(hdc, hFont);
            TextOut(hdc, 0, 0, L"这仅仅是一个debug版本拿出来玩玩的", 19);
            TextOut(hdc, 0, 20, L"菜单里面文件->第三选项是文件加解密，输绝对路径", 24);
            TextOut(hdc, 0, 40, L"菜单里面文件->创建密钥对必须每次打开程序先运行", 24);
            TextOut(hdc, 0, 60, L"其他选项基本就是划水的，没啥功能还不好用，别玩了", 24);
            TextOut(hdc, 0, 80, L"可以支持一个文件发给多个收件人，收件人相互无法认识", 25);
            TextOut(hdc, 0, 100, L"最大收件人个数大概是20个左右，虽然很容易扩充", 23);
            TextOut(hdc, 0, 120, L"大概写了写各种报错，遇到了及时反馈给我", 19);
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
            InitInstance(NULL, IDD_GENKEY);
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
        {
            tagOFNW ofn = { 0 };
            TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名
            ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
            ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
            ofn.lpstrFilter = TEXT("所有文件\0*.*\0\0");//设置过滤
            ofn.nFilterIndex = 1;//过滤器索引
            ofn.lpstrFile = strFilename;//接收返回的文件名，注意第一个字符需要为NULL
            ofn.nMaxFile = sizeof(strFilename);//缓冲区长度
            ofn.lpstrInitialDir = NULL;//初始目录为默认
            ofn.lpstrTitle = TEXT("选择要加密的文件");//使用系统默认标题留空即可
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项
            GetOpenFileName(&ofn);
            SetDlgItemText(hDlg, IDC_EDIT2, strFilename);
            //delete[] &ofn;
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            wchar_t text14[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text14, 2048);
            char* text16 = wchartochar(text14);
            if (strlen(text16) == 0) {
                MessageBox(NULL, TEXT("没选择文件"), NULL, MB_ICONERROR); return (INT_PTR)TRUE;
            }
            TrimSpace(text16);
            const char* text = text16;
            size_t len = strlen(text);
            char* text2 = new char[len + 5];
            const char* text4 = ".enc";
            memcpy_s(text2, len + 5, text, len);
            memcpy_s(text2 + len, len + 5, text4, 5);
            long long len2 = getFileSize6(text);


            unsigned char ephpublickey[crypto_box_PUBLICKEYBYTES];
            unsigned char ephsecretkey[crypto_box_SECRETKEYBYTES];
            crypto_box_keypair(ephpublickey, ephsecretkey);
            unsigned char noncehead[crypto_box_NONCEBYTES];
            unsigned char noncefile[crypto_box_NONCEBYTES];
            unsigned char pk[32];
            sodium_mprotect_readonly(prikey);
            crypto_scalarmult_base(pk, prikey);
            sodium_mprotect_noaccess(prikey);

            std::ifstream op;
            std::ofstream wr;
            op.open(text, std::ios::binary);
            wr.open(text2, std::ios::binary);
            wchar_t perpubkey[1024];
            GetDlgItemText(hDlg, IDC_EDIT3, perpubkey, 1024);
            char* pubkey2 = wchartochar(perpubkey);
            std::string pubkey3 = pubkey2;
            //delete[] pubkey2;
            char pubkey4[1024];
            int i2;
            for (i2 = 0; i2 < pubkey3.length(); i2++)
                pubkey4[i2] = pubkey3[i2];
            pubkey4[i2] = '\0';
            char** pksplit = split(pubkey4, ";");
            int m = 0;
            while (pksplit[m] != NULL) {
                m++;
            }
            if (m == 0) {
                MessageBox(NULL, TEXT("请输入公钥"), NULL, MB_ICONERROR);
                return (INT_PTR)TRUE;
            }
            int headerlen = m * 176;
            char headlen[4];
            sprintf_s(headlen,"%d", headerlen);
            if (headerlen < 1000) {
                headlen[3] = '\0';
            }
            wr.write("eccRypto", 8);
            wr.write(headlen, 4);
            wr.write((const char*)ephpublickey, 32);
            
            int n = 0;
            unsigned char key[crypto_secretbox_KEYBYTES];
            randombytes_buf(noncefile, 24);
            crypto_secretbox_keygen(key);
            while (m > n) {
                size_t leng = strlen(pksplit[n]);
                if (leng != 43) {
                    MessageBox(NULL, TEXT("公钥不对"), NULL, MB_ICONERROR); return (INT_PTR)TRUE;
                }
                randombytes_buf(noncehead, 24);
                wr.write((char*)noncehead, 24);
                //char* sdf = bin2base64(noncehead, 24);
                unsigned char temp[56];
                memcpy_s(temp, 56, key, 32);
                //sdf = bin2base64(key,32);
                memcpy_s(temp+32,56,noncefile, 24);
                //sdf = bin2base64(noncefile, 24);
                unsigned char temp1[72];
                sodium_mprotect_readonly(prikey);
                crypto_box_easy(temp1, temp, 56, noncehead,
                    base642bin(pksplit[n]), prikey);
                sodium_mprotect_noaccess(prikey);
                unsigned char temp2[72+32+32+136];
                memcpy_s(temp2, 136, temp1, 72);
                memcpy_s(temp2+72,136,pk, 32);
                unsigned char* pksplits = base642bin(pksplit[n]);
                memcpy_s(temp2+72+32,136, pksplits, 32);
                //sdf = bin2base64(temp1, 72);
                unsigned char temp3[152];
                //sdf = bin2base64(base642bin(pksplit[n]), 32);
                //sdf = bin2base64(pk, 32);
                crypto_box_easy(temp3, temp2, 136, noncehead,
                    pksplits, ephsecretkey);
                //sdf = bin2base64(temp3, 152);
                wr.write((char*)temp3, 152);
                n++;
            }
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            delete[] pksplit;

            unsigned long long i = 0;
            long long h = 102400;
            char* text6 = new char[102400];
                unsigned char* text8 = new unsigned char[102416];
                for (; h <= len2; len2-=102400) {

                    op.read(text6, 102400);
                    int s = crypto_secretbox_easy(text8, (unsigned char*)text6, 102400, noncefile, key);
                    sodium_increment(noncefile, crypto_box_NONCEBYTES);
                    wr.write((const char*)text8, 102416);

                }
                delete[] text8;
                delete[] text6;
            if(len2 % h != 0){
                size_t test = len2 % h;
                char* text10 = new char[len2 % h];
                op.read(text10, len2 % h);
                unsigned char* text12 = new unsigned char[len2 % h + (size_t)16];
                int s = crypto_secretbox_easy(text12, (unsigned char*)text10, len2 % 102400, noncefile, key);
                wr.write((const char*)text12, (len2 % 102400) + 16);
                op.close();
                wr.close();
                delete[] text12;
                delete[] text10;
            }
            delete[] text2, pubkey2, text16;
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON4)
        {
            //这里我先用的secret key加密（比如AES，但这里不是），因为简单
            wchar_t text13[2048];
            GetDlgItemText(hDlg, IDC_EDIT2, text13, 2048);
            char* text15 = wchartochar(text13);
            TrimSpace(text15);

            const char* text1 = text15;
            size_t len5 = strlen(text1);
            char* text3 = new char[len5];
            memset(text3, '\0', len5);
            memcpy_s(text3, len5, text1, len5 - 4);

            std::ifstream op1;
            std::ofstream wr1;
            op1.open(text1, std::ios::binary);
            wr1.open(text3, std::ios::binary);
            char crc[8];
            char len1[4];
            unsigned char ephpublickey1[crypto_box_PUBLICKEYBYTES];
            unsigned char nonce1[crypto_box_NONCEBYTES];
            unsigned char key1[crypto_secretbox_KEYBYTES];
            op1.read((char*)crc, 8);
            
            if (sodium_memcmp(crc,"eccRypto",8) != 0) {
                MessageBox(NULL, TEXT("不是加密文件"), NULL, MB_ICONERROR); return (INT_PTR)TRUE;
            }
            op1.read((char*)len1, 4);
            long long len3 = (long long)sLEN((unsigned char*)len1, 4);
            if ((len3) % 176 != 0) {
                MessageBox(NULL, TEXT("文件头坏了"), NULL, MB_ICONERROR); return (INT_PTR)TRUE;
            }
            long long len2 = getFileSize6(text1);
            len2 -= len3;
            len2 -= 12;
            len2 -= 32;
            op1.read((char*)ephpublickey1, 32);
            long long ui = len3 / 176;
            long long uiop = len2 % 102416;
            unsigned char* hispk = new unsigned char[32];
            int sig = 0;
            while (ui > 0) {
                op1.read((char*)nonce1, 24);
                unsigned char temp1[152];
                op1.read((char*)temp1, 152);
                unsigned char temp2[136];
                sodium_mprotect_readonly(prikey);
                int h =crypto_box_open_easy(temp2, temp1, 152, nonce1, ephpublickey1, prikey);

                if (h != 0) {
                    sig = 1;
                }
                else {
                    unsigned char mypk[32];
                    
                    crypto_scalarmult_base(mypk, prikey);
                    int g = sodium_memcmp(mypk, temp2 + 72 + 32, 32);
                    if (g != 0) {
                        sig = 1;
                    }
                    else {
                        
                        memcpy_s(hispk, 32, temp2 + 72, 32);
                        unsigned char temp3[56];
                        int hi = crypto_box_open_easy(temp3, temp2, 72, nonce1, hispk, prikey);
                        if (hi != 0) {
                            sig = 1;
                        }
                        else {
                            memcpy_s(key1, 32, temp3, 32);
                            memcpy_s(nonce1, 24, temp3+32, 24);
                        }
                    }
                    
                }
                if (sig == 1) {
                    ui--;;
                    sig--;
                }
                else if (sig == 0) {
                    ui--;
                }
            }
            sodium_mprotect_noaccess(prikey);
            if (sig == 1) {
                MessageBox(NULL, TEXT("你不是文件收件人或文件头已损坏"), NULL, MB_ICONERROR); return (INT_PTR)TRUE;
            }
                char* text5 = new char[102416];
                unsigned char* text7 = new unsigned char[102400];
                for (; 102416 <= len2; len2-=102416) {

                    op1.read(text5, 102416);

                    int op = crypto_secretbox_open_easy(text7, (unsigned char*)text5, 102416, nonce1, key1);
                    if (op != 0) {
                        MessageBox(NULL, TEXT("文件已损坏,解密失败"), NULL, MB_ICONERROR); return (INT_PTR)TRUE;
                    }
                    sodium_increment(nonce1, crypto_box_NONCEBYTES);
                    wr1.write((const char*)text7, 102400);

                }
                delete[] text5, text7;
                char* text9 = new char[uiop];
                op1.read(text9, uiop);
                unsigned char* text11 = new unsigned char[uiop-16];
                crypto_secretbox_open_easy(text11, (unsigned char*)text9, uiop, nonce1, key1);
                wr1.write((const char*)text11, uiop-16);
                
                delete[] text9, text11;


            //还在大文本框里显示，可能感觉没变，但是实际上是解密后结果
            delete[] text15, text3;
            char* hispk64 = bin2base64(hispk, 32);
            char show[60];
            sprintf_s(show, "发件人公钥：""%s", hispk64);

            SetDlgItemText(hDlg, IDC_EDIT3, chartowchar(show));
            delete[] hispk, hispk64;
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
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1 || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        /*else if (LOWORD(wParam) == IDC_BUTTON2)
        {
           
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            
        }
        else if (LOWORD(wParam) == IDC_BUTTON4)
        {
            return (INT_PTR)TRUE;
        }*/
        
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK  GenKey(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        if (count != 0) {
            unsigned char pk[32];
            sodium_mprotect_readonly(prikey);
            crypto_scalarmult_base(pk, prikey);
            sodium_mprotect_noaccess(prikey);
            //uint8_t crc[1];
            //blake2s(crc, 1, pk, 32, NULL, 0);
            //memcpy_s(pk + 32, 33, crc, 1);
            char* pkb64 = bin2base64(pk, 32);
            wchar_t* pk1 = chartowchar(pkb64);
            SetDlgItemText(hDlg, IDC_EDIT5, pk1);
            delete[] pkb64, pk1;
            SetDlgItemText(hDlg, IDC_EDIT4, L"这是你原先的公钥");
        }
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON2 || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON1)
        {
            /*bool mini = FALSE;
            if (IsDlgButtonChecked(hDlg, IDC_CHECK1) == 1) {
                mini = TRUE;
            }*/
            
            wchar_t wpass[300] = { '\0' };
            wchar_t wpass1[300] = { '\0' };
            GetDlgItemText(hDlg, IDC_EDIT2, wpass, 300);
            
            GetDlgItemText(hDlg, IDC_EDIT6, wpass1, 300);
            bool arrayequal = TRUE;
            for (int i = 0; i < 300; i++) {
                if (wpass[i] != wpass1[i]) {
                    arrayequal = FALSE;
                }
            }
            if (arrayequal == TRUE) {
                const char* pass = wchartochar(wpass);
                double entro = ZxcvbnMatch(pass, NULL, NULL);
                if (entro <= 100) {
                    char ent[30];
                    sprintf_s(ent, "%f", entro);
                    wchar_t* show1 = chartowchar(ent);
                    SetDlgItemText(hDlg, IDC_EDIT5, show1);
                    delete[] show1,pass;
                    SetDlgItemText(hDlg, IDC_EDIT4, L"上面是熵值，熵值太小，密码强度不够");
                }
                else {
                    wchar_t wemail[100];
                    GetDlgItemText(hDlg, IDC_EDIT1, wemail, 100);
                    char* email = wchartochar(wemail);
                    TrimSpace(email);
                    uint8_t pri[32];
                    unsigned char pk[32];
                    /*if (mini == TRUE) {
                        pri = scrypt_mini((uint8_t*)pass, strlen(pass), (uint8_t*)email,strlen(email));
                        crypto_scalarmult_base(pk, pri);
                    }*/
                    
                        unsigned char hash[crypto_generichash_BYTES];
                        crypto_generichash(hash, sizeof hash,
                            (unsigned char*)email, strlen(email),
                            NULL, 0);
                        unsigned char salt[16];
                        memcpy_s(salt, 16, hash, 16);
                        unsigned char key[32];
                        crypto_pwhash(key, 32, pass, strlen(pass), salt,
                            crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT);
                        
                        crypto_box_seed_keypair(pk, pri, key);
                    
                    //uint8_t crc[1];
                    //blake2s(crc, 1, pk, 32, NULL, 0);
                    //memcpy_s(pk + 32, 33, crc, 1);
                    //std::string pkb58 = EncodeBase58(pk, pk+33);
                    //const char* p = pkb58.c_str();
                    char* pk64 = bin2base64(pk, 32);
                    wchar_t* pk1 = chartowchar((char*)pk64);
                    SetDlgItemText(hDlg, IDC_EDIT5, pk1);
                    SetDlgItemText(hDlg, IDC_EDIT4, L"创建成功");
                    sodium_mprotect_readwrite(prikey);
                    for (int g = 0; g < 32; g++) {
                        prikey[g] = pri[g];
                    }
                    sodium_mprotect_noaccess(prikey);
                    count++;
                    sodium_stackzero(2000);
                    delete[] pass,pk1,pk64;
                }
            }
            else{
                SetDlgItemText(hDlg, IDC_EDIT4, L"两次输入的密码不符");
            }
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
