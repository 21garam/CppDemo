#include <stdio.h>
#include <io.h>
#include <string.h>
#include <windows.h>

#define STR_EQUAL 0		// strcmp, strncmp 는 두 문자열이 같을 때 0을 반환

bool builtInCommand(char* command);		// built in command(내장 명령어)를 파싱
void printDirectory();					// 디렉토리 내부 파일과 하위 디렉토리를 출력하는 함수. ls, dir 명령어에 해당
void changeDirectory();	// 디렉토리 변경. cd(change directory)에 해당
void concatPath(char* fileName, char* directoryPath);	// file을 directory path 에 붙이는 함수

char directoryPath[256];			// 현재 디렉토리를 지정하는 버퍼

int main()
{
	char buf[MAX_PATH];			// 사용자 입력 문자열을 저장하는 버퍼. MAX_PATH : 사용 가능한 디렉토리 경로의 최대 길이(windows.h에서 정의. =260)

	GetCurrentDirectoryA(MAX_PATH, buf);		// GetCurrentDirectory() : 현재 실행되는 파일의 경로 가져옴. A - ANSI 캐릭터, W - 워드. 유니코드 지원을 위해 사용

	strncpy_s(directoryPath, MAX_PATH, buf, MAX_PATH);	// 파일 경로의 초기 값 지정. strcpy : string copy. 문자열 복사함수. strncpy, strncpy_s는 보안을 위한 string copy 함수

	while (true)
	{
		printf("%s > ", directoryPath);	// 프롬프트 출력

		fflush(stdin);					// 사용자 입력 버퍼 비우기
		scanf_s("%s", buf, MAX_PATH);	// 사용자 입력

		if (builtInCommand(buf) == false)
			break;
	}

	return 0;
}

bool builtInCommand(char* command)		// 내장 명령어 처리
{
	if (strncmp(command, "ls", 2) == STR_EQUAL || strncmp(command, "dir", 3) == STR_EQUAL)			// ls : list. Linux style.  dir : directory. Dos style. 디렉토리 하위의 file들 출력
	{
		printDirectory();
	}
	else if (strncmp(command, "cd", 2) == STR_EQUAL)	// cd : change directory. 인자로 들어오는 디렉토리로 이동
	{
		changeDirectory();
	}
	else if (strncmp(command, "help", 4) == STR_EQUAL || strncmp(command, "h", 1) == STR_EQUAL)		// 명령어 도움말 출력
	{
		printf("help , h	도움말\ncd		디렉토리 변경\nls , dir	디렉토리에 있는 파일과 하위 디렉토리 출력\n");
	}
	else if (strncmp(command, "exit", 4) == STR_EQUAL || strncmp(command, "quit", 4) == STR_EQUAL)	// 프로그램 종료
	{
		return false;
	}
	else															// 존재하지 않는 명령어
	{
		printf("명령어가 존재하지 않습니다.\n도움말 'help'참조\n");
	}

	return true;
}

void printDirectory()
{
	WIN32_FIND_DATAA findData;		// LPWIN32_FIND_DATA : 파일 정보를 저장하는 구조체. A는 ANSI 캐릭터, W는 워드
	char filePathFormat[MAX_PATH + 5];							// 찾을 대상 파일을 지정하는 문자열
	strncpy_s(filePathFormat, directoryPath, MAX_PATH);
	strcat_s(filePathFormat, MAX_PATH + 5, "\\*.*");				// 찾을 파일 범위는 전체
	HANDLE hFind = FindFirstFileA(filePathFormat, &findData);	// 파일 탐색 관련 함수들을 사용하기 위한 핸들 생성. FindFirstFileA는 해당 디렉토리의 파일 핸들을 반환함.

	printf("  <dir>			 %s\n", findData.cFileName);			// 현재 디렉토리 정보 출력

	while (true)												// 하위 파일 및 디렉토리에 대해서
	{
		if (!FindNextFileA(hFind, &findData))
			break;

		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))	// dwFileAttributes : 파일의 속성 정보를 가진 bit들로 구성되어 있음. FILE_ATTRIBUTE_DIRECTORY : 0x10. 디렉토리 속성
			printf("  <dir>			 ");
		else
		{
			__int64 fileSize = (findData.nFileSizeHigh * ((__int64)MAXDWORD + 1)) + findData.nFileSizeLow;		// file size 계산. file size는 4GB가 넘을 수 있으므로 nFileSizeHigh와 nFileSizeLow에 32bit씩 따로 저장되어 있음. 64bit 정수로 파일 file size를 하나의 변수에 저장
			printf("	 %-16I64d", fileSize);	// 디렉토리가 아니면 파일이므로 file size 출력
		}

		printf("%s\n", findData.cFileName);		// fileName에는 경로를 제외한 파일의 이름이 저장되어 있음
	}

	FindClose(hFind);		// 파일 핸들 핸들 해제
}

void changeDirectory()
{
	char directoryName[MAX_PATH];
	char newDirectoryPath[MAX_PATH];

	strncpy_s(newDirectoryPath, MAX_PATH, directoryPath, MAX_PATH);

	scanf_s("%s", directoryName, MAX_PATH);	// 사용자 입력
	char* cpForCorrection = directoryName;
	while ((cpForCorrection = strchr(cpForCorrection, '/')) != NULL)		// strchr() : 문자열에서 특정 문자가 처음 등장하는 곳의 포인터를 반환. 존재하지 않으면 null 반환
		*cpForCorrection = '\\';		// 슬래쉬(/)를 역슬래쉬(\)로 정정

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
		else		// 더 이상 받아들일 토큰이 없으면 마지막 토큰을 붙여줌
		{
			concatPath(token, newDirectoryPath);
			break;
		}
	}

	if (_access_s(newDirectoryPath, 0) == 0)		// access() : 특정 파일의 존재성 또는 권한을 체크. 현재는 존재성 검사
		strncpy_s(directoryPath, MAX_PATH, newDirectoryPath, MAX_PATH);		// directory path를 새로운 directory path로 바꾸어줌
	else
		printf("지정된 경로를 찾을 수 없습니다.\n");
}

void concatPath(char* fileName, char* directoryPath)
{
	if (strncmp(fileName, "..", 2) == STR_EQUAL)		// 상위 directory로 이동
	{
		char* cp = strrchr(directoryPath, '\\');	// strrchr : 문자열의 뒤에서 해당 문자가 처음 등장하는 곳의 포인터를 반환
		if (cp == NULL)		// 이미 최상위 경로인 경우 입력 에러
		{
			printf("지정된 경로를 찾을 수 없습니다.\n");
			return;
		}
		else		// 최상위 경로가 아닌 경우 한 단계 올라감
			*cp = NULL;
	}
	else if (fileName[0] == '.' || fileName[0] == 0)		// '.' : 현재 directory인 경우 아무 처리도 안함. 0 : 파일 이름이 없는 경우 아무 처리도 안함
	{
	}
	else			// path가 있는 경우 경로를 만들어줌
	{
		strcat_s(directoryPath, MAX_PATH, "\\");
		strcat_s(directoryPath, MAX_PATH, fileName);
	}
}