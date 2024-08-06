#include "trine.h"
#include "core.h"
#include "truss.h"
#include "trineplus.h"

const std::string basedir = "../dataset/";
const std::string cnt_dir = "_cnt.txt";
const std::string edges_dir = "_edges.txt";
const std::string trine_res_dir = "_trine.txt";
const std::string truss_res_dir = "_truss.txt";
const std::string core_res_dir = "_core.txt";
const std::string degree_res_dir = "_degree.txt";
const std::string triangle_res_dir = "_triangle.txt";
const std::string dt_res_dir = "_dt.txt";
const std::string trineplus_res_dir = "_trineplus.txt";

int main(int argc, char **argv){
    ASSERT(2 == argc);

    std::string org_graph_dir_ = basedir + argv[1] + edges_dir;
    std::string checkins_cnt_dir_ = basedir + argv[1] + cnt_dir;
    printf("*****************************************************************\n");
    printf("the original graph: %s\n", org_graph_dir_.c_str());
    printf("the checkins cnt: %s\n", checkins_cnt_dir_.c_str());
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
    trineplus::Trineplus trineplus1(n,m);
    trine1.Load(gedges);
    core1.Load(gedges);
    truss1.Load(gedges);
    trineplus1.Load(gedges);
    FILE* insfile = fopen(checkins_cnt_dir_.c_str(), "r");
    fscanf(insfile, "%" SCNu32, &n);
    std::vector <uint32_t> countt_(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        fscanf(insfile, "%" SCNu32 " %" SCNu32 "\n", &x, &y);
        countt_[x] = y;
    }
    fclose(insfile);
    std::string trine_result_dir_ = basedir + argv[1] + trine_res_dir;
    std::string truss_result_dir_ = basedir + argv[1] + truss_res_dir;
    std::string core_result_dir_ = basedir + argv[1] + core_res_dir;
    std::string degree_result_dir_ = basedir + argv[1] + degree_res_dir;
    std::string triangle_result_dir_ = basedir + argv[1] + triangle_res_dir;
    std::string dt_result_dir_ = basedir + argv[1] + dt_res_dir;
    std::string trineplus_result_dir_ = basedir + argv[1] + trineplus_res_dir;

    {
        FILE* file = fopen(trine_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = trine1.TrinessOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 1; i < siz_; i++) {
            checkin_sum_[i] += checkin_sum_[i - 1];
            ness_cnt_[i] += ness_cnt_[i - 1];
        }
        for (int i = 0; i < checkin_sum_.size(); i++){
            if (ness_cnt_[i] == 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", ness_cnt_[i], 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(truss_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = truss1.NodetrussOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 1; i < siz_; i++) {
            checkin_sum_[i] += checkin_sum_[i - 1];
            ness_cnt_[i] += ness_cnt_[i - 1];
        }
        for (int i = 0; i < checkin_sum_.size(); i++){
            if (ness_cnt_[i] == 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", ness_cnt_[i], 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(core_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = core1.CorenessOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 1; i < siz_; i++) {
            checkin_sum_[i] += checkin_sum_[i - 1];
            ness_cnt_[i] += ness_cnt_[i - 1];
        }
        for (int i = 0; i < checkin_sum_.size(); i++){
            if (ness_cnt_[i] == 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", ness_cnt_[i], 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(degree_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = core1.DegreeOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 1; i < siz_; i++) {
            checkin_sum_[i] += checkin_sum_[i - 1];
            ness_cnt_[i] += ness_cnt_[i - 1];
        }
        for (int i = 0; i < checkin_sum_.size(); i++){
            if (ness_cnt_[i] == 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", ness_cnt_[i], 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(triangle_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = trine1.TriangleOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 1; i < siz_; i++) {
            checkin_sum_[i] += checkin_sum_[i - 1];
            ness_cnt_[i] += ness_cnt_[i - 1];
        }
        for (int i = 0; i < checkin_sum_.size(); i++){
            if (ness_cnt_[i] == 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", ness_cnt_[i], 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(dt_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = trine1.TriangleOut(i) + core1.DegreeOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 1; i < siz_; i++) {
            checkin_sum_[i] += checkin_sum_[i - 1];
            ness_cnt_[i] += ness_cnt_[i - 1];
        }
        for (int i = 0; i < checkin_sum_.size(); i++){
            if (ness_cnt_[i] == 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", ness_cnt_[i], 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(trineplus_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = trineplus1.TrineplusnessOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 1; i < siz_; i++) {
            checkin_sum_[i] += checkin_sum_[i - 1];
            ness_cnt_[i] += ness_cnt_[i - 1];
        }
        for (int i = 0; i < checkin_sum_.size(); i++){
            if (ness_cnt_[i] == 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", ness_cnt_[i], 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }

    std::string trine_single_result_dir_ = basedir + "_single" + argv[1] + trine_res_dir;
    std::string truss_single_result_dir_ = basedir + "_single" + argv[1] + truss_res_dir;
    std::string core_single_result_dir_ = basedir + "_single" + argv[1] + core_res_dir;
    std::string degree_single_result_dir_ = basedir + "_single" + argv[1] + degree_res_dir;
    std::string triangle_single_result_dir_ = basedir + "_single" + argv[1] + triangle_res_dir;
    std::string dt_single_result_dir_ = basedir + "_single" + argv[1] + dt_res_dir;
    std::string trineplus_single_result_dir_ = basedir + "_single" + argv[1] + trineplus_res_dir;

    {
        FILE* file = fopen(trine_single_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = trine1.TrinessOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int j = 0;
        int siz_ = checkin_sum_.size();
        for (int i = 0; i < siz_; i++){
            if (ness_cnt_[i] <= 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", j++, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(truss_single_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = truss1.NodetrussOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 0; i < siz_; i++){
            if (ness_cnt_[i] <= 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(core_single_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = core1.CorenessOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 0; i < siz_; i++){
            if (ness_cnt_[i] <= 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(degree_single_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = core1.DegreeOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 0; i < siz_; i++){
            if (ness_cnt_[i] <= 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(triangle_single_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = trine1.TriangleOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 0; i < siz_; i++){
            if (ness_cnt_[i] <= 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(dt_single_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = trine1.TriangleOut(i) + core1.DegreeOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 0; i < siz_; i++){
            if (ness_cnt_[i] <= 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(trineplus_single_result_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = trineplus1.TrineplusnessOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int siz_ = checkin_sum_.size();
        for (int i = 0; i < siz_; i++){
            if (ness_cnt_[i] <= 0)  continue;
            fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }


    std::string trine_merge_dir_ = basedir + argv[1] + "_merge" + trine_res_dir;
    std::string truss_merge_dir_ = basedir + argv[1] + "_merge" + truss_res_dir;
    std::string core_merge_dir_ = basedir + argv[1] + "_merge" + core_res_dir;
    std::string degree_merge_dir_ = basedir + argv[1] + "_merge" + degree_res_dir;
    uint32_t max_coreness_ = 0, max_triness_ = 0, max_trussness_ = 0, max_degree_ = 0;
    for (int i = 0; i < n; i++) {
        uint32_t k;
        k = trine1.TrinessOut(i);
        if (k > max_triness_)
            max_triness_ = k;
        k = truss1.NodetrussOut(i);
        if (k > max_trussness_)
            max_trussness_ = k;
        k = core1.CorenessOut(i);
        if (k > max_coreness_)
            max_coreness_ = k;
        k = core1.DegreeOut(i);
        if (k > max_degree_)
            max_degree_ = k;
    }
    {
        //core + trine
        std::string core_trine_dir_ = basedir + argv[1] + "core_trine.txt";
        FILE* file = fopen(core_trine_dir_.c_str(), "w");
        std::vector<std::vector<uint32_t > > checkin_sum_(max_coreness_ + 1, std::vector<uint32_t > (max_triness_ + 1, 0));
        std::vector<std::vector<uint32_t > > ness_cnt_(max_coreness_ + 1, std::vector<uint32_t > (max_triness_ + 1, 0));
        for (int i = 0; i < n; i++) {
            uint32_t k1 = core1.CorenessOut(i);
            uint32_t k2 = trine1.TrinessOut(i);
            ness_cnt_[k1][k2] ++;
            checkin_sum_[k1][k2] += countt_[i];
        }
        int idx = 0;
        for (int i = 0; i < max_coreness_; i++) {
            int tot_sum = 0;
            for (int j = 0; j < max_triness_; j++) {
                tot_sum += ness_cnt_[i][j];
            }
            int limit = tot_sum/5;
            int checkin_ssum_ = 0, ness_ccnt_ = 0;
            for (int j = 0; j < max_triness_; j++) {
                checkin_ssum_ += checkin_sum_[i][j];
                ness_ccnt_ += ness_cnt_[i][j];
                if (ness_ccnt_ > limit) {
                    fprintf(file, "%d %f\n", idx ++, 1.0 * checkin_ssum_ / ness_ccnt_);
                    checkin_ssum_ = ness_ccnt_ = 0;
                }
            }
            if (ness_ccnt_ > limit) {
                fprintf(file, "%d %f\n", idx ++, 1.0 * checkin_ssum_ / ness_ccnt_);
                checkin_ssum_ = ness_ccnt_ = 0;
            }
        }
        fclose(file);
    }


    int min_of_max = std::min(std::min(max_degree_, max_coreness_), std::min(max_trussness_, max_triness_));
    {
        FILE* file = fopen(trine_merge_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = trine1.TrinessOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int z = n/1000;
        int siz_ = checkin_sum_.size();
        int necc_sum = 0, checkin_ssum = 0, j = 0;
        for (int i = 0; i < siz_; i++){
            necc_sum += ness_cnt_[i];
            checkin_ssum += checkin_sum_[i];
            if (necc_sum >= z || i + 1 == siz_) {
                fprintf(file, "%" SCNu32 " %lf\n", j++, 1.0 * checkin_ssum /  necc_sum);
                necc_sum = 0, checkin_ssum = 0;
            }
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    // {
    //     FILE* file = fopen(trine_merge_dir_.c_str(), "w");
    //     std::vector<uint32_t> checkin_sum_(min_of_max + 1);
    //     std::vector<uint32_t> ness_cnt_(min_of_max + 1);
    //     for (int i = 0; i < n; i++) {
    //         uint32_t k = trine1.TrinessOut(i);
    //         uint32_t sizl = (1 + max_triness_) / (1 + min_of_max), lar_cnt = (1 + max_triness_) % (1 + min_of_max);
    //         uint32_t K = k / sizl <= (min_of_max - lar_cnt) ? k / sizl : (k - sizl * (min_of_max - lar_cnt + 1)) / (sizl + 1) + (min_of_max - lar_cnt + 1);
    //         ness_cnt_[K]++;
    //         checkin_sum_[K] += countt_[i];
    //     }
    //     int j = 0;
    //     for (int i = 0; i < checkin_sum_.size(); i++){
    //         if (ness_cnt_[i] <= 10)  continue;
    //         fprintf(file, "%" SCNu32 " %lf\n",j++ , 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
    //         // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
    //     }
    //     fclose(file);
    // }
    {
        FILE* file = fopen(truss_merge_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = truss1.NodetrussOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int z = n/1000;
        int siz_ = checkin_sum_.size();
        int necc_sum = 0, checkin_ssum = 0, j = 0;
        for (int i = 0; i < siz_; i++){
            necc_sum += ness_cnt_[i];
            checkin_ssum += checkin_sum_[i];
            if (necc_sum >= z || i + 1 == siz_) {
                fprintf(file, "%" SCNu32 " %lf\n", j++, 1.0 * checkin_ssum /  necc_sum);
                necc_sum = 0, checkin_ssum = 0;
            }
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    // {
    //     FILE* file = fopen(truss_merge_dir_.c_str(), "w");
    //     std::vector<uint32_t> checkin_sum_(min_of_max + 1);
    //     std::vector<uint32_t> ness_cnt_(min_of_max + 1);
    //     for (int i = 0; i < n; i++) {
    //         uint32_t k = truss1.NodetrussOut(i);
    //         uint32_t sizl = (1 + max_trussness_) / (1 + min_of_max), lar_cnt = (1 + max_trussness_) % (1 + min_of_max);
    //         uint32_t K = k / sizl <= (min_of_max - lar_cnt) ? k / sizl : (k - sizl * (min_of_max - lar_cnt + 1)) / (sizl + 1) + (min_of_max - lar_cnt + 1);
    //         ness_cnt_[K]++;
    //         checkin_sum_[K] += countt_[i];
    //     }
    //     for (int i = 0; i < checkin_sum_.size(); i++){
    //         if (ness_cnt_[i] == 0)  continue;
    //         fprintf(file, "%" SCNu32 " %lf\n",i , 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
    //         // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
    //     }
    //     fclose(file);
    // }
    {
        FILE* file = fopen(core_merge_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = core1.CorenessOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int z = n/1000;
        int siz_ = checkin_sum_.size();
        int necc_sum = 0, checkin_ssum = 0, j = 0;
        for (int i = 0; i < siz_; i++){
            necc_sum += ness_cnt_[i];
            checkin_ssum += checkin_sum_[i];
            if (necc_sum >= z || i + 1 == siz_) {
                fprintf(file, "%" SCNu32 " %lf\n", j++, 1.0 * checkin_ssum /  necc_sum);
                necc_sum = 0, checkin_ssum = 0;
            }
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    // {
    //     FILE* file = fopen(core_merge_dir_.c_str(), "w");
    //     std::vector<uint32_t> checkin_sum_(min_of_max + 1);
    //     std::vector<uint32_t> ness_cnt_(min_of_max + 1);
    //     for (int i = 0; i < n; i++) {
    //         uint32_t k = core1.CorenessOut(i);
    //         uint32_t sizl = (1 + max_coreness_) / (1 + min_of_max), lar_cnt = (1 + max_coreness_) % (1 + min_of_max);
    //         uint32_t K = k / sizl <= (min_of_max - lar_cnt) ? k / sizl : (k - sizl * (min_of_max - lar_cnt + 1)) / (sizl + 1) + (min_of_max - lar_cnt + 1);
    //         ness_cnt_[K]++;
    //         checkin_sum_[K] += countt_[i];
    //     }
    //     for (int i = 0; i < checkin_sum_.size(); i++){
    //         if (ness_cnt_[i] == 0)  continue;
    //         fprintf(file, "%" SCNu32 " %lf\n",i , 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
    //         // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
    //     }
    //     fclose(file);
    // }
    {
        FILE* file = fopen(degree_merge_dir_.c_str(), "w");
        std::vector<uint32_t> checkin_sum_;
        std::vector<uint32_t> ness_cnt_;
        for (int i = 0; i < n; i++) {
            uint32_t k = core1.DegreeOut(i);
            while (checkin_sum_.size() <= k) {
                checkin_sum_.push_back(0);
                ness_cnt_.push_back(0);
            }
            ness_cnt_[k]++;
            checkin_sum_[k] += countt_[i];
        }
        int z = n/1000;
        int siz_ = checkin_sum_.size();
        int necc_sum = 0, checkin_ssum = 0, j = 0;
        for (int i = 0; i < siz_; i++){
            necc_sum += ness_cnt_[i];
            checkin_ssum += checkin_sum_[i];
            if (necc_sum >= z || i + 1 == siz_) {
                fprintf(file, "%" SCNu32 " %lf\n", j++, 1.0 * checkin_ssum /  necc_sum);
                necc_sum = 0, checkin_ssum = 0;
            }
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    // {
    //     FILE* file = fopen(degree_merge_dir_.c_str(), "w");
    //     std::vector<uint32_t> checkin_sum_(min_of_max + 1);
    //     std::vector<uint32_t> ness_cnt_(min_of_max + 1);
    //     for (int i = 0; i < n; i++) {
    //         uint32_t k = core1.DegreeOut(i);
    //         uint32_t sizl = (1 + max_degree_) / (1 + min_of_max), lar_cnt = (1 + max_degree_) % (1 + min_of_max);
    //         uint32_t K = k / sizl <= (min_of_max - lar_cnt) ? k / sizl : (k - sizl * (min_of_max - lar_cnt + 1)) / (sizl + 1) + (min_of_max - lar_cnt + 1);
    //         ness_cnt_[K]++;
    //         checkin_sum_[K] += countt_[i];
    //     }
    //     for (int i = 0; i < checkin_sum_.size(); i++){
    //         if (ness_cnt_[i] == 0)  continue;
    //         fprintf(file, "%" SCNu32 " %lf\n",i , 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
    //         // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
    //     }
    //     fclose(file);
    // }

    std::string onep_trine_result_dir_ = basedir + "onep_" + argv[1] + trine_res_dir;
    std::string onep_truss_result_dir_ = basedir + "onep_" + argv[1] + truss_res_dir;
    std::string onep_core_result_dir_ = basedir + "onep_" + argv[1] + core_res_dir;
    std::string onep_degree_result_dir_ = basedir + "onep_" + argv[1] + degree_res_dir;
    {
        FILE* file = fopen(onep_trine_result_dir_.c_str(), "w");
        for (int i = 0; i < n; i++){
            fprintf(file, "%" SCNu32 " %d\n", trine1.TrinessOut(i), countt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(onep_truss_result_dir_.c_str(), "w");
        for (int i = 0; i < n; i++){
            fprintf(file, "%" SCNu32 " %d\n", truss1.NodetrussOut(i), countt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(onep_core_result_dir_.c_str(), "w");
        for (int i = 0; i < n; i++){
            fprintf(file, "%" SCNu32 " %d\n", core1.CorenessOut(i), countt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
    {
        FILE* file = fopen(onep_degree_result_dir_.c_str(), "w");
        for (int i = 0; i < n; i++){
            fprintf(file, "%" SCNu32 " %d\n", core1.DegreeOut(i), countt_[i]);
            // fprintf(file, "%" SCNu32 " %lf\n", i, 1.0 * checkin_sum_[i] /  ness_cnt_[i]);
        }
        fclose(file);
    }
}