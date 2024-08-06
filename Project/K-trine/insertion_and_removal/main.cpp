#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <iostream>
#include<random>

#include "defs.h"
#include "trine.h"

int main(int argc, char** argv) {

    //printf("HELLO\n");
    std::string ins_output_dir;
    std::string rem_output_dir;
    std::string dec_output_dir;
    FILE *ins_outfile,*rem_outfile,*dec_outfile;
    if(argc==4){
        ins_output_dir=argv[1];
        rem_output_dir=argv[2];
        dec_output_dir=argv[3];
        ins_outfile=fopen(ins_output_dir.c_str(),"w");
        rem_outfile=fopen(rem_output_dir.c_str(),"w");
        dec_outfile=fopen(dec_output_dir.c_str(),"w");
    }
    else if(argc==3){
        ins_output_dir=argv[1];
        rem_output_dir=argv[2];
        ins_outfile=fopen(ins_output_dir.c_str(),"w");
        rem_outfile=fopen(rem_output_dir.c_str(),"w");
    }
    else if(argc==2){
        ins_output_dir=argv[1];
        rem_output_dir=argv[1];
        ins_outfile=fopen(ins_output_dir.c_str(),"w");
        rem_outfile=ins_outfile;
    }

    std::string org_graph_dir_;
    std::cin>>org_graph_dir_;
    //std::cout<<org_graph_dir_<<std::endl;
    FILE* file = fopen(org_graph_dir_.c_str(), "r");
    uint32_t n, m;
    fscanf(file, "%" SCNu32 " %" SCNu32, &n, &m);
    std::vector<graph::EdgT> gedges;
    for (uint32_t e = 0; e < m; ++e) {
        uint32_t v1, v2;
        fscanf(file, "%" SCNu32 " %" SCNu32, &v1, &v2);
        gedges.push_back({v1, v2});
    }
    fclose(file);

    if(argc==4){
        std::cout<<ins_output_dir<<std::endl<<rem_output_dir<<std::endl;
        std::mt19937 rng(2333);
        std::shuffle(gedges.begin(), gedges.end(), rng);
        std::vector<graph::EdgT> cha_edges_;
        for(int i=0;i<m/100;i++)
                cha_edges_.push_back(gedges[i]);
        int delta=m/5;
        std::vector<graph::EdgT> base_edges_;
        for(int i=delta;i<=m;i+=delta){
            
            for(int j=i-delta;j<i;j++)
                base_edges_.push_back(gedges[j]);
            
            trine::Trine cm(n,i);

            const auto dec_beg = std::chrono::steady_clock::now();
            cm.Load(base_edges_);
            const auto dec_end = std::chrono::steady_clock::now();
            const auto dec_dif = dec_end - dec_beg;
            fprintf(dec_outfile,"%d %f\n",i/delta*20,std::chrono::duration<double, std::milli>(dec_dif).count());
            
            const auto rem_beg = std::chrono::steady_clock::now();
            cm.Remove(cha_edges_);
            const auto rem_end = std::chrono::steady_clock::now();
            const auto rem_dif = rem_end - rem_beg;

            //cm.CheckAns();

            fprintf(rem_outfile,"%d %f\n",i/delta*20,std::chrono::duration<double, std::milli>(rem_dif).count());

            const auto ins_beg = std::chrono::steady_clock::now();
            cm.Insert(cha_edges_);
            const auto ins_end = std::chrono::steady_clock::now();
            const auto ins_dif = ins_end - ins_beg;
            fprintf(ins_outfile,"%d %f\n",i/delta*20,std::chrono::duration<double, std::milli>(ins_dif).count());
        }
        return 0;
    }

    trine::Trine cm(n,m);
    cm.Load(gedges);

    bool flag=0;
    
    while(1){

        int opt;
        scanf("%d",&opt);
        std::cout << opt <<" ";
        
        if(opt==-1) break;
        
        if(opt==-2){
            flag=1;
            break;
        }
        std::string first_output_,cha_graph_dir_;
        std::cin>>first_output_>>cha_graph_dir_;
        std::cout<<cha_graph_dir_<<std::endl;
        FILE* cha_file = fopen(cha_graph_dir_.c_str(), "r");
        uint32_t cha_m = 0;
        fscanf(cha_file, "%" SCNu32, &cha_m);
        std::vector<graph::EdgT> cha_edges_;
        for (uint32_t e = 0; e < cha_m; ++e) {
            uint32_t v1, v2;
            fscanf(cha_file, "%" SCNu32 " %" SCNu32, &v1, &v2);
            cha_edges_.push_back({v1, v2});
            //ASSERT(v1 < n && v2 < n);
        }
        fclose(cha_file);

        if(opt==0){
            const auto check_beg = std::chrono::steady_clock::now();
            
            cm.Insert(cha_edges_);

            const auto check_end = std::chrono::steady_clock::now();
            const auto check_dif = check_end - check_beg;
            if(argc>1)
                fprintf(ins_outfile,"%s %f\n",first_output_.c_str(),std::chrono::duration<double, std::milli>(check_dif).count());
            else
                printf("%s %f\n",first_output_.c_str(),std::chrono::duration<double, std::milli>(check_dif).count());

            //cm.CheckAns();
        }
        else{//opt==1
            const auto check_beg = std::chrono::steady_clock::now();
            
            cm.Remove(cha_edges_);
            
            const auto check_end = std::chrono::steady_clock::now();
            const auto check_dif = check_end - check_beg;
            if(argc>1)
                fprintf(rem_outfile,"%s %f\n",first_output_.c_str(),std::chrono::duration<double, std::milli>(check_dif).count());
            else
                printf("%s %f\n",first_output_.c_str(),std::chrono::duration<double, std::milli>(check_dif).count());

            //cm.CheckAns();
        }
    }
    if(flag){

        int limit=0,is_tamp_,step_len_;
        scanf("%d %d %d",&is_tamp_,&limit,&step_len_);
        if(!is_tamp_){
            std::mt19937 rng(2333);
            std::shuffle(gedges.begin(), gedges.end(), rng);
        }
        limit=std::min((int)m,limit);
        std::vector<graph::EdgT> cha_edges_;
        for(int i=1;i<=limit;i+=step_len_){
            
            for(int j=i;j<i+step_len_&&j<=limit;j++)
                cha_edges_.push_back(gedges[m-j]);
            
            const auto rem_beg = std::chrono::steady_clock::now();
            
            cm.Remove(cha_edges_);
            
            const auto rem_end = std::chrono::steady_clock::now();
            const auto rem_dif = rem_end - rem_beg;

            //cm.CheckAns();


            if(argc>1)
                fprintf(rem_outfile,"%d %f\n",i+step_len_-1,std::chrono::duration<double, std::milli>(rem_dif).count());
            else
                printf("%d %f\n",i+step_len_-1,std::chrono::duration<double, std::milli>(rem_dif).count());

            
            const auto ins_beg = std::chrono::steady_clock::now();
            
            cm.Insert(cha_edges_);

            const auto ins_end = std::chrono::steady_clock::now();
            const auto ins_dif = ins_end - ins_beg;
            if(argc>1)
                fprintf(ins_outfile,"%d %f\n",i+step_len_-1,std::chrono::duration<double, std::milli>(ins_dif).count());
            else
                printf("%d %f\n",i+step_len_-1,std::chrono::duration<double, std::milli>(ins_dif).count());
        }
    }
}
