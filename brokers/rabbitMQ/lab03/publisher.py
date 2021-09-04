import pika, uuid;

connection = pika.BlockingConnection(
   pika.ConnectionParameters(host="localhost")
);

channel = connection.channel();

exchange_name = "find_bs_n";  # find biggest and smallest number
channel.exchange_declare(
   durable=True,
   exchange=exchange_name,
   exchange_type="direct"
);

message = "sending a hello world";
correlation_id = str(uuid.uuid4());

callback_queue_name = "found_bs_n";
found_bs_n_queue = channel.queue_declare(
   queue=callback_queue_name, 
   durable=True, 
   exclusive=True
).method.queue;

channel.basic_publish(
   body=message,
   exchange=exchange_name,
   routing_key=exchange_name,
   properties=pika.BasicProperties(
      correlation_id=correlation_id,
      reply_to=found_bs_n_queue,
      delivery_mode=2,
      content_type=None
   )
);

result = None;

def response_callback(ch, method, properties, body):
   global result;

   if properties.correlation_id == correlation_id:
      result = str(body);
      print("[x] Resposta foi %r" % result);
   return;

channel.basic_qos(prefetch_count=1);
channel.basic_consume(
   queue=callback_queue_name,
   auto_ack=True,
   on_message_callback=response_callback
);

print("[.] Esperando resposta...");
while result is None:
   connection.process_data_events();
