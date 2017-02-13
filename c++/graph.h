/**************************
jojoli
2017-02-09  
***************************/

#ifndef GRAPH
#define GRAPH
//Map username to index
#include <cstdint>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#incldue <cstddef>
using namespace std;

class Follow{
	public:
		string follower,following;
		//follwer follows
		//a follows b
		Follow(string follower_a,string following_b)
		{
			follower = follower_a;
			following = following_b;
		}
		string from(){ return follower;}
		string to(){ return following;}
};

class GitUser{
	public:
		struct Info{
			uint32_t id; 
			string  username,avatar_url,name,email,location,bio;
			uint32_t public_repos,public_gists,followers,following;
			string created_at;
		}info;
		vector<Follow> following;
		vector<Follow> followers;
		double rank;
		
		GitUser(Info info){
			info = info;
			rank =¡¡1; 
		}
		GitUser(){
		}
};

class graph{
	public:
		map<string,GitUser> index;
		//map is used to map username to its index in vector
		
	public:
		bool adduser(GitUser::Info info);
		bool isfollowing(string userid,string followid);
		bool isfollower(string userid,string follwerid);
		bool addfollowing(string userid,string followingid);
		bool addfollower(string userid,string followerid);
		GitUser* getallusers();
		GitUser* operator[](string username){
			if((*this).index.count(username)>0) 
				return (*this).index[username];
			else return NULL;
		}
};

bool graph::adduser(GitUser::Info userinfo)
{
	if(index.count(userinfo.username)>0)
		return false;
	
	index[userinfo.username] = userinfo;
	return true;
}

//is following return true else return false
bool graph::isfollowing(string userid,string followingid)
{
	vector<Follow> &it = index[userid].following;
	for(int i=0;i<it.size();i++)
	{
		if(it[i].follower==userid && it[i].following==followingid)
			return true;
	}
	return false;
}

bool graph::isfollower(string userid,string followerid)
{
	vector<Follow> &it = index[userid].followers;
	for(int i=0;i<it.size();i++)
	{
		if(it[i].follower==followerid && it[i].following==userid)
			return true;
	}
	return false;
}

//user following
bool graph::addfollowing(string userid,string followingid)
{
	//if user and the user he follows aready exist in graph
	if(index.count(userid) && index.count(followingid))
	{
		if(!isfollowing(userid,followingid))
			index[userid].following.push_back(Follow(userid,followingid));

		if(!isfollower(followingid,userid))
			index[followingid].followers.push_back(Follow(userid,followingid));
		return true;
	}
	return false;
}


//user followed
bool graph::addfollower(string userid,string followerid)
{
	//if user and the user he follows aready exist in graph
	if(index.count(userid) && index.count(followerid))
	{
		if(!isfollower(userid,followerid))
			index[userid].followers.push_back(Follow(followerid,userid));

		if(!isfollowing(followerid,userid))
			index[followerid].following.push_back(Follow(followerid,userid));
		return true;
	}
	return false;
}

//bool graph::getallusers(GitUser* &g,int &num)
//{
//	size_t size= index.count();
//	if(size==0)
//		return false;
//	num = size;
//	g = new size*sizeof(GitUser);
//	for(size_t i=0;i<size;i++)
//		g[i] = 
//}
#endif
