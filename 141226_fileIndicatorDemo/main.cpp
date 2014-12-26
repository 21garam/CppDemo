#include <stdio.h>

int main()
{
	FILE * fin;

	fopen_s(&fin, "string.txt", "r");		// fopen() : file open. 지정 경로의 파일을 열어 사용(읽기, 쓰기)할 수 있도록 file 구조체에 파일의 정보를 담는다.

	char buffer[8];

	for (int i = 0; i < 6; i++)
	{
		fscanf_s(fin, "%s", buffer, 8);
		printf("%s ", buffer);
	}

	printf("\n  file position : %d\n", ftell(fin));		// ftell() : 파일의 indicator(file position, file pointer)를 반환한다.
	fseek(fin, 0, SEEK_SET);							// fseek() : indicator를 조작한다. indicator를 파일의 시작, 끝, 현재 indicator 위치를 기준으로 offset만큼 떨어진 곳으로 이동시킨다.
	printf("  fseek(fin, 0, SEEK_SET);\n  file position : %d\n\n", ftell(fin));

	printf("string	: file position\n---------------------------\n");

	for (int i = 0; i < 4;i++)		// 'this water is very' 까지 출력
	{ 
		fscanf_s(fin, "%s", buffer, 8);
		printf("%s	: %d\n", buffer, ftell(fin));
	}

	fseek(fin, -4, SEEK_CUR);		// indicator를 현재 위치에서 파일 시작 방향으로 4만큼 이동. 'v'의 위치로
	printf("  fseek(fin, -4, SEEK_CUR);\n  file position : %d\n", ftell(fin));

	fscanf_s(fin, "%s", buffer, 8);
	printf("%s	: %d\n", buffer, ftell(fin));

	fseek(fin, -5, SEEK_END);		// indicator를 파일의 끝에서 파일 시작방향으로 5만큼 이동. 'c'의 위치로
	printf("  fseek(fin, -5, SEEK_END);\n  file position : %d\n", ftell(fin));

	fscanf_s(fin, "%s", buffer, 8);
	printf("%s	: %d\n", buffer, ftell(fin));

	return 0;
}