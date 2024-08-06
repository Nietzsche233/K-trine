#include <cstring>
#include <string>
const std::string root_dir = "../../../../Data/";

const std::string email = "email-Enron.txt";
const std::string dblp = "com-dblp.ungraph.txt";
const std::string dataset(std::string p){
    return root_dir + p;
}