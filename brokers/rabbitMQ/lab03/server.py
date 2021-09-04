import pika;

connection = pika.BlockingConnection(
   pika.ConnectionParameters(host="localhost")
);

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
channel.start_consuming();