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

template<class Organism>
class GA {
private:
    const size_t Generations, PopulationSize, Crossovers;

    mutable std::mt19937 generator{10};
    mutable std::uniform_int_distribution<size_t> organism_index_dist;

    const std::shared_ptr<OrganismFactory<Organism>> organism_factory;

public:
    GA(const size_t PopulationSize, const size_t Generations, const size_t Crossovers,
       std::shared_ptr<OrganismFactory<Organism>> organism_factory);

    std::vector<Organism> generate_population() const;

    /**
     * Returns vector of [min mean max]
     */
    std::vector<std::vector<double>> run() const;

    std::vector<Organism> next_generation(std::vector<Organism> V) const;

    std::vector<Organism> clean(std::vector<Organism> V) const;

private:
    std::vector<double> get_stats(const std::vector<Organism>& P) const;

    std::vector<std::vector<double>> get_population_stats(std::vector<Organism> &P) const;


    };


template<class Organism>
std::vector<double> GA<Organism>::get_stats(const std::vector<Organism>& P) const {
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
    double sum = 0;
    for(const auto& org : P){
        auto curr = org.fitness;
        sum += curr;
        if(curr<min)
            min = curr;
        if(curr>max)
            max = curr;
    }
    double mean = sum / (double)P.size();
    return {min, mean, max};
}

template<class Organism>
GA<Organism>::GA(const size_t PopulationSize, const size_t Generations, const size_t Crossovers,
                 std::shared_ptr<OrganismFactory<Organism>> organism_factory) :
        Generations(Generations),
        PopulationSize(PopulationSize),
        Crossovers(Crossovers),
        organism_index_dist(0, PopulationSize - 1),
        organism_factory(organism_factory) {
}

template<class Organism>
std::vector<Organism> GA<Organism>::generate_population() const {
    std::vector<Organism> Population;
    for (auto i = 0; i < PopulationSize; i++)
        Population.push_back(organism_factory->generate());

    return Population;
}

template<class Organism>
std::vector<std::vector<double>>
GA<Organism>::get_population_stats(std::vector<Organism> &P) const {
    std::vector<std::vector<double>> out;
    for(const auto& org : P) {
        auto vec(org.DNA.get_vect());
        vec.push_back(org.fitness);
        out.push_back(vec);
    }
    return out;
}

template<class Organism>
std::vector<std::vector<double>> GA<Organism>::run()  const {
    std::vector<std::vector<double>> stats;
	std::ofstream fout("C:\\Users\\dumitru.savva\\Documents\\ga.out");


    auto Population = generate_population();
    for(const auto& org : Population) {
        std::vector<double> tmp {0};
        auto V(org.DNA.get_vect());
        tmp.insert(tmp.end(), V.begin(), V.end());
        tmp.push_back(org.fitness);
        stats.push_back(tmp);
    }

	std::ofstream sout("C:\\Users\\dumitru.savva\\Documents\\stats.out");
	for (const auto& gen : stats) {
		for (const auto& item : gen)
			sout << item << " ";
		sout << std::endl;
	}
    for (size_t i = 1; i <= Generations; i++) {
        Population = next_generation(Population);

        for(const auto& org : Population) {
            std::vector<double> tmp = { double(i)};
            auto V(org.DNA.get_vect());
            tmp.insert(tmp.end(), V.begin(), V.end());
            tmp.push_back(org.fitness);
            stats.push_back(tmp);
			for (const auto& item : tmp)
				sout << item << " ";
			sout << std::endl;
        }

        std::cout << Population.front().fitness << std::endl;
		fout << Population.front().fitness << std::endl;
    }
    return stats;

}

template<class Organism>
std::vector<Organism> GA<Organism>::next_generation(std::vector<Organism> V) const {
    // Cross

    for (int i = 0; i < Crossovers; i++) {
        auto parent1 = V.at(organism_index_dist(generator));
        auto parent2 = V.at(organism_index_dist(generator));

        std::pair<Organism, Organism> res = organism_factory->crossover(parent1, parent2);

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

template<class Organism>
std::vector<Organism> GA<Organism>::clean(std::vector<Organism> V) const {
    for (auto it = V.begin(); it != V.end(); it++)
        if (std::isnan((*it).fitness) || std::isinf((*it).fitness)) {
            V.erase(it);
            it = V.begin();
        }
    while (V.size() > PopulationSize)
        V.pop_back();
    return V;
}
