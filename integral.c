#include <math.h>
#include <stdio.h>
#include <getopt.h>

typedef double afunc(double);

extern double f1(double);

extern double f2(double);

extern double f3(double);

double test_f1(double);

double test_f2(double);

double test_f3(double);

double test_f4(double);

double test_f5(double);

double test_f6(double);


// test functions
double test_f1(double x) {
    return 2.5 * x - 9.5;
}

double test_f2(double x) {
    return 3 * (0.5 / (x + 1) + 1);
}

double test_f3(double x) {
    return 0.6 * x + 3;
}

double test_f4(double x) {
    return pow((x - 2), 3) - 1;
}

double test_f5(double x) {
    return -2 * x + 8;
}

double test_f6(double x) {
    return -5 / x;
}

// https://www.opennet.ru/man.shtml?topic=getopt
struct {
    const char *name;
    int has_arg;
    int *flag;
    int val;
} option;

// iteration count
static int iterations = 0;


// calculate dots of intersection (the method of dividing a segment in half)
static double root(afunc *f, afunc *g, double a, double b, double eps1) {
    double m;
    iterations = 0;
    if (f(a) - g(a) < 0) {
        afunc *tmp = f;
        f = g;
        g = tmp;
    }
    while (b - a > eps1) {
        iterations++;
        m = (a + b) / 2;
        if (f(m) - g(m) > 0) a = m;
        else b = m;
    }
    return a;
}


// calculate area by integral (Simpson 's method)
static double integral(afunc *f, double a, double b, double eps2) {
    double I = eps2 + 1; // previous value of integral
    double I1 = 0; // I1 - new value of integral with greater N
    int N, i;
    double h, sum, sum2, sum4;
    for (N = 2; (N <= 4) || (fabs(I1 - I) > eps2); N *= 2) {
        sum = sum2 = sum4 = 0;
        h = (b - a) / (2 * N); // step of integrate
        for (i = 1; i <= 2 * N - 1; i += 2) {
            sum4 += f(a + h * i);
            sum2 += f(a + h * (i + 1));
        }
        sum = f(a) + 4 * sum4 + 2 * sum2 - f(b);
        I = I1;
        I1 = (h / 3) * sum;
    }
    return I1;
}

// get function to test for -R and -I flag
static afunc *get_function(int f_number) {
    if (f_number == 1) return &test_f1;
    else if (f_number == 2) return &test_f2;
    else if (f_number == 3) return &test_f3;
    else if (f_number == 4) return &test_f4;
    else if (f_number == 5) return &test_f5;
    return &test_f6;
}

int main(int argc, char *argv[]) {
    int f1_number, f2_number;
    double a, b, e, r, result, tmp;
    int *lidx = NULL;
    const char *s_opt = "R:I:hri";
    const struct option l_opts[] = {
            {"help",          0, NULL, 'h'},
            {"root",          0, NULL, 'r'},
            {"iterations",    0, NULL, 'i'},
            {"test-root",     1, NULL, 'R'},
            {"test-integral", 1, NULL, 'I'},
            {NULL,            0, NULL, 0}
    };
    int c = getopt_long(argc, argv, s_opt,
                        l_opts, lidx);
    switch (c) {
        case 'h':
            printf("--help, -h               print help info\n"
                   "--root, -r               abscissa of intersection points\n"
                   "--iterations, -i         print the number of iterations required for the approximation\n"
                   "--test-root, -R          test root function\n"
                   "--test-integral, -I      test integral function\n");
            break;
        case 'R':
            sscanf(optarg, "%d:%d:%lf:%lf:%lf:%lf", &f1_number, &f2_number, &a, &b, &e, &r);
            tmp = root(get_function(f1_number), get_function(f2_number), a, b, e);
            printf("%lf %lf %lf\n", tmp, fabs(tmp - r), fabs(tmp - r) / r);
            break;
        case 'I':
            sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &f1_number, &a, &b, &e, &r);
            tmp = integral(get_function(f1_number), a, b, e);
            printf("%lf %lf %lf\n", tmp, fabs(tmp - r), fabs(tmp - r) / r);
            break;
        case 'r':
            printf("f1 x f2 : %lf\n", root(&f1, &f2, -10, 10, 0.0000001));
            printf("f2 x f3 : %lf\n", root(&f2, &f3, -10, 10, 0.0000001));
            printf("f1 x f3 : %lf\n", root(&f1, &f3, -10, 10, 0.0000001));
            break;
        case 'i':
            tmp = root(&f1, &f2, -10.0, 10.0, 0.00001);
            printf("f1 n f2 : %d\n", iterations);
            tmp = root(&f2, &f3, -10.0, 10.0, 0.00001);
            printf("f2 n f3 : %d\n", iterations);
            tmp = root(&f1, &f3, -10.0, 10.0, 0.00001);
            printf("f1 n f3 : %d\n", iterations);
            break;
        case -1:
            a = root(&f1, &f3, -10, 10, 0.0000001);
            b = root(&f1, &f2, -10, 10, 0.0000001);
            tmp = root(&f2, &f3, -10, 10, 0.0000001);
            result = integral(&f1, a, b, 0.0001);
            result -= integral(&f3, a, tmp, 0.0001);
            result -= integral(&f2, tmp, b, 0.0001);
            printf("%lf\n", result);
            break;
        default:
            printf("Incorrect option, use --help for more information\n");
    }
    return 0;
}

