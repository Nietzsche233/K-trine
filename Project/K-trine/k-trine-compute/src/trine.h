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
#include "core.h"


namespace trine{
    using namespace core;

    #define COUNT_TS false

    class Trine:public Core{
        public:

            Trine(const uint32_t n,const uint32_t m);
            Trine(const Trine&) = delete;
            ~Trine();

            uint32_t K_;

            void ComputeK_trineFaster(uint32_t K,const EdgListT& gedges);
            void ComputeK_trine_v1(uint32_t K,const EdgListT& gedges);
            void ComputeK_trine(uint32_t K,const EdgListT& gedges);
            void TrineDecomposition(const EdgListT& gedges);
            
            void Output();
            std::vector<uint32_t> k_trine_nodes_;
            std::vector<uint32_t> triness_;
            //triness
            
        private:
            
            const void TriangleCount(std::vector<uint32_t>&cnt, int limit, std::vector<uint32_t>&limit_some_, std::vector<uint32_t>&del_stack_);
            // 
            
            std::vector<uint32_t> triangle_;
            std::vector<bool> isin_k_trine_;
            std::vector<uint32_t> sup_;
            //it is sup (in Tri Decomp) in paper
            
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