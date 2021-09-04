import pika;

connection = pika.BlockingConnection(
   pika.ConnectionParameters(host="localhost")
);

channel = connection.channel();
channel.basic_qos(prefetch_count=1);

find_p_bs_n_queue = "find_p_bs_n";   # find partial biggest and smallest number
channel.queue_declare(
   durable=True,
   queue=find_p_bs_n_queue
);

print("[*] Waiting messages... press CTRL+C to exit");

def find_p_bs_n_callback(ch, method, props, body):
   print("[x] Worker recebendo pedido...")

   partial_result = "resultado parcial meu xoven";
   # to-do: calcular resultado do vetor

   ch.basic_publish(
      exchange="",
      routing_key=props.reply_to,
      properties=pika.BasicProperties(
         correlation_id=props.correlation_id
      ),
      body=partial_result
   );
   ch.basic_ack(delivery_tag=method.delivery_tag);

   print("-> resultado parcial enviado...");
   return;

channel.basic_consume(
   queue=find_p_bs_n_queue,
   on_message_callback=find_p_bs_n_callback
);

channel.start_consuming();