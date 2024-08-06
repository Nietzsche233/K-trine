/*
main work about trine
*/

#include <cstdint>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <map>
#include <unordered_set>
#include <queue>
#include <functional>
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cmath>

#include "defs.h"
#include "graph.h"


namespace trine{
    using namespace graph;

    #define COUNT_TS false

    class Trine{
        public:

            Trine(const uint32_t n,const uint32_t m);
            Trine(const Trine&) = delete;
            ~Trine();
            void Load(const EdgListT& geders);
            double ClusteringCoefficient(uint32_t retio_);
            double Density(uint32_t retio_);
            int Rankof(uint32_t x) {
                return k_rank[x];
            }
            int Nodeat(uint32_t x) {
                return k_order[x];
            }
            int TrinessOut(uint32_t x) {
                return triness_[x];
            }
            //triness
            int TriangleOut(uint32_t x) {
                return triangle_[x];
            }
            
        private:
            const uint32_t n_,m_;
            //|V|
            Graph g_;
            // graph.h class Graph
            std::vector<uint32_t> triness_;
            //triness_
            std::vector<uint32_t> c_;
            //it is sup (in Tri Decomp) in paper
            std::vector<uint32_t> triangle_;
            std::vector<bool> bin_;
            std::vector<uint32_t> k_order;
            std::vector<uint32_t> k_rank;
            void SortSwap(
                uint32_t k,
                uint32_t ID,std::vector<uint32_t>& pos,
                std::vector<uint32_t>& vert,
                std::vector<uint32_t>& c_,
                std::vector<uint32_t> & bin
            );
            const void TriDecomp();
    };

}