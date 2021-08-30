# Lab04 - Compreendendo funcionalidades do BROKER Rabbitmq

<p align="center">
   <b>Universidade de Brasília</b><br>
Programação para Sistemas Paralelos e Distribuídos<br>
<br>
   Erick Giffoni - 170141161<br>
   Ailton Aires  - 180011600<br>
</p>


TO-DO: contextualizar o objetivo do trabalho

O objetivo desse experimento é que o aluno compreenda algumas funcionalidades <br>
dos brokers, que funcionam como intermediários em conexões envolvendo módulos <br>
cliente e servidor (ou produtor/consumidor), considerando a presença de um intermediário.<br>


TO-DO: explicar **AMQP**

O ***Advanced Message Queuing Protocol*** lero lero...<br>

## Tutoriais realizados

Na página do Rabbitmq na Internet, existe um conjunto de pequenos tutoriais<br>
que mostram diferentes maneiras de se utilizar tal *broker*. Assim, seguimos<br>
cada um desses exemplos e anotamos os entendimentos aprendidos.<br>

Para executar os experimentos relatados abaixo, o **Rabbitmq** precisa ser pre-<br>
viamente inicializado. Recomendamos a utilização do **docker**. Para tanto, faça:<br>

```docker run -it --rm --name rabbitmq -p 5672:5672 -p 15672:15672 rabbitmq:3.9-management```


### Hello-world

No primeiro experimento fomos capazes de entender o funcionamento mais básico<br>
do **Rabbitmq**. A arquitetura desse tutorial baseia-se em um (1) *producer*,<br>
que envia mensagens para uma (1) *fila*, a qual entrega as mensagens para um (1)<br>
*consumer*.<br>

**Visão geral:**<br>

![hello-world](https://www.rabbitmq.com/img/tutorials/python-one-overall.png)

* P: producer<br>
* <p style="color:red">F: fila</p>
* C: consumer<br>

Para o *producer*, o código é o [send.py](./hello-world/send.py). Nele uma conexão<br>
é feita em *localhost* para uma fila de nome "hello". Em seguida, uma publicação<br>
básica é feita com a mensagem "Hello World!", para a fila criada anteriormente,<br>
identificando-a com o parâmetro ```routing_key='hello'```.<br>

Já o *consumer*... TO-DO

**Uso:**<br>

* Producer: ```python send.py```<br>
* Consumer: ```python receive.py```<br>

### Work-queues

TO-DO

### Publish-subscribe

TO-DO

### Routing

TO-DO

### Topics

TO-DO

### RPC

TO-DO

### Publisher-confirms

TO-DO

## Opinião geral

A utilização de **BROKERS** na construção de aplicações distribuídas possibilita<br>
que a comunicação entre cliente e servidor seja persistente. Com isso, desde<br>
que o *broker* esteja sempre em funcionamento, consegue-se uma garantia de que<br>
uma mensagem seja enviada e também recebida.<br>

Essa abordagem, portanto, difere-se daquela realizada em experimentos anteriores<br>
([lab01](https://github.com/ErickGiffoni/Programming-for-Parallel-n-Distributed-Systems/tree/main/rpc-study/lab-01) e [lab02](https://github.com/ErickGiffoni/Programming-for-Parallel-n-Distributed-Systems/tree/main/rpc-study/lab-02)), já que estes apresentavam uma comunicação transiente<br>
usando **RPC** e **sockets**.<br>

É claro que não se pode afimar uma abordagem como sendo "melhor" que outra, uma<br>
vez que cada aplicação -- software ou serviço como um todo -- utiliza uma arqui-<br>
tetura conveniente para o problema que quer resolver. Nesse sentido estamos ape-<br>
nas relatando as diferentes características de cada abordagem.<br>

Como relatado nos experimentos anteriores, ao compararmos uma mesma aplicação<br>
sendo projetada e desenvolvida com **rpcgen** e com **sockets**, vale lembrar<br>
que a utilização da ferramenta de *rpc* (lab01) trouxe algumas facilidades, como<br>
uma programação de mais alto nível. De forma semelhante, a utilização da bliblioteca<br>
**pika**, em linguagem *python*, com o **Rabbitmq** funcionando via **docker**<br>
permite que o programador foque mais na solução lógica do problema e menos nas<br>
questões de comunicação inter-processos e de configuração de rede.<br>
