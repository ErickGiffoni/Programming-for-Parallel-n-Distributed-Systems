#include <stdio.h>      // printf
#include <sys/socket.h> // bind, sendto, recvfrom
#include <sys/types.h>  // bind, sendto
//#include <netinet/in.h>
#include <arpa/inet.h>  // inet_addr
//#include <netdb.h>
//#include <unistd.h>
#include <stdlib.h>     // atoi
#include <string.h>     // strncmp
#include <errno.h>
#include "calculator.h"

void calculate(int *descriptor, struct sockaddr_in *client, struct sockaddr_in *server);
void wrap_message(calculator_input *calc_in);
void unwrap_message(calculator_input *calc_in);

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

   client.sin_family       = AF_INET;
   client.sin_addr.s_addr  = htonl(INADDR_ANY);
   client.sin_port         = htons(0);

   int descriptor = socket(AF_INET, SOCK_DGRAM, 0);
   if(descriptor < 0){
      printf("%s: error trying to open socket :(\n", argv[0]);
      return 1;
   }

   int socket_binded = bind(descriptor, (struct sockaddr *)&client, sizeof(client));
   if(socket_binded != 0){
      printf("%s: error trying to bind\n", argv[0]);
      return 2;
   }

   printf("\t--- CALCULATOR ---\n\n\n");
   calculate(&descriptor, &client, &server);

   return 0;
}

void unwrap_message(calculator_input *calc_in){
   char *next;
   int result;
   result = (int) strtol(calc_in->message, &next, 10);
   if(strncmp(next, "ERROR", 5) == 0){
      printf("%s\n\n", next);
      return;
   }

   printf("%d\n\n", result);
   return;
}

void wrap_message(calculator_input *calc_in){
   /* WRAPS MESSAGE ACCORDING TO THE CALCULATOR_INPUT MESSAGE PATTERN IN CALCULATOR.H*/
   sprintf(calc_in->message, "%034d%c%034d%s", calc_in->op1, calc_in->opr, calc_in->op2, REQUEST_FLAG);
   return;
}

void calculate(int *descriptor, struct sockaddr_in *client, struct sockaddr_in *server){
   calculator_input *calc_input;

   while(1){
      calc_input = (calculator_input *) calloc(1, sizeof(calculator_input));

      printf("Inform the first operand\n> ");
      scanf(" %d", &calc_input->op1);
      printf("Inform the operator [ + - * / %% ]\n> ");
      scanf(" %c", &calc_input->opr);
      printf("Inform the second operand\n> ");
      scanf(" %d", &calc_input->op2);

      wrap_message(calc_input);

      int server_size      = sizeof(*server);
      int calc_input_size  = sizeof(calc_input);

      // printf("\ndesc: %d\nmess: %s\n", *descriptor, calc_input->message);

      int message_sent = sendto(*descriptor, 
                               calc_input->message, 
                               sizeof(char)*BUFFER_LENGTH, 
                               0, 
                               (struct sockaddr *) server, 
                               server_size);

      memset(calc_input->message, 0x0, BUFFER_LENGTH);
      
      if(message_sent == -1){
         printf("\n[CLIENT ERROR] COULD NOT SEND MESSAGE\n\n");
         perror(NULL);
         printf("errno = %d\n", errno);
         //printf("errno: %d\n", errno);
      }
      else{
         /* waiting server response */
         int server_response = recvfrom(*descriptor,
                                       calc_input->message,
                                       sizeof(char)*BUFFER_LENGTH,
                                       0,
                                       (struct sockaddr *) server,
                                       (socklen_t *) &server_size);
         if(server_response == -1){
            printf("\n[SERVER ERROR] COULD NOT RECEIVE RESPONSE\n\n");
         }
         else{
            printf("[%s:%u]> ", inet_ntoa(server->sin_addr), ntohs(server->sin_port));
            unwrap_message(calc_input);
         }
      }
   }

   return;  
}