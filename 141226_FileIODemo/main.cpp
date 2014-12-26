#include <stdio.h>

#include <stdlib.h>
#include <time.h>

int main()
{
	FILE *fin;
	FILE *fout;

	fopen_s(&fin, "input.txt", "rb");		// 입력 파일. b 옵션은 binary
	fopen_s(&fout, "output.txt", "wb");		// 출력 파일

	char buf[256];

	printf("fscanf(), fprintf()\n");
	fscanf_s(fin, "%s", buf, 256);		// fscanf() : 스트림에서 공백(' ')이나 개행문자 등을 만날 때 까지 읽어서 버퍼에 씀. 공백이나 개행문자는 버퍼에 쓰지 않는다
	fprintf_s(fout, "%s", buf, 256);	// fprintf() : 버퍼에서 NULL 바이트를 만날 때 까지 스트림에 씀
	printf("%s	: %d\n", buf, ftell(fin));

	printf("\nfread(buf, 1, 16, fin), fwrite(buf, 1, 16, fout)\n");
	int count = fread_s(buf, 256, sizeof(char), 16, fin);	// fread() : 스트림의 문자열을 element size만큼 count번 읽어서 실제로 읽은 회수를 반환
	fwrite(buf, sizeof(char), count, fout);					// fwrite() : 버퍼의 문자열을 element size만큼씩 count번 스트림에 씀
	fwrite(buf, sizeof(char), count, stdout);				// 표준 입출력 스트림도 사용 가능
	printf("	: %d\n", ftell(fin));

	printf("\nfread(buf, 16, 1, fin), fwrite(buf, 16, 1, fout)\n");
	count = fread_s(buf, 256, 16, 1, fin);			// 길게 한 번에 입출력 수행 가능
	fwrite(buf, 16, count, fout);
	fwrite(buf, 16, count, stdout);
	printf("	: %d\n", ftell(fin));

	printf("\nfgetc(), fputc()\n");
	char c = (char)fgetc(fin);			// fgetc() : 스트림으로부터 1개의 문자를 읽어들여 반환한다
	fputc(c, fout);						// fputc() : 스트림에 1개의 문자를 쓴다
	printf("%c	: %d\n", c, ftell(fin));

	printf("\nfgets(), fputs()\n");
	fgets(buf, 256, fin);				// fgets() : 스트림에서 개행문자(windows 에서는 CR LF)를 만나거나 MaxCount만큼 읽은 후 버퍼에 씀. 개행문자까지 읽은경우 개행문자까지 버퍼에 씀
	fputs(buf, fout);					// fputs() : 버퍼에서 NULL을 만날 때 까지의 문자열을 스트림에 씀
	printf("%s	: %d\n", buf, ftell(fin));

	fclose(fin);
	fclose(fout);

	return 0;
}