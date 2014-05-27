
#define MAX 100

typedef struct _Float
{
    long double eps[MAX];
    long double epshi;
    float val;
} Float;

long double getLastPrecision(float);
Float add(Float, Float);
Float multiply(Float, Float);
Float zero();
Float new_float(float);
void printinfo(Float f);
void epsToPositive(Float *f);
