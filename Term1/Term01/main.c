#pragma once
#include "csvParser.h"
#include "GUI.h"
#include "TextButton.h"
#include "Util.h"

#define INF 10000
#define _A 65 //키보드 입력값
#define _a 97
#define _D 68
#define _d 100
#define _p 112
#define _P 80 
#define ESC 27

//경로를 저장하는 구조체
typedef struct _path {
	int num;					//역의 개수
	int path[STATION_NUM + 1];	//역의 index들을 저장한 배열
	double dist;				//거리 총합
}Path;

typedef struct _StationListNode {//즐겨찾기 리스트 노드 구조체
	Station station;
	struct _StationListNode * link;
}StationListNode;

typedef struct {//즐겨찾기 리스트 구조체
	StationListNode * head;
	int length;
}StationList;

void InitGraph();
Path FindPath(int, int);
int choose(int n, int found[], Path paths[]);
void Dijkstra(int start, int dest, int n, int found[], Path paths[]);
int search(char*);
void insert_node(StationListNode ** phead, StationListNode * p, StationListNode * new_node);
void remove_node(StationListNode ** phead, StationListNode * p, StationListNode * remove);
int is_empty(StationList * list);
StationListNode * get_node_at(StationList * list, int position);
void InitializeList(StationList * list);
void AddStation(StationList * list, Station station);
void RemoveStation(StationList * list, int position);
void DisplayList(StationList * list);

Station stations[STATION_NUM + 1];
double weight[STATION_NUM + 1][STATION_NUM + 1];

int main(void) {
	int arr[20][20];
	char str1[100], str2[100];
	Parse(stations);
	InitGraph();
	TransferParse(weight, stations);
	//InitGUI();
	//Title();
	int a, b;
	printf("출발지를 검색하세요: ");
	scanf("%100s", str1);
	a = search(str1);

	printf("도착지를 검색하세요: ");
	scanf("%100s", str2);
	b = search(str2);

	Path p;
	p = FindPath(a, b);//신촌 안암
	printf("총 %d개의 역, %lf 분 소요\n", p.num, p.dist);
	for (int i = 0; i < p.num; i++) {
		printf("%d호선 %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
	}
	//-----------------------------------즐겨찾기 기능
	StationList favorites;
	InitializeList(&favorites);
	int key = 0;
	char input[100];
	int target;
	int position;
	printf("\nA/a: 즐겨찾는 역 추가\nD/d: 즐겨찾는 역 삭제\nP/p: 즐겨찾기 리스트 출력\nEsc: 프로그램 종료\n원하는 기능의 버튼을 누르세요.\n");
	while (1) {//추후에 GUI로 버튼 기능 이용하여 구현할 것
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case _A:
			case _a:
				printf("\n즐겨찾기에 추가할 역을 검색하세요: ");
				scanf("%100s", input);
				target = search(input); //예외처리필요
				printf("%d\n", target);
				AddStation(&favorites, stations[target]);
				printf("%s역이 즐겨찾기에 추가되었습니다.\n\n", stations[target].name);
				break;
			case _D:
			case _d:
				printf("\n삭제할 역의 번호를 입력하세요: ");
				scanf("%d", &position);
				RemoveStation(&favorites, position - 1);
				break;
			case _P:
			case _p:
				printf("\n_______즐겨찾기 리스트_______\n");
				DisplayList(&favorites);
				printf("____________________________\n");
				break;
			case ESC:
				printf("\n프로그램을 종료합니다.\n");
				exit(0);
			}
		}
	}
	//-----------------------------------
}

int search(char *string) {
	//Prototype: needs more efficient algorithm
	int i, k = 0;
	int input;
	int len = strlen(string);
	int list[STATION_NUM] = { 0, };
	for (i = 1; i <= STATION_NUM; i++) {
		if (!strncmp(string, stations[i].name, len))
			list[k++] = i;
	}
	printf("%s(으)로 검색된 역들:\n", string);
	for (i = 0; i < k; i++)
		printf("%d.%d호선 %s\n", i + 1, stations[list[i]].line, stations[list[i]].name);
	scanf("%d", &input);
	return list[input - 1];
}

