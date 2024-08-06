/*
trine.h
*/




#include "trine.h"
#include "defs.h"

namespace trine{

    Trine::Trine(const uint32_t n,const uint32_t m) :
        c_small(std::bind(&Trine::CSmall,this,std::placeholders::_1,std::placeholders::_2)),
        n_(n),
        m_(m),
        g_(n),
        order_obj(n,m),
        Cbin(n)
    {
        
        cmp_deg_=std::bind(&Trine::CmpDeg,this,std::placeholders::_1,std::placeholders::_2);
        c_.resize(n_,0);
        triness_.resize(n_);
        rem_.resize(n_,0);
        ext_.resize(n_,0);
        ts_.resize(n_,0);
        bin_.resize(n_,false);
        // new_edg_.resize(n_);
    }

    Trine::~Trine(){
        //we have no ptr
    }

    void Trine::Load(const EdgListT&gedges){
        for (const auto edg :gedges){
            const uint32_t v1 = edg.first;
            const uint32_t v2 = edg.second;
            ASSERT(v1 < n_ && v2 < n_ && v1 != v2);
            g_.Nie[v1].push_back(v2);
            g_.Nie[v2].push_back(v1);
            g_.deg_[v1]++,g_.deg_[v2]++;
        }
        TriangleCount(c_,!(COUNT_TS));
        std::vector<uint32_t> order;
        TriDecomp(order);
        TriangleCount(ts_,COUNT_TS);
        std::vector<bool> vis(n_, false);
        std::vector<uint32_t> listorder;
        for(uint32_t v:order){
            vis[v]=true;
            rem_[v]=c_[v];
            //we have a right c_
            ext_[v]=c_[v]=0;
            //we have c_ and needn't s_
            const uint32_t k=triness_[v];
            while(head_.size()<=k){
                uint32_t newk=head_.size();
                head_.push_back(n_+newk);
                listorder.push_back(head_[newk]);
                tail_.push_back(n_+newk);
            }
            listorder.push_back(v);
            tail_[k]=v;
            /*
            [...,n_+k,...some real vertexs and their triness=k...,n_+k+1,...]
            head_[k]->n_+k
            tail_[k]->the ele pre n_+k+1
            nomatter whether there is a triness=k vertex
            */
        }
        order_obj.Load(listorder);
        // OutNowTriness();
    }

    const void Trine::TriangleCount(std::vector<uint32_t>&cnt,bool typ){
        bin_.resize(n_,false);
        std::vector<int>_(n_);
        for(int i=0;i<n_;i++)   _[i]=i;
        // /*
        

        std::sort(_.begin(),_.end(),cmp_deg_);
        //when -O3 this maybe O(n)...anyway,O(nlogn)<O(m*a(m))
        // */
        for(uint32_t i=0;i<n_;i++){
            for(auto j:g_.Nie[_[i]])
                if(g_.deg_[j]<g_.deg_[_[i]]||(g_.deg_[j]==g_.deg_[_[i]]&&j<_[i]))
                    bin_[j]=true;
            for(auto j:g_.Nie[_[i]])
                if(g_.deg_[j]<g_.deg_[_[i]]||(g_.deg_[j]==g_.deg_[_[i]]&&j<_[i])){
                    for(auto k:g_.Nie[j])
                        if(bin_[k]){
                            if(typ||(triness_[_[i]]<=triness_[j]&&triness_[_[i]]<=triness_[k]))
                                cnt[_[i]]+=2;
                            if(typ||(triness_[j]<=triness_[_[i]]&&triness_[j]<=triness_[k]))
                                cnt[j]+=2;
                            if(typ||(triness_[k]<=triness_[_[i]]&&triness_[k]<=triness_[j]))
                                cnt[k]+=2;
                        }
                    bin_[j]=false;
                }
        }
        // printf("\nTriangle Count is finished:\n[");
        // for(auto x:cnt){
        //     printf(" %" PRIu32 , x);
        // }
        // printf(" ]\n\n");
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

    const void Trine::TriDecomp(std::vector<uint32_t>& order){
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
            order.push_back(v);
        }
    }

