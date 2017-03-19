//
// Created by dumitru on 3/3/17.
//
#pragma once

#include <bitset>
#include <vector>
#include <assert.h>
#include <iostream>

class Nucleotide {
public:
    std::bitset<2> value;

    Nucleotide();
    Nucleotide(const std::bitset<2> &bs);
    Nucleotide(const std::vector<bool> &V);
    Nucleotide(const bool a, const bool b);

    void print() const;

    bool operator==(const Nucleotide& a) const;
};


Nucleotide::Nucleotide() { value[0] = -1; value[1] = -1;}

Nucleotide::Nucleotide(const std::bitset<2> &bs) : value(bs) {}

Nucleotide::Nucleotide(const std::vector<bool> &V) {
    assert(V.size() == 2);
    value[0] = V.at(0);
    value[1] = V.at(1);
}

Nucleotide::Nucleotide(const bool a, const bool b) {
    value[0] = a;
    value[1] = b;
}

void Nucleotide::print() const {
    std::cout<<value[0]<<value[1];
}

bool Nucleotide::operator==(const Nucleotide &a) const {
    return (this->value == a.value);
}
