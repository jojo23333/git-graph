/**************************
jojoli
2017-02-09  
***************************/

#include "graph.h"

bool graph::adduser(GitUser::info userinfo)
{
	if(index.count(userinfo.username)>0)
		return false;
	
	index[userinfo.username] = userinfo;
	return true;
}

//is following return true else return false
bool graph::isfollowing(string userid,string followingid)
{
	vecter<Follow> *it = &(index[userid].following);
	for(int i=0;i<it->size();i++)
	{
		if((*it)[i].follower==userid && (*it)[i].following==followingid)
			return true;
	}
	return false;
}

bool graph::isfollower(string userid,string followerid)
{
	vecter<Follow> *it = &(index[userid].followers);
	for(int i=0;i<it->size();i++)
	{
		if((*it)[i].follower==followerid && (*it)[i].following==userid)
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
		return true
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

		if(!isfollowing(followingid,userid))
			index[following].following.push_back(Follow(followingid,userid));
		return true;
	}
	return false;
}
