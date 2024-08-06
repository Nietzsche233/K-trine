/*
trine.h
*/




#include "trine.h"
#include "defs.h"

namespace trine{

    Trine::Trine(const uint32_t n,const uint32_t m) :
        Core(n,m)
    {
        triangle_.resize(n_,0);
        isin_k_trine_.resize(n_,false);
        triness_.resize(n_,0);
    }

    Trine::~Trine(){
        //we have no ptr
    }

    void Trine::ComputeK_trine(uint32_t K,const EdgListT& gedges){
        K_ = K + (K&1);
        Load(gedges);
        for(uint32_t i=0;i<n_;i++){
            g_.deg_[i]=g_.Nie[i].size();
        }

        std::vector<uint32_t> bin(n_+1);
        for(auto x:g_.deg_)
            bin[x]++;
        uint32_t start = 0;
        for (uint32_t i = 0; i <= n_; ++i) {
            uint32_t temp = bin[i];
            bin[i] = start;
            start += temp;
        }
        k_order.resize(n_);
        //bin[i] how many vertex has a deg<i
        for (uint32_t i = 0; i < n_; ++i) {
            k_rank[i] = bin[g_.deg_[i]];
            k_order[k_rank[i]] = i;
            ++bin[g_.deg_[i]];
        }

        // k_order.resize(n_);
        // for(uint32_t i = 0; i < n_; ++i){
        //     k_rank[i] = k_order[i] = i;
        // }
        
        uint32_t limit = 0;
        std::vector<uint32_t>del_stack_;
        bin_.assign(n_,0);
        TriangleCount(triangle_,limit,g_.deg_,del_stack_);

        std::vector<bool> has_in_stack_(n_,false);
        for(auto u : del_stack_)
            has_in_stack_[u] = true;
        while(del_stack_.size()){
            uint32_t u = del_stack_[del_stack_.size()-1];
            del_stack_.pop_back();
            isin_k_trine_[u] = false;
            for(auto w : g_.Nie[u])
                if(isin_k_trine_[w])
                    bin_[w] = true;
            for(auto v : g_.Nie[u])
                if(bin_[v]){
                    for(auto w : g_.Nie[v])
                        if(bin_[w]){
                            triangle_[v] -= 2;
                            triangle_[w] -= 2;
                            if(has_in_stack_[v] == false && triangle_[v] < K_)
                                del_stack_.push_back(v),has_in_stack_[v] = true;
                            if(has_in_stack_[w] == false && triangle_[w] < K_)
                                del_stack_.push_back(w),has_in_stack_[w] = true;

                        }
                    bin_[v] = false;
                }
        }

        for(uint32_t u = 0; u < n_; u++)
            if(isin_k_trine_[u])
                k_trine_nodes_.push_back(u);
    }

    void Trine::ComputeK_trine_v1(uint32_t K,const EdgListT& gedges){
        K_ = K + (K&1);
        Load(gedges);
        for(uint32_t i=0;i<n_;i++){
            c_[i]=g_.deg_[i]=g_.Nie[i].size();
        }

        std::vector<uint32_t> bin(n_+1);
        for(auto x:g_.deg_)
            bin[x]++;
        uint32_t start = 0;
        for (uint32_t i = 0; i <= n_; ++i) {
            uint32_t temp = bin[i];
            bin[i] = start;
            start += temp;
        }
        k_order.resize(n_);
        //bin[i] how many vertex has a deg<i
        for (uint32_t i = 0; i < n_; ++i) {
            k_rank[i] = bin[g_.deg_[i]];
            k_order[k_rank[i]] = i;
            ++bin[g_.deg_[i]];
        }
        //bin[i] how many vertex has a deg<=i
        //pos[i] is i's pos
        //vert[pos[i]]=i 
        for (uint32_t i = n_; i > 0; --i) {
            bin[i] = bin[i-1];
        }
        bin[0] = 0;
        uint32_t limit = std::sqrt(K_+0.25)+0.5;
        uint32_t k = 0;
        for(uint32_t i = 0; i < n_; ++i){
            const uint32_t v = k_order[i];
            if(c_[v] >= limit)  break;
            if(c_[v] > k) k = c_[v];
            ++bin[k];
            bin_[v] = 1;
            for(uint32_t u : g_.Nie[v]){
                if(bin_[u]) continue;
                SortSwap(k,u,k_rank,k_order,c_,bin);
            }
        }
        
        std::vector<uint32_t>del_stack_;
        bin_.assign(n_,0);
        TriangleCount(triangle_,limit,c_,del_stack_);

        std::vector<bool> has_in_stack_(n_,false);
        for(auto u : del_stack_)
            has_in_stack_[u] = true;
        while(del_stack_.size()){
            uint32_t u = del_stack_[del_stack_.size()-1];
            del_stack_.pop_back();
            isin_k_trine_[u] = false;
            for(auto w : g_.Nie[u])
                if(isin_k_trine_[w])
                    bin_[w] = true;
            for(auto v : g_.Nie[u])
                if(bin_[v]){
                    for(auto w : g_.Nie[v])
                        if(bin_[w]){
                            triangle_[v] -= 2;
                            triangle_[w] -= 2;
                            if(has_in_stack_[v] == false && triangle_[v] < K_)
                                del_stack_.push_back(v),has_in_stack_[v] = true;
                            if(has_in_stack_[w] == false && triangle_[w] < K_)
                                del_stack_.push_back(w),has_in_stack_[w] = true;

                        }
                    bin_[v] = false;
                }
        }

        for(uint32_t u = 0; u < n_; u++)
            if(isin_k_trine_[u])
                k_trine_nodes_.push_back(u);
    }

