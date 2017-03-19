//
// Created by dumitru on 3/3/17.
//
#pragma once

namespace bit_ops {
    unsigned long long to_ull(double src);


    double to_double(unsigned long long src);


    bool read_bit(double src, int bit);

    void flip_bit(double &src, int bit);

    void clear_bit(double &src, int bit);

    void set_bit(double &src, int bit);

    void test();
}