#ifndef EQ_H
#define EQ_H

class EQ
{
private:
	#define WHITE 0
	#define GRAY 1
	#define BLACK 2
	#define maxn 100
	#define INF 1000000000

	int e;  // ����
	int capacity[maxn][maxn]; // ��������             
	int flowMatrix[maxn][maxn];     // ��������
	int augmentingPathArray[maxn];  // �б�洢����·
	int road[maxn];
	int head, tail; // ���ף���β
	int q[maxn + 2];

public:
	int n;  // ������
	int color[maxn]; // bfs��־
	int tree[maxn][maxn]; // ���ɵ�GH��
	int flowMatrix1[maxn][maxn];
	int min(int x, int y); // ���ؽ�Сֵ
	void enqueue(int x); // ���
	int dequeue(); // ����
	int bfs(int start, int target); // start-target������ȱ���
	int max_flow(int source, int sink); // ���������
	void read_input_file(); // ��ȡ�����ͼ���洢��capacity��
	int show_info(); // ���GH��
	void build_GH_tree(); // ����GH��
	int bfs_2(int start, int target);
	int mincut(int s, int t);
};

class MGH :public EQ {
public:
	void build_GH_tree(); // ����GH��
};

#endif // !EQ_H

#pragma once
