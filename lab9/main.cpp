#include "Generation.h"
#include <random>
#include <chrono>

void gen_Vn_Ek(int& n, int& k, const std::function<unsigned int()>& func)
{
    n = 20 + (func() % 11);
    n = 4;
    int max = n * (n-1) /2;
    do
        k =  func() % (max + 1);
    while (k < n - 1);
}

void print(ofstream& stream, vector<int>& v)
{
    for (auto item: v)
        stream << item << ";";
    stream << "\n";
}

void task(ofstream& stream)
{
    DynamicAdjMtx dynamicAdjMtx;
    DynamicVertList dynamicVertList;
    BranchAndBoundAdjMtx branchAndBoundAdjMtx;
    BranchAndBoundVertList branchAndBoundVertList;
    std::mt19937_64 generator(std::chrono::system_clock::now().time_since_epoch().count());

    int vertexes_number;
    //cin >> vertexes_number;
    //int edges_number_max = vertexes_number * (vertexes_number - 1) / 2;
    //int edges_number_min = vertexes_number - 1;
    int edges_number;
    gen_Vn_Ek(vertexes_number, edges_number, [&generator](){return generator();});
    // = edges_number_min + rand() % (edges_number_max - edges_number_min + 1);
    vector<vector<int>> adjacency_matrix;
    vector<vector<int>> prev_adjacency_matrix;
    vector<int> path;
    int start, end;
    bool cannotFindPath;
    do
    {
        cannotFindPath = false;
        prev_adjacency_matrix = adjacency_matrix_new(vertexes_number, edges_number);

        adjacency_matrix = hamiltonian_graph_new(prev_adjacency_matrix, edges_number);
        try
        {
            std::tie(start, end, path) = hamiltonian_path(adjacency_matrix);
        } catch (std::string &str)
        {
            cannotFindPath = true;
        }
    }
    while(cannotFindPath == true);

    stream << "Матрица смежности;\n";
    print(stream ,prev_adjacency_matrix);
    stream << "\n";

    stream << "Гамильтонов цикл;\n";
    print(stream ,adjacency_matrix);
    stream << "\n";

    stream << "Гамильтонов путь;\n";
    stream << "Начало пути;" << start <<";\n" ;
    stream << "Конец пути;" << end <<";\n" ;
    stream << "Путь;";
    print(stream, path);
    stream << "\n\n";

    auto cycle = hamiltonian_cycle(adjacency_matrix);
    stream << "Гамильтонов цикл;";
    print(stream, cycle);
    stream << "\n";

    fill_weights(adjacency_matrix);
    stream << "Матрица весов;\n";
    print_weight(stream, adjacency_matrix);
    stream << "\n";

    auto dynMtxRes = dynamicAdjMtx.Calc(adjacency_matrix);
    stream << "Гамильтонов цикл(dyn);";
    print(stream, dynMtxRes);
    stream << "\n";
    auto babMtxRes = branchAndBoundAdjMtx.Calc(adjacency_matrix);
    stream << "Гамильтонов цикл(bab);";
    print(stream, babMtxRes);
    stream << "\n";

    map<int, map<int, int>> vertexes_list = convert_to_vertexes_list(adjacency_matrix);
    stream << "Список смежности;\n";
    for (auto& list: vertexes_list)
    {
        stream << list.first - 1 << ";";
        for (auto& item: list.second)
            stream << "(" << item.first - 1 << "/" << item.second << ");";
        stream << std::endl;
    }
    stream << std::endl;

    auto dynVertList = dynamicVertList.Calc(vertexes_list);
    stream << "Гамильтонов цикл(dyn);";
    print(stream, dynVertList);
    stream << "\n";
    auto babVert = branchAndBoundVertList.Calc(vertexes_list);
    stream << "Гамильтонов цикл(bab);";
    print(stream, babVert);
    stream << "\n";
    auto gen = genetic_algorithm(vertexes_list);
    stream << "Генетический алгоритм;\n";
    for (int i = 0; i < gen.size(); i++)
    {
        stream << "Поколение " << i << ";Temperature:" << gen[i].first << ";\n";
        for (auto &individual: gen[i].second) {
            for (auto &genome: individual.genome)
                stream << genome;
            stream << ";" << individual.fitness << ";\n";
        }
    }
    stream << "\n";
}

int main()
{
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ofstream stream("lab9.csv");
    task(stream);
    stream.close();
}
