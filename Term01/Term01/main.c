#pragma once
#include <stdio.h>
#include "csvParser.h"
#include "GUI.h"

Station stations[STATION_NUM];

int main(void) {
	Parse(stations);
	Init();
	Title();
	
}

void Dijkstra(char Src) {

}

