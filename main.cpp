#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <bitset>
#include <cstring>

#include "GA.hpp"

static int Calls = 0;
constexpr int Dimension = 20;
constexpr int Generations = 300;
constexpr int PopulationSize = 100;
constexpr size_t Crossovers = static_cast<size_t >(PopulationSize * 0.7);
constexpr double MutationRate = 0.825;
constexpr size_t acc = 40;
constexpr int CrossLoci = 32;

static_assert(CrossLoci % 2 == 0, "Even Loci");

class GriewankFactory : public  OrganismFactory {
public:
    GriewankFactory(const std::vector<std::pair<double, double>> &Limits,
                    const double MutationRate, const size_t CrossLoci,
                    const size_t Dimension, size_t acc) :
    OrganismFactory(Limits, MutationRate, CrossLoci, Dimension, acc) {}

    double compute_fitness(const Organism a) const override {
        auto V = a.DNA.get_vect();

        for (size_t i = 0; i < Limits.size(); i++)
            if (V.at(i) < Limits.at(i).first || V.at(i) > Limits.at(i).second)
                return std::numeric_limits<double>::max();

        double res = 0;
        size_t idx = 0;
        for (auto item : V)
        {
            item = normalize(item, Limits.at(idx).first, Limits.at(idx).second);
            res += item * item / 4000.0;
        }
        double tmp = 1;
        double i = 1;
        for (const auto &item : V)
            tmp *= cos(item / sqrt(i++));
        res = res - tmp + 1;
        return res;
    }
};


int main() {
    auto Limits = std::vector<std::pair<double, double>>(Dimension, {-5, 5});

//    GriewankOrganism g({10,19},10);

    auto sof = std::make_shared<GriewankFactory>
            (Limits, MutationRate, CrossLoci, Dimension, acc);
    GA ga(PopulationSize, Generations, Crossovers, sof);

    auto stats = ga.run();
    std::ofstream fout("stats.out");
    for(const auto& item: stats) {
        for(const auto& val : item)
            fout<<val<<" ";
        fout<<std::endl;
    }
    std::cout<<Calls;
    return 0;
}