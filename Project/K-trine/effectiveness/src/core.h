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

#include "defs.h"
#include "graph.h"


namespace core{
    using namespace graph;

    class Core{
        public:

            Core(const uint32_t n,const uint32_t m);
            Core(const Core&) = delete;
            ~Core();
            void Load(const EdgListT& geders);
            double ClusteringCoefficient(uint32_t retio_);
            double Density(uint32_t retio_);
            void OutNowCoreness(){
                printf("\nNow, coreness is:\n[");
                for(auto x:coreness_){
                    printf(" %" PRIu32 , x);
                }
                printf(" ]\n\n");
            }
            int Rankof(uint32_t x) {
                return k_rank[x];
            }
            int Nodeat(uint32_t x) {
                return k_order[x];
            }
            int CorenessOut(uint32_t x) {
                return coreness_[x];
            }
            int DegreeOut(uint32_t x) {
                return g_.deg_[x];
            }
        protected:
            const uint32_t n_,m_;
            //|V|
            Graph g_;
            // graph.h class Graph

            // bool CmpDeg(int x,int y){
            //     return g_.deg_[x]>g_.deg_[y]||(g_.deg_[x]==g_.deg_[y]&&x>y);
            // }

            // std::function<bool (const uint32_t, const uint32_t)> cmp_deg_;
            //used by TriangleCount;

            // class Order;
            //an obj for K-order
            // HP CorN;
            //some ele has c_!=0 when insert
            
            
            std::vector<uint32_t> c_;
            //it is c (in Batch Insert) and sup (in Tri Decomp) in paper
            std::vector<uint32_t> coreness_;
            //coreness
            std::vector<uint32_t> triangle_;
            std::vector<bool> bin_;
            std::vector<uint32_t> k_order;
            std::vector<uint32_t> k_rank;
            inline void SortSwap(
                uint32_t k,
                uint32_t ID,std::vector<uint32_t>& pos,
                std::vector<uint32_t>& vert,
                std::vector<uint32_t>& c_,
                std::vector<uint32_t> & bin
            );
            const void CoreDecomp();
            //used in insert;
            // void MoveCtoOrder(uint32_t k,uint32_t las);
            // void MoveOrdertoC(uint32_t u,uint32_t K);
            
    };

}