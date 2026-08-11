#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define LIN 6
#define COL 6
#define MAX_SHIPS 12   // ajusta conforme o nº total de barcos por jogador

// Convenções:
// Defesa: 0=vazio, 2=barco
// Tiros:  0=desconhecido, 1=água, 2=acerto

typedef struct {
    int size;            // comprimento
    int cells[LIN];      // índices lineares r*COL + c
    int hits;            // células atingidas
    bool placed;         // já colocado
} Ship;

/* ============ Utilitários de matriz ============ */
void clear_int(int m[LIN][COL], int val){
    for(int r=0;r<LIN;r++) for(int c=0;c<COL;c++) m[r][c]=val;
}
void clear_bool(bool m[LIN][COL], bool val){
    for(int r=0;r<LIN;r++) for(int c=0;c<COL;c++) m[r][c]=val;
}

void print_grid_int(const int m[LIN][COL], const char *title){
    printf("\n%s:\n   ", title);
    for (int c=0;c<COL;c++) printf(" %c", 'A'+c);
    printf("\n   ");
    for (int c=0;c<COL;c++) printf("--");
    printf("\n");
    for (int r=0;r<LIN;r++){
        printf("%d |", r+1);
        for (int c=0;c<COL;c++) printf(" %d", m[r][c]);
        printf("\n");
    }
}

bool inside(int r,int c){ return r>=0 && r<LIN && c>=0 && c<COL; }

/* ============ Conversões ============ */
bool parse_coord(const char *txt, int *r, int *c){
    // aceita "A1".."F6" (maiúsculas/minúsculas)
    if (!txt || strlen(txt)<2) return false;
    if (!isalpha((unsigned char)txt[0]) || !isdigit((unsigned char)txt[1])) return false;
    int col = toupper((unsigned char)txt[0]) - 'A';
    int row = (txt[1]-'1');  // linhas 1..6
    if (!inside(row,col)) return false;
    *r=row; *c=col;
    return true;
}

/* ============ Validação e colocação de barcos ============ */
// safe_zone=true impede “encostar” (lados/diagonais)
bool can_place(const int def[LIN][COL], int r, int c, int size, int dir_h, bool safe_zone){
    // cabe?
    if (dir_h){ // horizontal
        if (c+size>COL) return false;
    } else {    // vertical
        if (r+size>LIN) return false;
    }
    // sem sobrepor (defesa só usa 0 e 2)
    for (int k=0;k<size;k++){
        int rr = r + (dir_h?0:k);
        int cc = c + (dir_h?k:0);
        if (def[rr][cc]!=0) return false;
    }
    if (!safe_zone) return true;

    // zona de segurança
    for (int k=0;k<size;k++){
        int rr = r + (dir_h?0:k);
        int cc = c + (dir_h?k:0);
        for (int dr=-1; dr<=1; dr++){
            for (int dc=-1; dc<=1; dc++){
                int r2=rr+dr, c2=cc+dc;
                if (inside(r2,c2) && def[r2][c2]!=0) return false;
            }
        }
    }
    return true;
}

// escreve na defesa (2) e regista as células no Ship
bool place_ship(int def[LIN][COL], int r, int c, int size, int dir_h, Ship *s, bool safe_zone){
    if (!can_place(def,r,c,size,dir_h,safe_zone)) return false;
    s->size=size; s->hits=0; s->placed=true;
    for(int k=0;k<size;k++){
        int rr = r + (dir_h?0:k);
        int cc = c + (dir_h?k:0);
        def[rr][cc] = 2; // barco
        s->cells[k] = rr*COL + cc;
    }
    return true;
}

