//
// Created by dumitru on 3/17/17.
//
#pragma once

#include <utility>
#include <vector>
#include <random>
#include <assert.h>
#include "Organism.hpp"

template<class Organism>
class OrganismFactory {
public:
    const std::vector<std::pair<double, double>> Limits;
    const double MutationRate;
    const size_t Dimension, CrossLoci;

    mutable std::mt19937 generator;

    OrganismFactory(const std::vector<std::pair<double, double>> &Limits,
                    const double MutationRate, const size_t CrossLoci,
                    const size_t Dimenstion);

    virtual Organism generate() const;

    virtual std::pair<Organism, Organism> crossover(const Organism &parent1,
                                                    const Organism &parent2) const;;

    virtual double compute_fitness(const Organism& org) const;

};
