#include <stdio.h>

#include <stdlib.h>
#include <time.h>

int main()
{
	FILE *fin;
	FILE *fout;

	fopen_s(&fin, "input.txt", "rb");		// �Է� ����. b �ɼ��� binary
	fopen_s(&fout, "output.txt", "wb");		// ��� ����

	char buf[256];

	printf("fscanf(), fprintf()\n");
	fscanf_s(fin, "%s", buf, 256);		// fscanf() : ��Ʈ������ ����(' ')�̳� ���๮�� ���� ���� �� ���� �о ���ۿ� ��. �����̳� ���๮�ڴ� ���ۿ� ���� �ʴ´�
	fprintf_s(fout, "%s", buf, 256);	// fprintf() : ���ۿ��� NULL ����Ʈ�� ���� �� ���� ��Ʈ���� ��
	printf("%s	: %d\n", buf, ftell(fin));

	printf("\nfread(buf, 1, 16, fin), fwrite(buf, 1, 16, fout)\n");
	int count = fread_s(buf, 256, sizeof(char), 16, fin);	// fread() : ��Ʈ���� ���ڿ��� element size��ŭ count�� �о ������ ���� ȸ���� ��ȯ
	fwrite(buf, sizeof(char), count, fout);					// fwrite() : ������ ���ڿ��� element size��ŭ�� count�� ��Ʈ���� ��
	fwrite(buf, sizeof(char), count, stdout);				// ǥ�� ����� ��Ʈ���� ��� ����
	printf("	: %d\n", ftell(fin));

	printf("\nfread(buf, 16, 1, fin), fwrite(buf, 16, 1, fout)\n");
	count = fread_s(buf, 256, 16, 1, fin);			// ��� �� ���� ����� ���� ����
	fwrite(buf, 16, count, fout);
	fwrite(buf, 16, count, stdout);
	printf("	: %d\n", ftell(fin));

	printf("\nfgetc(), fputc()\n");
	char c = (char)fgetc(fin);			// fgetc() : ��Ʈ�����κ��� 1���� ���ڸ� �о�鿩 ��ȯ�Ѵ�
	fputc(c, fout);						// fputc() : ��Ʈ���� 1���� ���ڸ� ����
	printf("%c	: %d\n", c, ftell(fin));

	printf("\nfgets(), fputs()\n");
	fgets(buf, 256, fin);				// fgets() : ��Ʈ������ ���๮��(windows ������ CR LF)�� �����ų� MaxCount��ŭ ���� �� ���ۿ� ��. ���๮�ڱ��� ������� ���๮�ڱ��� ���ۿ� ��
	fputs(buf, fout);					// fputs() : ���ۿ��� NULL�� ���� �� ������ ���ڿ��� ��Ʈ���� ��
	printf("%s	: %d\n", buf, ftell(fin));

	fclose(fin);
	fclose(fout);

	return 0;
}