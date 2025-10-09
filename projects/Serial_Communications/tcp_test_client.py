#!/usr/bin/env python3
"""
Simple TCP test client for Serial_Communications demo
Connects to localhost:9001, sends a handshake, and echoes received data.
"""
import socket
import sys

HOST = '127.0.0.1'
PORT = 9001

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((HOST, PORT))
        print(f'Connected to {HOST}:{PORT}')
        s.sendall(b'Hello from python client\r\n')

        while True:
            data = s.recv(1024)
            if not data:
                print('Connection closed by peer')
                break
            print('RX:', data.decode(errors='replace'))
            # Echo back
            msg = input('send> ')
            if msg.strip().lower() in ('quit', 'exit'):
                break
            s.sendall((msg + '\r\n').encode())

    except ConnectionRefusedError:
        print('Connection refused. Is the TCP bridge running and is SimulIDE configured?')
    except KeyboardInterrupt:
        print('\nInterrupted by user')
    finally:
        s.close()

if __name__ == '__main__':
    main()
