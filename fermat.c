/*******************************************************
/
/ fermat-little-theorem.c
/
/ A C program that uses Fermat's Little Theorem
/ to test for probable primes
/
/ Paul Soper
/
/ June, 2014
/
/******************************************************/

/******************************************************
/
/ Fermat's little theorem, stated in C-type language,
/ says that if p is prime, and 1 < a < p,
/ ((a^p - a) % p) == 0).
/
/ If ((a^p - a) % p) != 0), p is definitely composite,
/ a is called the composite witness.
/
/ If ((a^p - a) % p) == 0), p is probably prime.  The
/ probability of p being prime increases as more
/ values of a are tested and are found not to be
/ composite witnesses.
/
/ If ((a^p - a) % p) == 0), but p is not prime, a is
/ called a Fermat liar.
/
/ There are a small group of numbers - the Carmichael
/ numbers, for which all a's 1 < a < p are Fermat
/ liars. 561 is such a number, and the only one that
/ will be found by this program.
/
/*****************************************************/

#include "stdio.h"
#include "stdlib.h" /* because we're using malloc */
#include "time.h"   /* because we need to seed srand() */

/* a^p - a may be very, very big, so our 64 bit
   arithmetic will likely not suffice.  We will use the
   GNU large integer library, gmp */

#include "gmp.h"

int main()
{
    /* seed the random number generator with the current
       time */

    srand(time(NULL));

    /* we will look for primes less than 1000 */

    int max = 1000;
    unsigned int p;

    mpz_t a;  /* big number variable for a */
    mpz_t e;  /* big number variable for a^p - a */
    mpz_t r1; /* big number variable for (a^p - a) % p */

    /* initialize the large integer variables */

    mpz_init(a);
    mpz_init(e);
    mpz_init(r1);

    /* run through the candidates, from 3 to max, looking
      for probable primes */

    for (p = 3; p < max; ++p)
    {
        /* For each candidate, we're going to test 20 a's,
           counting them with trial */

        int trial = 0;

        /* if p is found to be composite, that is if we
           find an a such that ((a^p - a) mod p != 0), we
           want to report that a as our composite witness */

        int composite_witness = 0;

        /* if we find a fermat liar, report that as well */

        int fermat_liar = 0;

        /* a variable for a */

        unsigned int a1 = 0;

        while ((trial < 20) && (composite_witness == 0))
        {
            /* in case we discover that the old a1 was a
           fermat liar, keep track of the old a1 */

            unsigned int olda1 = a1;

            /* choose a random integer in the range 1 < a1 < p
           to be our a for the test */

            a1 = rand() % (p - 2) + 2;

            /* put a1 into the bug number variable a */

            mpz_set_ui(a, a1);

            /* raise a to the power p, and put the
           result in e */

            mpz_pow_ui(e, a, p);

            /* now take that result, and subtract a from it,
           putting the new result back into e */

            mpz_sub(e, e, a);

            /* get the remainder from dividing e by p */

            mpz_mod_ui(r1, e, p);

            /* compare that big number remainder to 0 */

            unsigned int cmp1 = mpz_cmp_ui(r1, 0);

            /* if the remainder is not 0, p is composite */

            if (cmp1 != 0)
            {
                /* identify the witness that showed p to be
                   composite, and store it in composite_witness -
                   this will also end the loop, as no further
                   tests are needed */

                composite_witness = a1;

                /* but, if this is not the first pass through the
                   loop, a previous a must have lied about the
                   primality of p rather than given witness to
                   the composite nature of p, so the previous a
                   is a fermat liar */

                fermat_liar = olda1;
            }
            ++trial;
        }

        if (composite_witness == 0)
        {
            printf("%d is a probable prime\n", p);
        }
        else
        {
            printf("%d is composite - %d is a composite witness",
                   p, composite_witness);
            if (fermat_liar > 0)
            {
                printf(" - %d is a fermat liar for %d",
                       fermat_liar, p);
            }
            printf("\n");
        }
    }

    /* clear the big number variables */

    mpz_clear(r1);
    mpz_clear(a);
    mpz_clear(e);

    return (0);
}