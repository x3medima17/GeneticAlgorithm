//
// Created by dumitru on 3/19/17.
//


#include <utility>
#include <vector>
#include "OrganismFactory.hpp"

template<class T>
OrganismFactory<T>::OrganismFactory(const std::vector<std::pair<double, double>> &Limits, const double MutationRate,
                                 const size_t CrossLoci, const size_t Dimenstion) :
        Limits(Limits),
        MutationRate(MutationRate),
        CrossLoci(CrossLoci),
        Dimension(Dimenstion) {

    assert(Dimension == Limits.size());
    assert(CrossLoci % 2 == 0);
}

template <class Organism>
Organism OrganismFactory<Organism>::generate() const {
    std::vector<double> curr(Dimension);
    size_t idx = 0;
    std::uniform_real_distribution<double> value_dist{Limits.at(idx).first,
                                                      Limits.at(idx).second};
    for (auto &val : curr) {
        idx++;
        val = value_dist(generator);
    }
    Organism o(curr);
    o.set_fitness(o.compute_fitness(Limits));
    return o;
}

template<class Organism>
std::pair<Organism, Organism> OrganismFactory<Organism>::crossover(const Organism &parent1, const Organism &parent2) const {

    auto res = Organism::cross_over(parent1, parent2, CrossLoci, MutationRate, generator);
    res.first.set_fitness(compute_fitness(res.first));
    res.second.set_fitness(compute_fitness(res.second));
    return res;
}


template<class Organism>
double OrganismFactory<Organism>::compute_fitness(const Organism &org) const {
    return org.compute_fitness(Limits);
}

