#ifdef UNICODE
#define UNICODE
#endif
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<lm.h>
#include<assert.h>

#pragma comment(lib,"netapi32.lib")

int wmain(int argc, wchar_t* argv[])
{
	      /// <summary>
		  /// NetUserEnum �û�ö��Api  ���ܵ�Ч�� net user
	      /// https://docs.microsoft.com/en-us/windows/win32/api/lmaccess/nf-lmaccess-netuserenum
		  ///
		  /// </summary>
		  
		  /*
				 NET_API_STATUS NET_API_FUNCTION NetUserEnum(
					LPCWSTR servername,						�����ַ��� ָ��DNS����NetBIOSԶ�̷����������ƣ�NULL��ʾ���ػ���
					DWORD   level,							ָ�����ݵ���Ϣ����  0 1 2 3 10 11 20  USER_INFO_0 �ṹ��
					DWORD   filter,							ָ��������ö���û��˻����͵�ֵ��0ֵ��ʾ�������������û����������ݺͻ����˻�����
					LPBYTE  *bufptr,						ָ��������ݻ����ָ�룬���ݵĸ�ʽ�����ڲ���level��ֵ
					DWORD   prefmaxlen,						���ȿ��Ƿ��������ֽ���󳤶� ���ָ����MAX_PREFERED_LENGTH��������������ݴ�С���ڴ�
					LPDWORD entriesread,					ָ�����ö��Ԫ���˻���ֵ
					LPDWORD totalentries,					ָ��һ��ֵ��ָ�룬��ֵ���տ��Դӵ�ǰ�ָ�λ��ö�ٵ���Ŀ����
					PDWORD  resume_handle					ָ��������ڼ��������û������Ļָ������ֵ��ָ��
					);

				Note:
					The buffer for this data is allocated by the system and 
					the application must call the NetApiBufferFree function to free the allocated memory 
					when the data returned is no longer needed. 
					Note that you must free the buffer even if the NetUserEnum function fails with ERROR_MORE_DATA.

					������ݵĻ�����ϵͳ���䣬���������ݲ�����Ҫʱ,Ӧ�ó���������NetApiBufferFree�����ͷŷ�����ڴ�.
					��ʹNetUserEnum������ERROR_MORE_DATAʧ��ʱ��������ͷŻ��塣

				Return value: ����ִ�гɹ����� NERR_Success
		  */
	if (argc > 2)
	{
		fwprintf(stderr, L"[-] Usage: %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	// ���ýṹ����Ϣ
	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;                 // MAX_PREFERED_LENGTH = ((DWORD-1)
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;

	wprintf(L"[+] User account on localhost\n");

	//
	// Call the NetUserEnum function,specifying level 0   0��ʾ global user account 0
	//

	do   // start do
	{
		nStatus = NetUserEnum(
			(LPCWSTR)pszServerName,
			dwLevel,                 // USER_INFO_0 
			FILTER_NORMAL_ACCOUNT,   // global users
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,          // NetUserEnum api����idwTotalCount=5   dwEntriesRead=5
			&dwTotalCount,
			&dwResumeHandle
		);

		// if call succeeds
 		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				// loop through the entries ѭ������Ŀ¼

				for (int i = 0; (i < dwEntriesRead); i++)
				{
					/*
							assert()  C++�÷�  ��������������������ش�������ֹ����ִ��
							assert() ������ʽ��ֵΪ�٣�����stderr��ӡһ��������Ϣ��Ȼ�����abort��ֹ����ִ��

							ʹ��assert() ȱ��: Ƶ���ص��ûἫ���Ӱ���������ܣ����Ӷ���Ŀ���   #define NDEBUG ���Խ�������assert����
					*/

					assert(pTmpBuf != NULL);

					if (pTmpBuf == NULL)
					{
						fprintf(stderr, "[-] An access violation has occurred\n");
						break;
					}

					// print the name of the user account

					wprintf(L"=> %s\n", pTmpBuf->usri0_name);

					pTmpBuf++;
					dwTotalCount++;
					
				}
			}
		}

		// otherwise,print the system error.
		else
		{
			fprintf(stderr,"[-] A system error has occurred: %d\n", nStatus);
		}

		// Free the allocated buffer
		if (pBuf != NULL)
		{

			// NetApiBufferFree �����ͷ�NetApiBufferAllocate����������ڴ�
			// https://docs.microsoft.com/en-us/windows/win32/api/lmapibuf/nf-lmapibuf-netapibufferfree
			// 

			/*
					NET_API_STATUS NET_API_FUNCTION NetApiBufferFree(
												_Frees_ptr_opt_ LPVOID Buffer
												);
					Buffer
					ָ����ǰ����һ���������������ͨ������NetApiBufferAllocate����������ڴ� ���صĻ�������ָ��
			*/


			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}

	}

	// continue to call NetUserEnum while there are more entries.
	while (nStatus == ERROR_MORE_DATA);   // end do

	// �ٴμ�������ڴ�
	if (pBuf != NULL)
	{
		NetApiBufferFree(pBuf);
	}

	// print the final count of users enumrated.
	fprintf(stderr, "Total of %d entries enumerated\n", dwTotalCount-5);

	return 0;
}