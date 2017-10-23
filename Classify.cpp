#include <cstring>
#include <iostream>
#include <cmath>
#include <queue>
#include <algorithm>
#include "data_handler.h"

int numofdoc = 0;
set<string> used_paper;
map<string, double> veclen;           // 正文向量长度
map<string, int> name2id;             // 论文编号
map<int, string> id2name;             // 论文标题
map<string, vector<string>> be_cited; // 被引用列表

double sqr(double a);

//返回被引用列表
vector<string> get_cited_list(string id)
{
    return be_cited[id];
}

//计算正文向量夹角cosine值
double calc_docvec_cosine_similarity(string id1, string id2)
{
    double sim = 0.0;
    for (int i = 0; i < docvecs[id1].size() && i < docvecs[id2].size(); ++i)
    {
        sim += docvecs[id1][i] * docvecs[id2][i];
    }
    return sim / veclen[id1] / veclen[id2];
}

//计算被引用次数
int calc_indegree(string id)
{
    int indegree = 0;
    indegree = be_cited[id].size();
    return indegree;
}

//计算引用数量
int calc_outdegree(string id)
{
    int outdegree = 0;
    outdegree = known_citings[id].size();
    return outdegree;
}

inline bool ordered(const vector<string> &a)
{
    for (int i = 1; i < a.size(); ++i)
    {
        if (a[i] <= a[i - 1])
            return false;
    }
    return true;
}

double calc_jaccard(vector<string> &a, vector<string> &b)
{
    if (!ordered(a))
    {
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
    }
    if (!ordered(b))
    {
        sort(b.begin(), b.end());
        b.erase(unique(b.begin(), b.end()), b.end());
    }
    vector<string>::iterator i = a.begin(), j = b.begin();
    double ans = 0;
    while (i != a.end() && j != b.end())
    {
        if (*i == *j)
        {
            ++ans;
            ++i;
            ++j;
        }
        else if (*i < *j)
            ++i;
        else
            ++j;
    }
    return ans / (a.size() + b.size() - ans);
}

//引用列表jaccard相似度
double citting_jaccard(const string &a, const string &b)
{
    return calc_jaccard(known_citings[a], known_citings[b]);
}

//引用列表jaccard相似度
double citted_jaccard(const string &a, const string &b)
{
    return calc_jaccard(be_cited[a], be_cited[b]);
}

int main()
{
    FILE *fp = fopen("laplace.txt", "w");
    load_all_data();
    for (auto &x : known_citings)
    {
        used_paper.insert(x.first);
        for (auto &y : x.second)
            be_cited[y].push_back(x.first), used_paper.insert(y);
    }
    for (auto &x : id_list)
        id2name[name2id[x] = numofdoc++] = x;
    for (auto &x : id_list)
    {
        double l = 0;
        for (auto &y : docvecs[x])
            l += sqr(y);
        veclen[x] = sqrt(l);
    }
    int tmp=0;
    bool *vis = new bool[numofdoc];
    for (int i = 0; i < numofdoc; ++i)
    {
        memset(vis, 0, numofdoc);
        int ct = 0;
        for (auto &x : known_citings[id2name[i]])
            if (id_list.find(x) != id_list.end())
                ++ct, vis[name2id[x]] = true;
        if (ct==0) ++tmp;
        /* for (int j = 0; j < numofdoc; ++j)
        {
            if (i == j)
                fprintf(fp, "%d", ct);
            else
                fprintf(fp, "%d", vis[j] ? -1 : 0);
            if (j != numofdoc - 1)
                fprintf(fp, ", ");
            else
                fprintf(fp, "\n");
        } */
    }
    printf("gg %d\n",tmp);
    return 0;
}

double sqr(double a)
{
	return a * a;
}