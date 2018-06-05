#ifndef _TEXT_ITEM_H
#define _TEXT_ITEM_H

#define TEXT_SIZE 128

typedef struct _textItem{
	char text[TEXT_SIZE];
	int totalByte;
}TextItem;

void TextItem_Create(TextItem* textItem, char* text);
const char* TextItem_GetText(TextItem* textItem);
const int TextItem_GetTotalByte(TextItem textItem);

#endif