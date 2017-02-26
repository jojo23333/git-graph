#pragma once
#include "GitGraph.h"
#include <assert.h>

class GN {
	//erver node is mapped to an intager id
	map<string, int> mapper;
	//flags record wheather the edge has been deleted
	struct edge {
	//v1 represent self v2 adjacent
		int v1, v2;
		double weight;
		double betweenness;
		bool deleted;
		edge(int v1, int v2, double weight) :v1(v1), v2(v2), weight(weight) { deleted = false; }
		edge() { deleted = false; }
	};
	vector<vector<edge> > G;
	//Disjoint set for discovering communities;
	vector<int> DJ_set;

	struct Q_status {
		vector<int> community;
		double Q;
	}Best_Q;
	__declspec(noinline) double delete_max_betweeness();
	int connected_component();
	bool check_connectivity();
public:
	GN(gitgraph &g);
	bool Gn_main();
	bool Gn_betweenness();
	double Gn_modularity();
};

GN::GN(gitgraph &g)
{
	//mapping process give eache user an id from 0-graphsize
	int size = 0;
	for (map<string, GitUser>::iterator it = g.index.begin(); it != g.index.end(); it++) {
		mapper[it->first] = size;
		size++;
	}

	//transfer directed graph to undirected graph
	for (map<string, GitUser>::iterator it = g.index.begin(); it != g.index.end(); it++) {
		vector<edge> e;
		set<string> adj;

		vector<Follow> &p = it->second.following;
		string v1 = it->first;
		for (int i = 0; i<p.size(); i++) {
			string v2 = p[i].following;
			//for user v2 that mutual follows with v1 give weight of 1
			if (g.isfollower(v1, v2))
				e.push_back(edge(mapper[v1], mapper[v2], 1));
			else
				e.push_back(edge(mapper[v1], mapper[v2], 0.5));
			//record this user has appeared
			adj.insert(v2);
		}

		vector<Follow> &q = it->second.followers;
		for (int i = 0; i<q.size(); i++) {
			string v2 = q[i].follower;
			//for user that hsa appeared jump
			if (adj.count(v2))	continue;
			e.push_back(edge(mapper[v1], mapper[v2], 0.5));
		}
		G.push_back(e);
	}

}

bool GN::check_connectivity()
{
	for (int i = 0; i < G.size(); i++) {
		for (vector<edge>::iterator it = G[i].begin(); it != G[i].end(); it++) {
			assert(it->v1 == i);
			int v2 = it->v2,v1=it->v1;
			bool flag = false;
			for (vector<edge>::iterator ia = G[v2].begin(); ia != G[v2].end(); ia++) {
				if (it->v1 == v2&&it->v2 == v1) {
					flag = true;
					break;
				}
			}
			if (flag) return false;
		}
	}
	return true;
}


//GravinNewman algorithm main proccess

bool GN::Gn_main()
{
	double Q = 0;
	Best_Q.Q = 0;
	size_t max_cmty = G.size(),cmty=1,new_cmty;
	//until every node is a unique community
	if (check_connectivity())	cout << "ok" << endl;
	while (cmty < max_cmty){
		Gn_betweenness();
		cout << delete_max_betweeness() << endl;;
		new_cmty = connected_component();
		if (new_cmty < cmty)
			Gn_modularity();
	}
	return true;
}


//Func to delete max betweeness
//delete the max of betweens from both sides of v1 v2 O(nm)

__declspec(noinline) double GN::delete_max_betweeness()
{
	vector<edge>::iterator max_node;
	double max_val = 0;
	for (int i = 0; i < G.size(); i++)
		for (vector<edge>::iterator it = G[i].begin(); it != G[i].end(); it++) {
			if (it->deleted) continue;
			if (it->betweenness > max_val) {
				max_node = it;
				max_val = it->betweenness;
			}
		}
	max_node->deleted = true;
	int v2 = max_node->v2;
	for (vector<edge>::iterator it = G[v2].begin(); it != G[v2].end(); it++) {
		if (it->v2 == max_node->v1&&it->v2 == v2)
			it->deleted = true;
	}
	return max_node->betweenness;
}


//Func to calculate number of connected sub graphs
//Using disjoint set O(nm+n)

