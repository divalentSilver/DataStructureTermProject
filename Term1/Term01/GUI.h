#pragma once
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

#define _1 49
#define _2 50
#define _3 51
#define _4 52
#define ESC 27
#define MENU_WIDTH 34

char *string[] = { "��","��","��","��","��","��","��","��" };
int color[] = { 1,10,14,11,5,4,2,13 };

void gotoxy(int, int);
void InitGUI();
int Title();
void PrintLineMark(int);
void DrawSubwayMap();
void DrawTitle();
void DrawMainMenu();
void DrawSubMenu(int key);
void GetReadyForInput(int, int);
COORD GetCurrCursorPos();
void setcursortype(CURSOR_TYPE);

HANDLE hConsole;

void gotoxy(int x, int y) {
	COORD Pos = { x, y };
	SetConsoleCursorPosition(hConsole, Pos);
}

COORD GetCurrCursorPos() {
	CONSOLE_SCREEN_BUFFER_INFO curInfo;        // �ܼ� ���â�� ������ ��� ���ؼ� ������ ����ü
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);  //���� Ŀ���� ��ġ ������ �����ϴ� �Լ�
	COORD Pos = { curInfo.dwCursorPosition.X, curInfo.dwCursorPosition.Y};
	return Pos;
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; // hide cursor

void setcursortype(CURSOR_TYPE c) { // hide cursor
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void InitGUI() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
}

void DrawSubwayMap() {
	HINSTANCE hinstance = GetModuleHandle(NULL);
	HWND hWnd = GetConsoleWindow();
	HBITMAP hImage, hOldBitmap;

	HDC hdc = GetDC(hWnd);
	HDC hMemDC = CreateCompatibleDC(hdc);
	hImage = (HBITMAP)LoadImage(NULL, TEXT("SubwayMap.bmp"), //�̹��� ���ϸ� �Է�
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hImage);
	BitBlt(hdc, 330, 20, 1389, 870, hMemDC, 0, 0, SRCCOPY); //�̹��� ��ġ x y ��ǥ, �̹��� ������ x y ũ��

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hImage);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hdc);
}


