

#pragma once

#include <vector>
#include "Nucleotide.hpp"

#include "Gene.hpp"

class Chromosome {
public:
    std::vector<Gene> genes;
    size_t acc;

    Chromosome(const std::vector<double> &V, size_t acc);

    Chromosome(const std::vector<bool> &V, size_t acc);

    std::vector<double> get_vect() const {
        std::vector<double> out;
        for (const auto &gene : genes) {
            out.push_back(gene.get_value());
        }
        return out;
    }

    std::vector<bool> get_genome() const;

    void print() const;

    bool operator==(const Chromosome &a) const;
};


Chromosome::Chromosome(const std::vector<double> &V, size_t acc): acc{acc} {
    for (const auto &item : V)
        genes.push_back(Gene(item, acc));
}

Chromosome::Chromosome(const std::vector<bool> &V, size_t acc) : acc{acc}  {
    assert(acc > 0);
    assert(V.size() % acc == 0);

    std::vector<bool> curr_gene;
    int k = 0;
    for(const auto& nuc : V)
    {
        curr_gene.push_back(nuc);
        k++;
        if(k == acc)
        {
            k = 0;
            genes.push_back(Gene(curr_gene));
            curr_gene.clear();
        }
    }
}

std::vector<bool> Chromosome::get_genome() const {
    std::vector<bool> out;
    for(const auto& gene : genes)
        out.insert(out.end(), gene.nucleotides.begin(), gene.nucleotides.end());
    return out;
}

void Chromosome::print() const {
    for(const auto& gene: genes)
        gene.print();

    std::cout<<"____________________"<<std::endl;
}

bool Chromosome::operator==(const Chromosome &a) const {
    return (this->genes == a.genes);
}
