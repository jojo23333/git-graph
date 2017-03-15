#include "FastUnfolding.h"

namespace FastUnfolding {    
    void Iteration::cmty_add_member(int v,Community &c)
    {
        c.members.push_back(v);
        double  kinc=0;
        for (int i = 0;i < G[v].size();i++) {
            if (c.cmty_num == get_cmty(G[v][i].v2)) {
                kinc += G[v][i].weight;
            }
        }
        c.sum_in += 2*kinc+in[v];
        c.sum_tot += tot[v];
        //cout << " add: " << 2 * kinc + in[v] <<"in[v]:"<<in[v]<< endl;
    }


    void Iteration::cmty_del_member(vector<int>::iterator it,Community &c)
    {
        int v = *it;
        c.members.erase(it);
        double  kinc=0;
        for (int i = 0;i < G[v].size();i++) {
            if (c.cmty_num == get_cmty(G[v][i].v2)) {
                kinc += G[v][i].weight;
            }
        }
        c.sum_in -= (2 * kinc + in[v]);
        c.sum_tot -= tot[v];
        //cout << "delete: " << 2 * kinc + in[v];
    }


    inline int Iteration::get_cmty(int id)
    {
        return cmty_status[id];
    }

    inline bool Iteration::in_same_cmty(int c1, int c2)
    {
        return cmty_status[c1] == cmty_status[c2];
    }

    //inline double Iteration::get_ki(int v)
    //{
    //  double ki = 0;
    //  for (int i = 0;i < G[v].size();i++) {
    //      ki += G[v][i].weight;
    //  }
    //  return ki;
    //}

    /*
    Usage: Init data,including cmty cmty_status m
    Description:
    1.In the begining each node is a communities
    2.Cmty_status maps node index to community index
    */
    void Iteration::community_init() 
    {
        m = 0;
        for (int i = 0;i < G.size();i++) {
            if (cmty[i].sum_tot == 0) {
                double ki = 0;
                for (vector<Edge>::iterator it = G[i].begin();it != G[i].end();it++) {
                    ki += it->weight;
                }
                cmty[i].sum_tot = ki;
            }
            m += cmty[i].sum_tot;
            cmty[i].members.push_back(i);
            cmty[i].cmty_num = i;
            cmty_status.push_back(i);
            in.push_back(cmty[i].sum_in);
            tot.push_back(cmty[i].sum_tot);
        }
        m = m / 2;
        //cout << "M: " << m << endl;
    }


    __declspec(noinline) double Iteration::modularity()
    {
        double Q = 0;
        for (int i = 0;i < cmty.size();i++) {
            Q += cmty[i].sum_in / (m * 2) - cmty[i].sum_tot/(2*m)*cmty[i].sum_tot/(2*m);
        //  cout << Q<<endl;
        }
        return Q;
    }

    /*
    Usage: Calculate the change of modularity when Moving v1 into v2's community
    Description: 
    1. The delta amount is divided into two part, moving an node from community
    decrease the modularity call it ngty_delta,moving an isolated node into a community increase
    modularity call it psty_delta.
    */
    double Iteration::delta_modularity(int v1, int v2)
    {
        int c1=get_cmty(v1), c2=get_cmty(v2);
        if (c1 == c2) return 0;
        double pstv_delta,ngtv_delta,ki=0,ki_inc1=0,ki_inc2=0;
        double totc1 = cmty[c1].sum_tot;
        double totc2 = cmty[c2].sum_tot;
        for (int i = 0;i < G[v1].size();i++) {
            Edge &temp = G[v1][i];
            ki += temp.weight;
            if (get_cmty(temp.v2) == c2)    ki_inc2 += temp.weight;
            if (get_cmty(temp.v2) == c1)    ki_inc1 += temp.weight;
        }
    
        pstv_delta = ki_inc2/m - (totc2*ki)/(2 * m*m);
        ngtv_delta = -1*ki_inc1/m + (totc1*ki)/(2 * m*m) - (ki*ki)/(2*m*m);
        return pstv_delta + ngtv_delta;
    }
    
    /*
    Usage: Step two, aggregate communities into nodes for next iterations,and update partion vector.Generating new G
    Description: 
    1. Reindex every community(removing empty ones) and using new index to update partion vector status
    2. Using Map-reduce like method to merge edges between two communities and generate new adjacent list G
    */
    void Iteration::aggregate_from_previous(Iteration &pre,vector<int>& status)
    {
        cmty.clear();
        G.clear();
        cmty_status.clear();

        int i = 0;
        map<int, int> old_to_new;
        for (int j = 0 ;j < pre.cmty.size() ;j++) {
            Community &c = pre.cmty[j],temp;
            if (c.deleted || c.members.empty()) continue;
            for (int q = 0; q < c.members.size() ;q++) {
                old_to_new[c.members[q]] = i;
            }
            cout << c.sum_in << "-" << c.sum_tot << endl;
            temp.sum_in = c.sum_in;
            temp.sum_tot = c.sum_tot;
            cmty.push_back(temp);
            G.push_back(vector<Edge>());
            i++;
        }

        for (int j = 0;j < status.size();j++) {
            status[j] = old_to_new[status[j]];
        }
        
        map<set<int>, double> sumw;
        for (int j = 0;j < pre.G.size();j++) {
            for (vector<Edge>::iterator it = pre.G[j].begin();it != pre.G[j].end();it++) {
                int newc1 = old_to_new[it->v1];
                int newc2 = old_to_new[it->v2];
                if (newc1 == newc2) continue;
                set<int> temp;
                temp.insert(newc1);
                temp.insert(newc2);
                sumw[temp] += it->weight;
            }
        }

        for (map<set<int>, double>::iterator it = sumw.begin();it != sumw.end();it++) {
            set<int>::iterator itemp = it->first.begin();
            int c1 =*(itemp++);
            int c2 = *itemp;
            G[c1].push_back(Edge(c1, c2, it->second/2));
            G[c2].push_back(Edge(c2, c1, it->second/2));
        }
    }


