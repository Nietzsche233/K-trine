/*
trineplus.h
*/




#include "trineplus.h"
#include "defs.h"

namespace trineplus{

    Trineplus::Trineplus(const uint32_t n,const uint32_t m) :
        n_(n),
        m_(m),
        g_(n)
    {
        c_.resize(n_,0);
        trineplusness_.resize(n_,0);
        triangle_.resize(n_,0);
        bin_.resize(n_,false);
        k_order.resize(n_,0);
        k_rank.resize(n_,0);
    }

    Trineplus::~Trineplus(){
        //we have no ptr
    }

    void Trineplus::Load(const EdgListT&gedges) {
        for (const auto edg : gedges) {
            const uint32_t v1 = edg.first;
            const uint32_t v2 = edg.second;
            ASSERT(v1 < n_ && v2 < n_)
            ASSERT(v1 != v2);
            g_.Nie[v1].push_back(v2);
            g_.Nie[v2].push_back(v1);
            g_.deg_[v1]++,g_.deg_[v2]++;
        }
        uint32_t max_d_ = 0;
        for (uint32_t i = 0; i < n_; i++) {
            g_.deg_[i]=g_.Nie[i].size();
            if(g_.deg_[i]>max_d_)    max_d_=g_.deg_[i];
        }
        std::vector<uint32_t> bin(max_d_+1,0);
        for(auto x:g_.deg_)
            ++bin[x];
        uint32_t start = 0;
        for (uint32_t i = 0; i <= max_d_; ++i) {
            uint32_t temp = bin[i];
            bin[i] = start;
            start += temp;
        }
        //bin[i] how many vertex has a deg<i
        for (uint32_t i = 0; i < n_; ++i) {
            k_rank[i] = bin[g_.deg_[i]];
            k_order[k_rank[i]] = i;
            ++bin[g_.deg_[i]];
        }
        //bin[i] how many vertex has a deg<=i
        //pos[i] is i's pos
        //vert[pos[i]]=i 
        for (uint32_t i = max_d_; i > 0; --i) {
            bin[i] = bin[i-1];
        }
        bin[0] = 0;
        //bin[i] how many vertex has a deg<i
        g_.TriangleCount(c_,k_order,k_rank);
        triangle_ = c_;
        for (int i = 0; i < n_; i++) {
            c_[i] += g_.deg_[i];
        }
        k_order.resize(0);
        TripDecomp();
    }

    double Trineplus::ClusteringCoefficient(uint32_t retio_) {
        uint32_t sub_n_ = 1ll * g_.n_ * retio_ / 100;
        if (retio_ >= 100) {
            uint32_t i;
            for (i = n_-1; i > 1; i--) {
                if(trineplusness_[k_order[i-1]] < trineplusness_[k_order[i]])
                    break;
            }
            sub_n_ = g_.n_ - i;
            printf("(%d-trineplus : %d)",trineplusness_[k_order[i]],sub_n_);
        }
        graph::Graph sub_g_(sub_n_);
        for (uint32_t i = n_-1; i >= g_.n_ - sub_n_; i--) {
            for (auto v : g_.Nie[k_order[i]]) {
                if(k_rank[v] < g_.n_ - sub_n_) continue;
                if(k_order[i] > v)   continue;
                uint32_t subu_ = i - (g_.n_ - sub_n_);
                uint32_t subv_ = k_rank[v] - (g_.n_ - sub_n_);
                sub_g_.Nie[subu_].push_back(subv_);
                sub_g_.Nie[subv_].push_back(subu_);
                sub_g_.deg_[subu_]++;
                sub_g_.deg_[subv_]++;
            }
        }
        std::vector<uint32_t > sub_k_order(sub_n_);
        std::vector<uint32_t > sub_k_rank(sub_n_);
        for (uint32_t i = 0; i < sub_n_; i++) 
            sub_k_order[i] = sub_k_rank[i] = i;
        triangle_.assign(sub_n_, 0);
        uint64_t tot_triangle = sub_g_.TriangleCount(triangle_,sub_k_order,sub_k_rank);
        uint64_t tot_nei_pair = 0;
        for (uint32_t i = 0; i < sub_n_; i++) 
            tot_nei_pair += (sub_g_.deg_[i] - 1ll) * sub_g_.deg_[i];
        return 1.0 * tot_triangle / tot_nei_pair;
    }

    double Trineplus::Density(uint32_t retio_) {
        uint32_t sub_n_ = 1ll * g_.n_ * retio_ / 100;
        if (retio_ >= 100) {
            uint32_t i;
            for (i = n_-1; i > 1; i--) {
                if(trineplusness_[k_order[i-1]] < trineplusness_[k_order[i]])
                    break;
            }
            sub_n_ = g_.n_ - i;
            printf("(%d-trineplus : %d)",trineplusness_[k_order[i]],sub_n_);
        }
        graph::Graph sub_g_(sub_n_);
        for (uint32_t i = n_-1; i >= g_.n_ - sub_n_; i--) {
            for (auto v : g_.Nie[k_order[i]]) {
                if(k_rank[v] < g_.n_ - sub_n_) continue;
                if(k_order[i] > v)   continue;
                uint32_t subu_ = i - (g_.n_ - sub_n_);
                uint32_t subv_ = k_rank[v] - (g_.n_ - sub_n_);
                sub_g_.Nie[subu_].push_back(subv_);
                sub_g_.Nie[subv_].push_back(subu_);
                sub_g_.deg_[subu_]++;
                sub_g_.deg_[subv_]++;
            }
        }
        uint64_t m = 0;
        for (uint32_t i = 0; i < sub_n_; i++) 
            m += sub_g_.deg_[i];
        return 1.0 * m / (1ll * sub_n_ * (sub_n_ - 1ll));
    }

    void Trineplus::SortSwap(
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

    const void Trineplus::TripDecomp(){
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
            ASSERT(bin[k] == i);
            ++bin[k];
            trineplusness_[v] = k;
            vis[v] = true;
            k_order.push_back(v);
            k_rank[v] = k_order.size()-1;
            for(uint32_t w:g_.Nie[v]){
                if (!vis[w]) {
                    thash[w]=true;
                    SortSwap(k,w,pos,vert,c_,bin);
                }
            }
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

}