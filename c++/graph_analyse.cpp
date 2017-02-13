#include "MysqlApi.h"

using namespace std;
int main()
{
	cout<<"ok"<<endl;
	mysql_api DB;
	graph G;
	DB.write_all(G);
	cout<<G.index.size()<<endl;
}


