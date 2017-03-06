#pragma once
#ifndef PAGERANK_H
#define PAGERANK_H
#include "GitGraph.h"

using namespace std;

class PageRank {
public:
	//record pr value 
	map<string, double> val;
	//record each user's income edge
	map<string,const vector<Follow>*> income_edge;
	map<string,const vector<Follow>*> outcome_edge;
	//alpha 
	double damping_factor;
	double min_delta;
	//max iterations 
	int max_iterations;
	bool simple_pr();
	bool map_reduce_pr();
	bool out_put_rank();

public:
	PageRank(gitgraph &g);
};

#endif