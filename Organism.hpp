//
// Created by dumitru on 3/14/17.
//

#pragma once

#include "Chromosome.hpp"

class Organism {
public:
    double fitness = 0;

    Chromosome DNA;
    Organism(const Chromosome& C);

    virtual void set_fitness(double value);

    void print() const;


    bool operator==(const Organism &a) const;


};

Organism::Organism(const Chromosome& C) :
        DNA(C)
{}

void Organism::set_fitness(double value) {
    fitness = value;
}

void Organism::print() const {
    std::cout << fitness << "\t";
    for (const auto &item: DNA.get_vect())
        std::cout << item << " ";
    std::cout << "\n";
}

bool Organism::operator==(const Organism &a) const {
    return (this->fitness == a.fitness && this->DNA == a.DNA);
}




