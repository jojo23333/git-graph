/**************************
jojoli
2017-02-09  
***************************/

#ifndef GRAPH
#define GRAPH
//Map username to index
#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>

class Follow{
	public:
		string follwer,following;
		//follwer follows
		//a follows b
		Follow(string follower_a,string following_b)
		{
			follwer = follower_a;
			following = following_b;
		}
};

class GitUser{
	public:
		struct Info{
			uint_32t id; 
			string  username,avata_url,name,email,location,bio;
			uint_32t public_repos,public_gists,followers,following;
			string created_at;
		}info;
		vector<Follow> following;
		vector<Follow> followers;
		
		GitUser(Info info){
			Info = info;
		}
};

class graph{
	public:
		map<string,GitUser> index;
		//map is used to map username to its index in vector
		
	public:
		bool adduser(GitUser::info info){};
		bool isfollowing(string userid,string followid);
		bool isfollower(string userid,string follwerid);
		bool addfollowing(string userid,string followingid);
		bool addfollower(string userid,string followerid);
};

#endif
