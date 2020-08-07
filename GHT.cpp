// ����ͼ���ڽӱ�洢
#include <iostream>
#include <queue>
using namespace std;

const int INF = 0x3f3f3f;
const int maxn = 100;
const int WHITE = 0;
const int GRAY = 1;
const int BLACK = 2;

int augmentingPathArray[maxn];  // �б�洢����·
int road[maxn];
int q[maxn + 2];
int color[maxn]; // bfs��־

struct EdgeNode { // �߽��
	int to; // ����ߵ���һ���ڽӵ�����
	int capacity; // �ߵ�Ȩ��
	int flow; // �ߵ�����
	EdgeNode* next; // ָ����һ���߽���ָ��
};

struct VertexNode { // ����
	int data; // ������Ϣ
	EdgeNode* head; // ��ͷָ��
};

struct Graph { // ����ͼ���ڽӱ�洢�ṹ
	VertexNode vertexs[maxn]; // ��������
	int vexNum, edgeNum; // �������ͱ���
};

Graph g;
Graph tree;

void createG() { // �����ڽӱ�洢��ʾ����������ͼG
	int i = 0; // ѭ������
	EdgeNode* p; // �������������ı߽��ָ��
	int v1, v2, w; // ����ߵ���������
	g.vexNum = g.edgeNum = 0;
	scanf_s("%d%d", &g.vexNum, &g.edgeNum); // �������붥������ͱ���

	for (i = 0; i < g.vexNum; i++) { // ��ʼ����ͷָ��Ϊ��0
		g.vertexs[i].head = NULL;
	}

	for (i = 0; i < g.edgeNum; i++) {
		scanf_s("%d%d%d", &v1, &v2, &w); // ����һ���ߵ������յ�
		v1--; v2--;
		p = new EdgeNode; // �ٶ����㹻�ռ�
		p->to = v2;
		p->capacity = w;
		p->flow = 0;
		p->next = g.vertexs[v1].head; // ��������(ͷ��)
		g.vertexs[v1].head = p;
	}
}

/*
void delectG() { // �ͷ�ͼG�ڽӱ������ı������е����б߽����ռ�Ĵ洢�ռ�
	int i = 0; // ѭ������
	EdgeNode* p; // ����ָ�������ı߽��ָ��

	for (i = 0; i < g.vexNum; i++) {
		p = g.vertexs[i].head;

		// �ͷŵ�i�������ڽӱ���߽����ռ�Ĵ洢�ռ�
		while (p != NULL) {
			g.vertexs[i].head = p->next;
			delete p;
			p = g.vertexs[i].head;
		}
	}
}

int visited1[maxn];

void DFS(int i) { // ������ȱ���
	visited1[i] = 1;
	EdgeNode* p = g.vertexs[i].head;
	while (p != NULL) {
		int j = p->to;
		if (visited1[j] != 1) {
			DFS(j);
		}
		p = p->next;
	}
}

int visited2[maxn];

void BFS(int i) { // ������ȱ���
	visited2[i] = 1;
	queue<int> q;
	q.push(i);
	while (!q.empty())
	{
		int k = q.front();
		EdgeNode* p = g.vertexs[k].head;
		while (p != NULL) {
			int j = p->to;
			if (visited2[j] != 1) {
				visited2[j] = 1;
				q.push(j);
			}
			p = p->next;
		}
	}
}
*/

int min(int x, int y) { // ����x��y�н�С��ֵ
	return x < y ? x : y;
}

int bfs(int start, int target) { // start-target������ȱ���
	int u, v;
	start--; target--;
	for (int i = 0; i < g.vexNum; i++) {
		color[i] = WHITE;
	}
	queue<int> q;
	q.push(start);
	color[start] = GRAY;
	augmentingPathArray[start] = -1;
	while (!q.empty())
	{
		u = q.front();
		color[u] = BLACK;
		q.pop();
		EdgeNode* p = g.vertexs[u].head;
		while (p != NULL) {
			v = p->to;
			if (color[v] == WHITE && ((p->capacity) - (p->flow)) > 0) {
				q.push(v);
				color[v] = GRAY;
				cout << v << endl;
				augmentingPathArray[v] = u;
			}
			p = p->next;
		}
	}

	// ��target����colorΪBLACK
	return color[target] == BLACK;
}

