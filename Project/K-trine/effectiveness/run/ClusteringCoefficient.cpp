#include "trine.h"
#include "core.h"
#include "truss.h"

const std::string basedir="../../../../Data/";

int main(int argc, char **argv){

    ASSERT(2 == argc);

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

    trine::Trine trine1(n,m);
    core::Core core1(n,m);
    truss::Truss truss1(n,m);
    trine1.Load(gedges);
    core1.Load(gedges);
    truss1.Load(gedges);
    printf("input:\n");
    while(1){
        int ro;
        scanf("%d",&ro);
        printf("core cc : %.6lf\n",core1.ClusteringCoefficient(ro));
        printf("trine cc : %.6lf\n",trine1.ClusteringCoefficient(ro));
        printf("truss cc : %.6lf\n",truss1.ClusteringCoefficient(ro));
    }
}