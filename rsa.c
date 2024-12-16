#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long int GCD(long long int p, long long int q) {

    if (p%q == 0) {
        return q;                  // returns q if the division is perfect (the Greatest Common Divisor)
    } else {
        return GCD(q, p%q);        // the function calls itself again and again until it achieves a perfect division -->
    }                              // by replacing p with q and q with the not perfect division p%q

}

int prime(long long int x) {

    if (x <= 1)
        return 0;
    else if (x <=3)
        return 1;
    else if (x % 2 == 0 || x % 3 == 0)
        return 0;
    else {
        for (long long int i = 5; i * i <= x; i += 6) { //we start from 5 because we already checked for the numbers smaller than 5
            if (x % i == 0 || x % (i + 2) == 0) {       //We also move i by 6 each time because prime numbers bigger than 3 take the form 6*x + 1 and 6*x -1...
                return 0;                               // and we already excluded numbers able to be divided by 3 and 2 earlier
            }
        }
    }
    return 1;
}

long long int power(long long int m, long long int x, long long int N) {
    
    long long int pow_result = 1;
    m = m % N; // Ensure m is within mod to avoid overflow
    while (x > 0) {
        // If x is odd, multiply m with the result
        if (x % 2 == 1) {
            pow_result = (pow_result * m) % N;
        }
        // Square m and reduce x by half
        x = x / 2;
        m = (m * m) % N;
    }
    return pow_result;
}

int main(int argc, char * argv[]) {

    char *op = argv[1];

    if (argc != 6) {       
        printf("Usage: %s enc|dec <exp_exp> <priv_exp> <prime1> <prime2>\n", argv[0]);
        return 1;
    }

    long long int e = atoll(argv[2]);
    long long int d = atoll(argv[3]);
    long long int p = atoll(argv[4]);
    long long int q = atoll(argv[5]);
    long long int m;
    long long int N = p * q;
    long long int phi_N = (p - 1) * (q - 1); // According to "orismos 5"

    if (strcmp(op, "enc") != 0 && strcmp(op, "dec") != 0) {        
        printf("First argument must be 'enc' or 'dec'\n");
        return 1;
    } else if (e < 0 || d < 0 || p < 0 || q < 0) {        
        printf("Negative numbers are not allowed\n");
        return 1;
    } else if (!prime(p) || !prime(q)) {        
        printf("p and q must be prime\n");
        return 1;
    } else if (GCD(e, phi_N) != 1) {        
        printf("e is not coprime with phi(N)\n");
        return 1;
    } else if (e * d % phi_N != 1) {
        printf("e * d mod phi(N) is not 1\n");
        return 1;
    }

   int m_scan_value =  scanf("%lld", &m);
    if (m_scan_value != 1) {
        return 1;
    } else if (m >= N) {
        printf("Message is larger than N\n");
        return 1;
    } else if (m < 0) {
        printf("Negative numbers are not allowed\n");
        return 1;
    }
    if (strcmp(op, "enc") == 0) {
            
        long long int encrypt_m = power(m, e, N) % N;
        printf("%lld\n", encrypt_m);
    }
    if (strcmp(op, "dec") == 0) {

        long long int decrypt_c = power(m, d, N) % N;
        printf("%lld\n", decrypt_c);
    }
    return 0;
}
