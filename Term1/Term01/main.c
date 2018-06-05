#pragma once
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
int search(char*);

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
	for (int i = 0; i < p.num; i++) {
		printf("%dȣ�� %s\n", stations[p.path[i]].line, stations[p.path[i]].name);
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
	printf("%s(��)�� �˻��� ����:\n", string);
	for (i = 0; i < k; i++)
		printf("%d.%dȣ�� %s\n", i + 1, stations[list[i]].line, stations[list[i]].name);
	scanf("%d", &input);
	return list[i - 1];
}

void InitGraph() {
	int i, j;
	for (i = 0; i <= STATION_NUM; i++) {
		for (j = 0; j <= STATION_NUM; j++)
			weight[i][j] = i == j ? 0 : INF;
	}
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
	found[start] = TRUE; // ���� ���� �湮 ǥ��
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