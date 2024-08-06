#include <gtest/gtest.h>
#include <fstream>
#include "exp.h"
#include "trine.h"
#include "core.h"

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

        trine::Trine trine1(n,m);
        core::Core core1(n,m);
        trine1.Load(gedges);
        core1.Load(gedges);
	});
}
