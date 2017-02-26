#pragma once
#include "GitGraph.h"

using namespace std;

class PageRank {
private:
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

public:
	PageRank(gitgraph &g);
};

PageRank::PageRank(gitgraph &g)
{
	map<string, GitUser>::iterator it = g.index.begin();
	size_t t = g.index.size();
	double total_rank = 0;

	for (it = g.index.begin(); it != g.index.end(); it++) {
		val[it->first] = it->second.rank;
		income_edge[it->first] = &it->second.followers;
		outcome_edge[it->first] = &it->second.following;
		total_rank += it->second.rank;
	}
	for (it = g.index.begin(); it != g.index.end(); it++) {
		//set init pr_value for each node according to its weight
		val[it->first] = it->second.rank / total_rank;
	}
	damping_factor = 0.85;
	min_delta = 0.0000001;
	max_iterations = 200;
}

//using the simplest caculation of matrix	time complexity:O(logN*N^2)

bool PageRank::simple_pr()
{
	double damping_value = (1 - damping_factor) / val.size();

	for (int i = 0; i<max_iterations; i++)
	{
		//copy a map of last iteration's pr value to use it unchanged at this turn of iteration
		map<string,double> temp(val);
		double change = 1;

		for (map<string, double>::iterator it = val.begin(); it != val.end(); it++)
		{
			//current processing node
			string node = it->first;
			const vector<Follow> *in_edge = income_edge[node];
			//add node's income adjacent's pr*1/N
			double pr_sum = 0;
			for (int i = 0; i<in_edge->size(); i++)
			{
				string in_adj = (*in_edge)[i].from();
				int N = outcome_edge[in_adj]->size();
				pr_sum += temp[in_adj] * 1.0 / N;
			}
			//curerent node's pr value stored to val after this iteration
			val[node] = damping_factor*pr_sum + damping_value;
			//get the max change
			change = max(change, abs(val[node] - temp[node]));
		}
		if (change<min_delta)	break;
	}

	return true;
}

//using map-reduce method 

bool PageRank::map_reduce_pr()
{
	double damping_value = (1 - damping_factor) / val.size();

	for (int i = 0; i<max_iterations; i++)
	{
		map<string, double> temp(val);
		//used to record whether a node has appeared
		set<string> record;
		double change = 1;

		for (map<string, double>::iterator it = val.begin(); it != val.end(); it++)
		{
			//current processing node
			string node = it->first;
			const vector<Follow> *in_edge = outcome_edge[node];
			int N = outcome_edge[node]->size();
			double node_pr = temp[node];

			for (int i = 0; i<in_edge->size(); i++)
			{
				string out_adj = (*in_edge)[i].to();
				//if has appeared time add current pr*1/N
				if (record.count(out_adj))
					val[out_adj] += 1.0 / N*node_pr;
				else {
					//if has't appeared set init value pr*1/N
					val[out_adj] = 1.0 / N*node_pr;
					record.insert(out_adj);
				}
			}
		}

		for (map<string, double>::iterator it = val.begin(); it != val.end(); it++)
		{
			string node = it->first;
			//count pr
			if (record.count(node))
				val[node] = val[node] * damping_factor + damping_value;
			//get the max change
			change = max(change, abs(val[node] - temp[node]));
		}
		if (change<min_delta)	break;
	}
}
