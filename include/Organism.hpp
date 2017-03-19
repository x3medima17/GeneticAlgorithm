//
// Created by dumitru on 3/14/17.
//

#pragma once

#include "Chromosome.hpp"

class Organism {
public:
    double fitness = 0;

    Chromosome DNA;

    Organism(const std::vector<double> V);


    virtual double compute_fitness(const std::vector<std::pair<double, double>>& Limits) const = 0;

    virtual void set_fitness(double value);

    void print() const;


    bool operator==(const Organism &a) const;

    template <class T>
    static std::pair<T, T> cross_over(const T& parent1,
                                      const T& parent2,
                                      size_t CrossLoci,
                                      double MutationRate,
                                      std::mt19937& generator);;

};


//template <size_t Dimension>
