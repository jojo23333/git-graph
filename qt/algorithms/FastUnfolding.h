#pragma once
#include "GitGraph.h"

#ifndef FASTUNFOLDING_H
#define FASTUNFOLDING_H

namespace FastUnfolding {

	struct Edge {
		int v1, v2;
		double weight;
		Edge(int v1, int v2, double weight) :v1(v1), v2(v2), weight(weight) { }
		Edge() {}
	};


	struct Community {
		double sum_in;
		double sum_tot;
		bool deleted;
		int cmty_num;
		vector<int> members;
		Community() { sum_in = 0;sum_tot = 0; deleted = false; }
		Community(const Community & c) :sum_in(c.sum_in), sum_tot(c.sum_tot), deleted(c.deleted),
			members(c.members) {}
	};



	struct Partition {
		vector<int> cmty;
		double modularity;
		Partition(vector<int> a, double q) :cmty(a), modularity(q) {}
	};

	//cmty record current info for every communities
	//cmty_status record community's index(index for cmty) for every node
	class Iteration {
	public:
		vector<vector<Edge> >G;
		vector<Community> cmty;
		vector<int> cmty_status;
		vector<double> in, tot;
		double m;

	public:
		void clear() { G.clear();cmty.clear();cmty_status.clear();in.clear();tot.clear(); }
		void cmty_add_member(int v,Community &c);
		void cmty_del_member(vector<int>::iterator it,Community &c);
		void community_init();
		inline int get_cmty(int id);
		inline bool in_same_cmty(int v1, int v2);
		//inline double get_ki(int v);
		__declspec(noinline) double modularity();
		double delta_modularity(int v1,int c1);
		double modularity_optimize();
		void aggregate_from_previous(Iteration &pre, vector<int>& status);
	};

}




#endif // !FASTUNFOLDING_H
