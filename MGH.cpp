#include <stdio.h>
#include "MGH.h"
#include <iostream>
using namespace std;


int MGH::min(int x, int y) { // 返回x，y中较小的值
    return x<y ? x : y;  
}

void MGH::enqueue(int x) { // x入队
    q[tail] = x;
    tail++;
    color[x] = GRAY;
}

int MGH::dequeue() { // 出队
    int x = q[head];
    head++;
    color[x] = BLACK;
    return x;
}

int MGH::bfs(int start, int target) { // start-target广度优先遍历
    int u, v;
    for (u = 0; u<n; u++) {
        color[u] = WHITE;
    }
    head = tail = 0;
    enqueue(start);
    augmentingPathArray[start] = -1;
    while (head != tail) {
        u = dequeue();
        // 搜索u所有的邻接顶点v，若v的color为WHITE且uv未满，则v入队
        for (v = 0; v<n; v++) {
            if (color[v] == WHITE && capacity[u][v] - flowMatrix[u][v]>0) {
                enqueue(v);
                augmentingPathArray[v] = u;
            }
        }
    }

    // 若target顶点color为BLACK
    return color[target] == BLACK;
}

int MGH::max_flow(int source, int sink) { // Ford-Fulkerson算法计算最大流
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

void MGH::read_input_file() { // 读入图
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

int MGH::show_info() { // 输出GH树
    cout << "Tree：" << "\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << tree[i][j] << "\t";
        cout << "\n";
    }
    return 0;
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

int MGH::bfs_2(int start, int target) { // 广度优先遍历寻找start-target的路径
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

int MGH::mincut(int s, int t) { // 根据GH树计算s，t的最小割
    int m = INF;

    while (bfs_2(s, t)) {
        for (int u = t; road[u] != (-1); u = road[u]) {
            m = min(m, tree[road[u]][u]);
        }
        break;
    }
    return m;
}

int main() {
    MGH tree;
    tree.read_input_file();
    tree.build_GH_tree();
    tree.show_info();

    cout << "\n";
    cout << tree.mincut(2, 3);
    return 0;
}