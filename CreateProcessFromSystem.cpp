// CreateProcessFromSystem.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "CreateProcess.h"
#include "SelectFolderDlg.h"
#include "CreateProcessFromSystem.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

    if (!CreateProcess::IsProcessRunAsAdmin()) {
        CreateProcess::RunAsAdmin();
        exit(0);
    }
    
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, WndProc);
    return 0;
}


INT_PTR CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            // 分析菜单选择:
            switch (wmId)
            {
            case ID_OK:
                {
                    //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    HWND hEdit = FindWindowEx(hWnd, NULL, "Edit", NULL);
                    char filePath[MAX_PATH] = { 0 };
                    if (hEdit != nullptr) {
                        GetWindowText(hEdit, filePath, MAX_PATH);
                        CreateProcess::CreateProcessFromParent(filePath);
                    }
                    break;
                }
            case ID_SELECT:
                {
                    HWND hEdit = FindWindowEx(hWnd,NULL,"Edit", NULL);
                    char* temFilePath = CSelectFolderDlg::SelectFile();
                    int len = strlen(temFilePath);
                    char filePath[MAX_PATH] = {0};
                    memcpy(filePath, temFilePath, len);
                    //char* strLogMsg1 = strLogMsg;0x00d7f0d8 "H:\\ali213pk_6.36\\ali213Pk.exe"
                   // MessageBox(NULL, strLogMsg, strLogMsg, 0);//输出获得的路径
                    if (hEdit != nullptr) {
                        SetWindowText(hEdit, filePath);
                    }
                    
                    break;
                }
            default:
                break;
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return 0;
}