/*
int head, tail;

void enqueue(int x) { // x���
	q[tail] = x;
	tail++;
	color[x] = GRAY;
}

int dequeue() { // ����
	int x = q[head];
	head++;
	color[x] = BLACK;
	return x;
}

int bfs(int start, int target) { // start-target������ȱ���
	int u, v;
	for (int i = 0; i < g.vexNum; i++) {
		color[i] = WHITE;
	}
	head = tail = 0;
	enqueue(start);
	augmentingPathArray[start] = -1;

	while (head != tail) {
		u = dequeue();
		// ����u���е��ڽӶ���v����v��colorΪWHITE��uvδ������v���
		EdgeNode* p = g.vertexs[u].head;
		while (p != NULL) {
			v = p->to;
			if (color[v] != WHITE && (p->capacity) - (p->flow) > 0) {
				enqueue(v);
				augmentingPathArray[v] = u;
			}
			p = p->next;
		}
	}

	// ��target����colorΪBLACK
	return color[target] == BLACK;
}
*/

int max_flow(int source, int sink) { // Ford-Fulkerson�㷨���������
	int u;
	int max_flow = 0;
	source--; sink--;

	// ������·���ڣ����¸Ľ���increment
	while (bfs(source, sink)) {
		// ����increment
		int increment = INF;
		for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
			EdgeNode* p = g.vertexs[augmentingPathArray[u]].head;
			while (p->to != u) {
				p = p->next;
			}
			increment = min(increment, p->capacity - p->flow);
		}

		// ������������
		for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
			EdgeNode* p = g.vertexs[augmentingPathArray[u]].head;
			while (p->to != u) {
				p = p->next;
			}
			p->flow += increment;
		}
		for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
			EdgeNode* p = g.vertexs[u].head;
			while (p->to != augmentingPathArray[u]) {
				p = p->next;
			}
			p->flow -= increment;
		}

		max_flow += increment;
	}

	return max_flow;
}

int isNear(int i, int j) { // �ж���tree��i��j�Ƿ�����
	EdgeNode* p = tree.vertexs[i].head;
	while (p!=NULL) {
		if (p->to == j) {
			return true;
		}
		p = p->next;
	}
	return false;
}

void addEdge(int i, int j) { // ��tree������i��j��Ȩ������Ϊ1
	if (!isNear(i, j)) {
		EdgeNode* p = new EdgeNode; // �ٶ����㹻�ռ�
		p->to = j;
		p->capacity = 1;
		p->flow = 0;
		p->next = tree.vertexs[i].head; // ��������(ͷ��)
		tree.vertexs[i].head = p;
	}
}

void delectEdge(int i, int j) { // ��tree��ɾ����i��j������i��j���ڣ�����i��j���ڣ�
	if (tree.vertexs[i].head->to == j) {
		tree.vertexs[i].head = tree.vertexs[i].head->next;
	}
	else {
		EdgeNode* p = tree.vertexs[i].head;
		while (p->next != NULL) {
			if (p->next->to == j) {
				p->next = p->next->next;
			}
			else {
				p = p->next;
			}
		}
	}
}

void assign(int i, int j, int value) { // ��i��j��㸳ֵvalue
	EdgeNode* p = new EdgeNode; // �ٶ����㹻�ռ�
	p = tree.vertexs[i].head;
	if (isNear(i, j)) {	
		while (p != NULL) {
			if (p->to == j) {
				p->capacity = value;
			}
			p = p->next;
		}
	}
	else {
		p->to = j;
		p->capacity = value;
		p->flow = 0;
		p->next = g.vertexs[i].head; // ��������(ͷ��)
		g.vertexs[i].head = p;
	}
}

