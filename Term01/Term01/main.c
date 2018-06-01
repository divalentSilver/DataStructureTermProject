#pragma once
#include <stdio.h>
#include "csvParser.h"
#include "GUI.h"

#define INF 10000

Station stations[STATION_NUM];
double weight[STATION_NUM][STATION_NUM];

void InitGraph();

int main(void) {
	Parse(stations);
	InitGraph();
	TransferParse(weight);
	InitGUI();
	//Title();	
}

void InitGraph() {
	for (int i = 0; i < STATION_NUM; i++) {
		for (int j = 0; j < STATION_NUM; j++)
			weight[i][j] = i == j ? 0 : INF;
	}
}

