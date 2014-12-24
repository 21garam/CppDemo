#include <stdio.h>
#include <io.h>
#include <string.h>
#include <windows.h>

#define STR_EQUAL 0		// strcmp, strncmp �� �� ���ڿ��� ���� �� 0�� ��ȯ

bool builtInCommand(char* command);		// built in command(���� ��ɾ�)�� �Ľ�
void printDirectory();					// ���丮 ���� ���ϰ� ���� ���丮�� ����ϴ� �Լ�. ls, dir ��ɾ �ش�
void changeDirectory();	// ���丮 ����. cd(change directory)�� �ش�
void concatPath(char* fileName, char* directoryPath);	// file�� directory path �� ���̴� �Լ�

char directoryPath[256];			// ���� ���丮�� �����ϴ� ����

int main()
{
	char buf[MAX_PATH];			// ����� �Է� ���ڿ��� �����ϴ� ����. MAX_PATH : ��� ������ ���丮 ����� �ִ� ����(windows.h���� ����. =260)

	GetCurrentDirectoryA(MAX_PATH, buf);		// GetCurrentDirectory() : ���� ����Ǵ� ������ ��� ������. A - ANSI ĳ����, W - ����. �����ڵ� ������ ���� ���

	strncpy_s(directoryPath, MAX_PATH, buf, MAX_PATH);	// ���� ����� �ʱ� �� ����. strcpy : string copy. ���ڿ� �����Լ�. strncpy, strncpy_s�� ������ ���� string copy �Լ�

	while (true)
	{
		printf("%s > ", directoryPath);	// ������Ʈ ���

		fflush(stdin);					// ����� �Է� ���� ����
		scanf_s("%s", buf, MAX_PATH);	// ����� �Է�

		if (builtInCommand(buf) == false)
			break;
	}

	return 0;
}

bool builtInCommand(char* command)		// ���� ��ɾ� ó��
{
	if (strncmp(command, "ls", 2) == STR_EQUAL || strncmp(command, "dir", 3) == STR_EQUAL)			// ls : list. Linux style.  dir : directory. Dos style. ���丮 ������ file�� ���
	{
		printDirectory();
	}
	else if (strncmp(command, "cd", 2) == STR_EQUAL)	// cd : change directory. ���ڷ� ������ ���丮�� �̵�
	{
		changeDirectory();
	}
	else if (strncmp(command, "help", 4) == STR_EQUAL || strncmp(command, "h", 1) == STR_EQUAL)		// ��ɾ� ���� ���
	{
		printf("help , h	����\ncd		���丮 ����\nls , dir	���丮�� �ִ� ���ϰ� ���� ���丮 ���\n");
	}
	else if (strncmp(command, "exit", 4) == STR_EQUAL || strncmp(command, "quit", 4) == STR_EQUAL)	// ���α׷� ����
	{
		return false;
	}
	else															// �������� �ʴ� ��ɾ�
	{
		printf("��ɾ �������� �ʽ��ϴ�.\n���� 'help'����\n");
	}

	return true;
}

