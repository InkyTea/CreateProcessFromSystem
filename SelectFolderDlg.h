

#pragma once

#include<windows.h>
#include <atlstr.h>
#include <shlobj.h>
#include "commdlg.h"
#include <string>

class CSelectFolderDlg {
public:
    //创建一个选择文件夹的对话框，返回所选路径  
    static CString SelectFolder() {
        TCHAR       szFolderPath[MAX_PATH] = { 0 };
        CString     strFolderPath = "";

        BROWSEINFO      sInfo;
        ::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
        sInfo.pidlRoot = 0;
        sInfo.lpszTitle = "请选择一个文件夹：";
        sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
        sInfo.lpfn = NULL;

        /*
        typedef struct _browseinfo {
            HWND hwndOwner;            // 父窗口句柄  
            LPCITEMIDLIST pidlRoot;    // 要显示的文件目录对话框的根(Root)  
            LPTSTR pszDisplayName;     // 保存被选取的文件夹路径的缓冲区  
            LPCTSTR lpszTitle;         // 显示位于对话框左上部的标题  
            UINT ulFlags;              // 指定对话框的外观和功能的标志  
            BFFCALLBACK lpfn;          // 处理事件的回调函数  
            LPARAM lParam;             // 应用程序传给回调函数的参数  
            int iImage;                // 文件夹对话框的图片索引  
        } BROWSEINFO, * PBROWSEINFO, * LPBROWSEINFO
        */

        // 显示文件夹选择对话框  
        LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
        if (lpidlBrowse != NULL) {
            // 取得文件夹名  
            if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath)) {
                strFolderPath = szFolderPath;
                
            }
        }
        if (lpidlBrowse != NULL) {
            ::CoTaskMemFree(lpidlBrowse);
        }
        return strFolderPath;
    }
    static CString SelectFile() {

        OPENFILENAME ofn;
        char szFile[300];

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        LPTSTR        lpstrCustomFilter;
        DWORD         nMaxCustFilter;
        ofn.nFilterIndex = 1;
        LPTSTR        lpstrFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "ALL\0*.*\0Text\0*.TXT\0";
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;

        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        CString path_image = "";
        if (GetOpenFileName(&ofn)) {//LPWSTR
            //std::string strTemp = std::Lpcwstr2String(ofn.lpstrFile);
            path_image = ofn.lpstrFile;
            //MessageBox(NULL, path_image, path_image, 0);//输出获得的路径
            return path_image;
        }
        else {
            return "";
        }
    }

    void FolderCallBack() {

    }

};

