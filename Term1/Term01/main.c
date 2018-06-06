#pragma once
#include "csvParser.h"
#include "GUI.h"
#include "TextButton.h"
#include "Util.h"

#define INF 10000
#define _A 65 //Ű���� �Է°�
#define _a 97
#define _D 68
#define _d 100
#define _p 112
#define _P 80 
#define ESC 27

//��θ� �����ϴ� ����ü
typedef struct _path {
	int num;					//���� ����
	int path[STATION_NUM + 1];	//���� index���� ������ �迭
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
	printf("������� �˻��ϼ���: ");
	scanf("%100s", str1);
	a = search(str1);

	printf("�������� �˻��ϼ���: ");
	scanf("%100s", str2);
	b = search(str2);

	Path p;
	p = FindPath(a, b);//���� �Ⱦ�
	printf("�� %d���� ��, %lf �� �ҿ�\n", p.num, p.dist);
	for (int i = 0; i < p.num; i++) {
		printf("%dȣ�� %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
	}
	//-----------------------------------���ã�� ���
	StationList favorites;
	InitializeList(&favorites);
	int key = 0;
	char input[100];
	int target;
	int position;
	printf("\nA/a: ���ã�� �� �߰�\nD/d: ���ã�� �� ����\nP/p: ���ã�� ����Ʈ ���\nEsc: ���α׷� ����\n���ϴ� ����� ��ư�� ��������.\n");
	while (1) {//���Ŀ� GUI�� ��ư ��� �̿��Ͽ� ������ ��
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case _A:
			case _a:
				printf("\n���ã�⿡ �߰��� ���� �˻��ϼ���: ");
				scanf("%100s", input);
				target = search(input); //����ó���ʿ�
				printf("%d\n", target);
				AddStation(&favorites, stations[target]);
				printf("%s���� ���ã�⿡ �߰��Ǿ����ϴ�.\n\n", stations[target].name);
				break;
			case _D:
			case _d:
				printf("\n������ ���� ��ȣ�� �Է��ϼ���: ");
				scanf("%d", &position);
				RemoveStation(&favorites, position - 1);
				break;
			case _P:
			case _p:
				printf("\n_______���ã�� ����Ʈ_______\n");
				DisplayList(&favorites);
				printf("____________________________\n");
				break;
			case ESC:
				printf("\n���α׷��� �����մϴ�.\n");
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
	printf("%s(��)�� �˻��� ����:\n", string);
	for (i = 0; i < k; i++)
		printf("%d.%dȣ�� %s\n", i + 1, stations[list[i]].line, stations[list[i]].name);
	scanf("%d", &input);
	return list[input - 1];
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
	if (is_empty(list)) printf("����Ʈ�� ������ϴ�.\n");
	for (int i = 0; i < list->length; i++) {
		printf("%d.%dȣ�� %s\n", i + 1, node->station.line, node->station.name);
		node = node->link;
	}
}
