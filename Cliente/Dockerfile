FROM ubuntu

COPY . /home/Cliente/

WORKDIR /home/Cliente

RUN apt-get update -y \
    && apt-get install gcc -y
    
RUN apt-get install net-tools && ifconfig -a

RUN gcc DocClient.c -o DocClient
