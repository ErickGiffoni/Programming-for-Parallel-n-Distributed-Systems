from typing import List
import pika, uuid;

connection = pika.BlockingConnection(
   pika.ConnectionParameters(host="localhost")
);

worker_connection = pika.BlockingConnection(
   pika.ConnectionParameters(host="localhost")
);
worker_channel = worker_connection.channel();

channel = connection.channel();

exchange_name = "find_bs_n";  # routing_key para acessar a exchange
# channel.exchange_declare(
#    durable=True,
#    exchange=exchange_name,
#    exchange_type="direct"
# );

queue_1 = channel.queue_declare(
   queue="",
   durable=True,
   exclusive=True
).method.queue;

channel.queue_bind(
   exchange=exchange_name,
   queue=queue_1,
   routing_key=exchange_name
);

def find_bs_n_callback(ch, method, props, body):
   print("[x] Recebi um pedido...");

   # divifir o trabalho p/ os workers

   def find_partial_result():
      find_p_bs_n_queue_name = "find_p_bs_n";   # find partial biggest and smallest number
      worker_channel.queue_declare(
         durable=True,
         queue=find_p_bs_n_queue_name
      );

      message = "sending a hello world";

      callback_queue_name = "found_p_bs_n";
      found_p_bs_n_queue = worker_channel.queue_declare(
         queue=callback_queue_name, 
         durable=True, 
         exclusive=True
      ).method.queue;

      message_count = 3;
      i=0;
      # correlation_id: List = [];
      while i < message_count:
         print("sending %r of %r" % (i+1, message_count));
         correlation_id.append(str(uuid.uuid4())); # problema na hora de verificar a resposta q chega
         worker_channel.basic_publish(
            body=message,
            routing_key=find_p_bs_n_queue_name,
            properties=pika.BasicProperties(
               correlation_id=correlation_id[i],
               reply_to=found_p_bs_n_queue,
               delivery_mode=2,
               content_type=None
            ),
            exchange=""
         );
         i+=1;

      def partial_response_callback(ch, method, properties, body):
         #global p_result;
         #if properties.correlation_id in correlation_id:
         p_result.append(str(body));
         # print("p_result: %r" % p_result);
         # print("-> corr_id %r" % correlation_id);
         ch.basic_ack(delivery_tag=method.delivery_tag);
         #else:
            # print("X. different correlation id");
            # print("corr_id: %r" % correlation_id);
            # print("props corr_id %r" % properties.correlation_id);
         return;

      worker_channel.basic_qos(prefetch_count=1);
      worker_channel.basic_consume(
         queue=callback_queue_name,
         on_message_callback=partial_response_callback
      );

      print("[..] esperando respostas dos workers");
      while len(p_result) < message_count:
         worker_connection.process_data_events();
      #correlation_id: List = [None];

   find_partial_result();
   print("-> mensagens chegaram: %r" % p_result);
   # mandar o resultado final

   result = "OK!!!";
   ch.basic_publish(
      exchange="",
      routing_key=props.reply_to,
      properties=pika.BasicProperties(
         correlation_id=props.correlation_id
      ),
      body=result
   );
   ch.basic_ack(delivery_tag=method.delivery_tag);

   print("-> resposta enviada");

   return;

channel.basic_qos(prefetch_count=1);
channel.basic_consume(
   queue=queue_1,
   exclusive=True,
   on_message_callback=find_bs_n_callback
);

print("[.] Esperando uma publicação...");
p_result: List = [];
correlation_id: List = [];
channel.start_consuming();
