# Lab03 - Construindo aplicações distribuídas usando o paradigma publish-subscriber

<p align="center">
   <b>Universidade de Brasília</b><br>
Programação para Sistemas Paralelos e Distribuídos<br>
<br>
   Erick Giffoni - 170141161<br>
   Ailton Aires  - 180011600<br>
</p>


Objetivo: O objetivo desse experimento é que o aluno compreenda as características <br>
inerentes à construção de aplicações distribuídas, incluindo passagem de parâmetros, <br>
envolvendo módulos cliente e servidor conectados por um broker RabbitMQ. <br>

O problema consiste na elaboração de um sistema distribuído, com diálogo intermediado <br>
por um broker, no modelo publish-subscriber. O cliente passa um vetor para o servidor,<br>
onde o servidor realiza as operações necessárias e retorna o valor esperado para o <br>
cliente, de modo que ele possa imprimir o resultado para o usuário. <br>

A solução proposta era pra funcionar de acordo com a imagem acima, porém houveram <br>
alguns contratempos. O fluxo funciona corretamente, porém não houve tempo hábil para <br>
adaptação de acordo com os requisitos do problema. <br>

Pode-se perceber no fluxo que o cliente não envia a mensagem diretamente para a fila <br>
**Q1**, e sim para o *exchange*, que é o intermediário entre os dois. Com isso, o <br>
*exchange* recebe a mensagem do cliente e envia para a fila. Após isso, o servidor <br>
consome as mensagens da fila **Q1** e envia para a fila **find_p_bs_n**. Após os *works* <br>
processarem as mensagens, eles retornam os valores para a fila **found_p_bs_n** por meio <br>
do *reply_to*. O servidor então consome esses dados e envia para a fila **found_bs_n**, <br>
por meio do *reply_to*, onde enfim o cliente **P** tem acesso. <br>


## Execução dos programas

Para executar os arquivos, o **Rabbitmq** precisa ser previamente <br>
inicializado. Recomendamos a utilização do **docker**. Para tanto, faça:<br>

```docker run -it --rm --name rabbitmq -p 5672:5672 -p 15672:15672 rabbitmq:3.9-management```


Também é necessário que execute os seguintes comandos via terminal: <br>

```
python server.py
python worker.py
python publisher.py
```
