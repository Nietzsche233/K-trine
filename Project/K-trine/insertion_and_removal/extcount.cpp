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
#include "defs.h"
#include "trine.h"

//ext.out indir outdir
int main(int argc, char** argv) {

    //printf("HELLO\n");
    std::string input_dir;
    std::string out_dir;
    input_dir = argv[1];
    out_dir = argv[2];
    FILE* infile_ = fopen(input_dir.c_str(), "r");
    FILE* outfile_ = fopen(out_dir.c_str(), "w");
    uint32_t n, m;
    fscanf(infile_, "%" SCNu32 " %" SCNu32, &n, &m);
    std::vector<graph::EdgT > gedges;
    for (uint32_t e = 0; e < m; ++e) {
        uint32_t v1, v2;
        fscanf(infile_, "%" SCNu32 " %" SCNu32, &v1, &v2);
        gedges.push_back({v1, v2});
    }
    fclose(infile_);
    trine::Trine cm(n, m);
    cm.Load(gedges);
    bool flag=0;
    for (int i = 0; i < m; i++) {
        std::vector<graph::EdgT > cha_edges_;
        cha_edges_.push_back(gedges[i]);
        cm.Remove(cha_edges_);
        cm.Insert(cha_edges_);
    }
    for (auto x : cm.sttstcs_.single_insert_) {
        fprintf(outfile_,"%f\n", std::chrono::duration<double, std::milli > (x._).count());
    }
}
