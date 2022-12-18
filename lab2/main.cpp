#include <functional>
#include <memory>
#include "View2.h"
#include "View4.h"
#include "View2-Linked.h"
#include "algorithms.h"

std::string src_dir = "lab2-";

void gen_Vn_Ek(int& n, int& k, const std::function<unsigned int()>& func)
{
    n = 20 + (func() % 11);
    n = 5;
    int max = n*(n-1) / 2;
    do
    {
        k =  func() % (max + 1);
    } while (k < n - 1);
}

void printDFS(std::ofstream& stream, IView2& view)
{
    for(int i = 0; i < view.size(); i ++)
    {
        stream << "Обход в глубину для вершины " << i << " :" << std::endl;
        std::stringstream stringstream;
        auto all = DFS(view, i, [&stringstream](const std::string&val) { stringstream <<"(V" +  val + ")->"; });
        auto str = stringstream.str();
        str.pop_back();
        str.pop_back();
        stream << "\t" << str;
        stream << std::endl;

        int sum = 0;
        for(auto item : all)
            if(item)
                sum++;
        stream << "Было посещено " << sum << " из " << all.size();
        stringstream.str("");
        DFS(view, i, [&stringstream](const std::string&val) { stringstream <<val + " "; });
        std::vector<int> vSequence;
        while (stringstream.rdbuf()->in_avail() != std::streamsize(0))
        {
            int val;
            stringstream >> val;
            vSequence.push_back(val);
        }
        stream << std::endl;
        std::vector<int> visited;
        for(int i = 0; i < vSequence.size() - 1; i++)
        {
            stream << "V" << vSequence[i] << " : ";
            for(int j = 0; j < view.size(); j++ )
                if(view[vSequence[i]][j] == 1)
                {
                    auto it = std::find(visited.begin(), visited.end(),j);
                    if(it == visited.end())
                        stream << j << " ";
                    //else
                    //    stream <<"-"<< j << "- ";
                }
            visited.push_back(vSequence[i]);
            stream << std::endl;
        }
        stream << std::endl<<std::endl;
    }
}

void printBFS(std::ofstream& stream, View4& view, IView2& toShowWay)
{
    for(int i = 0; i < view.size(); i ++)
    {
        stream << "Обход в ширину для вершины " << i << " :" << std::endl;
        std::stringstream stringstream;
        auto all = BFS(view, i, [&stringstream](const std::string &val) { stringstream <<"" +  val + ", ";});
        auto str = stringstream.str();
        str.pop_back();
        str.pop_back();
        stream << "\t" << str;
        stream << std::endl;
        int sum = 0;
        for(auto item : all)
            if(item)
                sum++;
        stream << "Было посещено " << sum << " из " << all.size();
        stringstream.str("");
        BFS(view, i, [&stringstream](const std::string&val) { stringstream <<val + " "; });
        std::vector<int> vSequence;
        while (stringstream.rdbuf()->in_avail() != std::streamsize(0))
        {
            int val;
            stringstream >> val;
            vSequence.push_back(val);
        }
        stream << std::endl;
        std::vector<int> visited;
        visited.push_back(vSequence[0]);
        for(int i = 0; i < vSequence.size() - 1; i++)
        {
            stream << "V" << vSequence[i] << " ( ";
            std::vector<int> linked;
            for(int j = 0; j < toShowWay.size(); j++ )
                if(toShowWay[vSequence[i]][j] == 1)
                {
                    auto it = std::find(visited.begin(), visited.end(),j);
                    if(it == visited.end())
                        linked.push_back(j);
                    visited.push_back(j);
                }
            if(linked.size())
            {
                for(int i = 0; i < linked.size() - 1; i++)
                    stream << linked[i] << ", ";
                stream << linked[linked.size() - 1];
            }
            stream  << ")"<< std::endl;
        }
        stream << std::endl<<std::endl;
    }
}

int main() {
    std::mt19937_64 generator(std::chrono::system_clock::now().time_since_epoch().count());
    int n,k;
    std::ofstream file;

    gen_Vn_Ek(n,k, [&generator](){return generator();});

    std::unique_ptr<IView2> matrix = std::make_unique<View2>(n,k);

    file.open(src_dir + "Data.txt");
    file << n << " " << k;
    file.close();

    file.open(src_dir + "DFS.txt");
    printDFS(file,*matrix);
    file.close();

    View4 mtx(*matrix);
    file.open(src_dir + "View4.csv");
    file << mtx.to_string();
    file.close();

    file.open(src_dir + "BFS.txt");
    printBFS(file,mtx,*matrix);
    file.close();

    matrix.reset(new View2_Linked(n,k,[&generator](){return generator();}));

    file.open(src_dir + "linked-DFS.txt");
    printDFS(file,*matrix);
    file.close();

    file.open(src_dir + "linked-View2.csv");
    file << matrix->to_string();
    file.close();

    file.open(src_dir + "linked-View2-Degree.txt");
    file << matrix->degree();
    file.close();

    mtx = View4(*matrix);
    file.open(src_dir + "linked-View4.csv");
    file << mtx.to_string();
    file.close();

    file.open(src_dir + "linked-BFS.txt");
    printBFS(file,mtx,*matrix);
    file.close();

    file.open(src_dir + "linked-View4-Degree.txt");
    file << mtx.degree();
    file.close();
    return 0;
}
