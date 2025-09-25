// POSICIONAMENTO DOS NAVIOS - JOGADOR 1
    printf("\nJOGADOR 1 - Posiciona os teus %d navios:\n", NAVIOS);
    for (i = 0; i < NAVIOS; i++) {
        do {
            printf("Navio %d (linha coluna): ", i + 1);
            scanf("%d %d", &x, &y);
        } while (x < 0 || x >= n || y < 0 || y >= n || tab1[x][y] == 'N');

        tab1[x][y] = 'N';
    }

    // POSICIONAMENTO DOS NAVIOS - JOGADOR 2
    printf("\nJOGADOR 2 - Posiciona os teus %d navios:\n", NAVIOS);
    for (i = 0; i < NAVIOS; i++) {
        do {
            printf("Navio %d (linha coluna): ", i + 1);
            scanf("%d %d", &x, &y);
        } while (x < 0 || x >= n || y < 0 || y >= n || tab2[x][y] == 'N');

        tab2[x][y] = 'N';
    }
    }