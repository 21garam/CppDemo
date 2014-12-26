#include <stdio.h>

int main()
{
	FILE * fin;

	fopen_s(&fin, "string.txt", "r");		// fopen() : file open. ���� ����� ������ ���� ���(�б�, ����)�� �� �ֵ��� file ����ü�� ������ ������ ��´�.

	char buffer[8];

	for (int i = 0; i < 6; i++)
	{
		fscanf_s(fin, "%s", buffer, 8);
		printf("%s ", buffer);
	}

	printf("\n  file position : %d\n", ftell(fin));		// ftell() : ������ indicator(file position, file pointer)�� ��ȯ�Ѵ�.
	fseek(fin, 0, SEEK_SET);							// fseek() : indicator�� �����Ѵ�. indicator�� ������ ����, ��, ���� indicator ��ġ�� �������� offset��ŭ ������ ������ �̵���Ų��.
	printf("  fseek(fin, 0, SEEK_SET);\n  file position : %d\n\n", ftell(fin));

	printf("string	: file position\n---------------------------\n");

	for (int i = 0; i < 4;i++)		// 'this water is very' ���� ���
	{ 
		fscanf_s(fin, "%s", buffer, 8);
		printf("%s	: %d\n", buffer, ftell(fin));
	}

	fseek(fin, -4, SEEK_CUR);		// indicator�� ���� ��ġ���� ���� ���� �������� 4��ŭ �̵�. 'v'�� ��ġ��
	printf("  fseek(fin, -4, SEEK_CUR);\n  file position : %d\n", ftell(fin));

	fscanf_s(fin, "%s", buffer, 8);
	printf("%s	: %d\n", buffer, ftell(fin));

	fseek(fin, -5, SEEK_END);		// indicator�� ������ ������ ���� ���۹������� 5��ŭ �̵�. 'c'�� ��ġ��
	printf("  fseek(fin, -5, SEEK_END);\n  file position : %d\n", ftell(fin));

	fscanf_s(fin, "%s", buffer, 8);
	printf("%s	: %d\n", buffer, ftell(fin));

	return 0;
}