    void Trine::ComputeK_trineFaster(uint32_t K,const EdgListT& gedges){
        K_ = K + (K&1);
        Load(gedges);
        CoreDecomp();

        uint32_t limit = std::sqrt(K_+0.25)+0.5;
        // printf("limit:%d\n",limit);
        std::vector<uint32_t>del_stack_;
        bin_.assign(n_,0);
        TriangleCount(triangle_,limit,coreness_,del_stack_);

        std::vector<bool> has_in_stack_(n_,false);
        for(auto u : del_stack_)
            has_in_stack_[u] = true;

        while(del_stack_.size()){
            uint32_t u = del_stack_[del_stack_.size()-1];
            del_stack_.pop_back();
            isin_k_trine_[u] = false;
            for(auto w : g_.Nie[u])
                if(isin_k_trine_[w])
                    bin_[w] = true;
            for(auto v : g_.Nie[u])
                if(bin_[v]){
                    for(auto w : g_.Nie[v])
                        if(bin_[w]){
                            triangle_[v] -= 2;
                            triangle_[w] -= 2;
                            if(has_in_stack_[v] == false && triangle_[v] < K_)
                                del_stack_.push_back(v),has_in_stack_[v] = true;
                            if(has_in_stack_[w] == false && triangle_[w] < K_)
                                del_stack_.push_back(w),has_in_stack_[w] = true;

                        }
                    bin_[v] = false;
                }
        }

        for(uint32_t u = 0; u < n_; u++)
            if(isin_k_trine_[u])
                k_trine_nodes_.push_back(u);
    }

    void Trine::TrineDecomposition(const EdgListT&gedges){
        K_ = 0;
        Load(gedges);
        for(uint32_t i=0;i<n_;i++){
            g_.deg_[i]=g_.Nie[i].size();
        }

        std::vector<uint32_t> bin(n_+1);
        for(auto x:g_.deg_)
            bin[x]++;
        uint32_t start = 0;
        for (uint32_t i = 0; i <= n_; ++i) {
            uint32_t temp = bin[i];
            bin[i] = start;
            start += temp;
        }
        k_order.resize(n_);
        //bin[i] how many vertex has a deg<i
        for (uint32_t i = 0; i < n_; ++i) {
            k_rank[i] = bin[g_.deg_[i]];
            k_order[k_rank[i]] = i;
            ++bin[g_.deg_[i]];
        }

        // k_order.resize(n_);
        // for(uint32_t i = 0; i < n_; ++i){
        //     k_rank[i] = k_order[i] = i;
        // }
        
        uint32_t limit = 0;
        std::vector<uint32_t>del_stack_;
        bin_.assign(n_,0);
        TriangleCount(c_,limit,g_.deg_,del_stack_);

        TriDecomp();
    }

    const void Trine::TriangleCount(std::vector<uint32_t>&cnt, int limit, std::vector<uint32_t>&limit_some_, std::vector<uint32_t>&del_stack_){
        
        for(uint32_t i = n_ - 1;; i--){
            if(limit_some_[k_order[i]] < limit) break;

            for(auto j : g_.Nie[k_order[i]])
                if(k_rank[j] < i && limit_some_[j] >=limit)
                    bin_[j] = true;
            
            for(auto j : g_.Nie[k_order[i]])
                if(k_rank[j] < i && limit_some_[j] >=limit){
                    for(auto k : g_.Nie[j])
                        if(bin_[k]){
                            cnt[k_order[i]]+=2;
                            cnt[j]+=2;
                            cnt[k]+=2;
                        }
                    bin_[j]=false;
                }
            
            if(cnt[k_order[i]]<K_)
                del_stack_.push_back(k_order[i]);
            isin_k_trine_[k_order[i]] =true;
            if(!i)  break;
        }
    }

    void Trine::SortSwap(
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

    const void Trine::TriDecomp(){
        uint32_t max_c_ = 0;
        for(auto x:c_)
            if(x>max_c_)    max_c_=x;
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
        auto vis = std::vector<bool>(n_, false);
        std::vector<bool> thash(n_,false);
        for (uint32_t i = 0; i < n_; ++i) {
            const uint32_t v = vert[i];
            if (c_[v] > k) k = c_[v];
            // ASSERT(bin[deg[v]] == i);
            ++bin[k];
            triness_[v] = k;
            vis[v] = true;
            for(uint32_t w:g_.Nie[v])
                if(!vis[w])
                    thash[w]=true;
            for(uint32_t u:g_.Nie[v])
                if(thash[u])
                    for(uint32_t w:g_.Nie[u])
                        if(thash[w]){
                            SortSwap(k,u,pos,vert,c_,bin);
                            SortSwap(k,w,pos,vert,c_,bin);
                        }
            for(uint32_t w:g_.Nie[v])
                thash[w]=false; 
        }
    }

    void Trine::Output(){
        for(auto u : k_trine_nodes_){
            printf("%" SCNd32 " ",u);
        }
    }

}