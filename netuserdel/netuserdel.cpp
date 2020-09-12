#ifdef UNICODE
#define UNICODE
#endif
#include<windows.h>
#include<stdio.h>
#include<Shlobj.h>
#include<stdlib.h>
#include<lm.h>

#pragma comment(lib,"netapi32.lib")

int wmain(int argc, wchar_t* argv[])
{
	 /// <summary>
	 /// NetUserDel ɾ���û�API              
	 /// https://docs.microsoft.com/en-us/windows/win32/api/lmaccess/nf-lmaccess-netuserdel
	 /// </summary>

	 
	 /*
			NET_API_STATUS NET_API_FUNCTION NetUserDel(
				LPCWSTR servername,              �����ַ��� ָ��DNS����NetBIOSԶ�̷����������ƣ�NULL��ʾ���ػ���          
				LPCWSTR username                 ָ��Ҫɾ�����û��˻�
				);


			Return value:  �������ִ�гɹ�������ֵΪ NERR_Success
	 */

	 
	// �ж��Ƿ��Թ���ԱȨ��ִ�У������޷�ɾ���û�
	BOOL IsAdmin = IsUserAnAdmin();                       // https://docs.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-isuseranadmin         
	if (!IsAdmin)
	{
		fwprintf(stderr, L"[-] Run as administrator level!\n");
		exit(EXIT_FAILURE);
	}

	if (argc != 2)
	{
		fwprintf(stderr, L"[-] Usage: %s username\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	DWORD dwError = 0;
	NET_API_STATUS nStatus;

	// ����NetUserDel  Api
	nStatus = NetUserDel(NULL, argv[1]);
	if (nStatus == NERR_Success)
	{
		fwprintf(stderr, L"[+] User %s has been successfully deleted on localhost!\n", argv[1]);
	}
	else
	{
		fwprintf(stderr, L"[-] A System error has occurred: %d\n", nStatus);
		exit(EXIT_FAILURE);
	}
	return 0;
	 
}