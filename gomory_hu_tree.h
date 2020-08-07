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

	int e;  // 边数
	int capacity[maxn][maxn]; // 容量矩阵             
	int flowMatrix[maxn][maxn];     // 流量矩阵
	int augmentingPathArray[maxn];  // 列表存储增广路
	int road[maxn];
	int head, tail; // 队首，队尾
	int q[maxn + 2];

public:
	int n;  // 顶点数
	int color[maxn]; // bfs标志
	int tree[maxn][maxn]; // 生成的GH树
	int flowMatrix1[maxn][maxn];
	int min(int x, int y); // 返回较小值
	void enqueue(int x); // 入队
	int dequeue(); // 出队
	int bfs(int start, int target); // start-target广度优先遍历
	int max_flow(int source, int sink); // 计算最大流
	void read_input_file(); // 读取输入的图，存储在capacity中
	int show_info(); // 输出GH树
	void build_GH_tree(); // 构造GH树
	int bfs_2(int start, int target);
	int mincut(int s, int t);
};

class MGH :public EQ {
public:
	void build_GH_tree(); // 构造GH树
};

#endif // !EQ_H

#pragma once
