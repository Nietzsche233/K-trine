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
#include "order.h"
#include "thebin.h"


namespace trine{
    using namespace graph;

    #define COUNT_TS false

    class Trine{
        public:

            Trine(const uint32_t n,const uint32_t m);
            Trine(const Trine&) = delete;
            ~Trine();
            void Load(const EdgListT& geders);
            //do triange count and trine decomposition for static graph
            void Insert(const EdgListT&nedges);
            void Remove(const std::vector<EdgT>& redges);

            void YikaiRemove(const std::vector<EdgT>& redges);

            bool CSmall(const uint32_t x, const uint32_t y){
                return c_[x]<c_[y]||(c_[x]==c_[y]&&x<y);
            }
            std::function<bool (const uint32_t, const uint32_t)> c_small;

            void CheckAns();

            void OutNowTriness(){
                printf("\nNow, Triness is:\n[");
                for(auto x:triness_){
                    printf(" %" PRIu32 , x);
                }
                printf(" ]\n\n");
                // printf("\nNow, Rem is:\n[");
                // for(auto x:rem_){
                //     printf(" %" PRIu32 , x);
                // }
                // printf(" ]\n\n");
                // printf("\nNow, ts is:\n[");
                // for(auto x:ts_){
                //     printf(" %" PRIu32 , x);
                // }
                // printf(" ]\n\n");
                // for(uint32_t i=0;i<n_;i++){
                //     printf("check %" PRId32 " is nex triness head %" PRId32 ":\n",i,triness_[i]);
                //     printf("%s\n",order_obj.Pred(head_[triness_[i]],i)?"Yes":"No");
                //     printf("check %" PRId32 " is pre or equil triness tail %" PRId32 ":\n",i,triness_[i]);
                //     if(tail_[triness_[i]]==i)   printf("EQ\n");
                //     else    printf("%s\n",order_obj.Pred(i,tail_[triness_[i]])?"Yes":"No");
                //     if(triness_[i]!=0){
                //         printf("tail[%" PRId32 "-1]=%" PRId32 ",check it is pre head[%" PRId32 "]=%" PRId32 "\n",triness_[i],tail_[triness_[i]-1],triness_[i],head_[triness_[i]]);
                //         printf("%s\n\n",order_obj.Pred(tail_[triness_[i]-1],head_[triness_[i]])?"Yes":"No");
                //     }
                // }
            }

            //statistics
            struct TimeCalc{
                std::chrono::steady_clock::time_point sti,eti;
                std::chrono::nanoseconds _;
                TimeCalc(){
                    _=std::chrono::steady_clock::now()-std::chrono::steady_clock::now();
                }
                ~TimeCalc(){
                }
                void Timestart(){
                    sti=std::chrono::steady_clock::now();
                }
                void Timepause(){
                    eti=std::chrono::steady_clock::now();
                    _ += eti - sti;
                }
            };

            struct STTSTCS{
                TimeCalc tnew_calc,c_update_calc,init_calc,moveCtoO_calc,type2_calc,type3_calc,change_triange_calc;
                int tnew_node=0,tnew_dalta=0,update_node=0;
                int clique_cnt=0,change_cnt=0,real_inf_cnt=0;
                int touch=0;
                int ext_cnt = 0;
                TimeCalc single_insert_[100000];
                void Output(){
                    printf("tnew_calc costs  %f ms\n",
                    std::chrono::duration<double, std::milli>(tnew_calc._).count());
                    printf("tnew_node=%d\n",tnew_node);
                    printf("tnew_dalta=%d\n",tnew_dalta);
                    printf("c_update_calc costs  %f ms\n",
                    std::chrono::duration<double, std::milli>(c_update_calc._).count());
                    printf("update_node=%d\n",update_node);
                    printf("clique_cnt=%d\n",clique_cnt);
                    printf("init_calc costs  %f ms\n",
                    std::chrono::duration<double, std::milli>(init_calc._).count());
                    printf("moveCtoO_calc costs  %f ms\n",
                    std::chrono::duration<double, std::milli>(moveCtoO_calc._).count());
                    printf("real_inf_cnt =%d\n",real_inf_cnt);
                    printf("type2_calc costs  %f ms\n",
                    std::chrono::duration<double, std::milli>(type2_calc._).count());
                    printf("type3_calc costs  %f ms\n",
                    std::chrono::duration<double, std::milli>(type3_calc._).count());
                    printf("change_triange_calc costs  %f ms\n",
                    std::chrono::duration<double, std::milli>(change_triange_calc._).count());
                }
            };
            STTSTCS sttstcs_;
            
