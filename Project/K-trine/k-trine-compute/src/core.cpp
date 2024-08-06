/*
core.h
*/




#include "core.h"
#include "defs.h"

namespace core{

    Core::Core(const uint32_t n,const uint32_t m) :
        n_(n),
        m_(m),
        g_(n)
    {
        cmp_deg_=std::bind(&Core::CmpDeg,this,std::placeholders::_1,std::placeholders::_2);
        c_.resize(n_,0);
        coreness_.resize(n_);
        bin_.resize(n_,false);
        k_rank.resize(n_,0);
    }

    Core::~Core(){
        //we have no ptr
    }

    void Core::Load(const EdgListT&gedges){
        for (const auto edg :gedges){
            const uint32_t v1 = edg.first;
            const uint32_t v2 = edg.second;
            //ASSERT(v1 < n_ && v2 < n_ && v1 != v2);
            g_.Nie[v1].push_back(v2);
            g_.Nie[v2].push_back(v1);
            //g_.deg_[v1]++,g_.deg_[v2]++;
            //c_[v1]++,c_[v2]++;
        }
    }

   
    inline void Core::SortSwap(
        uint32_t k,
        uint32_t ID,std::vector<uint32_t>& pos,
        std::vector<uint32_t>& vert,
        std::vector<uint32_t>& c_,
        std::vector<uint32_t> & bin
    ){
        unsigned oldc_=c_[ID]<k?k:c_[ID];
        unsigned oldpos=pos[ID];
        unsigned firpos=bin[oldc_];
        unsigned firvert=vert[firpos];
        if(ID!=firvert){//if(ID is not the first c_=c_[ID] one )
            //swap ID with the first one
            std::swap(pos[ID],pos[firvert]);
            vert[pos[ID]]=ID;
            vert[pos[firvert]]=firvert;
        }
        if(c_[ID]>k){
            ++bin[oldc_];
        }
        --c_[ID];
        // we need a right c_ and rem_=c_
    }

    const void Core::CoreDecomp(){
        uint32_t max_c_ = 0;
        for(uint32_t i=0;i<n_;i++){
            c_[i]=g_.deg_[i]=g_.Nie[i].size();
            if(c_[i]>max_c_)    max_c_=c_[i];
        }
        std::vector<uint32_t> bin(max_c_+1,0);
        for(auto x:c_)
            ++bin[x];
        uint32_t start = 0;
        for (uint32_t i = 0; i <= max_c_; ++i) {
            uint32_t temp = bin[i];
            bin[i] = start;
            start += temp;
        }
        //bin[i] how many vertex has a deg<i
        std::vector<uint32_t> vert(n_);
        std::vector<uint32_t> pos(n_);
        for (uint32_t i = 0; i < n_; ++i) {
            pos[i] = bin[c_[i]];
            vert[pos[i]] = i;
            ++bin[c_[i]];
        }
        //bin[i] how many vertex has a deg<=i
        //pos[i] is i's pos
        //vert[pos[i]]=i 
        for (uint32_t i = max_c_; i > 0; --i) {
            bin[i] = bin[i-1];
        }
        bin[0] = 0;
        //bin[i] how many vertex has a deg<i
        uint32_t k = 0;
        //auto vis = std::vector<bool>(n_, false);
        for (uint32_t i = 0; i < n_; ++i) {
            const uint32_t v = vert[i];
            if (c_[v] > k) k = c_[v];
            // ASSERT(bin[deg[v]] == i);
            ++bin[k];
            coreness_[v] = k;
            k_order.push_back(v);
            k_rank[v]=k_order.size()-1;
            bin_[v] = 1;
            for(uint32_t u: g_.Nie[v]){
                // printf("*");
                if (bin_[u]) continue;
                SortSwap(k,u,pos,vert,c_,bin);
            }// for-out  
        }
    }
}