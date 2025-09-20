# 🎓 Projeto Académico - Batalha Naval em C ⚓💻

Este projeto foi desenvolvido no âmbito das aulas do **Curso Técnico de Programação (IEFP 2025/2026)**.  
O objetivo foi praticar a **construção e manipulação de matrizes em C** através da implementação do jogo **Batalha Naval** numa grelha 6x6.

---

## 📌 Descrição
- A grelha é representada por uma matriz `6x6`.
- Os valores da matriz significam:
  - `0` → Água 🌊
  - `1` → Navio 🚢
  - `2` → Barco ⛵
- O programa cria a matriz, posiciona automaticamente as embarcações e mostra duas versões da grelha:
  - **Revelada** → para testes (mostra todos os valores da matriz).
  - **Oculta** → para o oponente (apenas exibe `.` e `#`).

---
gcc batalha_matriz.c -o batalha_matriz.exe
./batalha_matriz.exe

Grelha (revelada para testes):
  0 1 2 3 4 5
 -------------
0 | 1 2 2 0 0 0
1 | 1 0 0 0 0 0
...

Grelha (oculta para oponente):
  0 1 2 3 4 5
 -------------
0 | # # # . . .
1 | # . . . . .
...
## Projeto desenvolvido por Palmira Solochi no âmbito das aulas em Programação C (IEFP,2025/2026).
# Nota: Este repositório é académico, criado para fins de aprendizagem e prática em sala de aula.

