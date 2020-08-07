#include <iostream>
#include "gomory_hu_tree.h"
using namespace std;


int EQ::min(int x, int y) { // ����x��y�н�С��ֵ
	return x < y ? x : y;
}

void EQ::enqueue(int x) { // x���
	q[tail] = x;
	tail++;
	color[x] = GRAY;
}

int EQ::dequeue() { // ����
	int x = q[head];
	head++;
	color[x] = BLACK;
	return x;
}

int EQ::bfs(int start, int target) { // start-target������ȱ���
	int u, v;
	for (u = 0; u < n; u++) {
		color[u] = WHITE;
	}
	head = tail = 0;
	enqueue(start);
	augmentingPathArray[start] = -1;
	while (head != tail) {
		u = dequeue();
		// ����u���е��ڽӶ���v����v��colorΪWHITE��uvδ������v���
		for (v = 0; v < n; v++) {
			if (color[v] == WHITE && capacity[u][v] - flowMatrix[u][v] > 0) {
				enqueue(v);
				augmentingPathArray[v] = u;
			}
		}
	}

	// ��target����colorΪBLACK
	return color[target] == BLACK;
}

int EQ::max_flow(int source, int sink) { // Ford-Fulkerson�㷨���������
	int i, j, u;
	int max_flow = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			flowMatrix[i][j] = 0;
		}
	}
	// ������·���ڣ����¸Ľ���increment
	while (bfs(source, sink)) {
		// ����increment
		int increment = INF;
		for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
			increment = min(increment, capacity[augmentingPathArray[u]][u] - flowMatrix[augmentingPathArray[u]][u]);
		}

		// ������������
		for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
			flowMatrix[augmentingPathArray[u]][u] += increment;
			flowMatrix[u][augmentingPathArray[u]] -= increment;
		}

		max_flow += increment;
	}

	return max_flow;
}

void EQ::read_input_file() { // ����ͼ
	int a, b, c, i, j;
	// ���붥����n������e
	scanf_s("%d %d", &n, &e);
	// ��ʼ����������
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			capacity[i][j] = 0;
		}
	}

	for (i = 0; i < e; i++) { // �����
		scanf_s("%d %d %d", &a, &b, &c);
		capacity[a][b] = c;
		capacity[b][a] = c;
	}
}

int EQ::show_info() { // ���GH��
	cout << "Tree��" << "\n";

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << tree[i][j] << "\t";
		cout << "\n";
	}
	return 0;
}

void EQ::build_GH_tree() { // EQ������GH��
	int m_Flag = 0;

	for (int i = 0; i < n; i++) // ��ʼ����С����
		for (int j = 0; j < n; j++)
			tree[i][j] = 0;

	for (int i = 0; i < n; i++) { // ��ʼ���õ����������õ�������
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

	for (int i = m_Flag; i < n; i++) // �õ���Ҷ�ӽڵ�
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


	for (int i = 0; i < n; i++) // ������3,4��
		for (int j = i + 1; j < n; j++)
		{
			if (tree[i][j] != 0) // i,jΪ���ڽڵ�
			{
				int MaxFlow = max_flow(j, i); // ���������,�õ���С��

				tree[i][j] = tree[j][i] = MaxFlow;

				for (int k = j + 1; k < n; k++) // ��ÿһ������j�ĵ�k
					if (tree[k][i] != 0 && bfs(j, k)) // ��k��i���ڽӶ���(Tree��), ����j��һ�ߣ�ԭͼ�У�
					{
						tree[k][i] = tree[i][k] = 0; // �Ͽ�k��i
						tree[k][j] = tree[j][k] = 1; // ����k��j
					}
			}
		}
}

int EQ::bfs_2(int start, int target) { // ������ȱ���Ѱ��start-target��·��
	int u, v;
	for (int i = 0; i < n; i++) {
		color[i] = WHITE;
	}
	head = tail = 0;
	enqueue(start);
	road[start] = -1;
	while (head != tail) {
		u = dequeue();
		// ����u���е��ڽӶ���v����v��colorΪWHITE��uvδ������v���
		for (v = 0; v < n; v++) {
			if (color[v] == WHITE && tree[u][v] != 0) {
				enqueue(v);
				road[v] = u;
			}
		}
	}

	// ��target����colorΪBLACK
	return color[target] == BLACK;
}

int EQ::mincut(int s, int t) { // ����GH������s��t����С��
	int m = INF;

	while (bfs_2(s, t)) {
		for (int u = t; road[u] != (-1); u = road[u]) {
			m = min(m, tree[road[u]][u]);
		}
		break;
	}
	return m;
}

void MGH::build_GH_tree() {
	int i, j, s, p[maxn], t, f1[maxn], q[maxn], minCut;

	for (i = 0; i < n; i++) {
		p[i] = 0;
		f1[i] = 0;
		q[i] = 0;
		for (j = 0; j < n; j++) {
			flowMatrix1[i][j] = 0;
			tree[i][j] = 0;
		}
	}

	for (s = 1; s < n; s++) {
		t = p[s];
		minCut = max_flow(s, t);
		f1[s] = minCut;

		for (i = 0; i < n; i++) {
			q[i] = 0; //initialising array q to 0 
		}

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				tree[i][j] = 0;
			}
		}

		for (i = 0; i < n; i++) {
			if (color[i] == BLACK) {
				q[i] = BLACK;  //checking for matches for coloring
			}
		}

		for (i = 0; i < n; i++) {
			if (i != s && p[i] == t && q[i] == BLACK) {
				p[i] = s;
			}
		}

		if (q[p[t]] == BLACK) {     //checking whether p[t] is in the nodes (colored black) then implementing the pseudo code.
			p[s] = p[t];
			p[t] = s;
			f1[s] = f1[t];
			f1[t] = minCut;
		}

		flowMatrix1[s][t] = flowMatrix1[t][s] = minCut;
		for (int i = 0; i < s; i++)
			if (i != t)
				flowMatrix1[s][i] = flowMatrix1[i][s] = min(minCut, flowMatrix1[t][i]);

		for (i = 1; i <= s; i++) {
			tree[i][p[i]] = tree[p[i]][i] = f1[i];
		}
	}
}


int main() {

	EQ tree;
	tree.read_input_file(); // ����ͼ
	tree.build_GH_tree(); // ����GH��
	tree.show_info(); // ���GH��
	cout << "\n";
	cout << tree.mincut(2, 5) << endl; // ���s��t����С��

	MGH tree2;
	tree2.read_input_file(); // ����ͼ
	tree2.build_GH_tree(); // ����GH��
	tree2.show_info(); // ���GH��
	cout << "\n";
	cout << tree.mincut(2, 5) << endl; // ���s��t����С��

	return 0;
}