

#pragma once

#include<windows.h>
#include <atlstr.h>
#include <shlobj.h>
#include "commdlg.h"
#include <string>

class CSelectFolderDlg {
public:
    //����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��  
    static CString SelectFolder() {
        TCHAR       szFolderPath[MAX_PATH] = { 0 };
        CString     strFolderPath = "";

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
            //MessageBox(NULL, path_image, path_image, 0);//�����õ�·��
            return path_image;
        }
        else {
            return "";
        }
    }

    void FolderCallBack() {

    }

};

