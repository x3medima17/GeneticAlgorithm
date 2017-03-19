//
// Created by dumitru on 3/19/17.
//

#include <vector>
#include <bit_ops.hpp>
#include "Chromosome.hpp"

Chromosome::Chromosome(const std::vector<double> &V) {
    for (const auto &item : V)
    {
        std::vector<Nucleotide> curr_gene(sizeof(double)*4);

        for (int i = 0; i < sizeof(double) * 8; i += 2) {
            auto n = Nucleotide(bit_ops::read_bit(item, i + 1), bit_ops::read_bit(item, i));
            curr_gene.at(curr_gene.size()-i/2-1) = n;
        }
        genes.push_back(Gene(curr_gene));
    }
    auto Vp = get_vect();
}

Chromosome::Chromosome(const std::vector<Nucleotide> &V) {
    std::vector<Nucleotide> curr_gene;
    int k = 0;
    for(const auto& nuc : V)
    {
        curr_gene.push_back(nuc);
        k++;
        if(k == 32)
        {
            k = 0;
            genes.push_back(Gene(curr_gene));
            curr_gene.clear();
        }
    }
}

std::vector<Nucleotide> Chromosome::get_genome() const {
    std::vector<Nucleotide> out;
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
