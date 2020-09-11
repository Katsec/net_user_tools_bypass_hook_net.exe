#include<windows.h>
#include<lmaccess.h> 
#include<stdio.h>
#include<stdlib.h>
#include<lm.h>


#pragma comment(lib,"netapi32.lib")



int wmain(int argc, wchar_t * argv[])
{
	 /// <summary>
	 /// netusersetinfo   �����û�   =>  net user guest /active
	 /// https://docs.microsoft.com/en-us/windows/win32/api/lmaccess/nf-lmaccess-netusersetinfo
	 /// https://docs.microsoft.com/en-us/windows/win32/api/lmaccess/ns-lmaccess-user_info_1008
	 /// </summary>
	
	 /*
			NET_API_STATUS NET_API_FUNCTION NetUserSetInfo(
				LPCWSTR servername,        �����ַ��� ָ��DNS����NetBIOSԶ�̷����������ƣ�NULL��ʾ���ػ���
				LPCWSTR username,		   ָ�����ַ�����ָ�� ��Ҫ�������Ե��û�
				DWORD   level,			   information level of the data.  1008 ��ʾ�û�����Ϥ  ָ��һ���ṹ�� USER_INFO_1008
				LPBYTE  buf,			   ָ��ָ�����ݵ�ָ��  ���ݵĸ�ʽ������ level parameter
				LPDWORD parm_err           ���մ���ERROR_INVALID_PARAMETER������Ϣ��NULL��ʾû�з��ش���
				);

			NetUserSetInfo �ɹ��󷵻� NERR_Success 
	 */
	DWORD dwlevel = 1008;
	USER_INFO_1008 ui;
	NET_API_STATUS nStatus;

	if (argc != 2)
	{
		printf("Usage: %s username",argv[0]);
		exit(EXIT_FAILURE);
	}
	else
	{

	    ui.usri1008_flags = UF_LOCKOUT;            // �����˻�
		//ui.usri1008_flags = UF_ACCOUNTDISABLE;     // �����˻�

		nStatus = NetUserSetInfo(NULL, argv[1], dwlevel, (LPBYTE)&ui, NULL);
		if (nStatus == NERR_Success)
		{
			printf("user account %s has been activated", argv[1]);
		}
		else
		{
			printf("A systm error has occurred: %d\n", nStatus);
		}
	}
	return 0;
}