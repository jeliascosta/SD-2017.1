#!/bin/bash
java -classpath /home/kldev/NetBeansProjects/RMI/build/classes/ -Djava.rmi.server.codebase=file:/home/kldev/NetBeansProjects/RMI/build/classes/ rmi.Server &