void build_GH_tree() { // EQ������GH��
	// ��ʼ����С����
	tree.vexNum = g.vexNum;
	for (int i = 0; i < tree.vexNum; i++) { // ��ʼ����ͷָ��Ϊ��0
		tree.vertexs[i].head = NULL;
	}

	for (int i = 1; i < tree.vexNum; i++) {
		EdgeNode* p1 = new EdgeNode; // �ٶ����㹻�ռ�
		p1->to = i;
		p1->capacity = 1;
		p1->flow = 0;
		p1->next = tree.vertexs[0].head; // ��������(ͷ��)
		tree.vertexs[0].head = p1;

		EdgeNode* p2 = new EdgeNode;
		p2->to = 0;
		p2->capacity = 1;
		p2->flow = 0;
		p2->next = tree.vertexs[i].head; // ��������(ͷ��)
		tree.vertexs[i].head = p2;
	}

	void showInfo(Graph g);
	// showInfo(tree);

	//for (int i = 0; i < tree.vexNum; i++) // ������3,4��
	//	for (int j = i + 1; j < tree.vexNum; j++)
	//	{
	//		if (isNear(i,j)) { // i,jΪ���ڽڵ�
	//			int MaxFlow = max_flow(j, i); // ���������,�õ���С��
	//			assign(i, j, MaxFlow);
	//			assign(j, i, MaxFlow);
	//			for (int k = j + 1; k < tree.vexNum; k++) // ��ÿһ������j�ĵ�k
	//				if (isNear(k,i) && bfs(j, k)) { // ��k��i���ڽӶ���(Tree��), ����j��һ�ߣ�ԭͼ�У�
	//					delectEdge(k, j); // �Ͽ�k��i
	//					delectEdge(j, k);
	//					addEdge(k, j);
	//					addEdge(j, k);
	//				}
	//		}
	//	}

	for (int s = 1; s < tree.vexNum; s++) {
		EdgeNode* p;
		p = tree.vertexs[s].head;
		int t = p->to;
		int MaxFlow = max_flow(s, t); // ���������,�õ���С��
		p->capacity = MaxFlow;
		assign(t, s, MaxFlow);

		for (int i = s + 1; i < tree.vexNum; i++) {
			if (isNear(t, i)) { // && bfs(s, i)
				delectEdge(t, i);
				delectEdge(i, t);
				addEdge(i, s);
				addEdge(s, i);
			}
		}
	}
}

void showInfo(Graph g) {
	EdgeNode* p;
	for (int i = 0; i < g.vexNum; i++) {
		p = g.vertexs[i].head;
		cout << i << "    ";
		while (p != NULL) {
			cout << p->to << endl;
			p = p->next;
		}
		cout << endl;
	}
}

int main() {
	/*
		for (int i = 0; i < tree.vexNum; i++) {
			EdgeNode* p = new EdgeNode;
			p = tree.vertexs[i].head;
			while (p != NULL) {
				cout << i << "  " << p->to << "  " << p->capacity << endl;
				p = p->next;
			}
		}
	
		cout << bfs(1, 3) << endl;

		cout << "the augmentingPathNum is : " << endl;
		for (int i = 0; i < g.vexNum; i++) {
			cout << augmentingPathArray[i] << "  ";
		}
		cout << endl;
		cout << "the color is : " << endl;
		for (int i = 0; i < g.vexNum; i++) {
			cout << color[i] << "  ";
		}
	
		cout << endl;
		cout <<"the maxflow is : " << max_flow(2, 3) << endl;

		EdgeNode* p = new EdgeNode;
			p = tree.vertexs[1].head;
			while (p != NULL) {
				cout << 1 << "  " << p->to << "  " << p->capacity << endl;
				p = p->next;
			}
		*/

	createG();
	build_GH_tree();
	showInfo(tree);
}