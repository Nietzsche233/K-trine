#include "trine.h"
#include "core.h"
#include "truss.h"

const std::string basedir="../../../../Data/";

int main(int argc, char **argv){

    // ASSERT(2 == argc);

    std::string org_graph_dir_ = basedir+argv[1];
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

    trine::Trine trine1(n,m);
    core::Core core1(n,m);
    truss::Truss truss1(n,m);
    trine1.Load(gedges);
    core1.Load(gedges);
    truss1.Load(gedges);
    int max_core = 0, max_truss = 0, max_trine = 0;
    for (int i = 0; i < n; i++) {
        max_core = std::max(max_core, core1.CorenessOut(i));
        max_truss = std::max(max_truss, truss1.NodetrussOut(i));
        max_trine = std::max(max_trine,trine1.TrinessOut(i));
    }
    printf("core: %d\ntruss: %d\ntrine: %d\n", max_core, max_truss, max_trine);
}