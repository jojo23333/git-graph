#pragma once
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
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include <cstddef>
#include <algorithm>
#include "readgml.h"
using namespace std;

class Follow {
public:
	string follower, following;
	//follwer follows
	//a follows b
	Follow(string follower_a, string following_b)
	{
		follower = follower_a;
		following = following_b;
	}
	string from() const{ return follower; }
	string to() const{ return following; }
};

class GitUser {
public:
	struct Info {
		uint32_t id;
		string  username, avatar_url, name, email, location, bio;
		uint32_t public_repos, public_gists, followers, following;
		string created_at;
	}info;
	vector<Follow> following;
	vector<Follow> followers;
	double rank;

	GitUser(Info info) {
		info = info;
		rank = 1;
	}
	GitUser() {
		rank = 1;
	}
};

class gitgraph {
public:
	map<string, GitUser> index;
	//map is used to map username to its index in vector

public:
	gitgraph(NETWORK *net);
	gitgraph(){}
	bool selfcheck();
	bool adduser(GitUser::Info info);
	bool isfollowing(string userid, string followingid);
	bool isfollower(string userid, string followerid);
	bool addfollowing(string userid, string followingid);
	bool addfollower(string userid, string followerid);
	GitUser* getallusers();
	GitUser* operator[](string username) {
		if (this->index.count(username)>0)
			return &this->index[username];
		else return NULL;
	}
};



#endif