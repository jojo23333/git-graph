#include "GitGraph.h"
#include "GN.h"
#include "PageRank.h"
#include "FastUnfolding.h"

using namespace std;
int main(int argv, char **argc)
{
	NETWORK net;
	FILE *fp = NULL;

	if (argv<=2|| !strcmp(argc[1], "--help"))
	{
		if (argv <= 2) {
			cout << "Using --help to show options" << endl;
			return 1;
		}
		cout << "Graph Analyser" << endl;
		cout << "Format: <filename> <analyse method>" << endl;
		cout << "-GN: Using GN algorithm to detect community.Show the result partiontion." << endl;
		cout << "-FU: Using FastUfolding to detect community.Show partitions." << endl;
		cout << "-PR: Using PageRank to calculate key node.Show rank." << endl;
		cout << "-DG: Using degree to find key nodes.Show rank." << endl;
	}

	fopen_s(&fp, argc[1],"r");
	if (!fp) {
		cout << "Invalid filepath:" << argc[1] << endl;
		cout << "Using Sample data of karate" << endl;
		fopen_s(&fp, "C:\\Users\\77936\\Desktop\\karate\\karate.gml", "r");
	}
	read_network(&net,fp);
	gitgraph G(&net);
	cout << "Graph detected:" << endl;
	cout << "Graph Size:"<<G.index.size() << endl;
	cout << "Edge amount" << net.nvertices<<endl;
	cout << "Graph type:" << net.directed<<" (1 directed 0 undirected)"<<endl;
	free_network(&net);
	if (G.selfcheck())	cout << "Graph ok;" << endl;
	else	cout << "error" << endl;


	cout << endl;
	if (!strcmp(argc[2], "-PR")) {
		PageRank pr(G);
		pr.simple_pr();
		cout << "Output nodes' PageRank value in an increasing order:"<<endl;
		pr.out_put_rank();
	}
	else if (!strcmp(argc[2], "-FU")) {
		FastUnfolding::FastUnfolding fu(G);
		fu.fast_unfolding_main();
		fu.print_best_partition();
	}
	else if (!strcmp(argc[2], "-GN")) {
		GN com(G);
		com.Gn_main();
		com.print_partition();
	}

	//PageRank pr(G);
	//pr.simple_pr();
	//pr.out_put_rank();
	//pr.map_reduce_pr();
	//pr.out_put_rank();

	//GN com(G);
	//cout << "GN algorithm start" << endl;
	//com.Gn_main();
	//FastUnfolding::FastUnfolding fu(G);
	//fu.fast_unfolding_main();

	system("pause");
}
