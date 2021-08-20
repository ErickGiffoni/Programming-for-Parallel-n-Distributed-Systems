#include <stdio.h>      // printf
#include <sys/socket.h> // bind, sendto, recvfrom
#include <sys/types.h>  // bind, sendto
//#include <netinet/in.h>
#include <arpa/inet.h>  // inet_addr
//#include <netdb.h>
//#include <unistd.h>
#include <stdlib.h>     // atoi
#include <string.h>
#include "calculator.h"

void calculate(int *descriptor, struct sockaddr_in *client, struct sockaddr_in *server);
void manage_input(calculator_input *calc_input, struct sockaddr_in *client);
void set_input(calculator_input *calc_in);

int main(int argc, char *argv[]){

   if(argc < 2){
      printf("Use: <server-ip> <server-port>\n\thint: if your internal ip doesnt work, try loopback: 127.0.0.1\n");
      return 0;
   }

   struct sockaddr_in client;
   struct sockaddr_in server;

   server.sin_family       = AF_INET;
   server.sin_addr.s_addr  = inet_addr(argv[1]);
   server.sin_port         = htons(atoi(argv[2]));

   // client.sin_family       = AF_INET;
   // client.sin_addr.s_addr  = htonl(INADDR_ANY);
   // client.sin_port         = htons(0);

   int descriptor = socket(AF_INET, SOCK_DGRAM, 0);
   if(descriptor < 0){
      printf("%s: error trying to open socket :(\n", argv[0]);
      return 1;
   }

   int socket_binded = bind(descriptor, (struct sockaddr *)&server, sizeof(server));
   if(socket_binded != 0){
      printf("%s: error trying to bind\n", argv[0]);
      return 2;
   }

   printf("\t--- CALCULATOR [SERVER] ---\n\n\n");
   calculate(&descriptor, &client, &server);

   return 0;
}

void calculate(int *descriptor, struct sockaddr_in *client, struct sockaddr_in *server){
   calculator_input *calc_input;

   while(1){
      calc_input = (calculator_input *) calloc(1, sizeof(calculator_input));

      printf("> [%s:%u] listening...\n\n", inet_ntoa(server->sin_addr), ntohs(server->sin_port));

      int client_size = sizeof(client);

      int message_received = recvfrom(*descriptor,
                                      calc_input->message,
                                      sizeof(char)*BUFFER_LENGTH,
                                      0,
                                      (struct sockaddr *) client,
                                      (socklen_t *) &client_size);
      if(message_received == -1){
         printf("\n[CLIENT ERROR] COULD NOT RECEIVE MESSAGE\n\n");
      }
      else{
         manage_input(calc_input, client);
         int response_sent = sendto(*descriptor,
                                    calc_input->message,
                                    sizeof(char)*BUFFER_LENGTH,
                                    0,
                                    (struct sockaddr *) client,
                                    client_size);

         memset(calc_input->message, 0x0, BUFFER_LENGTH);

         if(response_sent == -1){
            printf("\n[SERVER ERROR] COULD NOT SEND RESPONSE\n\n");
         }
      }
   }

   return;  
}

void set_input(calculator_input *calc_in){
   char *next;
   calc_in->op1 = (int) strtol(calc_in->message, &next, 10);
   calc_in->opr = *next;
   next++;
   calc_in->op2 = (int) strtol(next, &next, 10);
   return;
}

void manage_input(calculator_input *calc_input, struct sockaddr_in *client){
   printf("> MESSAGE RECEIVED FROM -- [%s:%u]\n", inet_ntoa(client->sin_addr), ntohs(client->sin_port));

   // DO: logic to set op1, op2 and opr from calc_input.message
   set_input(calc_input);

   printf("\tMESSAGE:\n\t\t.op1: %d\n\t\t.opr: %c\n\t\t.op2: %d\n\n", calc_input->op1, calc_input->opr, calc_input->op2);

   int result;
   switch (calc_input->opr){

      case '+':
         result = calc_input->op1 + calc_input->op2;
         printf("> result: %d\n", result);
         // sprintf(calc_input->message, " = %d", result);
         break;

      case '-':
         result = calc_input->op1 - calc_input->op2;
         printf("> result: %d\n", result);
         // sprintf(calc_input->message, " = %d", result);
         break;

      case '*':
         result = calc_input->op1 * calc_input->op2;
         printf("> result: %d\n", result);
         // sprintf(calc_input->message, " = %d", result);
         break;

      case '/':
         if(calc_input->op2 == 0){
            printf("> DIVISION BY ZERO... DO NOT DO IT BRO\n\n");
            sprintf(calc_input->message, "%069d%s", 0x0, RESPONSE_DIV_ZERO);
            return;
         }
         result = calc_input->op1 / calc_input->op2;
         printf("> result: %d\n", result);
         // sprintf(calc_input->message, " = %d", result);
         break;

      case '%':
         if(calc_input->op2 == 0){
            printf("> DIVISION BY ZERO... DO NOT DO IT BRO\n\n");
            sprintf(calc_input->message, "%069d%s", 0x0, RESPONSE_DIV_ZERO);
            return;
         }
         result = calc_input->op1 % calc_input->op2;
         printf("> result: %d\n", result);
         // sprintf(calc_input->message, " = %d", result);
         break;

      default:
         printf("> OPERATION NOT SUPPORTED\n\n");
         sprintf(calc_input->message, "%069d%s", 0x0, RESPONSE_OP_NOT_SUPP);
         return;
   }

   // set response
   sprintf(calc_input->message, "%069d%s", result, RESPONSE_SUCCESS);

   return;
}