/* ============ Tiro / acerto / afundou ============ */
// tiros: 0=desconhecido, 1=água, 2=acerto
// def:   0=vazio, 2=barco
// hitsMask marca TRUE onde o barco foi atingido (na matriz de defesa)
bool shoot(int def[LIN][COL], int tiros[LIN][COL], bool hitsMask[LIN][COL],
           Ship ships[], int nShips, int r, int c){
    if (!inside(r,c)) return false;
    if (tiros[r][c] != 0) return false; // já atirado

    if (def[r][c] == 2){
        tiros[r][c] = 2;        // acerto
        hitsMask[r][c] = true;  // marcar que esta célula do barco foi atingida
        // atualizar hits do barco atingido
        int idx = r*COL + c;
        for (int i=0;i<nShips;i++){
            if (!ships[i].placed) continue;
            for (int k=0;k<ships[i].size;k++){
                if (ships[i].cells[k]==idx){
                    ships[i].hits++;
                    break;
                }
            }
        }
        return true;
    } else { // 0 = vazio (na defesa)
        tiros[r][c] = 1; // água
        return false;
    }
}

bool is_sunk(const Ship *s){ return s->placed && s->hits>=s->size; }

/* ============ Demonstração ============ */
int main(void){
    // 4 matrizes
    int defJ1[LIN][COL], tirosJ1[LIN][COL];
    int defJ2[LIN][COL], tirosJ2[LIN][COL];
    clear_int(defJ1,0); clear_int(tirosJ1,0);
    clear_int(defJ2,0); clear_int(tirosJ2,0);

    // Máscaras de acertos nas defesas (para saber o que já foi atingido)
    bool hitsMaskJ1[LIN][COL], hitsMaskJ2[LIN][COL];
    clear_bool(hitsMaskJ1,false); clear_bool(hitsMaskJ2,false);

    // Registos de barcos
    Ship shipsJ1[MAX_SHIPS] = {0}; int nJ1=0;
    Ship shipsJ2[MAX_SHIPS] = {0}; int nJ2=0;

    // --- Colocação (exemplos) ---
    // J1: navio tam 3 em A1 horizontal; barco tam 2 em C3 vertical (safe_zone = true)
    int r,c;
    parse_coord("A1",&r,&c);
    if (place_ship(defJ1, r, c, 3, /*H*/1, &shipsJ1[nJ1], true)) nJ1++;

    parse_coord("C3",&r,&c);
    if (place_ship(defJ1, r, c, 2, /*V*/0, &shipsJ1[nJ1], true)) nJ1++;

    // J2: navio tam 3 em D6 horizontal (começa em B6 para caber); barco tam 2 em B2 vertical
    parse_coord("B6",&r,&c);
    if (place_ship(defJ2, r, c, 3, /*H*/1, &shipsJ2[nJ2], true)) nJ2++;

    parse_coord("B2",&r,&c);
    if (place_ship(defJ2, r, c, 2, /*V*/0, &shipsJ2[nJ2], true)) nJ2++;

    // Mostrar defesas (só 0 e 2)
    print_grid_int(defJ1, "Defesa J1 (0=vazio, 2=barco)");
    print_grid_int(defJ2, "Defesa J2 (0=vazio, 2=barco)");

    // --- Tiros de J1 sobre J2 ---
    parse_coord("B2",&r,&c);
    bool hit = shoot(defJ2, tirosJ1, hitsMaskJ2, shipsJ2, nJ2, r, c);
    printf("\nJ1 atira em B2: %s\n", hit? "ACERTOU":"ÁGUA");

    parse_coord("F6",&r,&c);
    hit = shoot(defJ2, tirosJ1, hitsMaskJ2, shipsJ2, nJ2, r, c);
    printf("J1 atira em F6: %s\n", hit? "ACERTOU":"ÁGUA");

    // --- Tiros de J2 sobre J1 ---
    parse_coord("A1",&r,&c);
    hit = shoot(defJ1, tirosJ2, hitsMaskJ1, shipsJ1, nJ1, r, c);
    printf("J2 atira em A1: %s\n", hit? "ACERTOU":"ÁGUA");

    // Mostrar matrizes de tiros (0=desconhecido,1=água,2=acerto)
    print_grid_int(tirosJ1, "Tiros de J1 (sobre J2)");
    print_grid_int(tirosJ2, "Tiros de J2 (sobre J1)");

    // Verificar afundamentos em J2
    for (int i=0;i<nJ2;i++){
        if (is_sunk(&shipsJ2[i])){
            printf("Barco de J2 de tamanho %d AFUNDOU.\n", shipsJ2[i].size);
        }
    }

    return 0;
}
