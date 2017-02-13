/**************************
jojoli
2017-02-09  
***************************/
#include <mysql_connection.h>
#include "graph.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <boost/scoped_ptr.hpp>


string user="root";
string pass="li83916386";
string url="tcp://127.0.0.1:3306";
string database="git";

//sql::Connection *connect(user=user,pass=pass,url=url)
//{
//	sql::Driver * driver = sql::mysql::get_driver_instance();
//		/* Using the Driver to create a connection */
//	return driver->connect(url, user, pass);
//} 

//bool get_data()
//{
//	try{
//		boost::scoped_ptr< sql::Connection > con(connect());
//		/* Creating a "simple" statement - "simple" = not a prepared statement */
//		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
//		
//	}
//}
using namespace std;

class mysql_api{
	sql::Driver * driver;
	sql::Connection * con;
	sql::Statement * stmt;
	public:
		mysql_api(string user=user,string pass=pass,string url=url)
		{
			try{
				cout<<"init"<<endl; 
				driver = sql::mysql::get_driver_instance();
				cout<<"init"<<endl;
				//using the driver to create a connection
				con = driver->connect("tcp://127.0.0.1:3306", "root", "li83916386");
				cout<<"connect success!"<<endl;
				stmt = con->createStatement();
				stmt->execute("USE " + database);
			}
			catch (sql::SQLException &e) {
				cout<<"error"<<endl;
				/*
				The MySQL Connector/C++ throws three different exceptions:
		
				- sql::MethodNotImplementedException (derived from sql::SQLException)
				- sql::InvalidArgumentException (derived from sql::SQLException)
				- sql::SQLException (derived from std::runtime_error)
				*/
				cout << "# ERR: SQLException in " << __FILE__;
				cout <<"on line " << __LINE__ << endl;
				/* Use what() (derived from std::runtime_error) to fetch the error message */
				cout << "# ERR: " << e.what();
				cout << " (MySQL error code: " << e.getErrorCode();
				cout << ", SQLState: " << e.getSQLState() << " )" << endl;
				cout << "not ok 1 - examples/connect.php" << endl;
				con = NULL; 
				stmt = NULL;
			} catch (std::runtime_error &e) {
				cout<<"error"<<endl;
				cout << "# ERR: runtime_error in " << __FILE__;
				cout << " on line " << __LINE__ << endl;
				cout << "# ERR: " << e.what() << endl;
				cout << "not ok 1 - examples/connect.php" << endl;
				con = NULL;
				stmt = NULL;
			}
		}
		
		~mysql_api(){
			if(!con) delete con;
			if(!stmt)delete stmt;
		}
		
		uint32_t write_user(graph *g);
		//write all the user in the database into graph's user vector
		//return total amount of user
		uint32_t write_following(graph *g,string username);
		uint32_t write_follower(graph *g,string username);
		//write a single user's followers and followings
		//return total amount
		bool write_all(graph &g);
		//this method rewrite graph from database
}; 

uint32_t mysql_api::write_user(graph *g)
{
	boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT * from userinfo"));
	//automatically free res
	int rows = res->rowsCount();
	
	for(int i=0;i<rows;i++)
	{
		GitUser::Info info;
		info.id = res->getInt("id");	
		info.username = res->getString("login");
		info.avatar_url = res->getString("avatar_url");
		info.name = res->getString("name");
		info.email = res->getString("email");
		info.location = res->getString("location");
		info.bio = res->getString("bio");
		info.public_repos = res->getInt("public_repos");
		info.public_gists = res->getInt("public_gists");
		info.followers = res->getInt("followers");
		info.following = res->getInt("following");
		info.created_at = res->getString("created_at");
		//read from mysql
		
		g->adduser(info);
		//**deal with failure
	}
	return rows;
}

uint32_t mysql_api::write_following(graph *p,string username)
{
	boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery(\
		string("SELECT * from following where userid ='")+ username + string("'")));
	int rows = res->rowsCount();
	
	for(int i=0;i<rows;i++)
	{
		string followingid = res->getString("followingid");
		p->addfollowing(username,followingid);
	}
	return rows;
}

uint32_t mysql_api::write_follower(graph *p,string username)
{
	boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery(\
		string("SELECT * from follower where userid ='")+ username + string("'")));
	int rows = res->rowsCount();

	for(int i=0;i<rows;i++)
	{
		string followerid = res->getString("followerid");
		p->addfollower(username,followerid);
	}
	return rows; 
}

bool mysql_api::write_all(graph &g)
{
	write_user(&g);
	for(map<string,GitUser>::iterator it = g.index.begin();it!=g.index.end();it++)
	//for every user add their edges
	{
		write_following(&g,it->first);
		write_follower(&g,it->first);
	}
}
