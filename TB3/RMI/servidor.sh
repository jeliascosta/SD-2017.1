#!/bin/bash
java -Xmx4096m -classpath /home/kldev/NetBeansProjects/RMI/build/classes/ -Djava.rmi.server.codebase=file:/home/kldev/NetBeansProjects/RMI/build/classes/ rmi.Server &
