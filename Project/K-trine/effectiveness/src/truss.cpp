/*
truss.h
*/




#include "truss.h"
#include "defs.h"

namespace truss {

    Truss::Truss(const uint32_t n, const uint32_t m) :
        n_(n),
        m_(m),
        g_(n, m)
    {
        c_.resize(n_, 0);
        trussness_.resize(m_, 0);
        nodetruss_.resize(n_ ,0);
        triangle_.resize(n_, 0);
        bin_.resize(n_, false);
        k_order.resize(n_, 0);
        k_rank.resize(n_, 0);
    }

    Truss::~Truss() {
        //we have no ptr
    }

    void Truss::Load(const EdgListT&gedges) {
        for (uint32_t i=0;i<gedges.size();i++) {
            auto edg = gedges[i];
            g_.edg_.push_back(edg);
            const uint32_t v1 = edg.first;
            const uint32_t v2 = edg.second;
            ASSERT(v1 < n_ && v2 < n_ && v1 != v2);
            g_.Nie[v1].push_back(std::make_pair(v2,i));
            g_.Nie[v2].push_back(std::make_pair(v1,i));
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
        g_.TriangleCount(g_.esup_,k_order,k_rank);
        k_order.resize(0);
        TruDecomp();
    }

    double Truss::ClusteringCoefficient(uint32_t retio_) {
        uint32_t sub_n_ = 1ll * g_.n_ * retio_ / 100;
        if (retio_ >= 100) {
            uint32_t i;
            for (i = n_-1; i > 1; i--) {
                if(nodetruss_[k_order[i-1]] < nodetruss_[k_order[i]])
                    break;
            }
            sub_n_ = g_.n_ - i;
            printf("(%d-truss : %d)",nodetruss_[k_order[i]],sub_n_);
        }
        graph::Graph sub_g_(sub_n_);
        for (uint32_t i = n_-1; i >= g_.n_ - sub_n_; i--) {
            for (auto v : g_.Nie[k_order[i]]) {
                if(k_rank[v.first] < g_.n_ - sub_n_) continue;
                if(k_order[i] > v.first)   continue;
                uint32_t subu_ = i - (g_.n_ - sub_n_);
                uint32_t subv_ = k_rank[v.first] - (g_.n_ - sub_n_);
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

    double Truss::Density(uint32_t retio_) {
        uint32_t sub_n_ = 1ll * g_.n_ * retio_ / 100;
        if (retio_ >= 100) {
            uint32_t i;
            for (i = n_-1; i > 1; i--) {
                if(nodetruss_[k_order[i-1]] < nodetruss_[k_order[i]])
                    break;
            }
            sub_n_ = g_.n_ - i;
            printf("(%d-truss : %d)",nodetruss_[k_order[i]],sub_n_);
        }
        graph::Graph sub_g_(sub_n_);
        for (uint32_t i = n_-1; i >= g_.n_ - sub_n_; i--) {
            for (auto v : g_.Nie[k_order[i]]) {
                if(k_rank[v.first] < g_.n_ - sub_n_) continue;
                if(k_order[i] > v.first)   continue;
                uint32_t subu_ = i - (g_.n_ - sub_n_);
                uint32_t subv_ = k_rank[v.first] - (g_.n_ - sub_n_);
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

    void Truss::SortSwap(
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

    const void Truss::TruDecomp(){
        uint32_t max_c_ = 0;
        for(auto x:g_.esup_)
            if(x>max_c_)    max_c_=x;
        std::vector<uint32_t> bin(max_c_+1,0);
        for(auto x:g_.esup_)
            ++bin[x];
        uint32_t start = 0;
        for (uint32_t i = 0; i <= max_c_; ++i) {
            uint32_t temp = bin[i];
            bin[i] = start;
            start += temp;
        }
        //bin[i] how many vertex has a deg<i
        std::vector<uint32_t> vert(m_);
        std::vector<uint32_t> pos(m_);
        for (uint32_t i = 0; i < m_; ++i) {
            pos[i] = bin[g_.esup_[i]];
            vert[pos[i]] = i;
            ++bin[g_.esup_[i]];
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
        auto vis = std::vector<bool>(m_, false);
        std::vector<int > edgid_(n_, -1);
        for (uint32_t i = 0; i < n_; ++i) 
            if (g_.deg_[i] == 0){
                k_order.push_back(i);
                k_rank[i] = k_order.size() - 1;
                nodetruss_[i] = 0;
            }
        for (uint32_t i = 0; i < m_; ++i) {
            const uint32_t e = vert[i];
            if (g_.esup_[e] > k) k = g_.esup_[e];
            ++bin[k];
            trussness_[e] = k+2;
            vis[e] = true;
            uint32_t u=g_.edg_[e].first,v=g_.edg_[e].second;
            if(g_.Nie[u].size()>g_.Nie[v].size())
                std::swap(u,v);
            g_.deg_[u]--, g_.deg_[v]--;
            if(!g_.deg_[u]){
                k_order.push_back(u);
                k_rank[u] = k_order.size() - 1;
                nodetruss_[u] = k+2;
            }
            if(!g_.deg_[v]){
                k_order.push_back(v);
                k_rank[v] = k_order.size() - 1;
                nodetruss_[v] = k+2;
            }
            for(auto e1:g_.Nie[u])
                if(!vis[e1.second]){
                    bin_[e1.first]=true;
                    edgid_[e1.first]=e1.second;
                }
            for(auto e1:g_.Nie[v])
                if(bin_[e1.first]&&!vis[e1.second]){
                    SortSwap(k,edgid_[e1.first],pos,vert,g_.esup_,bin);
                    SortSwap(k,e1.second,pos,vert,g_.esup_,bin);
                }
            for(auto e1:g_.Nie[u])
                if(!vis[e1.second]){
                    bin_[e1.first]=false;
                    edgid_[e1.first]=-1;
                }
        }
        for (int i = 1; i < n_; i++)
            ASSERT(nodetruss_[k_order[i-1]] <= nodetruss_[k_order[i]]);
    }

}