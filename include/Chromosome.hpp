

#pragma once

#include <vector>
#include "Nucleotide.hpp"

#include "Gene.hpp"

class Chromosome {
public:
    std::vector<Gene> genes;


    Chromosome(const std::vector<double> &V);

    Chromosome(const std::vector<Nucleotide> &V);;

    std::vector<double> get_vect() const {
        std::vector<double> out;
        for (const auto &gene : genes) {
            out.push_back(gene.get_value());
        }
        return out;
    }

    std::vector<Nucleotide> get_genome() const;

    void print() const;

    bool operator==(const Chromosome &a) const;
};

