#define MAX 100

typedef struct _Float
{
    long double eps[MAX];
    long double epshi;
    float val;
    int valid_bit;
} Float;