//
// Created by dumitru on 3/14/17.
//

#pragma once

#include <random>
#include <algorithm>
#include <memory>
#include "Organism.hpp"

template <class Organism>
class GA {
private:
    const size_t Generations, PopulationSize, Crossovers, Dimension;
    const double MutationRate;

    std::vector<std::pair<double, double> > Limits;

    const size_t CrossLoci = 32;

    mutable std::mt19937 generator{10};
    mutable std::uniform_real_distribution<double> value_dist{-50, 50};
    mutable std::uniform_int_distribution<size_t> organism_index_dist;

    mutable std::uniform_int_distribution<uint8_t> bit_dist{0, 63};

public:
    GA(const size_t Dimension, const size_t PopulationSize, const size_t Generations,
       const size_t Crossovers, const double MutationRate,
       const std::vector<std::pair<double, double>>& Limits):
            Generations(Generations),
            PopulationSize(PopulationSize),
            Crossovers(Crossovers),
            MutationRate(MutationRate),
            organism_index_dist(0, PopulationSize - 1),
            Dimension(Dimension),
            Limits(Limits)
    {
        assert(Dimension == Limits.size());
        assert(CrossLoci%2 == 0);
    }

    std::vector<Organism> generate_population() const {
        std::vector<Organism> Population;
        for(auto i=0; i<PopulationSize; i++) {
            std::vector<double> curr(Dimension);
            size_t idx = 0;
            for (auto &val : curr) {
                std::uniform_real_distribution<double> value_dist{Limits.at(idx).first,
                                                                  Limits.at(idx).second};
                idx++;
                val = value_dist(generator);
            }
            Organism o(curr);
            o.compute_fitness(Limits);
            Population.push_back(o);
        }
        return Population;
    }

    void run() const{
        auto Population = generate_population();
        for(int i = 0; i<Generations; i++){
            Population = next_generation(Population);
            std::cout<<Population.front().fitness<<std::endl;
        }
    }

    std::vector<Organism> next_generation(std::vector<Organism> V) const {
        // Cross
        for(int i = 0; i < Crossovers; i++){
            auto parent1 = V.at(organism_index_dist(generator));
            auto parent2 = V.at(organism_index_dist(generator));

            std::pair<Organism, Organism> res =
                    Organism::cross_over(parent1,parent2, CrossLoci, MutationRate, generator);
            res.first.compute_fitness(Limits);
            res.second.compute_fitness(Limits);

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
        for(auto it = V.begin(); it != V.end(); it++)
            if(std::isnan((*it).fitness) || std::isinf((*it).fitness))
            {
                V.erase(it);
                it = V.begin();
            }
        while (V.size() > PopulationSize)
            V.pop_back();
        return V;
    }

};

