import pika, sys, os
import subprocess
def main():
    credentials = pika.PlainCredentials('test', 'test')
    connection = pika.BlockingConnection(pika.ConnectionParameters('79.110.52.18',5672,'/',credentials))
    channel = connection.channel()

    channel.queue_declare(queue='channel-1')

    channel2 = connection.channel()

    channel2.queue_declare(queue='channel-2')
    
    def callback(ch, method, properties, body):
        body = body.decode('ascii') 
        data = body.split('::') #PORTSCAN::IP::PORT
        command = ['python', 'port.py',data[0],data[1]]
        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        result , err = process.communicate()
        
        print(result.decode('ascii'))
        channel2.basic_publish(exchange='', routing_key='channel-2', body=result.decode('ascii'))
        
    channel.basic_consume(queue='channel-1', on_message_callback=callback, auto_ack=True)

    print(' [*] Waiting for messages. To exit press CTRL+C')
    channel.start_consuming()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('Interrupted')
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)
