
#define MAX 10000

typedef struct _Float
{
    long double eps[MAX];
    long double epshi;
    float val;
} Float;

Float add(Float, Float);
Float multiply(Float, Float);
Float zero();
Float new_float(float f, int decprecision);
void printinfo(Float f);
void epsToPositive(Float *f);
long double valid2toabs(float f, int valid);
int validabsto2(float val, long double eps);
int valid2to10(int bin);
int valid10to2(int dec);

