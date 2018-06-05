#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "TextButton.h"
#include "Util.h"

enum KeyCode{
	ENTER = 13 
};

enum Boolean{
	False, True
};

void TextButton_Create(TextButton* textButton, char* text, int x, int y){
	TextItem_Create(&textButton->text, text);
	textButton->width = TextItem_GetTotalByte(textButton->text);
	textButton->height = 1;
	textButton->startPoint.x = x;
	textButton->startPoint.y = y;
}

void TextButton_Print(TextButton* textButton, int color){
	int i;
	int j;
	Point startPoint = textButton->startPoint;
	CursorUtil_GotoXY(startPoint.x, startPoint.y++);
	FontUtil_ChangeFontColor(WHITE);
	printf("┏");
	for (i = 0; i < textButton->width + 1; i++){
		printf("━");
	}
	printf("┓");
	for (i = 0; i < textButton->height; i++){
		CursorUtil_GotoXY(startPoint.x, startPoint.y++);
		FontUtil_ChangeFontColor(WHITE);
		printf("┃");
		FontUtil_ChangeFontColor(color);
		printf(" %s", TextItem_GetText(&textButton->text));
		for (j = 0; j < textButton->width - TextItem_GetTotalByte(textButton->text); j++){
			printf(" ");
		}
		FontUtil_ChangeFontColor(WHITE);
		printf("┃");
	}
	CursorUtil_GotoXY(startPoint.x, startPoint.y++);
	printf("┗");
	for (i = 0; i < textButton->width + 1; i++){
		printf("━");
	}
	printf("┛");
}

const char* TextButton_ProcessKey(TextButton* textButton){
	while (True){
		if (_kbhit()){
			int key = _getch();
			if (key == ENTER){
				break;
			}
		}
	}
	TextButton_Print(textButton, WHITE_INVERSION);
	Sleep(100); // button click effect time
	TextButton_Print(textButton, WHITE);
	return TextItem_GetText(&textButton->text);
}

void DisplayTextButton(int x, int y, char text[]) { //지정한 위치와 텍스트로 화면에 버튼 출력
	TextButton textButton = { 0, };
	TextButton_Create(&textButton, text, x, y);
	TextButton_Print(&textButton, WHITE);
	TextButton_ProcessKey(&textButton);
	//printf("\n\n누른 버튼 문자열 내용 : %s\n", TextButton_ProcessKey(&textButton));
}
