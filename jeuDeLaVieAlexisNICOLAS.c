#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define X_DIM 8 // taille de la grille en x
#define Y_DIM 8 // taille de la grille en y
#define Z_DIM 8 // taille de la grille en z
#define MAX_ITERATIONS 10 // nombre d'iteration

// Fonction pour initialiser la grille de manière aléatoire entre 0 et 1
void initialisationGrille(int grid[X_DIM][Y_DIM][Z_DIM]) {
    for (int x = 0; x < X_DIM; x++) {
        for (int y = 0; y < Y_DIM; y++) {
            for (int z = 0; z < Z_DIM; z++) {
                grid[x][y][z] = rand() % 2;
            }
        }
    }
}

// Fonction pour afficher la grille en x et y, z fois 
void afficheGrille(int grid[X_DIM][Y_DIM][Z_DIM]) {
    for (int x = 0; x < X_DIM; x++) {
        for (int y = 0; y < Y_DIM; y++) {
            for (int z = 0; z < Z_DIM; z++) {
                printf("%d ", grid[x][y][z]);
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
}

// Fonction pour mettre à jour la grille 
void updateGrille(int grille[X_DIM][Y_DIM][Z_DIM], int debutX, int finX, int debutY, int finY, int debutZ, int finZ) {
    int newGrille[X_DIM][Y_DIM][Z_DIM];
    
    for (int x = debutX; x < finX; x++) {
        for (int y = debutY; y < finY; y++) {
            for (int z = debutZ; z < finZ; z++) {
                int voisin = 0;

                // Vérifier les voisins autour de la cellule
                for (int nx = -1; nx <= 1; nx++) {
                    for (int ny = -1; ny <= 1; ny++) {
                        for (int nz = -1; nz <= 1; nz++) {
                            if (nx == 0 && ny == 0 && nz == 0) continue;  // Ignorer la cellule actuelle
                                int nx_pos = (x + nx + X_DIM) % X_DIM;
                                int ny_pos = (y + ny + Y_DIM) % Y_DIM;
                                int nz_pos = (z + nz + Z_DIM) % Z_DIM;
                                voisin += grille[nx_pos][ny_pos][nz_pos];
                        }
                    }
                }

                // Appliquer les règles du Jeu de la Vie
                if (voisin == 6 || voisin == 7 || voisin == 8) {//reste dans l'etat
                    if (grille[x][y][z] == 1) {
                        newGrille[x][y][z] = 1;
                    }
                    else {
                        newGrille[x][y][z] = 0;
                    }
                }else {
                     if (voisin < 9 || voisin > 10) {
                        newGrille[x][y][z] = 0;  // Mort par sous-population ou surpopulation
                    } else {
                        newGrille[x][y][z] = 1;  // Survie ou naissance
                    }
                }
            }
        }
    }

    // Copier les résultats dans la grille
    for (int x = debutX; x < finX; x++) {
        for (int y = debutY; y < finY; y++) {
            for (int z = debutZ; z < finZ; z++) {
                grille[x - debutX][y][z] = newGrille[x][y][z];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, nbProcessus;
    int grille[X_DIM][Y_DIM][Z_DIM];

    //initialisation de mpi 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nbProcessus);

    // Divisez la grille 3D entre les processus MPI
    int xChunks = X_DIM / nbProcessus; //taille de la grille divisé
    int debutX = rank * xChunks; // debut en x dans la grille 
    int finX = (rank + 1) * xChunks; //derniere colonne à utiliser en X

    int grilleTraitement[xChunks][Y_DIM][Z_DIM]; //grille au dimension en x de la taille d'une division

    // Initialisation de la grille sur le processus racine
    if (rank == 0) {
        initialisationGrille(grille);
        printf("Grille:\n");
        afficheGrille(grille);

    }

    int tailleEnvoie[nbProcessus]; // tab de taille d'element à envoyer à chaque processus
    int depart[nbProcessus];

    for (int i = 0; i < nbProcessus; i++) {
        tailleEnvoie[i] = xChunks * Y_DIM * Z_DIM; //nombre de cellule
        depart[i] = i * xChunks * Y_DIM * Z_DIM; //cellule de depart
    }

    //envoie de la division aux processus 
    MPI_Scatterv(grille, tailleEnvoie, depart, MPI_INT, grilleTraitement, xChunks * Y_DIM * Z_DIM, MPI_INT, 0, MPI_COMM_WORLD);

    // Effectuer les itérations du Jeu de la Vie
    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        // Mettre à jour la grille localement
        updateGrille(grilleTraitement, debutX, finX, 0, Y_DIM, 0, Z_DIM);

        // Synchronisation entre les processus
        MPI_Barrier(MPI_COMM_WORLD);

        // Collecter les morceaux mis à jour dans la grille globale avec MPI_Gatherv
        MPI_Gatherv(grilleTraitement, xChunks * Y_DIM * Z_DIM, MPI_INT, grille, tailleEnvoie, depart, MPI_INT, 0, MPI_COMM_WORLD);

        // Afficher la grille après chaque itération
        if (rank == 0) {
            printf("Iteration %d:\n", iteration + 1);
            afficheGrille(grille);
        }
    }

    //ferme l'utilisation de MPI
    MPI_Finalize();
    return 0;

}

