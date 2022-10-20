#include <iostream>
#include "gmp.h"
#include "gmpxx.h"
#include <time.h>

using std::cout;
using std::endl;

void extended_euclidean_algorithm(mpz_t &gcd, mpz_t &s, mpz_t &t, mpz_t &a, mpz_t &b)
{

    mpz_t x, o_x, y, o_y, r, o_r;
    mpz_init_set_ui(x, 0);
    mpz_init_set_ui(o_x, 1);
    mpz_init_set_ui(y, 1);
    mpz_init_set_ui(o_y, 0);
    mpz_init_set(r, a);
    mpz_init_set(o_r, b);

    mpz_t quotient, temp, mult, sub;

    while (mpz_get_ui(r) != 0)
    {
        mpz_init(quotient);
        mpz_init(temp);
        mpz_init(mult);
        mpz_init(sub);

        mpz_tdiv_q(quotient, o_r, r);

        mpz_set(temp, r);
        mpz_mul(mult, quotient, r);
        mpz_sub(sub, o_r, mult);
        mpz_set(r, sub);
        mpz_set(o_r, temp);

        mpz_set(temp, x);
        mpz_mul(mult, quotient, x);
        mpz_sub(sub, o_x, mult);
        mpz_set(x, sub);
        mpz_set(o_x, temp);

        mpz_set(temp, y);
        mpz_mul(mult, quotient, y);
        mpz_sub(sub, o_y, mult);
        mpz_set(y, sub);
        mpz_set(o_y, temp);
    }

    // set return values
    mpz_set(gcd, o_r);
    mpz_set(s, o_x);
    mpz_set(t, o_y);
}

int main()
{

    mpz_t rn1, rn2, my_result, lib_result, s, t;
    gmp_randstate_t state;

    mpz_init(rn1);
    mpz_init(rn2);
    mpz_init(my_result);
    mpz_init(lib_result);
    mpz_init(s);
    mpz_init(t);
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(0));

    mpz_urandomb(rn1, state, 4096);
    mpz_urandomb(rn2, state, 4096);

    cout << "Random Integer a: " << rn1 << endl;
    cout << "Random Integer b: " << rn2 << endl;

    extended_euclidean_algorithm(my_result, s, t, rn1, rn2);
    mpz_gcd(lib_result, rn1, rn2);

    cout << "Computed Result: GCD(a, b): " << my_result << endl;
    cout << "Library Result: GCD(a, b): " << lib_result << endl;
    cout << "The corresponding Bezout Coefficients include" << endl
         << "s : " << s << endl
         << "t: " << t << endl
         << "Where as + bt = gdc(a,b)" << endl;
}
/*
TARGET = euclid
CFLAGS = -Wall -Werror -pedantic
CC = g++
GMPFLAGS = -lgmp -lgmpxx

default: $(TARGET)

euclid : $(TARGET).cpp
        $(CC) $(CFLAGS) $(TARGET).cpp -o $(TARGET) $(GMPFLAGS)

clean :
        rm -rf $(TARGET)
*/