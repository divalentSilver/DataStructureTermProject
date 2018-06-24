//included in csvParser.h
#define TRANSFER_NUM 93

typedef struct _trasfer{
	int fromIndex;
	int fromLine;
	int toIndex;
	int toLine;
} Transfer;

int isTransfer(int from, int to, Transfer transfer[]) {
	for (int i = 0; i < TRANSFER_NUM; i++) {
		if (from == transfer[i].fromIndex && to == transfer[i].toIndex)
			return i;
	}
	return -1;	//fail
}
int FindTransfer(int from, int to, int arr[], Transfer transfer[]) {
	int n = 0;
	if (from == to)
		return -1;
	for(int i = 0; i < TRANSFER_NUM; i++)
		if (from == transfer[i].fromLine && to == transfer[i].toLine) {
			arr[n++] = i;
		}
	return n;
}