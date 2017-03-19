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

