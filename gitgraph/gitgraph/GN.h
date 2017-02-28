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
	int connected_component_getf(int v);
	bool betweeness_init();
	bool check_connectivity();
public:
	GN(gitgraph &g);
	bool Gn_main();
	bool Gn_betweenness();
	double Gn_modularity();
};

//Convert directed git graph to undirected GN graph
//Graph generated,should followed with check_connectivity

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

//Ensuring that the graph is a undirected graph
//OK return true else false

bool GN::check_connectivity()
{
	int total = 0;
	//cout << "Vertex amount" << G.size() << endl;
	for (int i = 0; i < G.size(); i++) {
		total += G[i].size();
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
	cout << "Init components :"<< connected_component() << endl;
	cmty = connected_component();
//	for (map<string, int>::iterator it = mapper.begin();it != mapper.end();it++)
	//	cout << it->first << "-" << it->second<<endl;
	if (check_connectivity())	cout << "ok" << endl;
	while (cmty < max_cmty){
		Gn_betweenness();
		cout << delete_max_betweeness()<<endl;
		new_cmty = connected_component();
		if (new_cmty > cmty){
			cout<<Gn_modularity()<<endl;
			cmty = new_cmty;
		}
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
		if (it->v2 == max_node->v1&&it->v1 == v2)
			it->deleted = true;
	}
	cout << "Del:" << max_node->v1 << "-" << max_node->v2 << endl;
	return max_node->betweenness;
}


//Func to calculate number of connected sub graphs
//Using disjoint set O(nm+n)

int GN::connected_component()
{
	int sum=0;
	DJ_set.clear();
	for (int i = 0; i < G.size(); i++)
		DJ_set.push_back(i);
	for (int i = 0; i < G.size(); i++) {
		for (vector<edge>::iterator it = G[i].begin(); it != G[i].end(); it++) {
			if (it->deleted)	continue;
			//Disjoint set's merge step
			int f1 = connected_component_getf(it->v1);
			int f2 = connected_component_getf(it->v2);
			if (f1 != f2)	DJ_set[f2] = f1;
		}
	}
	for (int i = 0; i < G.size(); i++) {
		//To ensure nodes in the same community point to the same father
		DJ_set[i] = connected_component_getf(DJ_set[i]);
		if (DJ_set[i] == i)
			sum++;
	}
	return sum;
}


//Dsjoint set proccess to find father

int GN::connected_component_getf(int v)
{
	if (DJ_set[v] == v)	return v;
	else return DJ_set[v] = connected_component_getf(DJ_set[v]);
}


bool GN::betweeness_init() 
{
	for (int i = 0;i < G.size();i++) {
		for (vector<edge>::iterator it = G[i].begin();it != G[i].end();it++) {
			it->betweenness = 0;
		}
	}
	return true;
}


//Func to calculate betweeness

bool GN::Gn_betweenness()
{
	betweeness_init();
	int total = 0;
	for (int i = 0; i < G.size(); i++) {
		vector<edge> &N = G[i];

		int next_node,cur_node = i;
		//map graph index to bfs_cache node index
		map<int, int> mapper_n;
		//store graph node in BFS order bfs[i][0] store Distace S bfs[i][1] store num of shortest path
		vector<vector<int> > bfs_cache;
		vector<int> bfs_q;
		
		//push to both BFS vector and BFS queue
		bfs_cache.push_back(vector<int>(2, 1));
		mapper_n[cur_node] = bfs_cache.size()-1;
		bfs_q.push_back(cur_node);
		for (int bfs_p=0; bfs_p < bfs_q.size(); bfs_p++) {
			cur_node = bfs_q[bfs_p];
			int next_index,cur_index = mapper_n[cur_node];
			bool flag=true;

			//For every edge of cur_node BFS add
			for (vector<edge>::iterator it = G[cur_node].begin(); it != G[cur_node].end(); it++) {
				//Always remember to see whether the edge has been deleted	
				if (it->deleted)	continue;
				next_node = it->v2;
				if (mapper_n.count(next_node)) {
					next_index = mapper_n[next_node];
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
					mapper_n[next_node] = bfs_cache.size() - 1;
					bfs_q.push_back(next_node);
				}
			}
			//Code module to detect error
			if (flag&&cur_node!=i) {
				cout << "node:"<<cur_node <<"curindex:"<<cur_index<< endl;
				system("pause");
			}
		}

		//count part,bet correspond with bfs_cache's position
		vector<double> bet(bfs_cache.size(),0);
		for (int j = bfs_cache.size() - 1; j >= 0; j--) {
			int w = bfs_cache[j][1];
		//	cout <<"W:" <<w << endl;
			bet[j]++;
			for (int it = 2; it<bfs_cache[j].size(); it++) {
				bet[bfs_cache[j][it]] += 1.0 / w * bet[j];
			}
		}

		//cout << bfs_q.size() << endl;
		//Sum and save part
		for (int j = 0;j < G.size(); j++) {
			for (vector<edge>::iterator it = G[j].begin();it != G[j].end();it++) {
				if (it->deleted)	continue;
				int node1 = it->v1, node2 = it->v2;
				int index1 = mapper_n[node1], index2 = mapper_n[node2];
				if (bfs_cache[index1][0] == bfs_cache[index2][0] + 1)
					it->betweenness += bet[index1] / it->weight;
				else if (bfs_cache[index1][0] == bfs_cache[index2][0] - 1)
					it->betweenness += bet[index2] / it->weight;
			}
		}
	}
	
	/*for (int i = 0;i < G.size();i++) {
		for (vector<edge>::iterator it = G[i].begin();it != G[i].end();it++) {
			cout << it->v1 << "-" << it->v2 << ":" << it->betweenness<<endl;
		}
	}*/
	//cout << "Total" << total;
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
			//probably no need to ignore deletedfile
			//if (it->deleted)	continue;
			tmp += it->weight;
		}
		K.push_back(tmp);
		M += tmp;
	}

	M = M / 2;
	double sum=0;
	int gsize = G.size();
	vector<vector<double> > matrix(G.size(),vector<double>(G.size(),0));

	for (int i = 0;i < G.size();i++) {
		for (vector<edge>::iterator it = G[i].begin();it != G[i].end();it++) {
			//if (it->deleted)	continue;
			matrix[it->v1][it->v2] = it->weight;
		}
	}
	for(int i = 0;i < G.size();i++)
		for (int j = 0;j < G.size();j++) {
			matrix[i][j] -= K[i] * K[j] / (2 * M);
			if (DJ_set[i] == DJ_set[j])
				sum += matrix[i][j];
			else
				sum -= matrix[i][j];
		}
	//for (int i=0; i < g.size(); i++) {
	//	for (vector<edge>::iterator it = g[i].begin(); it != g[i].end(); it++) {
	//		if (it->deleted)	continue;
	//		// if does not belong to the same community
	//		if (dj_set[it->v1] != dj_set[it->v2])	continue;
	//		sum += it->weight - k[it->v1]*k[it->v2]/(2*m);
	//	}
	//}
	sum = sum / (4 * M);
	if (sum > Best_Q.Q)
	{
		Best_Q.Q = sum;
		Best_Q.community.clear();
		Best_Q.community.assign(DJ_set.begin(),DJ_set.end());
	}
	return sum;
}
