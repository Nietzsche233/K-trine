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
    truss1.Load(gedges);
    while (1){
        printf("input:\n");
        int k;
        scanf("%d",&k);
        std::string out_graph_dir_ = basedir+std::to_string(k) + "-truss" +argv[1];
        FILE* ofile;
        if (argc == 3)
            ofile= fopen(out_graph_dir_.c_str(), "w");
        std::vector<int > is_in_subG(n,0);
        std::vector<bool > is_in_indG(n,0);
        for (int i = 0; i < n; ++i) {
            if (trine1.TrinessOut(i)>= (k - 1) * (k - 2))
                is_in_subG[i] ++;
            if (truss1.NodetrussOut(i)>= k)
                is_in_subG[i] ++;
            if (is_in_subG[i]) {
                printf("%d %d\n", i, is_in_subG[i]);
                if (argc == 3)
                    fprintf(ofile, "%d %d\n", i, is_in_subG[i]);
            }
        }
        if (argc == 3)
            fprintf(ofile, "A\n");
        std::vector<int > target_node;
        for (auto e : gedges) 
            if (is_in_subG[e.first] && is_in_subG[e.second]) {
                if (is_in_subG[e.first] == 1 || is_in_subG[e.second] == 1)
                    is_in_indG[e.first] = is_in_indG[e.second] = true;
                if (argc == 3)
                    printf("%d %d\n", e.first, e.second);
                if (argc == 3)
                    fprintf(ofile, "%d %d\n", e.first, e.second);
            }
        if (argc == 3) {
            fclose(ofile);
        }
        if (argc == 3){
            std::string ind_graph_dir_ = basedir+"ind_"+std::to_string(k) + "-truss"+argv[1];
            FILE* indofile = fopen(ind_graph_dir_.c_str(), "w");;
            for (int i = 0; i < n; ++i)
                if(is_in_indG[i]){
                    fprintf(indofile, "%d %d\n", i, is_in_subG[i]);
                }
            fprintf(indofile, "A\n");
            for (auto e : gedges) 
                if ((is_in_subG[e.first] == 1 || is_in_subG[e.second] == 1) && is_in_subG[e.first] && is_in_subG[e.second]) {
                    fprintf(indofile, "%d %d\n", e.first, e.second);
                }
            fclose(indofile);
            break;
        }
        
    }
}
///dblp_edges_KDDlim6.txt  5