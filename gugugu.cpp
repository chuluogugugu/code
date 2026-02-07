#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<set>
#include<climits>
#include<cmath>
#include<map>
#include<unordered_set>
#include<stack>
using namespace std;
using ll = long long;
//const ll MOD = 1e9 + 7;
vector<ll>j(27, 1);
vector<bool>vis(27, false);
void f()
{
    for (int i = 1; i <= 26; i++)
    {
        j[i] = (j[i - 1] * i);
    }
}
ll g(string a)
{
    ll ans = 0;
    for (int i = 0; i < 12; i++)
    {
        int w = a[i] - 'a' + 1;
        int cnt = 0;
        for (int j = 1; j < w; j++)
        {
            if (!vis[j])cnt++;;
        }
        vis[w] = true;
        ans = (ans + cnt * j[12 - i - 1]);
    }
    return ans + 1;
}
void work()
{
    fill(vis.begin(), vis.end(), false);
    string a;
    cin >> a;
    cout << g(a) << "\n";
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int _ = 1;
    f();
    cin >> _;
    while (_--)
    {
        work();
    }
    return 0;
}