import pika, sys;

connection = pika.BlockingConnection(
   pika.ConnectionParameters(host="localhost")
);

channel = connection.channel();

queue_name = "task-queue";
channel.queue_declare(durable=True, queue=queue_name); # persistencia da fila

message = " ".join(sys.argv[1:]) or "Hello World!";

channel.basic_publish(
   body=message,
   routing_key=queue_name,
   exchange="",
   properties=pika.BasicProperties(
      delivery_mode=2   # garante a persistencia da mensagem
   )
);

print("[x] Sent message %r" % message);

connection.close();