/*
there are some define for Graph
*/

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <unordered_set>
#ifndef _GRAPH_H_
#define _GRAPH_H_
namespace graph{
    typedef std::pair<uint32_t,uint32_t > EdgT;
    //EdgT=(u,v)
    typedef std::vector<EdgT> EdgListT;
    //EdgListT[i]=(u,v) is an edg
    typedef std::vector<std::vector<uint32_t> > AdjListT;
    //AdjListT[i] is i's Neighbor
    typedef std::vector<std::unordered_set<uint32_t> > AdjMatrT;
    //AdjMatrT[u][v]=true => (u,v) is an edg

    class Graph{
        public:
            Graph(uint32_t n):
                n_(n)
            {
                Nie.resize(n);
                deg_.resize(n);
            }
            uint32_t n_;
            AdjListT Nie;
            //deg[u]=|Neighbor(u)|
            std::vector<uint32_t> deg_;

            const uint64_t TriangleCount(std::vector<uint32_t >&cnt, std::vector<uint32_t >&k_order, std::vector<uint32_t >&k_rank) {
                uint64_t tot_triangle = 0;
                std::vector<bool > bin_(n_,0);
                for (uint32_t i = n_ - 1; ; i--) {
                    for (auto j : Nie[k_order[i]])
                        if (k_rank[j] < i)
                            bin_[j] = true;
                    
                    for (auto j : Nie[k_order[i]])
                        if (k_rank[j] < i) {
                            for (auto k : Nie[j])
                                if (bin_[k]) {
                                    cnt[k_order[i]] += 2;
                                    cnt[j] += 2;
                                    cnt[k] += 2;
                                    tot_triangle += 6;
                                }
                            bin_[j]=false;
                        }
                    if (!i)  break;
                }
                return tot_triangle;
            }


            // EdgListT TriangleCountVectex(uint32_t u){

            // }
            // //meaningless
            // std::vector<uint32_t> TriangleCountEdge(uint32_t u,uint32_t v){

            // }
            // //meaningless
    };
    class TrussGraph:public Graph{
        public:
            TrussGraph(uint32_t n, uint32_t m):
                Graph(n)
            {
                esup_.resize(m);
                Nie.resize(n);
            }
            std::vector<std::vector<std::pair<uint32_t, uint32_t > > > Nie;
            EdgListT edg_;
            std::vector<uint32_t> esup_;

            const uint64_t TriangleCount(std::vector<uint32_t >&cnt, std::vector<uint32_t >&k_order, std::vector<uint32_t >&k_rank) {
                uint64_t tot_triangle = 0;
                std::vector<bool > bin_(n_, 0);
                std::vector<int > edgid_(n_, -1);
                for (uint32_t i = n_ - 1; ; i--) {
                    for (auto j : Nie[k_order[i]])
                        if (k_rank[j.first] < i){
                            bin_[j.first] = true;
                            edgid_[j.first] = j.second;
                        }
                    
                    for (auto j : Nie[k_order[i]])
                        if (k_rank[j.first] < i) {
                            for (auto k : Nie[j.first])
                                if (bin_[k.first]) {
                                    cnt[edgid_[k.first]]++;
                                    cnt[j.second]++;
                                    cnt[k.second]++;
                                    tot_triangle += 6;
                                }
                            bin_[j.first]=false;
                            edgid_[j.first]=-1;
                        }
                    if (!i)  break;
                }
                return tot_triangle;
            }
    };
}
#endif