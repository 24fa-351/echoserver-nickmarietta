# 351-echo server

Using TCP, create a server that echoes all messages it receives, line by line.

If you send "abcd"(newline), you should get back "abcd"(newline). Then type another line, the same thing. As long as the connection stays alive. 

Implement a "-p" argument that specifies the port it listens to.

It only needs to accept one connection at a time, but it must accept new connections after one is terminated.

Test this with telnet

## Example Usage:

The usage is intended as:
<./echoserve> (OPTIONAL) *<-p and the specified port>* 

In another terminal, run "telnet" along with the local ip 127.0.0.1 and then the port number.
Keep on typing things and you will receieve what you typed.
