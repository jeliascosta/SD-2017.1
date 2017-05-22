#!/bin/bash
export CLASSPATH=/home/kldev/NetBeansProjects/RMI/build/classes/
for k in 1 2 4 8 16 32 64
do
  printf "\n\nFunção Logaritmo\n\n"
  killall -9 java && killall -9 rmiregistry && sleep 2
  rmiregistry&
  ./servidor.sh && sleep 2
  for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  do
    ./client.sh $k LOGARITMO
  done
  printf "\n\nFunção Potência\n\n"
  killall -9 java && killall -9 rmiregistry && sleep 2
  rmiregistry&
  ./servidor.sh && sleep 2
  for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  do
    ./client.sh $k POTENCIA
  done
  printf "\n\nFunção Seno\n\n"
  killall -9 java && killall -9 rmiregistry && sleep 2
  rmiregistry&
  ./servidor.sh && sleep 2
  for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  do
    ./client.sh $k SENO
  done
  printf "\n\nFunção Média\n\n"
  killall -9 java && killall -9 rmiregistry && sleep 2
  rmiregistry&
  ./servidor.sh && sleep 2
  for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  do
    ./client.sh $k MEDIA
  done
  printf "\n\nFunção ContaMaiorQue\n\n"
  killall -9 java && killall -9 rmiregistry && sleep 2
  rmiregistry&
  ./servidor.sh && sleep 2
  for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  do
    ./client.sh $k CONTAMAIORQUE
  done
  printf "\n\nFunção ContaIntervalo\n\n"
  killall -9 java && killall -9 rmiregistry && sleep 2
  rmiregistry&
  ./servidor.sh && sleep 2
  for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  do
    ./client.sh $k CONTAINTERVALO
  done
done
