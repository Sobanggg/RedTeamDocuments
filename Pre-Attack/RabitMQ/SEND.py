import pika
import sys,os
credentials = pika.PlainCredentials('test', 'test')
connection = pika.BlockingConnection(pika.ConnectionParameters('79.110.52.18',5672,'/',credentials))
channel = connection.channel()

channel.queue_declare(queue='channel-1')

channel2 = connection.channel()

channel2.queue_declare(queue='channel-2')

#channel.basic_publish(exchange='', routing_key='channel-1', body='172.67.74.136::443')
#channel.basic_publish(exchange='', routing_key='channel-1', body='172.67.74.136::441')
channel.basic_publish(exchange='', routing_key='channel-1', body='172.67.74.136::80')

print(" [x] Sent Data")


def callback(ch, method, properties, body):
    print(body.decode('ascii'))
    connection.close()
    sys.exit(0)
    os._exit(0)
    
print(' [*] Waiting for message Result')
channel.basic_consume(queue='channel-2', on_message_callback=callback, auto_ack=True)
channel.start_consuming()



