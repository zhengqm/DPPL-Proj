#include <stdio.h>
#include "main.h"
int counter = 0;

int main(){
    Float a = new_float(0.1, 5);
    Float b = new_float(-0.2, 3);
    Float c = add(a,b);
    Float d = multiply(b,c);
    printinfo(a);
    printinfo(b);
    printinfo(c);
    printinfo(d);
    // int i;
    // for (i = 0; i < 1000; i++)
    // {
    //     d = add (d, a);
    //     if (i % 100 == 0)
    //         printinfo(d);
    // }
    return 0;
}

long double valid2toabs(float f, int valid)
{
    long double ld = f;
    long addr = (long)(&ld);
    *(long*)addr = 0x8000000000000000; //set Frac 0
    addr += 8;
    long Exp = (*(long*)(addr)) & 0x7FFF;
    if(Exp >= valid)
        Exp -= valid;
    else
        Exp = 0;
    *(long*)addr = (*(long*)addr) & 0x8000;
    *(long*)addr = (*(long*)addr) | Exp; //set Exp - 23
    ld /= 2;
    if (ld < 0)
        ld = -ld;
    return ld;
}
int validabsto2(float val, long double eps)
{
    long double ldval = val;
    long addr = (long)(&ldval);
    addr += 8;
    long valExp = (*(long*)(addr)) & 0x7FFF;

    addr = (long)(&eps);
    addr += 8;
    long epsExp = (*(long*)(addr)) & 0x7FFF;

    int diff = valExp - epsExp - 1;
    return (diff > 0)?diff:0;
}
int valid2to10(int bin)
{
    if (bin >= 32)
    {
        printf("Not support.\n");
        return 10;
    }
    int vec[32] = {0,1,1,1,2,2,2,3,3,3,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,9,9,9,10,10};
    return vec[bin];
}
int valid10to2(int dec)
{
    if (dec < 0)
        return 23;
    if (dec >= 10)
    {
        printf("Not support.\n");
        return 23;
    }
    int vec[10] = {0,1,4,7,10,14,18,20,24,27};
    return vec[dec];
}
Float add(Float a, Float b){

    Float result = zero();
    int i, j, k;
    // Calculate with high precision
    long double new_val = (long double)a.val + (long double)b.val;

    // Calculate in float
    float float_val = a.val + b.val;

    // New epsilon from above calculation
    long double new_eps = new_val - float_val;
    if (new_eps < 0) new_eps = -new_eps;
    // For each possible eps
    for( i = 0; i < counter; i++){
        result.eps[i] = a.eps[i] + b.eps[i];
    }

    // For new epsilon
    result.eps[counter++] = new_eps;


    // For epshi
    result.epshi = a.epshi + b.epshi;

    // For val
    result.val = float_val;
    epsToPositive(&result);
    return result;
}
Float sub(Float a, Float b){
    b.val = -b.val;
    return add(a, b);
}
Float new_float(float f, int decprecision){

    Float result = zero();
    long double new_eps = 0.0;
    
    // if (given_eps >= -0.0000000000001 && given_eps <= 0.0000000000001)
    //     new_eps = valid2toabs(f);
    // else
    //     new_eps = given_eps;

    new_eps = valid2toabs(f, valid10to2(decprecision));
    //printf("debug: %Le\n", new_eps);
    // Set value
    result.val = f;

    // Set initial epsilon
    result.eps[counter++] = new_eps;

    // No high order epsilon
    result.epshi = 0;
    return result;
}

Float multiply(Float a, Float b){
    int i, j, k;
    Float result = zero();
    long double new_eps = 0;
    long double new_epshi = 0;

    // Calculate the new epsilon
    long double new_val = (long double)a.val * (long double)b.val;
    float float_val = a.val * b.val;
    new_eps = new_val - float_val;
    if (new_eps < 0) new_eps = -new_eps;

    // Calculate eps
    for( i = 0; i < MAX; i++){
        result.eps[i] = a.val * b.eps[i] + b.val * a.eps[i];
    }

    // Calculate epshi
    for(i = 0; i < MAX; i++){
        for (j = 0; j < MAX; j++){
            new_epshi += a.eps[i] * b.eps[j];
        }
    }
    for (i = 0; i < MAX; i++){
        new_epshi += a.epshi * b.eps[i];
    }
    new_epshi += a.epshi * b.val;
    new_epshi += a.epshi * b.epshi;

    for (i = 0; i < MAX; i++){
        new_epshi += b.epshi * a.eps[i];
    }
    new_epshi += b.epshi * a.val;
    result.epshi = new_epshi;

    // Add new epsilon
    result.eps[counter++] = new_eps;

    // For val
    result.val = float_val;

    epsToPositive(&result);
    return result;
}


Float zero(){
    Float result;
    int i;
    result.val = 0;
    result.epshi = 0;
    for( i = 0; i < MAX; i++){
        result.eps[i] = 0;
    }
    return result;
}
void printinfo(Float f)
{
    int i;
    printf("\nvalue: %e\n", f.val);
    for (i = 0; i < counter; i++)
    {
        printf("eps[%d]: %Le\n", i, f.eps[i]);
    }
    printf("epshi: %Le\n", f.epshi);
    long double sumerror = f.epshi;
    for (i = 0; i < counter; i++)
    {
        sumerror += f.eps[i];
    }
    printf("Predicted Max Absolute Error: %Le\n", sumerror);
    if (f.val <= 1e-8 && f.val >= -1e-8)
        printf("Predicted Max Relative Error: NaN\n");
    else
        printf("Predicted Max Relative Error: %Le\n", sumerror/f.val >= 0? sumerror/f.val:-sumerror/f.val);
    printf("Valid bits in dec: %d\n", valid2to10(validabsto2(f.val, sumerror)));
}
void epsToPositive(Float *f)
{
    int i = 0;
    for (i = 0; i < counter; i++)
    {
        if (f->eps[i] < 0)
        {
            f->eps[i] = -f->eps[i];
        }
    }
    if (f->epshi < 0)
    {
        f->epshi = -(f->epshi);
    }
}