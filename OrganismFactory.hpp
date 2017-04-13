//
// Created by dumitru on 3/17/17.
//
#pragma once

#include <utility>
#include <vector>
#include <random>
#include <assert.h>
#include "Organism.hpp"

class OrganismFactory {
public:
    const std::vector<std::pair<double, double>> Limits;
    const double MutationRate;
    const size_t Dimension, CrossLoci;
    const size_t acc;

    mutable std::mt19937 generator;

    OrganismFactory(const std::vector<std::pair<double, double>> &Limits,
                    const double MutationRate, const size_t CrossLoci,
                    const size_t Dimenstion, size_t acc);

    virtual Organism generate() const;

    virtual std::pair<Organism, Organism> crossover(const Organism &parent1,
                                      const Organism &parent2) const;;

    virtual double normalize(double x, double L, double R) const;

    virtual std::vector<double> normalize(const std::vector<double>& V) const;


    virtual double compute_fitness(const Organism org) const = 0;

};

OrganismFactory::OrganismFactory(const std::vector<std::pair<double, double>> &Limits, const double MutationRate,
                                    const size_t CrossLoci, const size_t Dimenstion, size_t acc) :
        Limits(Limits),
        MutationRate(MutationRate),
        CrossLoci(CrossLoci),
        Dimension(Dimenstion),
        acc(acc) {

    assert(Dimension == Limits.size());
    assert(CrossLoci % 2 == 0);
}

double OrganismFactory::normalize(double x, double L, double R) const {
    return L + x*(R-L);
}

std::vector<double> OrganismFactory::normalize(const std::vector<double>& V) const {
    std::vector<double> out(V.size());
    for(size_t i=0; i<V.size(); i++)
        out.at(i) = Limits.at(i).first + V.at(i) * (Limits.at(i).second - Limits.at(i).first);
    return out;
};


Organism OrganismFactory::generate() const {
    std::vector<double> V(Dimension);
    size_t idx = 0;

    for (auto& val: V) {
        auto L = Limits.at(idx).first;
        auto R = Limits.at(idx).second;

        std::uniform_real_distribution<double> value_dist{L, R};
        val = value_dist(generator);
        val = (val - L)/(R-L);
        idx++;
    }

    Organism o{Chromosome(V, acc)};
    o.set_fitness(compute_fitness(o));
    return o;
}

std::pair<Organism, Organism> OrganismFactory::crossover(const Organism &parent1, const Organism &parent2) const {
    std::vector<bool> a(parent1.DNA.get_genome());
    std::vector<bool> b(parent2.DNA.get_genome());
    std::vector<size_t> loci(CrossLoci);

    std::uniform_int_distribution<size_t> nucleotide_index_dist{0,parent2.DNA.get_genome().size() -1};
    std::uniform_real_distribution<double> mutation_dist{0, 1};

    for(auto& locus : loci)
        locus = nucleotide_index_dist(generator);
    loci.at(0) = 0;
    for(size_t i = 0; i < CrossLoci - 1; i+=2)
        for(size_t j = loci.at(i); j<loci.at(i+1); j++)
            std::swap(a.at(j), b.at(j));

    //mutation_dist(generator) < 0.5 returns bit index [0,1]
    if(mutation_dist(generator) > MutationRate)
        a.at(nucleotide_index_dist(generator)) = mutation_dist(generator) < 0.5;

    if(mutation_dist(generator) > MutationRate)
        b.at(nucleotide_index_dist(generator)) = mutation_dist(generator) < 0.5;

    Organism child1{ Chromosome(a, acc) };
    Organism child2{ Chromosome(b, acc) };

    child1.set_fitness(compute_fitness(child1));
    child2.set_fitness(compute_fitness(child2));


    return std::make_pair(child1,child2);
}


