#include <stdio.h>
#include <gmp.h>

unsigned urandom(void)
{
    unsigned r;
    FILE *fp = fopen("/dev/urandom", "r");
    fread((void *)&r, sizeof r, 1, fp);
    fclose(fp);
    return r;
}

void factor2(mpz_t _n, unsigned long *r, mpz_t n)
{
    mpz_set(n, _n);
    int is_odd = mpz_odd_p(n);
    if (is_odd)
        mpz_sub_ui(n, n, 1);

    *r = mpz_scan1(n, *(mp_bitcnt_t[]){0});
    mpz_fdiv_q_2exp(n, n, *r);
}

int mr(mpz_t n, unsigned long k)
{
    if (mpz_cmp_ui(n, 3) <= 0 || mpz_even_p(n))
        return mpz_cmp_ui(n, 2) == 0 ||
               mpz_cmp_ui(n, 3) == 0;

    unsigned long r;
    mpz_t d, a, x;
    mpz_inits(d, a, x, NULL);
    factor2(n, &r, d);

    gmp_randstate_t rs;
    gmp_randinit_default(rs);
    gmp_randseed_ui(rs, urandom());

    mpz_t a_range;
    mpz_init_set(a_range, n);
    mpz_sub_ui(a_range, a_range, 3);

    mpz_t n_sub_1;
    mpz_init(n_sub_1);
    mpz_sub_ui(n_sub_1, n, 1);

    int is_prime = 1;

    for (unsigned long i = 0; i < k; i++)
    {
        mpz_urandomm(a, rs, a_range);
        mpz_add_ui(a, a, 2);

        mpz_powm(x, a, d, n);

        if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, n_sub_1) == 0)
            continue;

        is_prime = 0;
        for (unsigned long j = 0; j < r - 1; j++)
        {
            mpz_powm_ui(x, x, 2, n);
            if (mpz_cmp_ui(x, 1) == 0)
                break;
            if (mpz_cmp(x, n_sub_1) == 0)
            {
                is_prime = 1;
                break;
            }
        }
        if (!is_prime)
            break;
    }

    gmp_randclear(rs);
    mpz_clears(x, a, d, a_range, n_sub_1, NULL);
    return is_prime;
}

int main(int argc, char **argv)
{
    mpz_t n;
    mpz_init(n);
    for (int i = 1; i < argc; i++)
    {
        mpz_set_str(n, argv[i], 10);
        printf("%s: %d\n", argv[i], mr(n, 8));
    }
    mpz_clear(n);
}