    void Trine::Insert(const EdgListT&nedges){
        ASSERT(!nedges.empty());
        sttstcs_.ext_cnt = 0;
        //do for each nedg
        for(auto edg:nedges){
            uint32_t u,v;
            u=edg.first,v=edg.second;
            if(g_.deg_[u]>g_.deg_[v]) std::swap(u,v);
            for(auto w:g_.Nie[u])   bin_[w]=true;
            for(auto w:g_.Nie[v])
                if(bin_[w]){
                    uint32_t x;
                    x=order_obj.Pred(u,v)?u:v;
                    x=order_obj.Pred(x,w)?x:w;
                    ext_[x]+=2;
                    sttstcs_.ext_cnt += 2;
                    if(!order_obj.HPobj.IsIn(x))  order_obj.HPobj.Insert(x);
                    if(triness_[u]<=triness_[v]&&triness_[u]<=triness_[w])  ts_[u]+=2;
                    if(triness_[v]<=triness_[w]&&triness_[v]<=triness_[u])  ts_[v]+=2;
                    if(triness_[w]<=triness_[u]&&triness_[w]<=triness_[v])  ts_[w]+=2;
                }
            for(auto w:g_.Nie[u])   bin_[w]=false;
            g_.deg_[u]++,g_.Nie[u].push_back(v);
            g_.deg_[v]++,g_.Nie[v].push_back(u);
        }


        //for k 0 to ...
        if(order_obj.HPobj.IsEmpty()){
            return ;
        }

        sttstcs_.single_insert_[sttstcs_.ext_cnt].Timestart();   

        for(int32_t k=triness_[order_obj.HPobj.Top()];;){
            // printf("k=%" PRIu32 "\n",k);
            while (head_.size() <= k) {
                uint32_t newk=head_.size();
                head_.push_back(n_+newk);
                order_obj.Insert(head_[newk],tail_[newk-1]);
                order_obj.ListInsert(head_[newk],tail_[newk-1]);
                tail_.push_back(n_+newk);
            }
            MoveCtoOrder(k,head_[k]);
            
            while(!order_obj.HPobj.IsEmpty()&&triness_[order_obj.HPobj.Top()]==k){
                uint32_t u = order_obj.HPobj.Top();
                order_obj.HPobj.Delete(u);
                if(rem_[u]+ext_[u]>k){
                    MoveOrdertoC(u,k);
                }
                else{
                    // uint32_t las=order_obj.ListPrev(u);
                    // in_c_cnt_++;
                    // MoveOrdertoC(u,k);
                    // MoveCtoOrder(k,las);
                    rem_[u]=rem_[u]+ext_[u];
                    c_[u]=ext_[u]=0;
                    for(auto v:g_.Nie[u])
                        if(Cbin.IsIn(v)||order_obj.HPobj.IsIn(v))
                            bin_[v]=1;
                    for(auto v:g_.Nie[u]){
                        if(!bin_[v])    continue;
                        for(auto w:g_.Nie[v]){
                            if(!bin_[w])    continue;
                            if(!Cbin.IsIn(v)&&!Cbin.IsIn(w))    continue;
                            if(Cbin.IsIn(v)){
                                Cbin.Delete(v);
                                ASSERT(c_[v]>=2);
                                c_[v]-=2;
                                Cbin.Insert(v,c_[v]);
                            }
                            else if(order_obj.HPobj.IsIn(v)){
                                ASSERT(ext_[v]>=2);
                                ext_[v]-=2;
                                if(!ext_[v])
                                    order_obj.HPobj.Delete(v);
                            }
                            if(Cbin.IsIn(w)){
                                Cbin.Delete(w);
                                ASSERT(c_[w]>=2);
                                c_[w]-=2;
                                Cbin.Insert(w,c_[w]);
                            }
                            else if(order_obj.HPobj.IsIn(w)){
                                ASSERT(ext_[w]>=2);
                                ext_[w]-=2;
                                if(!ext_[w])
                                    order_obj.HPobj.Delete(w);
                            }
                        }
                        bin_[v]=0;
                    }
                    MoveCtoOrder(k,u);
                }
            }
            // printf("***\n");
            //next k
            if(order_obj.HPobj.IsEmpty()&&Cbin.IsEmpty())    break;
            if(order_obj.HPobj.IsEmpty())
                k=c_[Cbin.Top()];
            else if(Cbin.IsEmpty())
                k=triness_[order_obj.HPobj.Top()];
            else{
                if(triness_[order_obj.HPobj.Top()]<c_[Cbin.Top()])
                    k=triness_[order_obj.HPobj.Top()];
                else
                    k=c_[Cbin.Top()];
            }
        }
        // OutNowTriness();

        sttstcs_.single_insert_[sttstcs_.ext_cnt].Timepause();
    }

