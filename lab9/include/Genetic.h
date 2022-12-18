#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <map>
#include <deque>
#include <math.h>

using namespace std;

struct individual {
    int fitness;
    vector<int> genome;
};

/// Function to return a valid GENOME
/// required to create the population
vector<int> create_genome(map<int, map<int, int>>& vertexes_list, int starter_vertex) {
    vector<int> genome{ starter_vertex };
    while (true) {
        if (genome.size() == vertexes_list.size()) {
            genome.push_back(starter_vertex);
            break;
        }
        int temp;
        do
        {
            temp = rand() % vertexes_list.size();
        } while (temp == starter_vertex);
        if (find(genome.begin(), genome.end(), temp) == genome.end()) {
            genome.push_back(temp);
        }
    }
    return genome;
}

/// Function to return the fitness value of a gnome.
/// The fitness value is the path length
/// of the path represented by the GNOME.
int cal_fitness(map<int, map<int, int>>& vertexes_list, vector<int>& genome) {
    int f = 0;
    for (int i = 0; i < genome.size() - 1; i++) {
        for (auto vertex : vertexes_list[i + 1]) {
            if (vertex.first == i + 2) {
                f += vertexes_list[i + 1][i + 2];
            }
        }
    }
    return f;
}

/// Function to return a mutated GNOME
/// Mutated GNOME is a string
/// with a random interchange
/// of two genes to create variation in species
vector<int> mutatedGene(map<int, map<int, int>>& vertexes_list, vector<int> genome, int starter_vertex) {
    while (true) {
        int r;
        do
            r = rand() % vertexes_list.size();
        while (r == starter_vertex);
        int r1;
        do
            r1 = rand() % vertexes_list.size();
        while (r1 == starter_vertex);
        if (r1 != r)
        {
            std::swap(genome[r], genome[r1]);
            break;
        }
    }
    return genome;
}

std::vector<std::pair<int, std::vector<individual>>> genetic_algorithm(map<int, map<int, int>>& vertexes_list) {
    std::vector<std::pair<int, std::vector<individual>>> states;
    /// Generation Number
    int gen = 1;
    /// Number of Gene Iterations
    int gen_thres = 100;
    vector<individual> population;

    individual temp;
    ///√енерирование начальной попул€ции
    int population_size = 10;
    int starter_vertex = rand() % vertexes_list.size();
    for (int i = 0; i < population_size; i++)
    {
        temp.genome = create_genome(vertexes_list, starter_vertex);
        temp.fitness = cal_fitness(vertexes_list, temp.genome);
        population.push_back(temp);
    }



    int temperature = 10000;
    states.emplace_back(temperature, population);
    /// Iteration to perform
    /// population crossing and gene mutation.
    while (temperature > 1000 && gen <= gen_thres)
    {
        sort(population.begin(), population.end(), [](individual& a, individual& b){return a.fitness < b.fitness;});

        vector<individual> new_population;
        for (int i = 0; i < population_size; i++)
        {
            individual p1 = population[i];

            while (true) {
                vector<int> new_g = mutatedGene(vertexes_list, p1.genome, starter_vertex);
                individual new_genome;
                new_genome.genome = new_g;
                new_genome.fitness = cal_fitness(vertexes_list, new_genome.genome);

                if (new_genome.fitness <= population[i].fitness)
                {
                    new_population.push_back(new_genome);
                    break;
                }
                else
                {
                    /// Accepting the rejected children at
                    /// a possible probablity above threshold.
                    float prob = pow(2.7, -1 * ((float)(new_genome.fitness - population[i].fitness / temperature)));
                    if (prob > 0.5)
                    {
                        new_population.push_back(new_genome);
                        break;
                    }
                }
            }
        }
        states.emplace_back(temperature, new_population);
        temperature = (90 * temperature) / 100;
        population = new_population;
        gen++;
    }
    return states;
}