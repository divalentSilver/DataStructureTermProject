#pragma once
#include "csvParser.h"
#include "GUI.h"
#include "TextButton.h"
#include "Util.h"

#define INF 10000
#define _1 49
#define _2 50
#define _3 51
#define _4 52
#define ESC 27
#define ENTER 13
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
void ShowSubwayMap();
int SetTargetStation(char message[]);
void FindLeastTransferPath();
void FindShortestDistPath();
int EditFavorites();
int search(char *string);
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
StationList favorites;


int main(void) {
	//콘솔창 설정-------------------------
	setcursortype(NOCURSOR);
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	system("mode con cols=190 lines=60");
	system("title 지하철 어플리케이션");
	system("color f0");

	//Initializing-----------------------
	Parse(stations);
	InitGraph();
	InitializeList(&favorites);
	TransferParse(weight, stations, transfer);
	InitGUI();
	DrawTitle();//좀 더 깔끔한 UI로 수정 필요.
	DrawMainMenu();

	while (1) {
		int key = 0;
		if (_kbhit()) {//선택한 메뉴에 따라 함수호출
			key = _getch();
			switch (key) {
			case _1://지하철 노선도 보기
				system("cls");
				DrawMainMenu();
				printf("%c", key);
				DrawSubMenu(key);
				setcursortype(NOCURSOR);
				ShowSubwayMap();
				system("cls");
				DrawMainMenu();
				setcursortype(NORMALCURSOR);
				break;
			case _2://최단 거리 경로 찾기
				system("cls");
				DrawMainMenu();
				printf("%c", key);
				gotoxy(3, 31); printf("뒤로가려면 Esc를 누르세요.");
				FindShortestDistPath();
				gotoxy(27, 26); printf(" ");
				GetReadyForInput(27, 26);
				break;
			case _3://최소 환승 경로 찾기
				system("cls");
				DrawMainMenu();
				printf("%c", key);
				gotoxy(3, 31); printf("뒤로가려면 Esc를 누르세요.");
				FindLeastTransferPath();
				gotoxy(27, 26); printf(" ");
				GetReadyForInput(27, 26);
				break;
			case _4://즐겨찾는 역 리스트
				system("cls");
				DrawMainMenu();
				printf("%c", key);
				DrawSubMenu(key);
				EditFavorites();
				system("cls");
				DrawMainMenu();
				gotoxy(27, 26); printf(" ");
				GetReadyForInput(27, 26);
				break;
			case ESC://프로그램 종료
				exit(0);
			default:
				break;
			}
		}
	}

	system("pause>null");//계속하려면 아무키나 누르시오 없앰
}

void ShowSubwayMap() {
	DrawSubwayMap();
	int key;
	while (1) {
		if (_kbhit()) {
			key = _getch();
			printf("%c", key);
			if (key == ESC) break;
		}
	}
}

