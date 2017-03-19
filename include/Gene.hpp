//
// Created by dumitru on 3/7/17.
//

#pragma once

#include <iostream>
#include "Nucleotide.hpp"

class Gene {
public:
    std::vector<Nucleotide> nucleotides;
    Gene();
    Gene(std::vector<Nucleotide>& V);
    void print() const;

    double get_value() const;

    bool operator==(const Gene& a) const;


};
