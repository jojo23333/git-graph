#include "GitGraph.h"
#include "GN.h"
#include "PageRank.h"
#include "FastUnfolding.h"

using namespace std;
int main()
{
	NETWORK net;
	FILE *fp;
	fopen_s(&fp,"C:\\Users\\77936\\Desktop\\karate\\karate.gml", "r");
	read_network(&net,fp);
	gitgraph G(&net);
	cout << G.index.size() << endl;

	if (G.selfcheck())	cout << "Graph ok;" << endl;
	else	cout << "error" << endl;

	//PageRank pr(G);
	//pr.simple_pr();
	//pr.out_put_rank();
	//pr.map_reduce_pr();
	//pr.out_put_rank();

	//GN com(G);
	//cout << "GN algorithm start" << endl;
	//com.Gn_main();
	FastUnfolding::FastUnfolding fu(G);
	fu.fast_unfolding_main();

	system("pause");
}
