#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <queue>
#include <algorithm>
#include "data_handler.h"

int numofdoc = 0;
unordered_set<string> used_paper;
unordered_map<string, double> veclen;			// 正文向量长度
unordered_map<string, int> name2id;				// 论文编号
unordered_map<int, string> id2name;				// 论文标题
unordered_map<string, vector<string>> be_cited; // 被引用列表

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
	}
	if (!ordered(b))
	{
		sort(b.begin(), b.end());
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

namespace little
{
const int max_scc = 10000;
unordered_map<string, bool> inque;
unordered_map<string, int> dfn, low, belon;
int scc, tim;
vector<string> member[max_scc], sta;
vector<int> edges[max_scc];
int in_degree[max_scc];

void dfs(const string &x)
{
	sta.push_back(x);
	inque[x] = true;
	dfn[x] = low[x] = ++tim;
	for (auto &y : known_citings[x])
	{
		if (id_list.find(y) != id_list.end())
		{
			if (dfn[y] == 0)
			{
				dfs(y);
				low[x] = min(low[x], low[y]);
			}
			else if (inque[y])
				low[x] = min(low[x], dfn[y]);
		}
	}
	if (low[x] == dfn[x])
	{
		while (sta.size() > 0 && dfn[sta.back()] >= dfn[x])
		{
			member[belon[sta.back()] = scc].push_back(sta.back());
			inque[sta.back()] = false;
			sta.pop_back();
		}
		++scc;
	}
	return;
}

void output(const string &x)
{
	if (member[belon[x]].size() == 1)
		cout << x << endl;
	else
	{
		cout << "cirle ( ";
		cout << x;
		for (auto &y : member[belon[x]])
			if (y != x)
				cout << " " << y;
		cout << " )" << endl;
	}
	return;
}

// 拓扑排序，输出时形如：
// W96-0406
// W96-0501
// cirle ( W98-1404 W98-1411 )
// W98-1405
// W98-1406
void toposort()
{
	for (auto &x : id_list)
	{
		if (dfn[x] == 0)
			dfs(x);
	}
	for (int i = 0; i < scc; ++i)
		for (auto &x : member[i])
			for (auto &y : known_citings[x])
				if (id_list.find(y) != id_list.end())
					if (belon[y] != i)
						edges[i].push_back(belon[y]), ++in_degree[belon[y]];

	priority_queue<string, vector<string>, greater<string>> q;
	for (int i = 0; i < scc; ++i)
		if (in_degree[i] == 0)
			q.push(*min_element(member[i].begin(), member[i].end()));
	while (!q.empty())
	{
		string x = q.top();
		output(x);
		q.pop();
		int p = belon[x];
		for (auto &y : edges[p])
		{
			if (--in_degree[y] == 0)
				q.push(*min_element(member[y].begin(), member[y].end()));
		}
	}
	return;
}
}

int main()
{
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

	string id = "W09-1401";
	string id1 = "W02-0301", id2 = "W04-1213";

	vector<string> cited_list = get_cited_list(id);
	for (vector<string>::const_iterator itr = cited_list.begin(); itr != cited_list.end(); ++itr)
	{
		cout << *itr << endl;
	}
	cout << calc_docvec_cosine_similarity(id1, id2) << endl;
	cout << calc_indegree(id) << endl;
	cout << calc_outdegree(id) << endl;
	
	// 输出拓扑排序结果
	// cout << "toposort:\n";
	// little::toposort();
	return 0;
}

double sqr(double a)
{
	return a * a;
}