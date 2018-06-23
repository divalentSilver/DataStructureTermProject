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

//��θ� �����ϴ� ����ü
typedef struct _path {
	int num;					//���� ����
	int transfer;				//ȯ�� ȸ��

	int path[STATION_NUM + 1];	//���� index���� ������ �迭 (-1: ȯ��ó��)
	double dist;				//�Ÿ� ����
}Path;

typedef struct _StationListNode {//���ã�� ����Ʈ ��� ����ü
	Station station;
	struct _StationListNode * link;
}StationListNode;

typedef struct {//���ã�� ����Ʈ ����ü
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
	//�ܼ�â ����-------------------------
	setcursortype(NOCURSOR);
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	system("mode con cols=190 lines=60");
	system("title ����ö ���ø����̼�");
	system("color f0");

	//Initializing-----------------------
	Parse(stations);
	InitGraph();
	InitializeList(&favorites);
	TransferParse(weight, stations, transfer);
	InitGUI();
	DrawTitle();//�� �� ����� UI�� ���� �ʿ�.
	DrawMainMenu();

	while (1) {
		int key = 0;
		if (_kbhit()) {//������ �޴��� ���� �Լ�ȣ��
			key = _getch();
			switch (key) {
			case _1://����ö �뼱�� ����
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
			case _2://�ִ� �Ÿ� ��� ã��
				system("cls");
				DrawMainMenu();
				printf("%c", key);
				gotoxy(3, 31); printf("�ڷΰ����� Esc�� ��������.");
				FindShortestDistPath();
				gotoxy(27, 26); printf(" ");
				GetReadyForInput(27, 26);
				break;
			case _3://�ּ� ȯ�� ��� ã��
				system("cls");
				DrawMainMenu();
				printf("%c", key);
				gotoxy(3, 31); printf("�ڷΰ����� Esc�� ��������.");
				FindLeastTransferPath();
				gotoxy(27, 26); printf(" ");
				GetReadyForInput(27, 26);
				break;
			case _4://���ã�� �� ����Ʈ
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
			case ESC://���α׷� ����
				exit(0);
			default:
				break;
			}
		}
	}

	system("pause>null");//����Ϸ��� �ƹ�Ű�� �����ÿ� ����
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
	gotoxy(MENU_WIDTH, 3);				      printf("�������������� [%s �����ϱ�]��������������\n", message);
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("��                              ��\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("�� 1. �뼱�� ���� �Է�          ��\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("�� 2. ���ã�� ����Ʈ ���� �Է� ��\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("�� 3. ���� �˻����� �Է�        ��\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("����������������������������������������������������������������\n\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%s�� �����ϴ� ����� �����ϼ���: ", message);
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
				gotoxy(3, 31); printf("�ڷΰ����� Esc�� ��������.");
				ShowSubwayMap();
				system("cls");
				DrawMainMenu();
				gotoxy(MENU_WIDTH, 3); printf("%s�� �Է��ϼ���: ", message);
				break;
			case _2:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("_______���ã�� ����Ʈ_______\n");
				DisplayList(&favorites);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("____________________________\n\n");
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%s�� �Է��ϼ���: ", message);
				break;
			case _3:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("%s�� �Է��ϼ���: ", message);
				break;
			case ESC:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- �޴��� �������ּ���");
				return key;
			default:
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");//�� Ű �Դ��� Ȯ�� ��
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
			gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("ã�� ���� �������� �ʽ��ϴ�. ��� �� �������� ���ư��ϴ�.\n");
			setcursortype(NOCURSOR);
			Sleep(2000);
			return -1;
		}
		else {
			gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("-----------------------------------\n");
			gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%s(��)�� �˻��� ����:\n", string);
			for (i = 0; i < k; i++) {
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%d.%dȣ�� %s\n", i + 1, stations[list[i]].line, stations[list[i]].name);
			}
			do {
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("��ȣ�� �Է����ּ���: ");
				gotoxy(GetCurrCursorPos().X, GetCurrCursorPos().Y); scanf("%d", &input);
				if (input < 1 || input > k) {
					gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("�ٽ� ��ȣ�� �������ּ���: ");
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
			gotoxy(3, 31); printf("�ڷΰ����� Esc�� ��������.");
		}
		key_input = SetTargetStation("�����");
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
	gotoxy(3, 31); printf("�ڷΰ����� Esc�� ��������.");

	do {
		if (b == -1) {
			system("cls");
			DrawMainMenu();
			gotoxy(3, 31); printf("�ڷΰ����� Esc�� ��������.");
		}
		key_input = SetTargetStation("������");
		if (key_input == ESC) return;
		scanf("%100s", str2);
		b = search(str2);
	} while (b == -1);

	Path p;
	p = FindPath(a, b);
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("-----------------------------------\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("�� %d���� ��, %lf �� �ҿ�\n", p.num, p.dist);
	for (int i = 0; i < p.num; i++) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%dȣ�� %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
	}
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("**��� ã�� �Ϸ�**");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- �޴��� �������ּ���");
}

void FindLeastTransferPath() {
	char str1[100], str2[100];
	int a = 0, b = 0;
	int key_input;
	do {
		if (a == -1) {
			system("cls");
			DrawMainMenu();
			gotoxy(3, 31); printf("�ڷΰ����� Esc�� ��������.");
		}
		key_input = SetTargetStation("�����");
		if (key_input == ESC) return;
		scanf("%100s", str1);
		a = search(str1);
	} while (a == -1);

	system("cls");
	DrawMainMenu();
	printf("2");
	gotoxy(3, 31); printf("�ڷΰ����� Esc�� ��������.");

	do {
		if (b == -1) {
			system("cls");
			DrawMainMenu();
			gotoxy(3, 31); printf("�ڷΰ����� Esc�� ��������.");
		}
		key_input = SetTargetStation("������");
		if (key_input == ESC) return;
		scanf("%100s", str2);
		b = search(str2);
	} while (b == -1);

	Path p;
	p = FindMinTransfer(a, b);
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("-----------------------------------\n");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("�� %d���� ��, %lf �� �ҿ�\n", p.num, p.dist);
	for (int i = 0; i < p.num; i++) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("%dȣ�� %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
	}
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("**��� ã�� �Ϸ�**");
	gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- �޴��� �������ּ���");
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
				printf("���ã�⿡ �߰��� ���� �˻��ϼ���: ");
				scanf("%100s", input);
				target = search(input); //����ó���ʿ�
				AddStation(&favorites, stations[target]);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("%s���� ���ã�⿡ �߰��Ǿ����ϴ�.\n\n", stations[target].name);
				gotoxy(27, 36); printf(" "); gotoxy(27, 36);
				break;
			case _2:
				gotoxy(MENU_WIDTH, 3);
				printf("_______���ã�� ����Ʈ_______\n");
				DisplayList(&favorites);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("____________________________\n");
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y); printf("������ ���� ��ȣ�� �Է��ϼ���: ");
				scanf("%d", &position);
				RemoveStation(&favorites, position - 1);
				gotoxy(27, 36); printf(" "); gotoxy(27, 36);
				break;
			case _3:
				gotoxy(MENU_WIDTH, 3);
				printf("_______���ã�� ����Ʈ_______\n");
				DisplayList(&favorites);
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
				printf("____________________________\n");
				gotoxy(27, 36); printf(" "); gotoxy(27, 36);
				break;
			case ESC:
				gotoxy(MENU_WIDTH, 3); printf("���ã�� ������ �Ϸ�Ǿ����ϴ�.");
				gotoxy(MENU_WIDTH, GetCurrCursorPos().Y + 2); printf("<- �޴��� �������ּ���");//ȭ�� ���� �Ѿ�°� ����ó�� �ʿ�
				return;
			}
		}
	}
}

