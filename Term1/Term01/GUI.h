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

char *string[] = { "①","②","③","④","⑤","⑥","⑦","⑧" };
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
	CONSOLE_SCREEN_BUFFER_INFO curInfo;        // 콘솔 출력창의 정보를 담기 위해서 정의한 구조체
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);  //현재 커서의 위치 정보를 저장하는 함수
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
	hImage = (HBITMAP)LoadImage(NULL, TEXT("SubwayMap.bmp"), //이미지 파일명 입력
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hImage);
	BitBlt(hdc, 330, 20, 1389, 870, hMemDC, 0, 0, SRCCOPY); //이미지 위치 x y 좌표, 이미지 사이즈 x y 크기

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
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒");
	gotoxy(10, 6);
	printf("▒   지하철 경로 찾기   ▒");
	gotoxy(10, 7);
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒");
	gotoxy(10, 9);
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒");
	gotoxy(10, 10);
	printf("▒                      ▒");
	gotoxy(10, 11);
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒");
	gotoxy(10, 13);
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒");
	gotoxy(10, 14);
	printf("▒                      ▒");
	gotoxy(10, 15);
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒");

	gotoxy(cursor.X, cursor.Y);
	printf("▶");

	while ((in = getch()) != 13) { //엔터 키
		switch (in) {
			case 224:
				switch (getch()) {
					case 72: //위 화살표
						if (choice > 1) {
							gotoxy(cursor.X, cursor.Y);
							printf("  ");
							choice--;
							cursor.Y -= 4;
							gotoxy(cursor.X, cursor.Y);
							printf("▶");
						}
						break;

					case 80: //아래 화샬표
						if (choice < 3) {
							gotoxy(cursor.X, cursor.Y);
							printf("  ");
							choice++;
							cursor.Y += 4;
							gotoxy(cursor.X, cursor.Y);
							printf("▶");
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

	gotoxy(x, y + 0); printf("┌───────────────────────────────────┐");
	gotoxy(x, y + 1); printf("│                                   │");
	gotoxy(x, y + 2); printf("│                                   │");
	gotoxy(x, y + 3); printf("│                                   │");
	gotoxy(x, y + 4); printf("└───────────────────────────────────┘");

	gotoxy(x + 11, y + 2); printf("지하철 경로 찾기");
	gotoxy(x + 3, y + 7); printf("Please Enter Any Key to Start..");

	while (1) {
		if (_kbhit()) break;
	}

	while (_kbhit()) _getch();
}

void DrawMainMenu() {//수정 필요
	system("cls");
	int x = 0;
	int y = 0;

	//menu bar
	gotoxy(x, y + 0); printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
	for (int i = 1; i <= 5; i++) {
		gotoxy(x, y + i); printf("▒                            ▒");
	}
	gotoxy(x, y + 3); printf("▒          M E N U           ▒");
	gotoxy(x, y + 6); printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

	y = 6;
	for (int i = 1; i <= 3; i++) {
		gotoxy(x, y + i); printf("▒                            ▒");
	}
	gotoxy(x, y + 2); printf("▒   1. 지하철 노선도 보기    ▒");
	gotoxy(x, y + 4); printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

	y = 10;
	for (int i = 1; i <= 3; i++) {
		gotoxy(x, y + i); printf("▒                            ▒");
	}
	gotoxy(x, y + 2); printf("▒   2. 최단 거리 경로 찾기   ▒");
	gotoxy(x, y + 4); printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

	y = 14;
	for (int i = 1; i <= 3; i++) {
		gotoxy(x, y + i); printf("▒                            ▒");
	}
	gotoxy(x, y + 2); printf("▒   3. 최소 환승 경로 찾기   ▒");
	gotoxy(x, y + 4); printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

	y = 18;
	for (int i = 1; i <= 3; i++) {
		gotoxy(x, y + i); printf("▒                            ▒");
	}
	gotoxy(x, y + 2); printf("▒   4. 즐겨찾는 역 리스트    ▒");
	gotoxy(x, y + 4); printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

	y = 24;
	gotoxy(x, y + 0); printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
	for (int i = 1; i <= 23; i++) {
		gotoxy(x, y + i); printf("▒                            ▒");
	}
	gotoxy(x, y + 2); printf("▒  원하는 메뉴 번호 입력:    ▒");
	gotoxy(x, y + 3); printf("▒____________________________▒");
	gotoxy(x, y + 24); printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");

	GetReadyForInput(x + 27, y + 2);

}

void DrawSubMenu(int key) {
	int x = 4;
	int y = 29;
	gotoxy(x, y + 0); printf("<%c번 메뉴의 부가 기능>", key);
	switch (key) {
	case _1://지하철 노선도 보기
		gotoxy(x, y + 2); printf("Esc: 취소\n");
		//gotoxy(x, y + 4); printf("원하는 기능 번호 입력: ");
		//GetReadyForInput(x + 23, y + 4);
		break;
	case _2://최단 거리 경로 찾기
		//커서 지나가면 그림 지워지는 문제로 따로 처리함
		break;
	case _3://최소 환승 경로 찾기
		//커서 지나가면 그림 지워지는 문제로 따로 처리함
		break;
	case _4://즐겨찾는 역 리스트
		gotoxy(x, y + 2); printf("1. 즐겨찾는 역 추가\n");
		gotoxy(x, y + 3); printf("2. 즐겨찾는 역 삭제\n");
		gotoxy(x, y + 4); printf("3. 즐겨찾기 리스트 출력\n");
		gotoxy(x, y + 5); printf("Esc: 취소\n");
		gotoxy(x, y + 7); printf("원하는 기능 번호 입력: ");
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
