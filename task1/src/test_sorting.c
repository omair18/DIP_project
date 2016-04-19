#include <stdio.h>
#include <stdlib.h>

int A[] = {0, 1, 4, 5, 7};
int B[]= {5, 3, 8, 2, 2, 7, 1, 6, 3, 9};

 int my_cmp(const int *a_, const int *b_)
 {
   const int *a = a_, *b = b_;

   if(B[*a] == B[*b])
       return 0;
    else if (B[*a] < B[*b])
        return -1;
    else
        return 1;

}

int main(int argc,char *arga[])
{
    int i;

    qsort(A,sizeof A/sizeof A[0] ,sizeof A[0],my_cmp);

    puts("Sorted A");
    for(i = 0 ; i < sizeof A/sizeof A[0]; i++) {
        printf("A[%d] : %d B[A[%d]] : %d\n",i,A[i],i,B[A[i]]);
    }

    return 0;
}