void InitGraph() {
	int i, j;
	//�׷����� INF �Ǵ� 0���� �ʱ�ȭ
	for (i = 0; i <= STATION_NUM; i++) {
		for (j = 0; j <= STATION_NUM; j++)
			weight[i][j] = i == j ? 0 : INF;
	}
	//���� �������� �Ÿ��̹Ƿ� �� ȣ���� ���ۿ��ǰ�� �ǳʶ�
	for (i = 1; i <= STATION_NUM; i++) {
		if (i == 63	||	//2ȣ�� ��û
			i == 106||	//2ȣ�� �������� ���
			i == 110||	//2ȣ�� �������� ����õ
			i == 114||	//3ȣ�� ����
			i == 148||	//4ȣ�� ���
			i == 196||	//5ȣ�� ��ȭ
			i == 240||	//5ȣ�� ��õ����
			i == 247||  //6ȣ�� ����
			i == 285||	//7ȣ�� ���
			i == 336	//8ȣ�� �ϻ�
			) continue;
		weight[i - 1][i] = stations[i].spacing;
		weight[i][i - 1] = stations[i].spacing;
	}
	//Ư�����̽� ����ó�� - 2ȣ��/6ȣ�� ��ȯ
	//2ȣ�� ��ȯ
	weight[105][63] = stations[63].spacing;	//������->��û
	weight[63][105] = stations[63].spacing; //��û->������
	//���� ��ȯ
	weight[248][247] = INF;	//����->���� �Ұ���
	weight[249][248] = INF; //�ұ�->���� �Ұ���
	weight[250][249] = INF; //������->�ұ� �Ұ���
	weight[251][250] = INF; //���ų�->������ �Ұ���
	weight[252][251] = INF; //����->���ų� �Ұ���
	weight[252][253] = INF;	//����->���� �Ұ���
	weight[253][252] = INF;	//����->���� �Ұ���

	weight[252][247] = stations[247].spacing;	//����->���� ����
	weight[247][253] = stations[253].spacing;	//����->���� ����
	weight[253][247] = stations[253].spacing;	//����->���� ����
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
	
	//���ۿ��� ���� ����Է�
	paths[start].path[0] = start;
	paths[start].num = 1;
	for (i = 1; i <= n - 2; i++) {
		u = choose(n, found, paths);
		found[u] = TRUE;
		for (w = 1; w <= n; w++)
			if (!found[w]) {
				if (paths[u].dist + weight[u][w] <= paths[w].dist) {	//w������ �ִܰŸ��� u������ �ִܰŸ� + u->w �� ���
					paths[w] = paths[u];								//u������ ��θ� �״�� ������
					paths[w].path[paths[w].num++] = w;					//��ο� w�� �߰���
					paths[w].dist = paths[u].dist + weight[u][w];		//u������ �Ÿ��� u->w �Ÿ��� �߰���
				}
			}

		//�������� ���� ��θ� ã�������
		if (found[dest])
			break;
	}

	paths[dest].dist += (double)paths[dest].num * 0.5;	//�����ð� ����
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
		printf("����Ʈ�� ������ϴ�.\n");
	}
	for (int i = 0; i < list->length; i++) {
		gotoxy(MENU_WIDTH, GetCurrCursorPos().Y);
		printf("%d.%dȣ�� %s\n", i + 1, node->station.line, node->station.name);
		node = node->link;
	}
}
