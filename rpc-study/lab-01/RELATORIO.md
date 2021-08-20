# Lab01 – Construindo aplicações distribuídas usando RPC


Universidade de Brasília | Faculdade do Gama

Programação para sistemas paralelos e distribuídos - PSPD

Erick Giffoni - 170141161

07/2021


## Introdução


Este laboratório tem como objetivo exemplificar, por meio de uma
aplicação simples, o uso de RPC (*Remote Procedure Call*) em lin-
guagem C.


A arquitetura RPC é do tipo cliente e servidor. Sendo assim, o
cliente faz chamadas de procedimento remoto no servidor, o qual
executa a chamada e devolve uma resposta.


## A solução


No contexto desse laboratório, foi construída uma calculadora
simples, a qual realiza as 4 operações básicas da matemática.
O módulo cliente lê a entrada do usuário e passa isso como pa-
râmetro para o servidor, por meio de RPC, o qual faz o cálculo
e devolve para o cliente o resultado.


Foi utilizado o comando ```rpcgen -a calculadora.x``` para ob-
ter-se um conjunto de programas que facilitassem o uso de RPC
em linguagem C. Entretanto, várias dificuldades foram enfrenta-
das, tendo em vista que a ferramenta ```rpcgen``` não parece
estar atualizada para o sistema operacional utilizado, macOS.


Isso porque aquela ferramenta deveria ter gerado um arquivo
```makefile``` para ajudar na compilação dos programas.
Isso foi a primeira barreira, mas não a última porque, além
disso, os programas gerados tinham diversos erros de tipagem
incoerente com a biblioteca ".h" gerada, chamadas de funções
incorretas, bibliotecas faltando (*#include <string.h>*, por
exemplo) etc.


Assim, foi necessário fazer a compilação do cliente e do ser-
vidor de maneira individual e manual, sem a ajuda do utili-
tário ```make```. Foi a partir dessas tentativas de compila-
ção que os erros do *rpcgen* foram apresentados. Logo, foi
necessário editar vários dos programas para consertar os er-
ros e, finalmente, conseguir executar a aplicação.


- Primeiramente, deve-se executar o *portmapper* ```rpcbind```
para que ele atue como middleware entre o cliente e o ser-
vidor;


- Em seguida, executa-se o servidor (não é necessário informar
nenhum parâmetro);


- Agora, executa-se o cliente conforme o exemplo abaixo:


> $ ./cliente.out 127.0.0.1 <X> <W> <Y>


   - O 1o parâmetro é o IP do servidor. No exemplo acima foi
   utilizado *localhost*;


   - O 2o parâmetro é a operação matemática desejada, em que
   X representa um número inteiro, W um operador (ex.: +) e Y
   o outro número inteiro da operação.


## Opinião geral


Apesar da limitação da ferramenta *rpcgen* e dos erros que ela
comete no macOS, gostei do experimento. Eu, particularmente,
gosto dessa parte mais "baixo nível" de programação, e nesse
Lab 01 foi possível explorá-la.


### Comandos úteis


```
$ rpcgen -a calculadora.x                    // executa o rpcgen a partir do ".x" e gera os programas auxiliares em C

$ sudo launchctl start com.apple.rpcbind     // inicia o portmapper RPC no macOS

$ sudo launchctl stop com.apple.rpcbind      // desabilita o portmapper RPC no macOS

$ ./server.out                               // executa o servidor

$ ./cliente.out 127.0.0.1 1 + 2              // executa o cliente, informando o IP do servidor e a operação matemática desejada

$ gcc calculadora_server.c calculadora.h calculadora_svc.c calculadora_xdr.c  // compilação do servidor

$ gcc calculadora_client.c calculadora_clnt.c calculadora_xdr.c calculadora.h // compilação do cliente

$ mv a.out ./client.out                      // renomeia o binário a.out para client.out
```