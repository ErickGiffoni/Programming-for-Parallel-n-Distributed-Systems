import pika
from pika import channel

connection = pika.BlockingConnection(
   pika.ConnectionParameters(host="localhost")
);

channel = connection.channel();

channel.queue_declare(queue="hello");

channel.basic_publish(body="Hello World!", exchange="", routing_key="hello");

print("[x] Sent 'Hello World!'");

connection.close();