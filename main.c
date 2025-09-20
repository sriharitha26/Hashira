#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned long long convertToDecimal(const char *value, int base) {
    return strtoull(value, NULL, base);
}

unsigned long long lagrangeInterpolation(int k, int *x, unsigned long long *y) {
    unsigned long long result = 0;
    for (int i = 0; i < k; i++) {
        long double term = y[i];
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0.0 - x[j]) / (x[i] - x[j]);
            }
        }
        result += (unsigned long long)(term + 0.5); // rounding
    }
    return result;
}

void parseAndSolve(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    char line[1024];
    int x[100];
    unsigned long long y[100];
    int count = 0, k = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "\"k\"")) {
            sscanf(line, " \"k\": %d", &k);
        } else if (strchr(line, '{') || strchr(line, '}') || strstr(line, "\"n\"")) {
            continue;
        } else {
            int key, base;
            char value[256];
            sscanf(line, " \"%d\": { \"base\": \"%d\", \"value\": \"%[^\"]\" }", &key, &base, value);
            x[count] = key;
            y[count] = convertToDecimal(value, base);
            count++;
        }
    }

    fclose(fp);

    unsigned long long secret = lagrangeInterpolation(k, x, y);
    printf("Secret from %s: %llu\n", filename, secret);
}

int main() {
    parseAndSolve("testcase1.json");
    parseAndSolve("testcase2.json");
    return 0;
}
