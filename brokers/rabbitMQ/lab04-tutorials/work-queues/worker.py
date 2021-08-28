import pika, time, sys;

def main():
   connection = pika.BlockingConnection(
      pika.ConnectionParameters(host="localhost")
   );

   channel = connection.channel();
   channel.basic_qos(prefetch_count=1); # rabbitMQ vai mandar uma mensagem da fila por vez
   # in other words, don't dispatch a new message to a worker until it has processed and 
   # acknowledged the previous one
   # Erro com basic_qos : https://stackoverflow.com/a/12426980/10536382

   queue_name = "task-queue";
   channel.queue_declare(durable=True, queue=queue_name); # persistencia da fila

   print("[*] Waiting messages... press CTRL+C to exit");


   def callback(ch, method, properties, body):
      print("[x] Received %r" % body.decode());
      time.sleep(body.count(b'.'));
      print("[x] Done :)");
      channel.basic_ack(delivery_tag=method.delivery_tag) # manda o ack para o rabbitmq

   channel.basic_consume(on_message_callback=callback, queue=queue_name);

   channel.start_consuming();

if __name__ == "__main__":
   try:
      main();
   except KeyboardInterrupt:
      print(" Interrupted");
      sys.exit(0);