void printDirectory()
{
	WIN32_FIND_DATAA findData;		// LPWIN32_FIND_DATA : ���� ������ �����ϴ� ����ü. A�� ANSI ĳ����, W�� ����
	char filePathFormat[MAX_PATH + 5];							// ã�� ��� ������ �����ϴ� ���ڿ�
	strncpy_s(filePathFormat, directoryPath, MAX_PATH);
	strcat_s(filePathFormat, MAX_PATH + 5, "\\*.*");				// ã�� ���� ������ ��ü
	HANDLE hFind = FindFirstFileA(filePathFormat, &findData);	// ���� Ž�� ���� �Լ����� ����ϱ� ���� �ڵ� ����. FindFirstFileA�� �ش� ���丮�� ���� �ڵ��� ��ȯ��.

	printf("  <dir>			 %s\n", findData.cFileName);			// ���� ���丮 ���� ���

	while (true)												// ���� ���� �� ���丮�� ���ؼ�
	{
		if (!FindNextFileA(hFind, &findData))
			break;

		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))	// dwFileAttributes : ������ �Ӽ� ������ ���� bit��� �����Ǿ� ����. FILE_ATTRIBUTE_DIRECTORY : 0x10. ���丮 �Ӽ�
			printf("  <dir>			 ");
		else
		{
			__int64 fileSize = (findData.nFileSizeHigh * ((__int64)MAXDWORD + 1)) + findData.nFileSizeLow;		// file size ���. file size�� 4GB�� ���� �� �����Ƿ� nFileSizeHigh�� nFileSizeLow�� 32bit�� ���� ����Ǿ� ����. 64bit ������ ���� file size�� �ϳ��� ������ ����
			printf("	 %-16I64d", fileSize);	// ���丮�� �ƴϸ� �����̹Ƿ� file size ���
		}

		printf("%s\n", findData.cFileName);		// fileName���� ��θ� ������ ������ �̸��� ����Ǿ� ����
	}

	FindClose(hFind);		// ���� �ڵ� �ڵ� ����
}

void changeDirectory()
{
	char directoryName[MAX_PATH];
	char newDirectoryPath[MAX_PATH];

	strncpy_s(newDirectoryPath, MAX_PATH, directoryPath, MAX_PATH);

	scanf_s("%s", directoryName, MAX_PATH);	// ����� �Է�
	char* cpForCorrection = directoryName;
	while ((cpForCorrection = strchr(cpForCorrection, '/')) != NULL)		// strchr() : ���ڿ����� Ư�� ���ڰ� ó�� �����ϴ� ���� �����͸� ��ȯ. �������� ������ null ��ȯ
		*cpForCorrection = '\\';		// ������(/)�� ��������(\)�� ����

	char* cpForTokenizing = directoryName;
	char* token = directoryName;
	while (true)
	{
		if ((cpForTokenizing = strchr(cpForTokenizing, '\\')) != NULL)
		{
			*cpForTokenizing = NULL;

			concatPath(token, newDirectoryPath);

			cpForTokenizing++;
			token = cpForTokenizing;
		}
		else		// �� �̻� �޾Ƶ��� ��ū�� ������ ������ ��ū�� �ٿ���
		{
			concatPath(token, newDirectoryPath);
			break;
		}
	}

	if (_access_s(newDirectoryPath, 0) == 0)		// access() : Ư�� ������ ���缺 �Ǵ� ������ üũ. ����� ���缺 �˻�
		strncpy_s(directoryPath, MAX_PATH, newDirectoryPath, MAX_PATH);		// directory path�� ���ο� directory path�� �ٲپ���
	else
		printf("������ ��θ� ã�� �� �����ϴ�.\n");
}

void concatPath(char* fileName, char* directoryPath)
{
	if (strncmp(fileName, "..", 2) == STR_EQUAL)		// ���� directory�� �̵�
	{
		char* cp = strrchr(directoryPath, '\\');	// strrchr : ���ڿ��� �ڿ��� �ش� ���ڰ� ó�� �����ϴ� ���� �����͸� ��ȯ
		if (cp == NULL)		// �̹� �ֻ��� ����� ��� �Է� ����
		{
			printf("������ ��θ� ã�� �� �����ϴ�.\n");
			return;
		}
		else		// �ֻ��� ��ΰ� �ƴ� ��� �� �ܰ� �ö�
			*cp = NULL;
	}
	else if (fileName[0] == '.' || fileName[0] == 0)		// '.' : ���� directory�� ��� �ƹ� ó���� ����. 0 : ���� �̸��� ���� ��� �ƹ� ó���� ����
	{
	}
	else			// path�� �ִ� ��� ��θ� �������
	{
		strcat_s(directoryPath, MAX_PATH, "\\");
		strcat_s(directoryPath, MAX_PATH, fileName);
	}
}