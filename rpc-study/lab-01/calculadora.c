#include <stdio.h>
#include <stdlib.h>

int calcula(int op1, char opr, int op2){

   switch (opr){
      case '+':
         return (op1 + op2);

      case '-':
         return (op1 - op2);

      case '/':
         return (op1 / op2);

      case '*':
         return (op1 * op2);

      default:
         printf("\nERRO: operação não suportada!\n");
         printf("Operações suportadas:\n\t-> +\n\t-> -\n\t-> *\n\t-> /\n");
         exit(1);
   }
}

int main(int argc, char * argv[]){

   int op1 = atoi(argv[1]);
   int op2 = atoi(argv[3]);
   char opr = *argv[2];

   printf("\n--> Calculadora: o resultado é %d\n", calcula(op1, opr, op2));
   
   return 0;
}