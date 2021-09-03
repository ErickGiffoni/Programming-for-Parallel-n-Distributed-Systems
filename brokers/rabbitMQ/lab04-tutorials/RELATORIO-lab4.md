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

Entre na pasta ```hello-world```.<br>

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

![WQ](https://rabbitmq.com/img/tutorials/python-two.png) <br>

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

Entre na pasta ```work-queues```.<br>

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

O terceiro experimento consiste no envio de mensagens para vários consumidores diferentes. <br>
Nesse caso a configuração do *exchange* foi definida com *fanout*, que basicamente faz com <br>
todas as mensagens sejam transmitidas para todas as filas que conhece. <br>

**Visão geral:**<br>

Após a definição das características do *exchange*, é necessário que existam as filas para <br>
que os logs sejam enfileirados. Para isso, ao invés de definir um nome específico para a fila, <br>
podemos definir um argumento vazio para *queue_declare*, que garantirá que o sistema gerará um <br>
nome aleatório para nossa fila. Em seguida um bind é adicionado ao programa para que o *exchange* <br>
possa se relacionar com a *queue*, que chamados de ligação. <br>

![PS](https://rabbitmq.com/img/tutorials/python-three-overall.png) <br>

A imagem acima mostra como o sistema funciona. O produtor envia a mensagem, onde essa é recebida pelas <br>
filas existentes e são logo consumidas por C¹ e C². <br>

**Uso:** <br>

Entre na pasta ```publish-subscribe```.<br>

- Shell 1 (caso queira salvar os logs em arquivos) <br>
-- ```python receive_logs.py > logs_from_rabbit.log``` <br>

- Shell 2 (caso queira ver os logs no terminal) <br>
-- ```python receive_logs.py``` <br>

- Shell 3 (para criar os logs)<br>
-- ```python emit_log.py``` <br>


### Routing

O quarto experimento consiste no direcionamento de mensagens para filas específicas, com base na sua <br>
categoria. <br>

**Visão geral:**<br>

Entramos com um novo conceito, que é a chave de ligação. Anteriormente, utilizamos o *fanout* <br>
como um *exchange_type*. Isso não garante muitas possibilidades, já que a transmissão não tinha um direcionamento. <br>
Agora a chave direta é uma opção mais formidável, haja vista que mensagens serão transmitidas para as filas com <br>
base na chave de ligação, ou seja, o direcionamento das mensagens para as filas é feito quando a chave de ligação <br>
corresponde exatamente à chave de roteamento da mensagem.<br>

![R](https://rabbitmq.com/img/tutorials/direct-exchange.png) <br>

A imagem acima ilustra bem a ideia supracitada, onde existe uma ligação direta, determinada pelas chaves de ligação <br>
*orange, black e green*, onde, cada qual identifica a fila que deve receber os dados. Em tal configuração, uma mensagem <br>
publicada na central com uma chave de roteamento *orange* será roteada para a fila Q1 . Mensagens com uma chave de <br>
roteamento *black ou green* irão para Q2 . Todas as outras mensagens serão descartadas. <br>


**Uso:** <br>

Entre na pasta ```routing_key```.<br>

- Shell 1 (caso queira salvar os logs em arquivos) <br>
-- ```python receive_logs_direct.py warning error> logs_from_rabbit.log``` <br>

- Shell 2 (caso queira ver os logs no terminal) <br>
-- ```python receive_logs_direct.py info warning error``` <br>

- Shell 3 (para criar os logs)<br>
-- ```python emit_log_direct.py error "Run. Run. Or it will explode."``` <br>


### Topics

O quinto experimento consiste no direcionamento de mensagens para filas específicas, com base na sua <br>
categoria (gravidade), igual no anterior, porém com uma diferença fundamental, que é, também com base <br>
na fonte que emitiu o *log*. <br>


**Visão geral:**<br>

Essa nova diferença permite um espectro muito mais amplo na captação dos *logs*. Conforme a imagem <br>
abaixo, podemos entender melhor um pouco desse funcionamento. Inicialmente podemos perceber que existe <br>
um padrão na chave de ligação, que é definido como sendo 3 palavras para esse problema. Na imagem <br>
podemos perceber o uso do asterisco e da hashtag. Respectivamente, elas denotam que: pode substituir <br>
exatamente uma palavra e pode substituir zero ou mais palavras. <br>

Pode-se perceber que Q1 está interessado em todos os animais da cor laranja e Q2 quer tudo sobre as <br>
outras duas características. Caso seja apresentada chaves que não sejam identificadas, essa mensagem <br>
será descartada. <br>

O código é bem parecido com o anterior, mudando apenas que as chaves de roteamento dos logs terão duas <br>
palavras.

![R](https://rabbitmq.com/img/tutorials/python-five.png) <br>




**Uso:** <br>

Entre na pasta ```topics```.<br>

- Shell 1 (Para receber todos os logs) <br>
-- ```python receive_logs_topic.py "#"``` <br>

- Shell 2 (Para receber todos os logs da instalação " kern ") <br>
-- ```python receive_logs_topic.py "kern. *"``` <br>

- Shell 3 (Ou se você quiser ouvir apenas sobre registros " críticos ")<br>
-- ```python receive_logs_topic.py "* .critical"``` <br>

- Shell 4 (Você pode criar várias ligações) <br>
-- ```python receive_logs_topic.py "kern. *"  "* .critical"``` <br>

- Shell 5 (E para emitir um log com uma chave de roteamento do tipo " kern.critical ")<br>
-- ```python emit_log_topic.py "kern.critical"  "Um erro crítico de kernel"``` <br>



### RPC

O sexto experimento traz consigo a ideia do RPC (Remote Procedure Call), que nada mais <br>
é do que o processamento de tarefas dentro de um servidor, onde o mesmo retorna algum  <br>
resultado para o cliente. <br>

**Visão geral:**<br>

Inicialmente é criado uma classe de cliente, onde existirá um método chamado *call*, que <br>
envia uma solicitação RPC e fica bloqueado até que se receba uma resposta. <br>
Dentro dessa comunicação, podemos criar uma fila de retorno de chamada para cada <br>
solicitação RPC. Isso é ineficiente. Temos outra forma então, que é criar uma fila de <br>
retorno de chamada para cada cliente. Utilizamos então a correlation_id. <br>

![R](https://rabbitmq.com/img/tutorials/python-six.png) <br>

* Na inicialização do cliente, ele cria a fila de retorno de chamada
* No envio da mensagem, temos o *reply_to e correlation_id*
* A solicitação é enviada para uma fila *rpc_queue*
* O servidor aguarda alguma atividade na fila e retorna um resultado na fila do campo *reply_to*
* O cliente aguarda o retorno e compara o valor da solicitação com o *correlation_id* <br>

**Uso:**<br>

Entre na pasta ```rpc```.<br>

- Shell 1 (inicia o servidor) <br>
-- ```python rpc_server.py``` <br>

- Shell 2 (inicia o cliente) <br>
-- ```python rpc_client.py``` <br>


### Publisher-confirms

Para o sétimo e último experimento, só precisamos codificar o lado do *producer*,<br>
que é um **publisher**. Isso porque a ideia do experimento é mostrar a possibilidade<br>
de utilizar o *broker* Rabbitmq para enviar uma confirmação de recebimento de uma<br>
mensagem.<br>

**Visão geral:**<br>

Funciona da seguinte maneira: o *broker* recebe a mensagem publicada pelo *publisher*<br>
e manda de volta um reconhecimento, dizendo que a mensagem foi recebida. Dessa ma-<br>
neira, quem publicou a mensagem entende que ela foi tratada no lado do servidor.<br>

Há basicamente 3 formas de fazer isso acontecer:<br>

- A primeira maneira é a publicação individual de mensagens, ou seja, para cada <br>
mensagem enviada, espera-se o reconhecimento chegar para, então, prosseguir para<br>
a próxima publicação;<br>

- A segunda maneira é parecida com a anterior, porém utiliza-se *batches* ao invés<br>
de mensagens individuais. Assim, envia-se um conjunto de dados por vez;<br>

- A terceira maneira é assíncrona. Cada mensagem é publicada individualmente, porém<br>
usa-se funções de *callback* para tratar de modo assíncrono a resposta devolvida<br>
pelo *broker*.<br>

Essas 3 formas estão implementadas em Java no programa `PublisherConfirms.java`.<br>
A tabela a seguir apresenta prós e contras sobre as abordagens mencionadas:<br>

| Abordagem | Prós | Contras |
| :-------: | :--: | :-----: |
| Publicação individual +<br>reconhecimento síncrono | é simples | taxa de transferência limitada |
| Publicação em *batch* +<br>reconhecimento síncrono | <li>é simples<br><li>taxa de transferência razoável | difícil de entender e tratar quando<br>algum reconhecimento dá errado |
| Modo assíncrono | <li>melhor desempenho e uso de recursos<br><li>bom controle de erro | pode ser difícil de entender e<br>implementar corretamente |

**Uso:**<br>

Entre na pasta `publisher-confirms`.<br>

> Obs.: é necessário ter o Java pré-instalado<br>

- 1\) Baixe as dependências do Rabbitmq para java<br>
-- ```source get-jar-files.sh```<br>

- 2\) Configure a variável de ambiente para apontar para as dependências baixadas<br>
-- ```source set-env-var.sh```<br>

- 3\) Compile e execute o programa `PublisherConfirms.java`<br>
-- ```source compile-execute-java.sh```<br>

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
