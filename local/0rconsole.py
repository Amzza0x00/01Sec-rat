#!/usr/bin/env python
import socket,select,sys
server=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
if not server:
    print("create socket error\n")
    exit(1);
server.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
server.bind(('0.0.0.0',1))
server.listen(5)
socks=[server,sys.stdin]
zombies = dict

def parse_cmdline(s) :
    

while 1:
    rs,ws,es=select.select(socks,[],[],1)
    for r in rs:
        if r is server:
            clientsock,clientaddr=r.accept();
            socks.append(clientsock);
            zombies[clientsock] = dict(output_not_empty = false)

        elif r is sys.stdin:
                #print (sys.stdin.readline())
                line = sys.stdin.readline()

        else:
            data=r.recv(1024);
            if not data:
                socks.remove(r);
            else:
                print (data)

