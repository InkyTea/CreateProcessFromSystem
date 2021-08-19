

#pragma once

//#include<windows.h>
#include <shlobj.h>
#include "commdlg.h"

class CSelectFolderDlg {
public:
    //����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��  
    static string SelectFolder() {
        TCHAR       szFolderPath[MAX_PATH] = { 0 };
        string     strFolderPath;

        BROWSEINFO      sInfo;
        ::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
        sInfo.pidlRoot = 0;
        sInfo.lpszTitle = "��ѡ��һ���ļ��У�";
        sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
        sInfo.lpfn = NULL;

        /*
        typedef struct _browseinfo {
            HWND hwndOwner;            // �����ھ��  
            LPCITEMIDLIST pidlRoot;    // Ҫ��ʾ���ļ�Ŀ¼�Ի���ĸ�(Root)  
            LPTSTR pszDisplayName;     // ���汻ѡȡ���ļ���·���Ļ�����  
            LPCTSTR lpszTitle;         // ��ʾλ�ڶԻ������ϲ��ı���  
            UINT ulFlags;              // ָ���Ի������ۺ͹��ܵı�־  
            BFFCALLBACK lpfn;          // �����¼��Ļص�����  
            LPARAM lParam;             // Ӧ�ó��򴫸��ص������Ĳ���  
            int iImage;                // �ļ��жԻ����ͼƬ����  
        } BROWSEINFO, * PBROWSEINFO, * LPBROWSEINFO
        */

        // ��ʾ�ļ���ѡ��Ի���  
        LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
        if (lpidlBrowse != NULL) {
            // ȡ���ļ�����  
            if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath)) {
                strFolderPath = szFolderPath;
            }
        }
        if (lpidlBrowse != NULL) {
            ::CoTaskMemFree(lpidlBrowse);
        }
        return strFolderPath;
    }
    //����һ��ѡ���ļ��ĶԻ��򣬷�����ѡ·��  
    static string SelectFile() {

        OPENFILENAME ofn;
        string     strFilePath;
        char szFile[MAX_PATH] = {0};

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        LPTSTR        lpstrCustomFilter;
        DWORD         nMaxCustFilter;
        ofn.nFilterIndex = 1;
        LPTSTR        lpstrFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = "ALL\0*.*\0Text\0*.TXT\0";
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;

        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn)) {//LPWSTR
            strFilePath = ofn.lpstrFile;
            return strFilePath;
        }
        else {
            return strFilePath;
        }
    }

    void FolderCallBack() {

    }
};

