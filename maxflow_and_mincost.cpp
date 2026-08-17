#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include<queue>
#include<map>
#include<set>
#include<climits>
#include <unordered_map>
using namespace std;
class Node
{
public:
    int to;      // 边的终点
    int cap;     // 残留容量：还能流过多少流量（反向边初始为 0，流过流量后用来"退流"）
    int cost;    // 单位费用（反向边为负：退流时费用也要退）
    int rev;     // 反向边在对方邻接表中的下标
    Node(){}
    Node(int a, int b, int c,int d)
    {
        to = a;
        cap = b;
        cost = c;
        rev = d;
    }
};

class maxflow_and_mincost
{
public:
    vector<vector<Node>>arr;   // 邻接表：arr[u] 存所有从 u 出发的边（正向+反向）
    vector<bool>vis;           // SPFA 中标记"是否在队列里"，避免重复入队
    vector<int>pre_v, pre_e;   // 前驱点 / 前驱边：记录最短费用路的路径，用于回退找瓶颈并增广
    vector<int>sum;            // sum[v] = s 到 v 的最短单位费用（残留网络中）
    maxflow_and_mincost(){}
    maxflow_and_mincost(int n)
    {
        arr.resize(n + 7);
        vis.resize(n + 7, false);
        pre_v.resize(n + 7);
        pre_e.resize(n + 7);
        sum.resize(n + 7);
    }
    // 加边 u->v，容量 cap，单位费用 cost；同时建容量 0、费用 -cost 的反向边
    void add(int u, int v, int cap,int cost)
    {
        arr[u].push_back({ v,cap,cost,arr[v].size() });      // 正向边 rev = 反向边将要插入的位置
        arr[v].push_back({ u,0,-cost,arr[u].size() - 1 });   // 反向边 rev = 正向边位置
    }
    // SPFA 找"单位费用最短路"（在残留网络中）
    // 注意必须用 SPFA 而不是 Dijkstra：残留网络含负费用边（反向边 -cost），Dijkstra 失效
    // 返回 s 能否到 t（到不了 = 没有增广路 = 费用流结束）
    bool SPFA(int s, int t)
    {
        for (int i = 0; i < vis.size(); i++)vis[i] = false;
        for (int i = 0; i < sum.size(); i++)sum[i] = INT_MAX;
        queue<int>q;
        q.push(s);
        vis[s] = true;
        sum[s] = 0;
        while (q.size())
        {
            int u = q.front();
            q.pop();
            vis[u] = false;                    // 出队，之后可能再次入队（SPFA 可重复松弛）
            for (int i = 0; i < arr[u].size(); i++)
            {
                Node& v = arr[u][i];
                // 只走残留容量 > 0 的边；能松弛就更新最短费用 + 记录前驱
                if (v.cap > 0 && sum[v.to] > sum[u] + v.cost)
                {
                    sum[v.to] = sum[u] + v.cost;
                    pre_v[v.to] = u;           // v.to 的前驱点是 u
                    pre_e[v.to] = i;           // 前驱边是 arr[u] 里的第 i 条
                    if (!vis[v.to])
                    {
                        vis[v.to] = true;
                        q.push(v.to);
                    }
                }
            }
        }
        return sum[t] != INT_MAX;
    }
    // 主过程：反复"SPFA 找最短费用路 → 沿路增广"，直到没有增广路
    // 原理（贪心定理）：每次沿当前残留网络的最小单位费用路增广，得到的流始终是"同流量中最便宜的"
    // 返回 { 最大流, 最小总费用 }
    pair<int,int> get_maxflow_mincost(int s,int t)
    {
        int flow = 0, cost = 0;
        while (SPFA(s, t))
        {
            // 1) 沿前驱链回退，找这条增广路的瓶颈（最小残留容量）
            int add = INT_MAX;
            for (int v = t; v != s; v = pre_v[v])
            {
                int u = pre_v[v];
                int e = pre_e[v];
                add = min(add, arr[u][e].cap);
            }
            // 2) 沿路推送 add 流量：正向边扣、反向边加（反向边 = 可退流）
            for (int i = t; i != s; i = pre_v[i])
            {
                int u = pre_v[i];
                int e = pre_e[i];
                Node& v = arr[u][e];
                v.cap -= add;
                arr[v.to][v.rev].cap += add;
            }
            flow += add;
            cost += (add * sum[t]);   // 总费用 = 瓶颈流量 × 这条路的单位费用和
        }
        return { flow,cost };
    }
};

void work() 
{
    // 用法：
    // maxflow_and_mincost mcf(n);
    // mcf.add(u, v, cap, cost);            // 建图（每条边都要 add，自动配反向边）
    // auto [flow, cost] = mcf.get_maxflow_mincost(s, t);   // C++17 结构化绑定
}

int main() 
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int _  = 1;
    cin >> _;
    while (_--) 
    {
        work();
    }
    return 0;
}
