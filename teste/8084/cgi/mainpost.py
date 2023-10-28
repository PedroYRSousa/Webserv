#!/usr/bin/env python
import sys
import os

# Obtenha o tamanho dos dados de entrada a partir da variável de ambiente CONTENT_LENGTH
content_length = int(os.environ.get("CONTENT_LENGTH", 0))

# Leia os dados de entrada com base no tamanho especificado
data = sys.stdin.read(content_length)

# Defina o tipo de conteúdo para text/plain
print("Content-Type: text/plain\r\n\r\n")

# Saída dos dados lidos
print("Data received from stdin (length: {}): ".format(content_length))
print(len(data))
print(data)
