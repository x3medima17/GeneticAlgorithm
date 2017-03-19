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


    virtual double compute_fitness(const std::vector<std::pair<double, double>>& Limits) const;

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

Organism::Organism(const std::vector<double> V) :
        DNA(V)
{}

void Organism::set_fitness(double value) {
    fitness = value;
}

void Organism::print() const {
    std::cout << fitness << "\t";
    for (const auto &item: DNA.get_vect())
        std::cout << item << " ";
    std::cout << "\n";
}

bool Organism::operator==(const Organism &a) const {
    return (this->fitness == a.fitness && this->DNA == a.DNA);
}


double Organism::compute_fitness(const std::vector<std::pair<double, double>>& Limits) const {
    throw std::runtime_error("Not implemented");
}

template<class T>
std::pair<T, T>
Organism::cross_over(const T &parent1, const T &parent2, size_t CrossLoci, double MutationRate, std::mt19937 &generator) {
    std::vector<Nucleotide> a(parent1.DNA.get_genome());
    std::vector<Nucleotide> b(parent2.DNA.get_genome());
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
        a.at(nucleotide_index_dist(generator)).value.flip(mutation_dist(generator) < 0.5);

    if(mutation_dist(generator) > MutationRate)
        b.at(nucleotide_index_dist(generator)).value.flip(mutation_dist(generator) < 0.5);

    T child1(Chromosome(a).get_vect());
    T child2(Chromosome(b).get_vect());


    return std::make_pair(child1,child2);
}


