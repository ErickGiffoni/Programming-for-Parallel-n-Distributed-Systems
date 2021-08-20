#ifndef CALCULATOR_H
#define CALCULATOR_H

#define BUFFER_LENGTH 100
#define NUMBER_OF_CLIENTS 127

#define REQUEST_FLAG "CALCULATE REQUESTED OPERATION ."
#define RESPONSE_SUCCESS "SUCCESSFUL OPERATION DONE .    "
#define RESPONSE_OP_NOT_SUPP "ERROR: OPERATION NOT SUPPORTED."
#define RESPONSE_DIV_ZERO "ERROR: DIVISION BY ZERO .      "

typedef struct calculator_input {
   int op1;
   int op2;
   char opr;
   char message[BUFFER_LENGTH];
   /* MESSAGE PATTERN
   *
   *     REQUEST                 RESPONSE
   *
   *  op1oprop2FLAG     |     resultFLAG
   *                    |     
   *  op1: 34 bytes     |     result: 69 bytes   
   *  opr: 1 byte       |     
   *  op2: 34 bytes     |       
   *  FLAG: 31 bytes    |     FLAG: 31 bytes 
   */
} calculator_input;

#endif