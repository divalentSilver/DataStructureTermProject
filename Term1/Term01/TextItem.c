#include <string.h>
#include "TextItem.h"

void TextItem_Create(TextItem* textItem, char* text){
	strcpy(textItem->text, text);
	textItem->totalByte = strlen(text);
}

const char* TextItem_GetText(TextItem* textItem){
	return textItem->text;
}

const int TextItem_GetTotalByte(TextItem textItem){
	return textItem.totalByte;
}