int SetTargetStation(char message[]) {
	gotoxy(MENU_WIDTH, 3);				      printf("┌────── [%s 설정하기]──────┐\n", message);
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("│                              │\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("│ 1. 노선도 보고 입력          │\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("│ 2. 즐겨찾기 리스트 보고 입력 │\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("│ 3. 역명 검색으로 입력        │\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("└──────────────────────────────┘\n\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%s를 설정하는 방법을 선택하세요: ", message);
	GetReadyForInput(GetCurrCursorPos().X, GetCurrCursorPos().Y);

	while (1) {
		int key = 0;
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case _1:
				system("cls");
				DrawMainMenu();
				setcursortype(NOCURSOR);
				gotoxy(3, 31); printf("뒤로가려면 Esc를 누르세요.");
				ShowSubwayMap();
				system("cls");
				DrawMainMenu();
				gotoxy(MENU_WIDTH, 3); printf("%s를 입력하세요: ", message);
				break;
			case _2:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("_______즐겨찾기 리스트_______\n");
				DisplayList(&favorites);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("____________________________\n\n");
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%s를 입력하세요: ", message);
				break;
			case _3:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("%s를 입력하세요: ", message);
				break;
			case ESC:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- 메뉴를 선택해주세요");
				return key;
			default:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("잘못된 입력입니다. 다시 입력해주세요.\n");//이 키 먹는지 확인 요
				break;
			}
		}
		if (key != 0) break;
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
	while (1) {
		if (list[0] == 0) {
			gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("찾는 역이 존재하지 않습니다. 잠시 후 이전으로 돌아갑니다.\n");
			setcursortype(NOCURSOR);
			Sleep(2000);
			return -1;
		}
		else {
			gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("-----------------------------------\n");
			gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%s(으)로 검색된 역들:\n", string);
			for (i = 0; i < k; i++) {
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%d.%d호선 %s\n", i + 1, stations[list[i]].line, stations[list[i]].name);
			}
			do {
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("번호를 입력해주세요: ");
				gotoxy(GetCurrCursorPos().X, GetCurrCursorPos().Y); scanf("%d", &input);
				if (input < 1 || input > k) {
					gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("다시 번호를 선택해주세요: ");
				}
				else {
					return list[input - 1];
				}
			} while (input < 1 || input > k);
		}
	}
}

void FindShortestDistPath() {
	char str1[100], str2[100];
	int a = 0, b = 0;
	int key_input;
	do{
		if (a == -1) {
			system("cls");
			DrawMainMenu();
			gotoxy(3, 31); printf("뒤로가려면 Esc를 누르세요.");
		}
		key_input = SetTargetStation("출발지");
		if (key_input == ESC) return;
		/*
		else if (key_input == 2) {
			StationListNode* tmp;
			scanf("%d", a);
			tmp = get_node_at(&favorites, a - 1);
			printf("%d", (tmp->station).code);
			a = (tmp->station).code;
		}
		*/
		else {
			scanf("%100s", str1);
			a = search(str1);
		}
	} while (a == -1);

	system("cls");
	DrawMainMenu();
	printf("2");
	gotoxy(3, 31); printf("뒤로가려면 Esc를 누르세요.");

	do {
		if (b == -1) {
			system("cls");
			DrawMainMenu();
			gotoxy(3, 31); printf("뒤로가려면 Esc를 누르세요.");
		}
		key_input = SetTargetStation("도착지");
		if (key_input == ESC) return;
		scanf("%100s", str2);
		b = search(str2);
	} while (b == -1);

	Path p;
	p = FindPath(a, b);
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("-----------------------------------\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("총 %d개의 역, %lf 분 소요\n", p.num, p.dist);
	for (int i = 0; i < p.num; i++) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%d호선 %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
	}
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("**경로 찾기 완료**");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- 메뉴를 선택해주세요");
}

void FindLeastTransferPath() {
	char str1[100], str2[100];
	int a = 0, b = 0;
	int key_input;
	do {
		if (a == -1) {
			system("cls");
			DrawMainMenu();
			gotoxy(3, 31); printf("뒤로가려면 Esc를 누르세요.");
		}
		key_input = SetTargetStation("출발지");
		if (key_input == ESC) return;
		scanf("%100s", str1);
		a = search(str1);
	} while (a == -1);

	system("cls");
	DrawMainMenu();
	printf("2");
	gotoxy(3, 31); printf("뒤로가려면 Esc를 누르세요.");

	do {
		if (b == -1) {
			system("cls");
			DrawMainMenu();
			gotoxy(3, 31); printf("뒤로가려면 Esc를 누르세요.");
		}
		key_input = SetTargetStation("도착지");
		if (key_input == ESC) return;
		scanf("%100s", str2);
		b = search(str2);
	} while (b == -1);

	Path p;
	p = FindMinTransfer(a, b);
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("-----------------------------------\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("총 %d개의 역, %lf 분 소요\n", p.num, p.dist);
	for (int i = 0; i < p.num; i++) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%d호선 %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
	}
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("**경로 찾기 완료**");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- 메뉴를 선택해주세요");
}

int EditFavorites() {
	int key = 0;
	char input[100];
	int target;
	int position;
	while (1) {
		if (_kbhit()) {
			key = _getch();
			printf("%c", key);
			system("cls");
			DrawMainMenu();
			DrawSubMenu(_4);
			switch (key) {
			case _1:
				gotoxy(MENU_WIDTH, 3);
				printf("즐겨찾기에 추가할 역을 검색하세요: ");
				scanf("%100s", input);
				target = search(input); //예외처리필요
				AddStation(&favorites, stations[target]);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("%s역이 즐겨찾기에 추가되었습니다.\n\n", stations[target].name);
				gotoxy(27, 36); printf(" "); gotoxy(27, 36);
				break;
			case _2:
				gotoxy(MENU_WIDTH, 3);
				printf("_______즐겨찾기 리스트_______\n");
				DisplayList(&favorites);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("____________________________\n");
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("삭제할 역의 번호를 입력하세요: ");
				scanf("%d", &position);
				RemoveStation(&favorites, position - 1);
				gotoxy(27, 36); printf(" "); gotoxy(27, 36);
				break;
			case _3:
				gotoxy(MENU_WIDTH, 3);
				printf("_______즐겨찾기 리스트_______\n");
				DisplayList(&favorites);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("____________________________\n");
				gotoxy(27, 36); printf(" "); gotoxy(27, 36);
				break;
			case ESC:
				gotoxy(MENU_WIDTH, 3); printf("즐겨찾기 수정이 완료되었습니다.");
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- 메뉴를 선택해주세요");//화면 길이 넘어가는거 예외처리 필요
				return;
			}
		}
	}
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
