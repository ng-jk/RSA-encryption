#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <cmath>

using namespace std;

// Function to calculate gcd

// Two random prime numbers  

int* d;
int* e;
int* n;

int gcd(int a, int h) {
    int temp;
    while (1) {
        temp = a % h;
        if (temp == 0)
            return h;
        a = h;
        h = temp;
    }
}

void key(int p, int q){
    int N = p * q;
    n = &N;
    int phi = (p - 1) * (q - 1);
    int E = 2;
        while (E < phi) {
            if (gcd(E, phi) == 1)
                break;
            else
                E++;
        }
    e= &E;

    int k = 1; // A constant value to satisfy equation d*e = 1 + k*phi
    int D;
        while (true) {
            if ((1 + k * phi) % E == 0) {
                D = (1 + k * phi) / E;
                break;
            }
            k++;
        } 
    d= &D;

}

// (base^exp) % mod using integer arithmetic
int mod_exp(int base, int exp, int mod) {
    int result = 0;
    double temp;
    temp = pow((double)base,(double)exp);
    temp = fmod(temp, (double)mod);
    result = round(temp);
    cout<<result<<" the result ";
    return result;
}

// Code to demonstrate RSA algorithm
int RSAdecryption(int ciphertext,int p, int q) {
    // Decryption m = (c ^ d) % n
    key(p,q);
    int m = mod_exp(ciphertext, *d, *n);
    return m;

}
int RSAencryption(int plaintext,int p,int q) {
    // Encryption c = (msg ^ e) % n
    key(p,q);
    int c = mod_exp(plaintext, *e, *n);
    return c;
}

#endif
