#ifndef _TEXT_BUTTON_H
#define _TEXT_BUTTON_H

#include "TextItem.h"
#include "Point.h"

typedef struct _textButton{
	TextItem text;
	int width;
	int height;
	Point startPoint;
}TextButton;

void TextButton_Create(TextButton* textButton, char* text, int startPositionXToPrint, int startPositionYToPrint);
void TextButton_Print(TextButton* textButton, int color);
const char* TextButton_ProcessKey(TextButton* textButton);

#endif