    void Trine::MoveCtoOrder(uint32_t K,uint32_t las){


        while(!Cbin.IsEmpty()&&c_[Cbin.Top()]<=K){
            uint32_t u=Cbin.Top();
            Cbin.Delete(u);
            if(triness_[u]!=K){
                // printf("Vid= %" PRIu32 ": %" PRIu32 " => %" PRIu32 "\n",u,triness_[u],K);
                //ts change 
                ts_[u]=0;
                for(auto v:g_.Nie[u])  bin_[v]=true;
                for(auto v:g_.Nie[u]){
                    for(auto w:g_.Nie[v])
                        if(bin_[w]){
                            if(K<=triness_[v]&&K<=triness_[w])
                                ts_[u]+=2;
                            if(triness_[v]<=triness_[w]&&triness_[v]<=K&&triness_[v]>triness_[u])
                                ts_[v]+=2;
                            if(triness_[w]<=triness_[v]&&triness_[w]<=K&&triness_[w]>triness_[u])
                                ts_[w]+=2;
                        }//Equivalence with Yikai Zhang order.cc line 137-143
                    bin_[v]=false;
                }
            }
            triness_[u]=K;
            rem_[u]=c_[u];
            c_[u]=ext_[u]=0;
            for(auto v:g_.Nie[u])
                if(Cbin.IsIn(v)||order_obj.HPobj.IsIn(v))
                    bin_[v]=1;
            for(auto v:g_.Nie[u]){
                if(!bin_[v])   continue;
                for(auto w:g_.Nie[v]){
                    if(!bin_[w])    continue;
                    //ASSERT(CorN.IsIn(v)||order_obj.HPobj.IsIn(v));
                    //ASSERT(CorN.IsIn(w)||order_obj.HPobj.IsIn(w));
                    if(Cbin.IsIn(v)){
                        Cbin.Delete(v);
                        //ASSERT(c_[v]>=2);
                        c_[v]-=2;
                        Cbin.Insert(v,c_[v]);
                    }
                    else if(order_obj.HPobj.IsIn(v)){
                        //ASSERT(ext_[v]>=2);
                        ext_[v]-=2;
                        if(!ext_[v])
                            order_obj.HPobj.Delete(v);
                    }
                    if(Cbin.IsIn(w)){
                        Cbin.Delete(w);
                        //ASSERT(c_[w]>=2);
                        c_[w]-=2;
                        Cbin.Insert(w,c_[w]);
                    }
                    else if(order_obj.HPobj.IsIn(w)){
                        //ASSERT(ext_[w]>=2);
                        ext_[w]-=2;
                        if(!ext_[w])
                            order_obj.HPobj.Delete(w);
                    }
                }
                bin_[v]=0;
            }
            order_obj.Insert(u,las);
            order_obj.ListInsert(u,las);
            if(tail_[K]==las)   tail_[K]=u;
            las=u;
        }

    }//remove one from CvN

