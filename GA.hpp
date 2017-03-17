//
// Created by dumitru on 3/14/17.
//

#pragma once

#include <random>
#include <algorithm>
#include <memory>
#include "Organism.hpp"
#include "OrganismFactory.hpp"

template<class Organism>
class GA {
private:
    const size_t Generations, PopulationSize, Crossovers;

    mutable std::mt19937 generator{10};
    mutable std::uniform_int_distribution<size_t> organism_index_dist;

    const std::shared_ptr<OrganismFactory<Organism>> organism_factory;

public:
    GA(const size_t PopulationSize, const size_t Generations, const size_t Crossovers,
       std::shared_ptr<OrganismFactory<Organism>> organism_factory):
            Generations(Generations),
            PopulationSize(PopulationSize),
            Crossovers(Crossovers),
            organism_index_dist(0, PopulationSize - 1),
            organism_factory(organism_factory) {
    }

    std::vector<Organism> generate_population() const {
        std::vector<Organism> Population;
        for (auto i = 0; i < PopulationSize; i++)
            Population.push_back(organism_factory->generate());

        return Population;
    }

    void run() const {
        auto Population = generate_population();
        for (int i = 0; i < Generations; i++) {
            Population = next_generation(Population);
            std::cout << Population.front().fitness << std::endl;
        }
    }

    std::vector<Organism> next_generation(std::vector<Organism> V) const {
        // Cross
        for (int i = 0; i < Crossovers; i++) {
            auto parent1 = V.at(organism_index_dist(generator));
            auto parent2 = V.at(organism_index_dist(generator));

            std::pair<Organism, Organism> res = organism_factory->crossover(parent1,parent2);

            V.push_back(res.first);
            V.push_back(res.second);
        }
        //Sort
        std::sort(V.begin(), V.end(),
                  [](const Organism &a, const Organism &b) {
                      return a.fitness < b.fitness;
                  });
        return clean(V);
    }


    std::vector<Organism> clean(std::vector<Organism> V) const {
        for (auto it = V.begin(); it != V.end(); it++)
            if (std::isnan((*it).fitness) || std::isinf((*it).fitness)) {
                V.erase(it);
                it = V.begin();
            }
        while (V.size() > PopulationSize)
            V.pop_back();
        return V;
    }

};

