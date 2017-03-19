//
// Created by dumitru on 3/19/17.
//

#include "bit_ops.hpp"
#include "Gene.hpp"

Gene::Gene() {}

Gene::Gene(std::vector<Nucleotide> &V) : nucleotides(V){
    assert(V.size() == 32);
}

void Gene::print() const {
    for(const auto& nuc : nucleotides)
    {
        nuc.print();
        std::cout<<" ";
    }
    std::cout<<std::endl;
}

double Gene::get_value() const {
    double out = 0;
    int k = 63;
    for(const auto& nuc : nucleotides) {
        if(nuc.value[0] == 1)
            bit_ops::set_bit(out, k);
        k--;
        if(nuc.value[1] == 1)
            bit_ops::set_bit(out, k);
        k--;
    }
    return out;
}

bool Gene::operator==(const Gene &a) const {
    return (this->nucleotides == a.nucleotides);
}
