#include <iostream>
#include "EQ.h"
using namespace std;


int EQ::min(int x, int y) { // 返回x，y中较小的值
	return x < y ? x : y;
}

void EQ::enqueue(int x) { // x入队
	q[tail] = x;
	tail++;
	color[x] = GRAY;
}

int EQ::dequeue() { // 出队
	int x = q[head];
	head++;
	color[x] = BLACK;
	return x;
}

int EQ::bfs(int start, int target) { // start-target广度优先遍历
	int u, v;
	for (u = 0; u < n; u++) {
		color[u] = WHITE;
	}
	head = tail = 0;
	enqueue(start);
	augmentingPathArray[start] = -1;
	while (head != tail) {
		u = dequeue();
		// 搜索u所有的邻接顶点v，若v的color为WHITE且uv未满，则v入队
		for (v = 0; v < n; v++) {
			if (color[v] == WHITE && capacity[u][v] - flowMatrix[u][v] > 0) {
				enqueue(v);
				augmentingPathArray[v] = u;
			}
		}
	}

	// 若target顶点color为BLACK
	return color[target] == BLACK;
}


int EQ::max_flow(int source, int sink) { // Ford-Fulkerson算法计算最大流
	int i, j, u;
	int max_flow = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			flowMatrix[i][j] = 0;
		}
	}
	// 若增广路存在，更新改进量increment
	while (bfs(source, sink)) {
		// 计算increment
		int increment = INF;
		for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
			increment = min(increment, capacity[augmentingPathArray[u]][u] - flowMatrix[augmentingPathArray[u]][u]);
		}

		// 更新流量网络
		for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
			flowMatrix[augmentingPathArray[u]][u] += increment;
			flowMatrix[u][augmentingPathArray[u]] -= increment;
		}

		max_flow += increment;
	}

	return max_flow;
}

void EQ::read_input_file() { // 读入图
	int a, b, c, i, j;
	// 读入顶点数n，边数e
	scanf_s("%d %d", &n, &e);
	// 初始化容量矩阵
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			capacity[i][j] = 0;
		}
	}

	for (i = 0; i < e; i++) { // 读入边
		scanf_s("%d %d %d", &a, &b, &c);
		capacity[a][b] = c;
		capacity[b][a] = c;
	}
}

int EQ::show_info(){ // 输出GH树
	cout << "Tree：" << "\n";

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << tree[i][j] << "\t";
		cout << "\n";
	}
	return 0;
}

void EQ::build_GH_tree(){ // EQ法生成GH树
	int m_Flag = 0;

	for (int i = 0; i < n; i++) // 初始化最小割树
		for (int j = 0; j < n; j++)
			tree[i][j] = 0;

	for (int i = 0; i < n; i++) { // 初始化得到星形树，得到树根点
		for (int k = i + 1; k < n; k++) {
			if (capacity[i][k] != 0)
			{
				m_Flag = 1;
				break;
			}
		}
		if (m_Flag == 1)
		{
			for (int j = i + 1; j < n; j++)
			{
				tree[i][j] = tree[j][i] = 1;
			}
			m_Flag = i;
			break;
		}
	}

	for (int i = m_Flag; i < n; i++) // 得到树叶子节点
	{
		int m_Flag1 = 0;
		for (int j = m_Flag; j < n; j++)
			if (capacity[i][j] != 0)
			{
				m_Flag1 = 1;
				break;
			}

		if (m_Flag1 == 0)
			tree[i][m_Flag] = tree[m_Flag][i] = 0;
	}


	for (int i = 0; i < n; i++) // 论文中3,4步
		for (int j = i + 1; j < n; j++)
		{
			if (tree[i][j] != 0) // i,j为相邻节点
			{
				int MaxFlow = max_flow(j, i); // 计算最大流,得到最小割

				tree[i][j] = tree[j][i] = MaxFlow;

				for (int k = j + 1; k < n; k++) // 对每一个大于j的点k
					if (tree[k][i] != 0 && bfs(j, k)) // 若k是i的邻接顶点(Tree中), 且是j这一边（原图中）
					{
						tree[k][i] = tree[i][k] = 0; // 断开k与i
						tree[k][j] = tree[j][k] = 1; // 连接k与j
					}
			}
		}
}

int EQ::bfs_2(int start, int target) { // 广度优先遍历寻找start-target的路径
	int u, v;
	for (int i = 0; i < n; i++) {
		color[i] = WHITE;
	}
	head = tail = 0;
	enqueue(start);
	road[start] = -1;
	while (head != tail) {
		u = dequeue();
		// 搜索u所有的邻接顶点v，若v的color为WHITE且uv未满，则v入队
		for (v = 0; v < n; v++) {
			if (color[v] == WHITE && tree[u][v] != 0) {
				enqueue(v);
				road[v] = u;
			}
		}
	}

	// 若target顶点color为BLACK
	return color[target] == BLACK;
}

int EQ::mincut(int s, int t) { // 根据GH树计算s，t的最小割
	int m = INF;

	while (bfs_2(s, t)) {
		for (int u = t; road[u] != (-1); u = road[u]) {
			m = min(m, tree[road[u]][u]);
		}
		break;
	}
	return m;
}

int main(){
	EQ tree;
	tree.read_input_file(); // 读入图
	tree.build_GH_tree(); // 生成GH树
	tree.show_info(); // 输出GH树

	cout << "\n";
	cout << tree.mincut(2, 5) << endl; // 输出s，t的最小割
	return 0;
}