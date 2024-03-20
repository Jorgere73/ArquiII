#!/bin/bash

#Limpiar terminal
clear

javac Veiga.java

if [ $? -eq 0 ]; then

    java Veiga
else
    echo "Error de compilacion. No se puede ejecutar Veiga."
fi    
