#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATION_NUM 352

typedef struct _station {
	char code[6];
	int line;
	char name[100];
	double spacing;
} Station;

int CodeToIndex(char code[]) {
	if (strncmp(code, "211-", 4) == 0)
		return 106 + (int)code[5] - '1';
	if (strncmp(code, "234-", 4) == 0)
		return 110 + (int)code[5] - '1';
	if (code[0] == '1')
		return atoi(code) - 99;
	if (code[0] == '2')
		return atoi(code) - 201 + 63;
	if (code[0] == '3')
		return atoi(code) - 319 + 114;
	if (code[0] == '4')
		return atoi(code) - 409 + 148;
	if (code[0] == '5')
		return atoi(code) - 510 + 196;
	if (code[0] == 'P')
		return atoi(&code[1]) - 549 + 240;
	if (code[0] == '6')
		return atoi(code) - 610 + 247;
	if (code[0] == '7')
		return atoi(code) - 709 + 285;
	if (code[0] == '8')
		return atoi(code) - 810 + 336;
	return 0;
}

int Parse(Station stations[]) {
	FILE* fp;
	fopen_s(&fp, "testdata.csv", "rb");
	if (fp != NULL) {	
		while (!feof(fp)) {	
			for (int i = 0; i < STATION_NUM; i++) {
				fscanf(fp, "%[^,],%d,%[^,],%lf\n", 
					&stations[i].code,
					&stations[i].line, 
					&stations[i].name, 
					&stations[i].spacing);
				
				printf("%3d: %-6s %d, %-35s, %3.1f\n", 
					i,
					stations[i].code,
					stations[i].line, 
					stations[i].name, 
					stations[i].spacing);
					
			}
		}
	}
	else printf("파일을 찾을 수 없습니다.\n");
	fclose(fp);
	return 0;
}

void TransferParse(double weight[STATION_NUM][STATION_NUM]) {
	FILE *fp;
	char string[10];
	fopen_s(&fp, "transfer.csv", "rb");
	if (fp == NULL)
		return;
	while (!feof(fp)) {
		int i1, i2;
		for (int i = 0; i < 93; i++) {
			char s1[10];
			char s2[10];
			fscanf(fp, "%[^,],%[^,]\n", s1, s2);
			i1 = CodeToIndex(s1);
			i2 = CodeToIndex(s2);
			weight[i1][i2] = 5.0;
		}
		break;
	}
	fclose(fp);
}