#include "GitGraph.h"
#include "GN.h"

using namespace std;
int main()
{
	NETWORK net;
	FILE *fp;
	fopen_s(&fp,"C:\\Users\\jojo\\git\\github-gragh\\c++\\gml_paraser\\polblogs.gml", "r");
	read_network(&net,fp);
	gitgraph G(&net);
	cout << G.index.size() << endl;
	if (G.selfcheck())	cout << "Graph ok;" << endl;
	else	cout << "error" << endl;
	GN com(G);
	cout << "GN algorithm start" << endl;
	com.Gn_main();
	system("pause");
}
