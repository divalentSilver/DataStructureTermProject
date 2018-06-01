#pragma once
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

char *string[] = { "¨ç","¨è","¨é","¨ê","¨ë","¨ì","¨í","¨î" };
int color[] = { 1,10,14,11,5,4,2,13 };

HANDLE hConsole;

void Init();
void gotoxy(int, int);
void Title();
void PrintLineMark(int);


void Init() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void gotoxy(int x, int y) {
	COORD Pos = { x, y };
	SetConsoleCursorPosition(hConsole, Pos);
}

void Title() {
	printf("This is Title\n");
	PrintLineMark(0);
}

void PrintLineMark(int line) {
	for (int k = 0; k < 8; k++) {
		gotoxy(10, k * 2);
		SetConsoleTextAttribute(hConsole, color[k]);
		printf("%s", string[k]);
		SetConsoleTextAttribute(hConsole, 7);
	}
}
