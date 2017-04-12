//
// Created by dumitru on 3/7/17.
//

#pragma once

#include <iostream>
#include "Nucleotide.hpp"
#include "bit_ops.hpp"

class Gene {
public:
    std::vector<bool> nucleotides;
    Gene();
    Gene(double val, size_t n);
    Gene(const std::vector<bool> &V);
	Gene& operator= (const Gene& rhs);

    size_t acc;
    void print() const;

    double get_value() const;

    bool operator==(const Gene& a) const;


};

Gene& Gene::operator= (const Gene& rhs) 
{
	this->nucleotides = rhs.nucleotides;
	this->acc = rhs.acc;
	return *this;
}

Gene::Gene():acc{0} {}

Gene::Gene(const std::vector<bool> &V) : nucleotides(V), acc(V.size()) {}

Gene::Gene(double val, size_t n): acc{n} {
    assert(val >= 0 && val <= 1 && n > 1);
    nucleotides.resize(n);
    int sum = static_cast<int>(val * (1<<n - 1));
    while(sum) {
        n--;
        nucleotides.at(n) = sum%2;
        sum /= 2;
    }
}

void Gene::print() const {
    for(const auto& nuc : nucleotides)
        std::cout<<nuc<<" ";
    std::cout<<std::endl;
}

double Gene::get_value() const {
    double out = 0;
    double sum = 0;
    for(int i=0; i<acc;i++)
        sum += nucleotides.at(i) * (1<<i);

    out = sum/((1<<acc) - 1);
    return out;
}

bool Gene::operator==(const Gene &a) const {
    return (this->nucleotides == a.nucleotides);
}
