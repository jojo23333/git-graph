#include "GitGraph.h"

//transter a graph from .gml to gitgraph

using namespace std;
gitgraph::gitgraph(NETWORK *net)
{
    int node_amount = net->nvertices;
    VERTEX * v = net->vertex;
    for (int i = 0; i < node_amount; i++) {
        GitUser user;
        string username = string(v[i].label);
        user.info.username = username;
        int out_amount = v[i].degree;
        EDGE *edges = v[i].edge;
        for (int j = 0; j < out_amount; j++) {
            string following;
            following = string(v[edges[j].target].label);
            user.following.push_back(Follow(username, following));
        }

        index[username] = user;
    }
    //add income edge list
    for (map<string, GitUser>::iterator it = index.begin(); it != index.end(); it++) {
        string follower = it->second.info.username;
        vector<Follow> &out = it->second.following;
        for (vector<Follow>::iterator ia = out.begin(); ia != out.end(); ia++) {
            string username = ia->following;
            index[username].followers.push_back(Follow(follower, username));
        }
    }
}


//Function used to check whether for every edge of v1:v1->v2 has been recorded in v2
//
//bool gitgraph::selfcheck()
//{
//  map<set<string>,int> counter;
//  set<set<string> > muti_follow;
//  for (map<string, GitUser>::iterator it = index.begin(); it != index.end(); it++) {
//      vector<Follow> &out = it->second.following;
//      vector<Follow> &in = it->second.followers;
//      set<set<string> >followship;
//      set<string>tmp;
//      for (vector<Follow>::iterator ia = out.begin(); ia != out.end(); ia++) {
//          string v1 = ia->follower; string v2 = ia->following;
//          tmp.insert(v1); tmp.insert(v2);
//          counter[tmp] = counter[tmp] + 1;
//          tmp.clear();
//          followship.insert(tmp);
//      }
//      for (vector<Follow>::iterator ia = in.begin(); ia != in.end(); ia++) {
//          string v1 = ia->follower; string v2 = ia->following;
//          tmp.insert(v1); tmp.insert(v2);
//          counter[tmp] = counter[tmp] + 1;
//          if (followship.count(tmp) && !muti_follow.count(tmp))
//              muti_follow.insert(tmp);
//          tmp.clear();
//      }
//  }
//
//  //For v1 v2 in muti_follow sum in counter should be 4
//  //Else should be 2
//  for (map<set<string>, int>::iterator it = counter.begin(); it != counter.end(); it++) {
//      if (it->second == 4) {
//          if (!muti_follow.count(it->first))  
//              return false;
//      }
//      else if (it->second == 2) {
//          if (muti_follow.count(it->first))  
//              return false;
//      }
//      else
//          return false;
//  }
//  return true;
//}

//Self checking for graph integrity
bool gitgraph::selfcheck()
{
    for (map<string,GitUser>::iterator it = index.begin();it != index.end();it++) {
        vector<Follow> &out = it->second.following;
        vector<Follow> &in = it->second.followers;
        //Check for out edge
        for (vector<Follow>::iterator outit = out.begin();outit != out.end();outit++) {
            const string user = outit->from();
            const string following = outit->to();
            if (isfollower(following, user))    continue;
            cout << "following:" << following << " user" << user << endl;
            return false;
        }
        //Check for in edge
        for (vector<Follow>::iterator init = in.begin();init != in.end();init++) {
            const string follower = init->from();
            const string user = init->to();
            if (isfollowing(follower, user))    continue;
            cout << "follower" << follower << " user" << user << endl;
            return false;
        }
    }
    return true;
}




bool gitgraph::adduser(GitUser::Info userinfo)
{
    if (index.count(userinfo.username)>0)
        return false;

    index[userinfo.username] = userinfo;
    return true;
}


//is following return true else return false

bool gitgraph::isfollowing(string userid, string followingid)
{
    vector<Follow> &it = index[userid].following;
    for (int i = 0; i<it.size(); i++)
    {
        if (it[i].follower == userid && it[i].following == followingid)
            return true;
    }
    return false;
}

bool gitgraph::isfollower(string userid, string followerid)
{
    vector<Follow> &it = index[userid].followers;
    for (int i = 0; i<it.size(); i++)
    {
        if (it[i].follower == followerid && it[i].following == userid)
            return true;
    }
    return false;
}



//user following
bool gitgraph::addfollowing(string userid, string followingid)
{
    //if user and the user he follows aready exist in graph
    if (index.count(userid) && index.count(followingid))
    {
        if (!isfollowing(userid, followingid))
            index[userid].following.push_back(Follow(userid, followingid));

        if (!isfollower(followingid, userid))
            index[followingid].followers.push_back(Follow(userid, followingid));
        return true;
    }
    return false;
}



//user followed
bool gitgraph::addfollower(string userid, string followerid)
{
    //if user and the user he follows aready exist in graph
    if (index.count(userid) && index.count(followerid))
    {
        if (!isfollower(userid, followerid))
            index[userid].followers.push_back(Follow(followerid, userid));

        if (!isfollowing(followerid, userid))
            index[followerid].following.push_back(Follow(followerid, userid));
        return true;
    }
    return false;
}

//bool graph::getallusers(GitUser* &g,int &num)
//{
//  size_t size= index.count();
//  if(size==0)
//      return false;
//  num = size;
//  g = new size*sizeof(GitUser);
//  for(size_t i=0;i<size;i++)
//      g[i] = 
//}