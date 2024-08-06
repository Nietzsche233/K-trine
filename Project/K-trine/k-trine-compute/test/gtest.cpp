#include <gtest/gtest.h>
#include <fstream>
#include "exp.h"
#include "trine.h"

#if defined(__linux__)
const std::vector<std::string> k_graph_dataset = {
	email,dblp
};
#else
const std::vector<std::string> k_graph_dataset = {
	
};
#endif

TEST(HelloTest, BasicAssertions){
    EXPECT_STRNE("hello", "world");
    ASSERT_EQ(7*6,42);
}

typedef void process_func(std::string path);

void run_all_graph_dataset(process_func check)
{
	for (auto f : k_graph_dataset) {
		check(dataset(f));
	}
}

TEST(GraphSum, LocalBuildTest) {
	run_all_graph_dataset([](std::string f) {
        FILE* file = fopen(f.c_str(), "r");

        uint32_t n, m;
        fscanf(file, "%" SCNu32 " %" SCNu32, &n, &m);
        std::vector<graph::EdgT> gedges;
        for (uint32_t e = 0; e < m; ++e) {
            uint32_t v1, v2;
            fscanf(file, "%" SCNu32 " %" SCNu32, &v1, &v2);
            gedges.push_back({v1, v2});
        }
        fclose(file);

        trine::Trine tri_0_(n,m);
        tri_0_.TrineDecomposition(gedges);

        for(int k = 0 ;k <= 100 ;k+=2){
            printf("%d:\n",k);
            trine::Trine tri_1_(n,m);
            trine::Trine tri_2_(n,m);
            trine::Trine tri_3_(n,m);
            tri_1_.ComputeK_trineFaster(k,gedges);
            tri_2_.ComputeK_trine(k,gedges);
            tri_3_.ComputeK_trine_v1(k,gedges);

            ASSERT_EQ(tri_1_.k_trine_nodes_.size(),tri_2_.k_trine_nodes_.size());
            ASSERT_EQ(tri_3_.k_trine_nodes_.size(),tri_2_.k_trine_nodes_.size());
            int siz = tri_1_.k_trine_nodes_.size();
            ASSERT_EQ(tri_1_.k_trine_nodes_,tri_2_.k_trine_nodes_);
            ASSERT_EQ(tri_3_.k_trine_nodes_,tri_2_.k_trine_nodes_);
            for(int i = 0; i < siz; i++){
                ASSERT_GE(tri_0_.triness_[tri_1_.k_trine_nodes_[i]],k);
            }    
        }
	});
}