        private:

            const uint32_t n_,m_;
            //|V|

            Graph g_;
            // graph.h class Graph

            bool CmpDeg(int x,int y){
                return g_.deg_[x]>g_.deg_[y]||(g_.deg_[x]==g_.deg_[y]&&x>y);
            }
            bool CmpTriness(int x,int y){
                return triness_[x]>triness_[y];
            }

            std::function<bool (const uint32_t, const uint32_t)> cmp_deg_;
            //used by TriangleCount;

            std::function<bool (const uint32_t, const uint32_t)> cmp_triness_;

            // class Order;
            Order order_obj;
            //an obj for K-order
            TB Cbin;
            //some ele has c_!=0 when insert
            
            
            std::vector<uint32_t> c_;
            //it is c (in Batch Insert) and sup (in Tri Decomp) in paper
            std::vector<uint32_t> triness_;
            //triness

            std::vector<uint32_t> triness_plus_;

            std::vector<uint32_t> rem_;
            //rem
            std::vector<uint32_t> ext_;
            //ext
            std::vector<uint32_t> ts_;
            //ts

            struct TriangleT{
                std::pair<uint32_t,uint32_t> vertex[3];
                TriangleT(){
                    vertex[0]=std::make_pair(0,0);
                    vertex[1]=std::make_pair(0,0);
                    vertex[2]=std::make_pair(0,0);
                }
                TriangleT(uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t e,uint32_t f){
                    vertex[0]=std::make_pair(a,b);
                    vertex[1]=std::make_pair(c,d);
                    vertex[2]=std::make_pair(e,f);
                }
                //ext_tri_id_[triangle_[i].vertex[x][0]][triangle_[i].vertex[x][1]]=i;
            };

            std::vector<bool> bin_;

            std::vector<uint32_t> head_;
            std::vector<uint32_t> tail_;
            //list head and tail

            // AdjMatrT new_edg_;
            
            
            
            const void TriangleCount(std::vector<uint32_t>&cnt,bool typ);
            // count triangle in orig-graph. O(m*a(m))
            //result save in cnt
            //type=1 count all triangles
            //type=0 count ts


            void SortSwap(
                uint32_t k,
                uint32_t ID,std::vector<uint32_t>& pos,
                std::vector<uint32_t>& vert,
                std::vector<uint32_t>& c_,
                std::vector<uint32_t> & bin
            );
            const void TriDecomp(std::vector<uint32_t>& order);
            //used in insert;
            void MoveCtoOrder(uint32_t k,uint32_t las);
            uint32_t trinesschange(uint32_t u,uint32_t K);

            bool check(int ans,uint32_t u,std::vector<int>& esup);


            //DEBUG
            bool DEBUGrem(uint32_t u,uint32_t val){
                uint32_t realval=0;
                for(auto v:g_.Nie[u])
                    if(Cbin.IsIn(v)||order_obj.Pred(u,v))
                        bin_[v]=1;
                for(auto v:g_.Nie[u])
                    if(bin_[v]){
                        for(auto w:g_.Nie[v])
                            if(bin_[w])
                                realval+=2;
                        bin_[v]=0;
                    }
                if(val!=realval){
                    printf("Vid=%" PRId32 " rem=%" PRId32 " real=%" PRId32 "\n",u,val,realval);
                    printf("%s\n",order_obj.HPobj.IsIn(u)?"isinorder":"not");
                    return false;
                }
                return true;
            }
            bool DEBUGC(uint32_t u,uint32_t val){
                uint32_t realval=0;
                for(auto v:g_.Nie[u])
                    if((Cbin.IsIn(v)&&triness_[v]>=triness_[u])||(!Cbin.IsIn(v)))
                        bin_[v]=1;
                for(auto v:g_.Nie[u])
                    if(bin_[v]){
                        for(auto w:g_.Nie[v])
                            if(bin_[w])
                                realval+=2;
                        bin_[v]=0;
                    }
                if(val!=realval){
                    printf("Vid=%" PRId32 " rem=%" PRId32 " real=%" PRId32 "\n",u,val,realval);
                    printf("%s\n",order_obj.HPobj.IsIn(u)?"isinorder":"not");
                    return false;
                }
                return true;
            }
        
    };

}