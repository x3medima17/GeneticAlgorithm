//
// Created by dumitru on 3/3/17.
//
#pragma once

namespace bit_ops {

    unsigned long long to_ull(double src) {
        return *reinterpret_cast<unsigned long long *>(&src);

    }

    double to_double(unsigned long long src) {
        return *reinterpret_cast<double *>(&src);
    }

    bool read_bit(double src, int bit) {
        static_assert(sizeof(double) == sizeof(uint64_t), "Sizes do not match");
        assert(bit < sizeof(double) * 8);

        unsigned long long mask = 1ULL << bit;

        return (to_ull(src) & mask) != 0;
    }

    void flip_bit(double &src, int bit) {
        static_assert(sizeof(double) == sizeof(uint64_t), "Sizes do not match");
        unsigned long long mask = 1ULL << bit;
        unsigned long long tmp = to_ull(src);
        tmp ^= mask;
        src = to_double(tmp);
    }

    void clear_bit(double &src, int bit) {
        static_assert(sizeof(double) == sizeof(uint64_t), "Sizes do not match");
        assert(bit < sizeof(double) * 8);
        unsigned long long mask = -1LL ^(1ULL << bit);
        src = to_double(to_ull(src) & mask);

    }

    void set_bit(double &src, int bit) {
        static_assert(sizeof(double) == sizeof(uint64_t), "Sizes do not match");
        assert(bit < sizeof(double) * 8);
        src = to_double(to_ull(src) | 1ULL << bit);
    }

    void test() {
        assert(to_ull(1) == 0x3FF0000000000000);
        assert(to_ull(78.9) == 0x4053B9999999999A);
        assert(to_double(0x4053B9999999999A) == 78.9);
        assert(to_ull(5465.000122070312) == 0x40b5590007ffffff);
        assert(std::bitset<64>(to_ull(78.9)) == std::bitset<64>(0x4053B9999999999A));
        double x = 5465.000122070312;
        unsigned long long xp = to_ull(x);
        std::cout << "0100000010110101010110010000000000000111111111111111111111111111\n";
        std::cout << std::bitset<64>(xp) << std::endl;

        clear_bit(x, 1);
        std::cout << "Clear bit 1\n" << std::bitset<64>(to_ull(x)) << std::endl;
        flip_bit(x, 2);
        std::cout << "Flip bit 2\n" << std::bitset<64>(to_ull(x)) << std::endl;
        set_bit(x, 2);
        std::cout << "Set bit 2\n" << std::bitset<64>(to_ull(x)) << std::endl;
        std::cout << "Read bit 3\n" << read_bit(x, 3) << "\nRead bit 1\n" << read_bit(x, 1) << std::endl;
        std::cout << "Read bit 62\n" << read_bit(x, 62) << "\nRead bit 61\n" << read_bit(x, 61) << std::endl;
    }
}