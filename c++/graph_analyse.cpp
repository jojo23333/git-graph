#include "MysqlApi.h"

using namespace std;
int main()
{
	cout<<"ok"<<endl;
	mysql_api DB;
	gitgraph G;
	DB.write_all(G);
	cout<<G.index.size()<<endl;
}

class GN{
	//erver node is mapped to an intager id
	map<string,int> mapper;
	//flags record wheather the edge has been deleted
	struct edge{
		int v1,v2;
		int weight;
		double betweenness;
		bool flag;
		edge(int v1,int v2,int weight):v1(v1),v2(v2),weight(weight){}
		edge(){flag=true;}
	};
	vector<vector<edge> > G;
	
	struct Q_status{
		edge deleted_edge;
		vector<int> communities;
		double Q;
	};
	vector<Q_status> Q_list;
	public:
		GN(gitgraph &g);
		bool cal_betweenness();
		bool cal_current_Q();
}

GN::GN(gitgraph &g)
{
	//mapping process give eache user an id from 0-graphsize
	int size = 0;
	for(map<string,GitUser>::iterator it = g.index.begin(); it!=g.index.end(); it++){
		mapper[it->first] = size;
		size++;
	}
	
	//transfer directed graph to undirected graph
	for(map<string,GitUser>::iterator it = g.index.begin(); it!=g.index.end(); it++){
		vector<edge> e; 
		set<string> adj;
		
		vector<GitUser> &p = it->second.following;
		string v1 = it->first
		for(int i=0;i<p.size();i++){
			string v2 = p[i].following;
			//for user v2 that mutual follows with v1 give weight of 1
			//only following ohthers give 0.6 else give 0.4
			if(g.isfollower(v1,v2))
				e.push_back(edge(mapper[v1],mapper[v2],1));
			else
				e.push_back(edge(mapper[v1],mapper[v2],0.6));
			//record this user has appeared
			adj.insert(v2);
		}
		
		vector<GitUser> &q = it->second.followers;
		for(int i=0;i<q.size();i++){
			string v2 = p[i].follower;
			//for user that hsa appeared jump
			if(adj.count(v2))	continue;
			e.push_back(edge(mapper[v1],mapper[v2],0.4));
		}
		G.push_back(e);
	}
	
}
