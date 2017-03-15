#pragma once
#include "GitGraph.h"
#include <assert.h>
#ifndef GN_H
#define GN_H

class GN {
	//erver node is mapped to an intager id
	map<string, int> mapper;
	map<int, string> reverse_mapper;
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


#endif
