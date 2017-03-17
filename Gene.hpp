//
// Created by dumitru on 3/7/17.
//

#pragma once

#include <iostream>
#include "Nucleotide.hpp"
#include "bit_ops.h"

class Gene {
public:
    const int LEN = 32;
    std::vector<Nucleotide> nucleotides;
    Gene(){}
    Gene(std::vector<Nucleotide>& V): nucleotides(V){
        assert(V.size() == 32);
    }
    void print() const {
        for(const auto& nuc : nucleotides)
        {
            nuc.print();
            std::cout<<" ";
        }
        std::cout<<std::endl;
    }

    double get_value() const {
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

    bool operator==(const Gene& a) const {
        return (this->nucleotides == a.nucleotides);
    }


};
