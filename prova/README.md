Obs: o servidor não está recebendo todos os números corretamente.

> Compilação

```$ gcc main_server.c main.h main_svc.c main_xdr.c``` : servidor

```$ mv a.out server.out``` : renomeia o executável do servidor

```$ gcc main_client.c main_clnt.c main_xdr.c main.h``` : cliente

```$ mv a.out client.out``` : renomeia o executável do cliente

> Execução

```$ ./server.out``` : executa o servidor

```$ ./client.out 127.0.0.1 <num1> <num2> ... <numN>``` : executa o cliente