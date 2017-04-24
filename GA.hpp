//
// Created by dumitru on 3/14/17.
//

#pragma once

#include <random>
#include <algorithm>
#include <memory>
#include <fstream>

#include "Organism.hpp"
#include "OrganismFactory.hpp"


class GA {
private:
    const size_t Generations, PopulationSize, Crossovers;

    mutable std::mt19937 generator{10};
    mutable std::uniform_int_distribution<size_t> organism_index_dist;

    const std::shared_ptr<OrganismFactory> organism_factory;


public:
    using misc_map = std::map<std::string, double>;


    GA(const size_t PopulationSize, const size_t Generations, const size_t Crossovers,
       std::shared_ptr<OrganismFactory> organism_factory);

    std::vector<Organism> generate_population() const;

    /**
     * Returns vector of [min mean max]
     */
    std::vector<std::vector<double>> run(std::function<void(const std::vector<Organism>&)>) const;

    std::vector<Organism> next_generation(std::vector<Organism> V) const;

    std::vector<Organism> clean(std::vector<Organism> V) const;

private:
    std::vector<double> get_stats(const std::vector<Organism> &P) const;

    std::vector<std::vector<double>> get_population_stats(std::vector<Organism> &P) const;


};


std::vector<double> GA::get_stats(const std::vector<Organism> &P) const {
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
    double sum = 0;
    for (const auto &org : P) {
        auto curr = org.fitness;
        sum += curr;
        if (curr < min)
            min = curr;
        if (curr > max)
            max = curr;
    }
    double mean = sum / (double) P.size();
    return {min, mean, max};
}

GA::GA(const size_t PopulationSize, const size_t Generations, const size_t Crossovers,
       std::shared_ptr<OrganismFactory> organism_factory) :
        Generations(Generations),
        PopulationSize(PopulationSize),
        Crossovers(Crossovers),
        organism_index_dist(0, PopulationSize - 1),
        organism_factory(organism_factory) {



}

std::vector<Organism> GA::generate_population() const {
    std::vector<Organism> Population;
    for (auto i = 0; i < PopulationSize; i++)
        Population.push_back(organism_factory->generate());

    //Sort
    std::sort(Population.begin(), Population.end(),
              [](const Organism &a, const Organism &b) {
                  return a.fitness < b.fitness;
              });
    return Population;
}

std::vector<std::vector<double>>
GA::get_population_stats(std::vector<Organism> &P) const {
    std::vector<std::vector<double>> out;
    for (const auto &org : P) {
        auto vec(org.DNA.get_vect());
        vec.push_back(org.fitness);
        out.push_back(vec);
    }
    return out;
}

std::vector<std::vector<double>> GA::run(std::function<void(const std::vector<Organism>&)> func) const {
    std::vector<std::vector<double>> stats;


    auto Population = generate_population();
    func(Population);

    for (size_t i = 1; i <= Generations; i++) {
        Population = next_generation(Population);
        func(Population);
    }
    return stats;
}

std::vector<Organism> GA::next_generation(std::vector<Organism> V) const {
    std::vector<Organism> Buffer;

    //No fitness computed
    for (int i = 0; i < Crossovers; i++) {
        auto parent1 = V.at(organism_index_dist(generator));
        auto parent2 = V.at(organism_index_dist(generator));

        std::pair<Organism, Organism> res = organism_factory->crossover(parent1, parent2);

        Buffer.push_back(res.first);
        Buffer.push_back(res.second);
    }

    //Assign fitnesses
    for(auto& org : Buffer) {
        org.misc = organism_factory->compute_fitness(org);
        org.set_fitness(org.misc.find("delta")->second);
    }


    V.insert(V.end(), Buffer.begin(), Buffer.end());
    //Sort
    std::sort(V.begin(), V.end(),
              [](const Organism &a, const Organism &b) {
                  return a.fitness < b.fitness;
              });
    return clean(V);
}

std::vector<Organism> GA::clean(std::vector<Organism> V) const {
    for (auto it = V.begin(); it != V.end(); it++)
        if (std::isnan((*it).fitness) || std::isinf((*it).fitness)) {
            V.erase(it);
            it = V.begin();
        }
    while (V.size() > PopulationSize)
        V.pop_back();
    return V;
}