    void Trine::MoveOrdertoC(uint32_t u,uint32_t K){
        if(order_obj.HPobj.IsIn(u)) order_obj.HPobj.Delete(u);
        c_[u]=rem_[u]+ext_[u];
        rem_[u]=ext_[u]=0;
        Cbin.Insert(u,c_[u]);
        for(auto v:g_.Nie[u])
            if(!Cbin.IsIn(v)&&order_obj.Pred(u,v))
                bin_[v]=true;
        for(auto v:g_.Nie[u]){
            if(!bin_[v])   continue;
            for(auto w:g_.Nie[v])
                if(bin_[w]){
                    // printf("%" PRId32 "%" PRId32 "\n",w,v);
                    ext_[v]+=2,ext_[w]+=2;
                    if(!order_obj.HPobj.IsIn(v))  order_obj.HPobj.Insert(v);
                    if(!order_obj.HPobj.IsIn(w))  order_obj.HPobj.Insert(w);
                }
            bin_[v]=false;
        }
        order_obj.ListRemove(u,head_[K],tail_[K]);
        order_obj.Remove(u);
    }//insert one into CvN

    void Trine::Remove(const std::vector<EdgT>& redges){
        ASSERT(!redges.empty());

        std::queue<uint32_t> Q;

        //do for each redg
        for(auto edg:redges){
            uint32_t u,v;
            u=edg.first,v=edg.second;
            ASSERT(u < n_ && v < n_ && u != v);
            if(g_.deg_[u]>g_.deg_[v]) std::swap(u,v);

            for(auto w:g_.Nie[u])   bin_[w]=true;
            for(auto w:g_.Nie[v])
                if(bin_[w]){
                    uint32_t x;
                    x=order_obj.Pred(u,v)?u:v;
                    x=order_obj.Pred(x,w)?x:w;
                    rem_[x]-=2;
                    //p=u,v,w:
                    for(auto p:{u,v,w}){
                        if(triness_[p]==triness_[x]){
                            if(ts_[p]==triness_[p])   Q.push(p);
                            ts_[p]-=2;
                        }
                    }
                }
            for(auto w:g_.Nie[u])   bin_[w]=false;

            //p=u,v;q=v,u;
            for(auto p:{u,v}){
                uint32_t q=u+v-p;
                int i;
                g_.deg_[p]--;
                for(int i=0;i<g_.Nie[p].size();i++)
                    if(g_.Nie[p][i]==q){
                        std::swap(g_.Nie[p][i],g_.Nie[p][g_.Nie[p].size()-1]);
                        g_.Nie[p].pop_back();
                        break;
                    }
                // if(!g_.deg_[p]){
                //     order_obj.Remove(p);
                //     order_obj.ListRemove(p,head_[triness_[p]],tail_[triness_[p]]);
                // }
            }
        }
        //Q---
        
        while(!Q.empty()){
            uint32_t u=Q.front();

            Q.pop();
            
            std::vector<std::pair<int ,int > > infl_trangle;
            std::vector<uint32_t> sum(triness_[u]+1,0);
            for(auto v:g_.Nie[u])   bin_[v]=true;
            for(auto v:g_.Nie[u]){
                for(auto w:g_.Nie[v]){
                    if(!bin_[w])    continue;
                    uint32_t min_triness;
                    min_triness=triness_[v]<=triness_[w]?triness_[v]:triness_[w];
                    if(min_triness>triness_[u])
                        min_triness=triness_[u];
                    else
                        infl_trangle.push_back(std::make_pair(v,w));
                    sum[min_triness]+=2;
                }
                bin_[v]=false;
            }
            uint32_t tnew;
            for(tnew=triness_[u]-1;tnew>0;tnew--){
                sum[tnew]+=sum[tnew+1];
                if(sum[tnew]>=tnew)   break;
            }
            ts_[u]=sum[tnew];

            if(tnew==triness_[u])   continue;

            for(auto triange_:infl_trangle){
                int v=triange_.first,w=triange_.second;
                if(triness_[v]<=tnew||triness_[w]<=tnew)    continue;
                // triness_[p]>tnew
                for(auto p:{v,w}){
                    uint32_t q=v+w-p;
                    if(triness_[p]>triness_[q]) continue;
                    //ts for v,w
                    if(ts_[p]>=triness_[p]){
                        /*if(triness_[p]>tnew)*/{
                            ts_[p]-=2;
                            if(ts_[p]==triness_[p]-2)
                                Q.push(p);
                        }
                    }
                    //rem for u and v,w
                    if(order_obj.Pred(q,p))    continue;
                    if(order_obj.Pred(p,u)/*&&triness_[p]>tnew*/)
                        rem_[p]-=2,rem_[u]+=2;
                }
            }
            order_obj.Remove(u);
            order_obj.ListRemove(u,head_[triness_[u]],tail_[triness_[u]]);

            order_obj.Insert(u,tail_[tnew]);
            order_obj.ListInsert(u,tail_[tnew]);
            tail_[tnew]=u;
            triness_[u]=tnew;
        }
    }
    
