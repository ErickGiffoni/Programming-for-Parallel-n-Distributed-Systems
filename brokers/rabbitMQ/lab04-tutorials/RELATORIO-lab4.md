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



O ***Advanced Message Queuing Protocol*** é um protocolo avançado de enfileiramento de mensagens <br>
presente no RabbitMQ. Esse protocolo se encontra dentro da camada de aplicação e é comumente <br> 
utilizado em *Message Oriented Middleware (MOM)*. Muito utilizado em sistemas distribuídos na <br>
transmissão de mensagens, o **AMPQ** "conecta sistemas, alimenta os processos de negócios com as <br>
informações de que precisam e transmite de forma confiável as instruções que alcançam seus objetivos" [1]. <br>
As principais características do **AMPQ** são: **seguranca, confiabilidade, interoperabilidade, <br>
orientação de mensagens, enfileiramento e roteamento (point-to-point e publish-and-subscribe)**.<br>
Um breve esquema de funcionamento do **AMPQ**, análogo aos Correios,

![AMPQ](https://www.embarcados.com.br/wp-content/uploads/2018/08/amqp-structure.png)
<h6><p align="center"> Disponível em https://www.embarcados.com.br/amqp-protocolo-de-comunicacao-para-iot/ </p></h6><br>
   
onde: <br>
* Publisher: remetente
* Exchange: caixa de correios
* Broker: Correios
* Bindings: logística
* Queue: caixa postal
* Consumer: destinatário <br>

Vale lembrar que os *publishers* nunca enviam mensagens diretas para as *queues*, haja vista que o <br>
*exchange* é quem gerencia e encaminha as mensagens para determinada fila ou descarte, com base nas <br>
no seu tipo e nas configurações do *bind*.

   
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

O segundo experimento simula uma fila de trabalho, onde temos um *producer* que <br>
produz as mensagens, tal qual são encapsuladas e posteriormente enviadas para a <br>
*queue*. A ideia principal é distribuir essas tarefas entre vários *consumers*, <br>
para que elas sejam rapidamente consumidas e finalizadas. Vale lembrar que isso <br>
depende da quantidade de *consumers* que estarão sendo executados, já que as    <br>
tarefas serão compartilhadas entre eles. <br>

**Visão geral:**<br>

![PS](https://rabbitmq.com/img/tutorials/python-two.png) <br>

Conforme apresentado na figura, o produtor (P) encapsula e envia as mensagens para <br>
a fila, com base no *exchange* e nas configurações do *bind*. Essa fila (em vermelho), <br>
por usa vez, é consumida pelos consumidores (C¹ e C²). <br><br>
A fila de tarefas traz uma característica importante, que é a de escalar facilmente o <br>
problema. Nesse caso, podemos paralelizar a realização das tarefas, criando vários <br>
trabalhadores, e com isso escalando o problema. <br><br>
Dentro do problema também foi inserido o *ack(nowledgement)*, que é um suporte de <br>
confirmação de mensagens. A mensagem é recebida pelo consumidor, e caso ela tenha <br>
sido executada sem nenhum problema, o consumidor enviará de volta um ack para que <br>
o RabbitMQ possa destruir a tarefa. Isso evita que a tarefa não seja executada caso <br>
um consumidor encerre e não tenha finalizado a tarefa. Caso o ack não seja recebido, <br>
entende-se que a tarefa não foi executada. Isso é feito basicamente retirando o <br>
sinalizador **auto_ack = true** e configurando corretamente a estrutura. <br><br>
Já para garantir que as mensagens persistam e não se percam mesmo ao reiniciar o <br>
servidor RabbitMQ, declaramos a fila como durável e também as mensagens como persistentes. <br><br>
Também podemos definidr o *prefetch_count = 1* para que não haja sobrecarga na realização <br>
das tarefas por parte dos consumidores. Com isso, o RabbitMQ não irá enviar mais de uma <br>
tarefa por vez para um consumidor, ou seja, ele só receberá outra tarefa quando estiver livre. <br>

**Uso:** <br>

- Shell 1 <br>
-- ```python worker.py```<br>

- Shell 2 <br>
-- ```python worker.py```<br>

- Shell 3 <br>
-- ```python new_task.py First message.``` <br>
-- ```python new_task.py Second message..``` <br>
-- ```python new_task.py Third message...``` <br>
-- ```python new_task.py Fourth message....``` <br>
-- ```python new_task.py Fifth message.....``` <br>


### Publish-subscribe




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
