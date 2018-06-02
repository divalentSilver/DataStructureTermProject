#pragma once
#include <stdio.h>
#include "csvParser.h"
#include "GUI.h"

#define INF 10000

typedef struct _path {
	int num;
	int path[STATION_NUM + 1];
	double dist;
}Path;

void InitGraph();
int choose(int, int[], int[]);
Path FindPath(int, int);
void Dijkstra(int, int, int, int[], int[], Path[]);

Station stations[STATION_NUM + 1];
double weight[STATION_NUM + 1][STATION_NUM + 1];


int main(void) {
	Parse(stations);
	InitGraph();
	//InitGUI();
	//Title();
	Path p;
	p = FindPath(102, 276);//신촌 안암
	for (int i = 0; i < p.num; i++) {
		printf("%d호선 %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
	}
}

void InitGraph() {
	int i, j;
	for (i = 0; i <= STATION_NUM; i++) {
		for (j = 0; j <= STATION_NUM; j++)
			weight[i][j] = i == j ? 0 : INF;
	}
	TransferParse(weight);
	for (i = 1; i <= STATION_NUM; i++) {
		if (i == 63	||	//2호선 시청
			i == 106||	//2호선 성수지선 용답
			i == 110||	//2호선 신정지선 도림천
			i == 114||	//3호선 지축
			i == 148||	//4호선 노원
			i == 196||	//5호선 방화
			i == 240||	//5호선 마천지선
			i == 247||  //6호선 응암
			i == 285||	//7호선 장암
			i == 336	//8호선 암사
			) continue;
		weight[i - 1][i] = stations[i].spacing;
		weight[i][i - 1] = stations[i].spacing;
	}

	//2호선 순환
	weight[105][63] = stations[63].spacing;	//충정로->시청
	weight[63][105] = stations[63].spacing; //시청->충정로
	//응암 순환
	weight[248][247] = INF;	//역촌->응암 불가능
	weight[249][248] = INF; //불광->역촌 불가능
	weight[250][249] = INF; //독바위->불광 불가능
	weight[251][250] = INF; //연신내->독바위 불가능
	weight[252][251] = INF; //구산->연신내 불가능
	weight[252][253] = INF;	//구산->새절 불가능
	weight[253][252] = INF;	//새절->구산 불가능

	weight[252][247] = stations[247].spacing;	//구산->응암 가능
	weight[247][253] = stations[253].spacing;	//응암->새절 가능
	weight[253][247] = stations[253].spacing;	//새절->응암 가능
}

Path FindPath(int start, int dest) {
	static int found[STATION_NUM + 1] = { 0, };
	static int distance[STATION_NUM + 1] = { 0, };
	static Path paths[STATION_NUM + 1];

	for (int i = 0; i <= STATION_NUM; i++) {
		paths[i].num = 0;
		paths[i].dist = 0.0;
		for (int j = 0; j <= STATION_NUM; j++) {
			paths[i].path[j] = 0;
		}
	}
	Dijkstra(start, dest, STATION_NUM, distance, found, paths);

	return paths[dest];
}
int choose(int n, int distance[], int found[]) {
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for (i = 1; i <= n; i++)
		if (distance[i] < min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	return minpos;
}
void Dijkstra(int start, int dest, int n, int distance[], int found[], Path paths[]) {
	int i, u, w;
	for (i = 1; i <= n; i++) {
		distance[i] = weight[start][i];
		found[i] = FALSE;
	}
	found[start] = TRUE; // 시작 정점 방문 표시
	distance[start] = 0;
	for (i = 1; i <= n - 2; i++) {
		u = choose(n, distance, found);
		found[u] = TRUE;
		for (w = 1; w <= n; w++)
			if (!found[w])
				if (distance[u] + weight[u][w] < distance[w]) {
					distance[w] = distance[u] + weight[u][w];
					paths[w] = paths[u];
					paths[w].path[paths[w].num++] = w;
					paths[w].dist = distance[w];
				}
	}
}