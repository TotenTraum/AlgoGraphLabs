#include <functional>
#include <filesystem>
#include "View2.h"
#include "View3.h"
#include "View4.h"
#include "View2-Linked.h"

std::string src_dir = "lab1-";

void gen_Vn_Ek(int& n, int& k,const std::function<unsigned int()>& func)
{
    n = 20 + (func() % 11);
    n = 5;
    int max = n*(n-1) / 2;
    k = (n - 1) + func() % (max - (n - 1) + 1);
}

int main() {
    std::mt19937_64 generator(std::chrono::system_clock::now().time_since_epoch().count());
    int n,k;
    std::ofstream file;

    gen_Vn_Ek(n,k, [&generator](){return generator();});

    file.open(src_dir + "Data.txt");
    file << n << " " << k;
    file.close();

    View2_Linked matrix(n,k,[&generator](){return generator();});

    file.open(src_dir + "View2.csv");
    file << matrix.to_string();
    file.close();

    file.open(src_dir + "View2-Degree.txt");
    file << matrix.degree();
    file.close();

    View4 mtx(matrix);
    file.open(src_dir + "View4.csv");
    file << mtx.to_string();
    file.close();

    file.open(src_dir + "View4-Degree.txt");
    file << mtx.degree();
    file.close();

    View3 list(mtx);
    file.open(src_dir + "View3.txt");
    file << list.to_string();
    file.close();


    file.open(src_dir + "View3-Degree.txt");
    file << list.degree();
    file.close();

    std::system(("start " +std::filesystem::current_path().string()).c_str());
    return 0;
}
