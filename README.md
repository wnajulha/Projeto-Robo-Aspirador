# 🤖 Simulação de Robô Aspirador (Agente Reflexo)

Este projeto implementa um **Agente Reflexo Simples** em C que simula o comportamento de um robô aspirador em uma grade bidimensional. O robô toma decisões em tempo real com base apenas no que percebe na sua célula atual e nas adjacentes.

## 🎯 Objetivo
Limpar o máximo de sujeira (`*`) em um mapa contendo obstáculos (`#`), respeitando um limite de tempo/passos (`T`).

## 🧠 Lógica de Decisão (Hierarquia)
O robô segue estas regras de prioridade em cada turno:
1. **Regra 1 (Limpar):** Se a célula atual estiver suja, limpa imediatamente.
2. **Regra 2 (Atração):** Se houver sujeira ao Norte, Sul, Leste ou Oeste, move-se para ela.
3. **Regra 3 (Varrer):** Caso não veja sujeira, segue um padrão sistemático de **Zig-Zag**.
4. **Fallback:** Se encontrar um obstáculo ou limite, tenta uma direção alternativa para destravar.



## 🛠️ Especificações Técnicas
- **Linguagem:** C
- **Estruturas de Dados:** - Matriz para representação do ambiente.
  - **Log (Buffer Circular):** Armazena as últimas 64 ações do robô para histórico e depuração.
- **Modo Passo-a-Passo:** Visualização em tempo real do robô (`R`) no mapa via terminal.

## 📊 Métricas de Saída
Ao final da execução, o programa gera um relatório com:
- Total de passos e limpezas realizadas.
- Quantidade de bloqueios (colisões com obstáculos).
- **Percentual de eficiência** (sujeira limpa vs. sujeira total).
- Tempo de processamento (CPU).



## 🚀 Como Rodar
1. Compile o código:
   ```$ gcc robo.c -o robo.exe
2. 
   ```./robo < mapa_facil.txt
