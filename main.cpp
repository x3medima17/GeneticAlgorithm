#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <bitset>
#include <cstring>
#include <assert.h>
#include "bit_ops.h"
#include "utility"
#include <cmath>
#include "Chromosome.hpp"
#include "GA.hpp"

using namespace bit_ops;

static int Calls = 0;
constexpr int Dimension = 10;
constexpr int Generations = 400;
constexpr int PopulationSize = 100;
constexpr size_t Crossovers = int(sqrt(PopulationSize));
constexpr double MutationRate = 0.225;

constexpr int CrossLoci = 32;

static_assert(CrossLoci%2 == 0, "Even Loci");


class GriewankOrganism : public Organism {
public:
    GriewankOrganism(std::vector<double> V) : Organism(V) {}


    virtual void compute_fitness(const std::vector<std::pair<double, double>>& Limits) override {

        auto V = this->DNA.get_vect();

        for(size_t i=0; i<Limits.size();i++)
            if(V.at(i) < Limits.at(i).first || V.at(i) > Limits.at(i).second)
            {
                this->fitness = std::numeric_limits<double>::max();
                return;
            }
        double res = 0;
        for (const auto &item : V)
            res += item * item / 4000.0;
        double tmp = 1;
        double i = 1;
        for (const auto &item : V)
            tmp *= cos(item / sqrt(i++));
        res = res - tmp + 1;
        this->fitness = res;

        std::mt19937 gen;
    }
};





int main() {
    GA<GriewankOrganism> ga(Dimension, PopulationSize, Generations, Crossovers, MutationRate,
        std::vector<std::pair<double, double>>(10, {-50,50})
    );
    ga.run();
    return 0;
}