void InitGraph() {
	int i, j;
	//그래프를 INF 또는 0으로 초기화
	for (i = 0; i <= STATION_NUM; i++) {
		for (j = 0; j <= STATION_NUM; j++)
			weight[i][j] = i == j ? 0 : INF;
	}
	//이전 역까지의 거리이므로 각 호선의 시작역의경우 건너뜀
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
	//특수케이스 간선처리 - 2호선/6호선 순환
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
	static Path paths[STATION_NUM + 1];

	for (int i = 0; i <= STATION_NUM; i++) {
		paths[i].num = 0;
		paths[i].dist = 0.0;
		for (int j = 0; j <= STATION_NUM; j++) {
			paths[i].path[j] = 0;
		}
	}
	Dijkstra(start, dest, STATION_NUM, found, paths);

	return paths[dest];
}
int choose(int n, int found[], Path paths[]) {
	int i, minpos;
	double min;
	min = (double)INT_MAX;
	minpos = -1;
	for (i = 1; i <= n; i++)
		if (paths[i].dist < min && !found[i]) {
			min = paths[i].dist;
			minpos = i;
		}
	return minpos;
}
void Dijkstra(int start, int dest, int n, int found[], Path paths[]) {
	int i, u, w;
	for (i = 1; i <= n; i++) {
		paths[i].dist = weight[start][i];
		found[i] = FALSE;
	}
	
	//시작역에 대한 경로입력
	paths[start].path[0] = start;
	paths[start].num = 1;
	for (i = 1; i <= n - 2; i++) {
		u = choose(n, found, paths);
		found[u] = TRUE;
		for (w = 1; w <= n; w++)
			if (!found[w]) {
				if (paths[u].dist + weight[u][w] <= paths[w].dist) {	//w까지의 최단거리가 u까지의 최단거리 + u->w 일 경우
					paths[w] = paths[u];								//u까지의 경로를 그대로 대입합
					paths[w].path[paths[w].num++] = w;					//경로에 w를 추가함
					paths[w].dist = paths[u].dist + weight[u][w];		//u까지의 거리에 u->w 거리를 추가함
				}
			}

		//도착지에 대한 경로를 찾았을경우
		if (found[dest])
			break;
	}
}

void insert_node(StationListNode ** phead, StationListNode * p, StationListNode * new_node) {
	if (*phead == NULL) {
		new_node->link = NULL;
		*phead = new_node;
	}
	else if (p == NULL) {
		new_node->link = *phead;
		*phead = new_node;
	}
	else {
		new_node->link = p->link;
		p->link = new_node;
	}
}

void remove_node(StationListNode** phead, StationListNode* p, StationListNode* remove) {
	if (p == NULL) {
		*phead = (*phead)->link;
	}
	else {
		p->link = remove->link;
	}
	free(remove);
}

int is_empty(StationList* list) {
	if (list->head == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

StationListNode* get_node_at(StationList* list, int position) {
	StationListNode* tmp = list->head;
	if (position < 0) return NULL;
	for (int i = 0; i < position; i++) {
		tmp = tmp->link;
	}
	return tmp;
}

void InitializeList(StationList* list) {
	if (list == NULL) return;
	list->head = NULL;
	list->length = 0;
}

void AddStation(StationList* list, Station station) {
	int position = list->length;
	StationListNode* p;
	StationListNode* node = (StationListNode*)malloc(sizeof(StationListNode));
	if (node == NULL) printf("memory allocation error\n");
	node->station = station;
	p = get_node_at(list, position - 1);
	insert_node(&list->head, p, node);
	list->length++;
	return;
}

void RemoveStation(StationList* list, int position) {
	if (!is_empty(list) && (position >= 0) && (position < list->length)) {
		StationListNode* p = get_node_at(list, position - 1);
		remove_node(&(list->head), p, (p != NULL) ? p->link : NULL);
		list->length--;
	}
	else {
		printf("error while removing an item from the list\n");
		printf("%d", position);
	}
}

void DisplayList(StationList* list) {
	StationListNode* node = list->head;
	if (is_empty(list)) printf("리스트가 비었습니다.\n");
	for (int i = 0; i < list->length; i++) {
		printf("%d.%d호선 %s\n", i + 1, node->station.line, node->station.name);
		node = node->link;
	}
}