    /*
    Usage: Use to optimize current modularity 
    Description: 
    1.The nested three loops are to consider each node sequently, once the modularity 
    don't change for n nodes, algorithm stops.
    2.For each node algorithm trys to find an adjacent which, by adding current node to
    it's community,gains maxium increase of modularity.
    */
    double Iteration::modularity_optimize()
    {
        community_init();
        int node_without_change = 0;
        double Q = modularity();
        while (node_without_change <= G.size()) {
            for (int i = 0;i < cmty.size(); i++) {
            for (vector<int>::iterator it = cmty[i].members.begin() ;it!=cmty[i].members.end() ;it++) {
                int cur_node = *it,m_adj;
                double max_delta_change = 0;
                for (int j = 0;j < G[cur_node].size();j++) {
                    int adj = G[cur_node][j].v2;
                        //cout << cur_node << ":" << adj << endl;
                    double delta_Q=0;
                    delta_Q = delta_modularity(cur_node, adj);
                    if (delta_Q < max_delta_change) continue;
                    max_delta_change = delta_Q; 
                    m_adj = adj;
                }
                if (max_delta_change > 0) {
                    cmty_del_member(it--,cmty[i]);
                    cmty_add_member(cur_node, cmty[get_cmty(m_adj)]);
                    cmty_status[cur_node] = get_cmty(m_adj);
                    //cout << Q;
                    Q += max_delta_change;
                    //cout<<"+"<< max_delta_change << "=" << Q << endl;
                }
                else
                    node_without_change++;
            }
            }
        }
        return Q;
    }


    //G record graph as adjacent table  
    //partitions record cmty partition in every iteration
    //Communities always init with every singal node is a community
    //Unfolding steps:loop of modularity_optimize and community_aggregation
    class FastUnfolding {
        map<string, int> mapper;
        map<int, string> reverse_mapper;
        vector<vector<Edge> >G;
        vector<Partition> partitions;

        int max_iterations = 10;

        /*void cmty_init();*/

    public:
        FastUnfolding(gitgraph &g);
        double modularity;
        void   fast_unfolding_main();
    };


    void FastUnfolding::fast_unfolding_main()
    {
        vector<int> partition;
        Iteration it,temp;
        double Q;
        for (int i = 0;i < G.size();i++)
            partition.push_back(i);
        temp.G = G;
        temp.cmty = vector<Community>(G.size(),Community());
        for (int i = 0;i < max_iterations;i++)
        {
            Q = temp.modularity_optimize();
            it.clear();
            it = temp;
            temp.clear();
            temp.aggregate_from_previous(it, partition);
            cout << temp.modularity() << endl;
            partitions.push_back(Partition(partition, temp.modularity()));
            if (temp.G.size() <= 1) break;
        }
    }


    FastUnfolding::FastUnfolding(gitgraph &g)
    {
        //mapping process give eache user an id from 0-graphsize
        int size = 0;
        for (map<string, GitUser>::iterator it = g.index.begin(); it != g.index.end(); it++) {
            mapper[it->first] = size;
            reverse_mapper[size] = it->first;
            size++;
        }

        //transfer directed graph to undirected graph
        for (map<string, GitUser>::iterator it = g.index.begin(); it != g.index.end(); it++) {
            vector<Edge> e;
            set<string> adj;

            vector<Follow> &p = it->second.following;
            string v1 = it->first;
            for (int i = 0; i<p.size(); i++) {
                string v2 = p[i].following;
                //for user v2 that mutual follows with v1 give weight of 1
                if (g.isfollower(v1, v2))
                    e.push_back(Edge(mapper[v1], mapper[v2], 1));
                else
                    e.push_back(Edge(mapper[v1], mapper[v2], 0.5));
                //record this user has appeared
                adj.insert(v2);
            }

            vector<Follow> &q = it->second.followers;
            for (int i = 0; i<q.size(); i++) {
                string v2 = q[i].follower;
                //for user that hsa appeared jump
                if (adj.count(v2))  continue;
                e.push_back(Edge(mapper[v1], mapper[v2], 0.5));
            }
            G.push_back(e);
        }
    }
}