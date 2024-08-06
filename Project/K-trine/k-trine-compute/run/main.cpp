#include "trine.h"

const std::string basedir="../../../../Data/";

int main(int argc, char **argv){

    ASSERT(3 == argc);
    
    int k = atoi(argv[2]);

    std::string org_graph_dir_ = basedir+argv[1];;
    printf("*****************************************************************\n");
    printf("the original graph: %s\n", org_graph_dir_.c_str());
    printf("*****************************************************************\n");
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

    printf("Init finish\n");

    trine::Trine tri_1_(n,m);
    trine::Trine tri_2_(n,m);
    trine::Trine tri_3_(n,m);

    // const auto faster_beg = std::chrono::steady_clock::now();
    // tri_1_.ComputeK_trineFaster(k,gedges);
    // const auto faster_end = std::chrono::steady_clock::now();
    // const auto faster_dif = faster_end - faster_beg;
    // printf("the faster costs %f ms\n",
    // std::chrono::duration<double, std::milli>(faster_dif).count());

    // const auto baseline_beg = std::chrono::steady_clock::now();
    // tri_2_.ComputeK_trine(k,gedges);
    // const auto baseline_end = std::chrono::steady_clock::now();
    // const auto baseline_dif = baseline_end - baseline_beg;
    // printf("the baseline costs %f ms\n",
    // std::chrono::duration<double, std::milli>(baseline_dif).count());

    const auto v1_beg = std::chrono::steady_clock::now();
    tri_3_.ComputeK_trine_v1(k,gedges);
    const auto v1_end = std::chrono::steady_clock::now();
    const auto v1_dif = v1_end - v1_beg;
    printf("the v1 costs %f ms\n",
    std::chrono::duration<double, std::milli>(v1_dif).count());

}