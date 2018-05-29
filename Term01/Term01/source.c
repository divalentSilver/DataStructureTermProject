#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "csvParser.h"

Station stations[STATION_NUM];
int main(void) {
	Parse(stations);
}