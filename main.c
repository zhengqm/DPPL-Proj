# include <stdio.h>

int counter = 0;
#define MAX 100

typedef struct _Float
{
    long double eps[MAX];
    long double epshi;
    float val;
} Float;




long double extract_eps(float);
Float add(Float, Float);
Float multiply(Float, Float);
Float zero();
Float new_float(float);



int main(){
    return 0;
}



long double extract_eps(float b){
    return 0.005;

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

    // For each possible eps
    for( i = 0; i < counter; i++){
        result.eps[i] = a.eps[i] + b.eps[i];
    }

    // For new epsilon
    result.eps[counter++] = new_eps;


    // For epshi
    result.epshi = a.epshi + b.epshi;

    return result;
}

Float new_float(float f){
    Float result = zero();
    long double new_eps = extract_eps(f);
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

    // Calculate eps
    for( i = 0; i < MAX; i++){
        result.eps[i] += a.val * b.eps[i] + b.val * a.eps[i];
    }

    // Calculate epshi
    for(i = 0; i < MAX; i++){
        for (j = 0; j < MAX; j++){
            new_epshi = a.eps[i] * b.eps[j];
        }
    }
    for (i = 0; i < MAX; i++){
        new_epshi += a.epshi * b.eps[i];
    }
    new_epshi = a.epshi * b.val;
    new_epshi = a.epshi * b.epshi;

    for (i = 0; i < MAX; i++){
        new_epshi += b.epshi * a.eps[i];
    }
    new_epshi = b.epshi * a.val;
    new_epshi = b.epshi * a.epshi;
    result.epshi = new_epshi;

    // Add new epsilon
    result.eps[counter++] = new_eps;

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

