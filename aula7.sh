#!/bin/bash

# Compilar o programa C++
g++ -o aula7 aula7.cpp

# Verifica se a compilação foi bem-sucedida
if [ $? -ne 0 ]; then
    echo "Erro: A compilação falhou."
    exit 1
fi

# Executar o programa compilado para cada arquivo que começa com 'entrada' e termina com '.txt'
for file in entrada*.txt; do
    if [ -f "$file" ]; then
        # Gerar o nome do arquivo de saída substituindo 'entrada' por 'saida'
        output_file=$(echo "$file" | sed 's/entrada/saida/')

        echo "Executando aula7 para o arquivo: $file"
        echo "Salvando a saída em: $output_file"

        # Executar o programa e redirecionar tanto a saída padrão quanto erros para o arquivo de saída
        ./aula7 "$file" > "$output_file" 2>&1

        echo "Execução completa para $file."
        echo "---------------------------------------"
    else
        echo "Nenhum arquivo de entrada encontrado."
    fi
done
