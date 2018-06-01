#pragma once
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

char *string[] = { "��","��","��","��","��","��","��","��" };
int color[] = { 1,10,14,11,5,4,2,13 };

void gotoxy(int, int);
void InitGUI();
int Title();
void PrintLineMark(int);

HANDLE hConsole;

void gotoxy(int x, int y) {
	COORD Pos = { x, y };
	SetConsoleCursorPosition(hConsole, Pos);
}

void InitGUI() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
}

int Title() {
	int choice = 1;
	COORD cursor = { 12, 6 };
	unsigned char in;
	system("cls");
	gotoxy(10, 5);
	printf("��������������");
	gotoxy(10, 6);
	printf("��   ����ö ��� ã��   ��");
	gotoxy(10, 7);
	printf("��������������");
	gotoxy(10, 9);
	printf("��������������");
	gotoxy(10, 10);
	printf("��                      ��");
	gotoxy(10, 11);
	printf("��������������");
	gotoxy(10, 13);
	printf("��������������");
	gotoxy(10, 14);
	printf("��                      ��");
	gotoxy(10, 15);
	printf("��������������");

	gotoxy(cursor.X, cursor.Y);
	printf("��");

	while ((in = getch()) != 13) { //���� Ű
		switch (in) {
			case 224:
				switch (getch()) {
					case 72: //�� ȭ��ǥ
						if (choice > 1) {
							gotoxy(cursor.X, cursor.Y);
							printf("  ");
							choice--;
							cursor.Y -= 4;
							gotoxy(cursor.X, cursor.Y);
							printf("��");
						}
						break;

					case 80: //�Ʒ� ȭ��ǥ
						if (choice < 3) {
							gotoxy(cursor.X, cursor.Y);
							printf("  ");
							choice++;
							cursor.Y += 4;
							gotoxy(cursor.X, cursor.Y);
							printf("��");
						}
						break;

					default:
						break;
				}
				break;

			default:
				break;
		}
	}

	return choice;
}

void PrintLineMark(int line) {
	for (int k = 0; k < 8; k++) {
		gotoxy(10, k * 2);
		SetConsoleTextAttribute(hConsole, color[k]);
		printf("%s", string[k]);
		SetConsoleTextAttribute(hConsole, 7); 
	}
}
