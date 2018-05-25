#include <stdio.h>
#define STATION_NUM 332

typedef struct _station {
	int line;
	char name[100];
	double spacing;
	double spacingSum;
} Station;

int main() {
	Station stations[STATION_NUM];
	FILE* fp;
	fopen_s(&fp, "stationSpacing_extended.csv", "rb");
	if (fp != NULL) {	
		while (!feof(fp)) {	
			for (int i = 0; i < STATION_NUM; i++) {
				fscanf(fp, "%d,%[^,],%lf,%lf\n", 
					&stations[i].line, 
					&stations[i].name, 
					&stations[i].spacing, 
					&stations[i].spacingSum);
				printf("%3d: %d, %-35s, %3.1f, %3.1f\n", 
					i,
					stations[i].line, 
					stations[i].name, 
					stations[i].spacing, 
					stations[i].spacingSum);
			}
		}
	}
	else printf("파일을 찾을 수 없습니다.\n");
	fclose(fp);
	return 0;
}