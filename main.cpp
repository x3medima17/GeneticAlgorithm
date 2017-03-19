#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <bitset>
#include <cstring>
#include "Nucleotide.hpp"

#include "GA.hpp"

static int Calls = 0;
constexpr int Dimension = 10;
constexpr int Generations = 400;
constexpr int PopulationSize = 100;
constexpr size_t Crossovers = static_cast<size_t >(sqrt(PopulationSize));
constexpr double MutationRate = 0.225;

constexpr int CrossLoci = 32;

static_assert(CrossLoci % 2 == 0, "Even Loci");


class GriewankOrganism : public Organism {
public:
    GriewankOrganism(std::vector<double> V) : Organism(V) {}


    virtual double compute_fitness(const std::vector<std::pair<double, double>> &Limits) const override {

        auto V = this->DNA.get_vect();

        for (size_t i = 0; i < Limits.size(); i++)
            if (V.at(i) < Limits.at(i).first || V.at(i) > Limits.at(i).second)
                return std::numeric_limits<double>::max();

        double res = 0;
        for (const auto &item : V)
            res += item * item / 4000.0;
        double tmp = 1;
        double i = 1;
        for (const auto &item : V)
            tmp *= cos(item / sqrt(i++));
        res = res - tmp + 1;
        return res;
    }
};

class Foo {
public:
    double fx(const std::vector<double> V, const std::vector<std::pair<double, double>> &Limits) {
        for (size_t i = 0; i < Limits.size(); i++)
            if (V.at(i) < Limits.at(i).first || V.at(i) > Limits.at(i).second)
                return std::numeric_limits<double>::max();

        double res = 0;
        for (const auto &item : V)
            res += item * item / 4000.0;
        double tmp = 1;
        double i = 1;
        for (const auto &item : V)
            tmp *= cos(item / sqrt(i++));
        res = res - tmp + 1;
        return res;
    }
};


class POrganismFactory : public OrganismFactory<GriewankOrganism> {
public:
    Foo *ptr;

    POrganismFactory(const std::vector<std::pair<double, double>> &Limits,
                     double MutationRate, size_t CrossLoci, size_t Dimensions,
                     Foo *ptr) :
            OrganismFactory<GriewankOrganism>(Limits, MutationRate, CrossLoci, Dimensions),
            ptr(ptr)
    {}

    double compute_fitness(const GriewankOrganism& org) const override {
        return ptr->fx(org.DNA.get_vect(), this->Limits);
    }


};

class X2Organism : public Organism {
public:
    X2Organism(std::vector<double> V) : Organism(V) {}

};

class XOF : public OrganismFactory<X2Organism> {
public:
    XOF(const std::vector<std::pair<double, double>> &Limits,
                     double MutationRate, size_t CrossLoci, size_t Dimensions) :
            OrganismFactory<X2Organism>(Limits, MutationRate, CrossLoci, Dimensions)
    {}

    double compute_fitness(const X2Organism& org) const override {
        auto V = org.DNA.get_vect();
        return std::accumulate(V.begin(), V.end(), 0, std::plus<double >());
    }
};

//class template test<int>;

int main() {
    auto Limits = std::vector<std::pair<double, double>>(10, {-50, 50});

    XOF curr(Limits, MutationRate, CrossLoci, Dimension);
    GriewankOrganism g({10,19});
    auto sof = std::make_shared<POrganismFactory>(Limits, MutationRate, CrossLoci, Dimension, new Foo);
    GA<GriewankOrganism> ga(PopulationSize, Generations, Crossovers, sof);
    auto stats = ga.run();
    std::ofstream fout("stats.out");
    for(const auto& gen : stats) {
        fout<<gen[0]<<" "<<gen[1]<<" "<<gen[2]<<std::endl;
    }
    return 0;
}