    void Trine::CheckAns(){

        ASSERT(order_obj.HPobj.IsEmpty());

        for(auto x:bin_)    ASSERT(x==false);
        for(auto x:c_)  ASSERT(x==0);
        for(auto x:ext_)    ASSERT(x==0);
        
        //deg
        for(uint32_t u=0;u<n_;u++){
            uint32_t cnt=0;
            for(auto v:g_.Nie[u])   cnt++;
            if(cnt!=g_.deg_[u]) printf("vid=%" PRId32 " :deg_=%" PRId32 ", cnt=%" PRId32 "\n",u,g_.deg_[u],cnt);
            ASSERT(cnt==g_.deg_[u]);
        }
        //ts
        for(uint32_t u=0;u<n_;u++){
            uint32_t cnt=0;
            for(auto v:g_.Nie[u])
                if(triness_[v]>=triness_[u])
                    bin_[v]=true;
            for(auto v:g_.Nie[u]){
                if(triness_[v]<triness_[u]) continue;
                for(auto w:g_.Nie[v])
                    if(triness_[w]>=triness_[u]&&bin_[w])
                        cnt+=2;
                bin_[v]=false;
            }
            if(cnt!=ts_[u])
                printf("cnt==%" PRId32 ", ts_[%" PRId32 "]==%" PRId32 "\n",cnt,u,ts_[u]);
            ASSERT(cnt==ts_[u]);
        }
        //rem order
        for(uint32_t u=0;u<n_;u++){
            uint32_t cnt=0;
            for(auto v:g_.Nie[u])
                if(order_obj.Pred(u,v)){
                    bin_[v]=true;
                }
            for(auto v:g_.Nie[u]){
                if(!bin_[v])    continue;
                for(auto w:g_.Nie[v])
                    if(bin_[w])
                        cnt+=2;
                bin_[v]=false;
            }
            if(rem_[u]!=cnt)    printf("vid=%" PRId32 " :rem_=%" PRId32 ", cnt=%" PRId32 "\n",u,rem_[u],cnt);
            //ASSERT(cnt==rem_[u]);
        }
        //triness
        std::vector<uint32_t> ans_;
        for(uint32_t i=0;i<n_;i++)
            ans_.push_back(triness_[i]),triness_[i]=0;
        TriangleCount(c_,!(COUNT_TS));
        std::vector<uint32_t> order;
        printf("Here\n");
        TriDecomp(order);
        for(uint32_t i=0;i<n_;i++)
            ASSERT(ans_[i]==triness_[i]);
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ACCEPT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        for(uint32_t i=0;i<n_;i++)
            c_[i]=0;
        // OutNowTriness();
    }
}
