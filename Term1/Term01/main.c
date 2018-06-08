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
#define MENU_WIDTH 34

//경로를 저장하는 구조체
typedef struct _path {
	int num;					//역의 개수
	int transfer;				//환승 회수

	int path[STATION_NUM + 1];	//역의 index들을 저장한 배열 (-1: 환승처리)
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
Path FindMinTransfer(int start, int dest);
int choose(int n, int found[], Path paths[]);
void Dijkstra(int start, int dest, int n, int found[], Path paths[]);
void FindLeastTransferPath();
void FindShortestDistPath();
void EditFavorites();
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
Transfer transfer[TRANSFER_NUM];
double weight[STATION_NUM + 1][STATION_NUM + 1];

int main(void) {
	//-------------------------콘솔창 설정
	setcursortype(NOCURSOR);
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	system("mode con cols=200 lines=50");
	system("title 지하철 어플리케이션");
	system("color f0");
	//------------------------
	Parse(stations);
	InitGraph();
	TransferParse(weight, stations, transfer);
	InitGUI();
	DrawTitle();//좀 더 깔끔한 UI로 수정 필요.
	DrawMain();
	GetReadyForInput(MENU_WIDTH, 3);
	//testcode----
	FindLeastTransferPath();
	//------------
	while (1) {
		int key = 0;
		if (_kbhit()) {//선택한 메뉴에 따라 함수호출
			key = _getch();
			printf("%c", key);
			switch (key) {
			case 49://지하철 노선도 보기
				setcursortype(NOCURSOR);
				ShowSubwayMap();
				break;
			case 50://최단 거리 경로 찾기
				GetReadyForInput(MENU_WIDTH, 3);
				FindShortestDistPath();
				break;
			case 51://즐겨찾는 역 리스트
				GetReadyForInput(MENU_WIDTH, 3);
				EditFavorites();
				break;
			case ESC://프로그램 종료
				exit(0);
			default:
				break;
			}
		}
		//while (_kbhit()) system("pause");
		//system("cls");
		//DrawTitle();
	}

	system("pause>null");//계속하려면 아무키나 누르시오 없앰
}

void FindShortestDistPath() {
	//int arr[20][20];
	char str1[100], str2[100];
	int a, b;
	printf("출발지를 검색하세요: ");//노선도 보고 찾기 & 즐겨찾는역 보고 찾기 & 검색해서 찾기 기능 세분화 필요
	scanf("%100s", str1);
	a = search(str1);

	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("도착지를 검색하세요: ");
	scanf("%100s", str2);
	b = search(str2);

	Path p;
	p = FindPath(a, b);//신촌 안암
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("총 %d개의 역, %lf 분 소요\n", p.num, p.dist);
	for (int i = 0; i < p.num; i++) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%d호선 %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
	}
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("**경로 찾기 완료**");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- 메뉴를 선택해주세요");//화면 길이 넘어가는거 예외처리 필요
}

void FindLeastTransferPath() {
	char str1[100], str2[100];
	int a, b;
	printf("출발지를 검색하세요: ");//노선도 보고 찾기 & 즐겨찾는역 보고 찾기 & 검색해서 찾기 기능 세분화 필요
	scanf("%100s", str1);
	a = search(str1);

	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("도착지를 검색하세요: ");
	scanf("%100s", str2);
	b = search(str2);

	Path p;
	p = FindMinTransfer(a, b);//신촌 안암
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("총 %d개의 역, %lf 분 소요\n", p.num, p.dist);
	for (int i = 0; i < p.num; i++) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%d호선 %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
	}
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("**경로 찾기 완료**");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- 메뉴를 선택해주세요");//화면 길이 넘어가는거 예외처리 필요
}

void EditFavorites() {
	StationList favorites;
	InitializeList(&favorites);
	int key = 0;
	char input[100];
	int target;
	int position;
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
	printf("A/a: 즐겨찾는 역 추가\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
	printf("D/d: 즐겨찾는 역 삭제\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
	printf("P/p: 즐겨찾기 리스트 출력\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
	printf("Esc: 취소\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
	printf("원하는 기능을 선택하세요: ");
	while (1) {//추후에 GUI로 버튼 기능 이용하여 구현할 것
		if (_kbhit()) {
			key = _getch();
			printf("%c", key);
			switch (key) {
			case _A:
			case _a:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 1);
				printf("즐겨찾기에 추가할 역을 검색하세요: ");
				scanf("%100s", input);
				target = search(input); //예외처리필요
				printf("%d\n", target);
				AddStation(&favorites, stations[target]);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("%s역이 즐겨찾기에 추가되었습니다.\n\n", stations[target].name);
				break;
			case _D:
			case _d:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("삭제할 역의 번호를 입력하세요: ");
				scanf("%d", &position);
				RemoveStation(&favorites, position - 1);
				break;
			case _P:
			case _p:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("_______즐겨찾기 리스트_______\n");
				DisplayList(&favorites);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("____________________________\n");
				break;
			case ESC:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("즐겨찾기 수정이 완료되었습니다.");
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- 메뉴를 선택해주세요");//화면 길이 넘어가는거 예외처리 필요
				break;
			}
		}
	}
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
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%s(으)로 검색된 역들:\n", string);
	for (i = 0; i < k; i++) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%d.%d호선 %s\n", i + 1, stations[list[i]].line, stations[list[i]].name);
	}
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); scanf("%d", &input);
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
		paths[i].transfer = 0;
		for (int j = 0; j <= STATION_NUM; j++) {
			paths[i].path[j] = 0;
		}
	}
	Dijkstra(start, dest, STATION_NUM, found, paths);

	return paths[dest];
}
Path FindMinTransfer(int start, int dest) {
	Path p;
	p.dist = (double)INT_MAX;
	int arr[5] = { 0, };
	int n = FindTransfer(stations[start].line, stations[dest].line, arr, transfer);
	
	for (int i = 0; i < n; i++) {
		Transfer t = transfer[arr[i]];
		Path p1 = FindPath(start, t.fromIndex);
		Path p2 = FindPath(t.toIndex, dest);
		if (p1.dist + p2.dist + 5.0 < p.dist) {
			p = p1;
			for (int j = 0; j < p2.num; j++)
				p.path[p.num++] = p2.path[j];
			p.dist += 5.0;
		}
	}
	return p;
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

	paths[dest].dist += (double)paths[dest].num * 0.5;	//정차시간 보정
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
	if (is_empty(list)) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
		printf("리스트가 비었습니다.\n");
	}
	for (int i = 0; i < list->length; i++) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
		printf("%d.%d호선 %s\n", i + 1, node->station.line, node->station.name);
		node = node->link;
	}
}
