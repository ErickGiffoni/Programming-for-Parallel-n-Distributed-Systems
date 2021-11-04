#include <stdio.h>
#include <stdlib.h>

int somavet(int *x, int n){
   int i, result;
   printf("Requisicao para adicao de %d numeros\n", n);
   result =0;
   for(int i; i<n; i++){
      result += x[i];
   }
   return (result);
}

int main(int argc, char *argv[]){
   int *ints, n_termos, i, res;

   if(argc<2){
      fprintf(stderr, "Uso correto: %s num1 num 2 ...\n", argv[0]);
      exit(0);
   }

   n_termos = argc -1;

   ints = (int *) malloc(n_termos * sizeof(int));

   if(ints==NULL){
      fprintf(stderr, "Erro na alocacao de memoria\n");
      exit(0);
   }

   for(int i=1; i<argc; i++){
      ints[i-1] = atoi(argv[i]);
   }

   res = somavet(ints, n_termos);
   printf("%d", ints[0]);

   for(int i=1; i<n_termos; i++){
      printf(" + %d", ints[i]);
   }

   printf(" = %d\n", res);

   return 0;
}