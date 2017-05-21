#!/bin/bash
java -Xmx4096m -classpath $CLASSPATH rmi.Client $1 $2
