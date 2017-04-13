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

long long pow2(int e){
    if (e==0)
        return 1;
    if(e%2 == 0){
        auto tmp = pow2(e/2);
        return tmp*tmp;
    }

    return pow2(e-1)*2;
}

double Gene::get_value() const {
    double out = 0;
    long double sum = 0;
    long long c = pow2(acc)-1;
    for(int i=0; i<acc;i++)
        sum += nucleotides.at(i) * pow2(i);

    out = sum/c;
    assert(sum <= c);
    assert(out >= 0 && out <= 1);
    return out;
}

bool Gene::operator==(const Gene &a) const {
    return (this->nucleotides == a.nucleotides);
}