int Title() {
	int choice = 1;
	COORD cursor = { 12, 6 };
	unsigned char in;
	system("cls");
	gotoxy(10, 5);
	printf("�ƢƢƢƢƢƢƢƢƢƢƢƢ�");
	gotoxy(10, 6);
	printf("��   ����ö ��� ã��   ��");
	gotoxy(10, 7);
	printf("�ƢƢƢƢƢƢƢƢƢƢƢƢ�");
	gotoxy(10, 9);
	printf("�ƢƢƢƢƢƢƢƢƢƢƢƢ�");
	gotoxy(10, 10);
	printf("��                      ��");
	gotoxy(10, 11);
	printf("�ƢƢƢƢƢƢƢƢƢƢƢƢ�");
	gotoxy(10, 13);
	printf("�ƢƢƢƢƢƢƢƢƢƢƢƢ�");
	gotoxy(10, 14);
	printf("��                      ��");
	gotoxy(10, 15);
	printf("�ƢƢƢƢƢƢƢƢƢƢƢƢ�");

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

void DrawTitle() {
	int x = 5;
	int y = 4;

	gotoxy(x, y + 0); printf("��������������������������������������������������������������������������");
	gotoxy(x, y + 1); printf("��                                   ��");
	gotoxy(x, y + 2); printf("��                                   ��");
	gotoxy(x, y + 3); printf("��                                   ��");
	gotoxy(x, y + 4); printf("��������������������������������������������������������������������������");

	gotoxy(x + 11, y + 2); printf("����ö ��� ã��");
	gotoxy(x + 3, y + 7); printf("Please Enter Any Key to Start..");

	while (1) {
		if (_kbhit()) break;
	}

	while (_kbhit()) _getch();
}

void DrawMainMenu() {//���� �ʿ�
	system("cls");
	int x = 0;
	int y = 0;

	//menu bar
	gotoxy(x, y + 0); printf("�ƢƢƢƢƢƢƢƢƢƢƢƢƢƢƢ�");
	for (int i = 1; i <= 5; i++) {
		gotoxy(x, y + i); printf("��                            ��");
	}
	gotoxy(x, y + 3); printf("��          M E N U           ��");
	gotoxy(x, y + 6); printf("�ƢƢƢƢƢƢƢƢƢƢƢƢƢƢƢ�");

	y = 6;
	for (int i = 1; i <= 3; i++) {
		gotoxy(x, y + i); printf("��                            ��");
	}
	gotoxy(x, y + 2); printf("��   1. ����ö �뼱�� ����    ��");
	gotoxy(x, y + 4); printf("�ƢƢƢƢƢƢƢƢƢƢƢƢƢƢƢ�");

	y = 10;
	for (int i = 1; i <= 3; i++) {
		gotoxy(x, y + i); printf("��                            ��");
	}
	gotoxy(x, y + 2); printf("��   2. �ִ� �Ÿ� ��� ã��   ��");
	gotoxy(x, y + 4); printf("�ƢƢƢƢƢƢƢƢƢƢƢƢƢƢƢ�");

	y = 14;
	for (int i = 1; i <= 3; i++) {
		gotoxy(x, y + i); printf("��                            ��");
	}
	gotoxy(x, y + 2); printf("��   3. �ּ� ȯ�� ��� ã��   ��");
	gotoxy(x, y + 4); printf("�ƢƢƢƢƢƢƢƢƢƢƢƢƢƢƢ�");

	y = 18;
	for (int i = 1; i <= 3; i++) {
		gotoxy(x, y + i); printf("��                            ��");
	}
	gotoxy(x, y + 2); printf("��   4. ���ã�� �� ����Ʈ    ��");
	gotoxy(x, y + 4); printf("�ƢƢƢƢƢƢƢƢƢƢƢƢƢƢƢ�");

	y = 24;
	gotoxy(x, y + 0); printf("�ƢƢƢƢƢƢƢƢƢƢƢƢƢƢƢ�");
	for (int i = 1; i <= 23; i++) {
		gotoxy(x, y + i); printf("��                            ��");
	}
	gotoxy(x, y + 2); printf("��  ���ϴ� �޴� ��ȣ �Է�:    ��");
	gotoxy(x, y + 3); printf("��____________________________��");
	gotoxy(x, y + 24); printf("�ƢƢƢƢƢƢƢƢƢƢƢƢƢƢƢ�");

	GetReadyForInput(x + 27, y + 2);

}

void DrawSubMenu(int key) {
	int x = 4;
	int y = 29;
	gotoxy(x, y + 0); printf("<%c�� �޴��� �ΰ� ���>", key);
	switch (key) {
	case _1://����ö �뼱�� ����
		gotoxy(x, y + 2); printf("Esc: ���\n");
		//gotoxy(x, y + 4); printf("���ϴ� ��� ��ȣ �Է�: ");
		//GetReadyForInput(x + 23, y + 4);
		break;
	case _2://�ִ� �Ÿ� ��� ã��
		//Ŀ�� �������� �׸� �������� ������ ���� ó����
		break;
	case _3://�ּ� ȯ�� ��� ã��
		//Ŀ�� �������� �׸� �������� ������ ���� ó����
		break;
	case _4://���ã�� �� ����Ʈ
		gotoxy(x, y + 2); printf("1. ���ã�� �� �߰�\n");
		gotoxy(x, y + 3); printf("2. ���ã�� �� ����\n");
		gotoxy(x, y + 4); printf("3. ���ã�� ����Ʈ ���\n");
		gotoxy(x, y + 5); printf("Esc: ���\n");
		gotoxy(x, y + 7); printf("���ϴ� ��� ��ȣ �Է�: ");
		break;
	default:
		break;
	}
}

void GetReadyForInput(int x, int y) {
	setcursortype(NORMALCURSOR);
	gotoxy(x, y);
}

void PrintLineMark(int line) {
	for (int k = 0; k < 8; k++) {
		gotoxy(10, k * 2);
		SetConsoleTextAttribute(hConsole, color[k]);
		printf("%s", string[k]);
		SetConsoleTextAttribute(hConsole, 7); 
	}
}
