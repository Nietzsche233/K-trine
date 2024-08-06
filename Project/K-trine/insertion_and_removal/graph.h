/*
there are some define for Graph
*/

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <unordered_set>
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


            // EdgListT TriangleCountVectex(uint32_t u){

            // }
            // //meaningless
            // std::vector<uint32_t> TriangleCountEdge(uint32_t u,uint32_t v){

            // }
            // //meaningless
    };
}