// 无向图的邻接表存储
#include <iostream>
#include <queue>
using namespace std;

const int INF = 0x3f3f3f;
const int maxn = 100;
const int WHITE = 0;
const int GRAY = 1;
const int BLACK = 2;

int augmentingPathArray[maxn];  // 列表存储增广路
int road[maxn];
int q[maxn + 2];
int color[maxn]; // bfs标志

struct EdgeNode { // 边结点
	int to; // 无向边的另一个邻接点的序号
	int capacity; // 边的权重
	int flow; // 边的流量
	EdgeNode* next; // 指向下一个边结点的指针
};

struct VertexNode { // 顶点
	int data; // 顶点信息
	EdgeNode* head; // 表头指针
};

struct Graph { // 无向图的邻接表存储结构
	VertexNode vertexs[maxn]; // 顶点数组
	int vexNum, edgeNum; // 顶点数和边数
};

Graph g;
Graph tree;

void createG() { // 采用邻接表存储表示，构造有向图G
	int i = 0; // 循环变量
	EdgeNode* p; // 用来构造边链表的边结点指针
	int v1, v2, w; // 无向边的两个顶点
	g.vexNum = g.edgeNum = 0;
	scanf_s("%d%d", &g.vexNum, &g.edgeNum); // 首先输入顶点个数和边数

	for (i = 0; i < g.vexNum; i++) { // 初始化表头指针为空0
		g.vertexs[i].head = NULL;
	}

	for (i = 0; i < g.edgeNum; i++) {
		scanf_s("%d%d%d", &v1, &v2, &w); // 输入一条边的起点和终点
		v1--; v2--;
		p = new EdgeNode; // 假定有足够空间
		p->to = v2;
		p->capacity = w;
		p->flow = 0;
		p->next = g.vertexs[v1].head; // 插入链表(头部)
		g.vertexs[v1].head = p;
	}
}

/*
void delectG() { // 释放图G邻接表各顶点的边链表中的所有边结点所占的存储空间
	int i = 0; // 循环变量
	EdgeNode* p; // 用来指向边链表的边结点指针

	for (i = 0; i < g.vexNum; i++) {
		p = g.vertexs[i].head;

		// 释放第i个顶点邻接表各边结点所占的存储空间
		while (p != NULL) {
			g.vertexs[i].head = p->next;
			delete p;
			p = g.vertexs[i].head;
		}
	}
}

int visited1[maxn];

void DFS(int i) { // 深度优先遍历
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

void BFS(int i) { // 广度优先遍历
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

int min(int x, int y) { // 返回x，y中较小的值
	return x < y ? x : y;
}

int bfs(int start, int target) { // start-target广度优先遍历
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

	// 若target顶点color为BLACK
	return color[target] == BLACK;
}

/*
int head, tail;

void enqueue(int x) { // x入队
	q[tail] = x;
	tail++;
	color[x] = GRAY;
}

int dequeue() { // 出队
	int x = q[head];
	head++;
	color[x] = BLACK;
	return x;
}

int bfs(int start, int target) { // start-target广度优先遍历
	int u, v;
	for (int i = 0; i < g.vexNum; i++) {
		color[i] = WHITE;
	}
	head = tail = 0;
	enqueue(start);
	augmentingPathArray[start] = -1;

	while (head != tail) {
		u = dequeue();
		// 搜索u所有的邻接顶点v，若v的color为WHITE且uv未满，则v入队
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

	// 若target顶点color为BLACK
	return color[target] == BLACK;
}
*/

int max_flow(int source, int sink) { // Ford-Fulkerson算法计算最大流
	int u;
	int max_flow = 0;
	source--; sink--;

	// 若增广路存在，更新改进量increment
	while (bfs(source, sink)) {
		// 计算increment
		int increment = INF;
		for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
			EdgeNode* p = g.vertexs[augmentingPathArray[u]].head;
			while (p->to != u) {
				p = p->next;
			}
			increment = min(increment, p->capacity - p->flow);
		}

		// 更新流量网络
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

int isNear(int i, int j) { // 判断在tree中i，j是否相邻
	EdgeNode* p = tree.vertexs[i].head;
	while (p!=NULL) {
		if (p->to == j) {
			return true;
		}
		p = p->next;
	}
	return false;
}

void addEdge(int i, int j) { // 在tree中连接i，j，权重设置为1
	if (!isNear(i, j)) {
		EdgeNode* p = new EdgeNode; // 假定有足够空间
		p->to = j;
		p->capacity = 1;
		p->flow = 0;
		p->next = tree.vertexs[i].head; // 插入链表(头部)
		tree.vertexs[i].head = p;
	}
}

void delectEdge(int i, int j) { // 在tree中删除边i，j（假设i，j相邻，即边i，j存在）
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

void assign(int i, int j, int value) { // 给i，j结点赋值value
	EdgeNode* p = new EdgeNode; // 假定有足够空间
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
		p->next = g.vertexs[i].head; // 插入链表(头部)
		g.vertexs[i].head = p;
	}
}

void build_GH_tree() { // EQ法生成GH树
	// 初始化最小割树
	tree.vexNum = g.vexNum;
	for (int i = 0; i < tree.vexNum; i++) { // 初始化表头指针为空0
		tree.vertexs[i].head = NULL;
	}

	for (int i = 1; i < tree.vexNum; i++) {
		EdgeNode* p1 = new EdgeNode; // 假定有足够空间
		p1->to = i;
		p1->capacity = 1;
		p1->flow = 0;
		p1->next = tree.vertexs[0].head; // 插入链表(头部)
		tree.vertexs[0].head = p1;

		EdgeNode* p2 = new EdgeNode;
		p2->to = 0;
		p2->capacity = 1;
		p2->flow = 0;
		p2->next = tree.vertexs[i].head; // 插入链表(头部)
		tree.vertexs[i].head = p2;
	}

	void showInfo(Graph g);
	// showInfo(tree);

	//for (int i = 0; i < tree.vexNum; i++) // 论文中3,4步
	//	for (int j = i + 1; j < tree.vexNum; j++)
	//	{
	//		if (isNear(i,j)) { // i,j为相邻节点
	//			int MaxFlow = max_flow(j, i); // 计算最大流,得到最小割
	//			assign(i, j, MaxFlow);
	//			assign(j, i, MaxFlow);
	//			for (int k = j + 1; k < tree.vexNum; k++) // 对每一个大于j的点k
	//				if (isNear(k,i) && bfs(j, k)) { // 若k是i的邻接顶点(Tree中), 且是j这一边（原图中）
	//					delectEdge(k, j); // 断开k与i
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
		int MaxFlow = max_flow(s, t); // 计算最大流,得到最小割
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