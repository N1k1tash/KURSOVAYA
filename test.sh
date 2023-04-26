#!/bin/bash

# 5-ый тестовый сценарий
printf "Использование программы с выводом ошибки\n"
./Client -s 2130706433 -p44444 -i vectors.bin -o resultat.bin -c./config/vclient.conf