int GN::connected_component()
{
	DJ_set.clear();
	for (int i = 0; i < G.size(); i++)
		DJ_set.push_back(i);
	for (int i = 0; i < G.size(); i++) {
		for (vector<edge>::iterator it = G[i].begin(); it != G[i].end(); it++) {
			DJ_set[it->v2] = DJ_set[i];
		}
	}
	set<int> com;
	for (int i = 0; i < G.size(); i++)
		com.insert(DJ_set[i]);
	return com.size();
}


//Func to calculate betweeness

bool GN::Gn_betweenness()
{
	for (int i = 0; i < G.size(); i++) {
		vector<edge> &N = G[i];

		int next_node,cur_node = i;
		//map graph index to bfs_cache node index
		map<int, int> mapper;
		//store graph node in BFS order bfs[i][0] store Distace S bfs[i][1] store num of shortest path
		vector<vector<int> > bfs_cache;
		queue<int> bfs_q;
		
		//push to both BFS vector and BFS queue
		bfs_cache.push_back(vector<int>(2, 1));
		mapper[cur_node] = bfs_cache.size()-1;
		bfs_q.push(cur_node);
			
		while (!bfs_q.empty()) {
			int next_index,cur_index = mapper[cur_node];
			cur_node = bfs_q.front();
			bfs_q.pop();
			
			bool flag=true;

			//For every edge of cur_node BFS add
			for (vector<edge>::iterator it = G[cur_node].begin(); it != G[cur_node].end(); it++) {
				//Always remember to see whether the edge has been deleted	
				if (it->deleted)	continue;
				next_node = it->v2;
				if (mapper.count(next_node)) {
					next_index = mapper[next_node];
					//if met with possiblities of shortest path
					if(bfs_cache[cur_index][0]==bfs_cache[next_index][0]+1){
						flag = false;
						//next_node is one of cur_node's father add father's w to son's w and record father
						bfs_cache[cur_index][1] += bfs_cache[next_index][1];
						bfs_cache[cur_index].push_back(next_index);
					}
				}
				else {
					vector<int> tmp; 
					tmp.push_back(bfs_cache[cur_index][0] + 1);
					tmp.push_back(0);
					//push to both BFS vector and BFS queue
					bfs_cache.push_back(tmp);
					mapper[next_node] = bfs_cache.size() - 1;
					bfs_q.push(next_node);
				}
			}
			if (flag) {
				cout << cur_node << endl;
				system("pause");
			}
		}

		//count part
		vector<double> bet(bfs_cache.size(),0);
		for (int j = bfs_cache.size() - 1; j >= 0; j--) {
			int w = bfs_cache[j][1];
		//	cout << bet[j] << ":" << w << endl;
			bet[j]++;
			for (int it = 2; it<bfs_cache[j].size(); it++) {
				bet[bfs_cache[j][it]] += 1.0 / w * bet[j];
			}
		}
		cout << bet.size() << endl;
		for (vector<edge>::iterator it = G[cur_node].begin(); it != G[cur_node].end(); it++)
		{
			if (it->deleted)	continue;
			//Weighted GN betweeness
			it->betweenness = bet[mapper[it->v2]]/it->weight;
		//	cout << mapper[it->v2]<<" "<<bet[mapper[it->v2]] << endl;
		}
		system("pause");
	}
	return true;
}


//Func to calculate modularity

double GN::Gn_modularity()
{
	vector<double> K;
	double M=0;
	for (int i=0; i < G.size(); i++) {
		double tmp = 0;
		for (vector<edge>::iterator it = G[i].begin(); it != G[i].end(); it++) {
			tmp += it->weight;
		}
		K.push_back(tmp);
		M += tmp;
	}

	M = M / 2;
	double sum=0;
	for (int i=0; i < G.size(); i++) {
		for (vector<edge>::iterator it = G[i].begin(); it != G[i].end(); it++) {
			if (it->deleted)	continue;
			// if does not belong to the same community
			if (DJ_set[it->v1] != DJ_set[it->v2])	continue;
			sum += it->weight - K[it->v1]*K[it->v2]/(2*M);
		}
	}
	sum = sum / (2 * M);
	if (sum > Best_Q.Q)
	{
		Best_Q.Q = sum;
		Best_Q.community.clear();
		Best_Q.community.assign(DJ_set.begin(),DJ_set.end());
	}
	return sum;
}
