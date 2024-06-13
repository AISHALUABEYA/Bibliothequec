#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure pour repr�senter un livre
typedef struct {
    char titre[100];
    char auteur[100];
    int annee;
    int disponible;
    time_t date_emprunt;
} Livre;

// Structure pour repr�senter un emprunt
typedef struct {
    char nom_emprunteur[100];
    time_t date_emprunt;
    time_t date_retour_prevue;
} Emprunt;

// Fonction pour afficher un livre
void afficherLivre(Livre livre) {
    printf("Titre : %s\n", livre.titre);
    printf("Auteur : %s\n", livre.auteur);
    printf("Annee de publication : %d\n", livre.annee);
    if (livre.disponible) {
        printf("Disponibilit� : Disponible\n");
    } else {
        printf("Disponibilit� : Emprunt�\n");
    }
}

// Fonction pour afficher tous les livres de la biblioth�que
void afficherBibliotheque(Livre bibliotheque[], int nbLivres) {
    printf("Biblioth�que :\n");
    for (int i = 0; i < nbLivres; i++) {
        printf("\nLivre %d :\n", i+1);
        afficherLivre(bibliotheque[i]);
    }
    printf("\n");
}

// Fonction pour ajouter un nouveau livre
void ajouterLivre(Livre bibliotheque[], int *nbLivres, char* nomFichier) {
    Livre nouvelLivre;

    printf("Entrez l'ann�e de publication du livre : ");
    scanf("%d", &nouvelLivre.annee);
    printf("Entrez le titre du livre : ");
    scanf(" %[^\n]s", nouvelLivre.titre);
    printf("Entrez l'auteur du livre : ");
    scanf(" %[^\n]s", nouvelLivre.auteur);
    nouvelLivre.disponible = 1;

    bibliotheque[*nbLivres] = nouvelLivre;
    (*nbLivres)++;

    printf("Le livre a �t� ajout� avec succ�s � la biblioth�que.\n");

    sauvegarderBibliotheque(bibliotheque, *nbLivres, nomFichier);
}

// Fonction pour emprunter un livre
void emprunterLivre(Livre bibliotheque[], int nbLivres, Emprunt *emprunt) {
    int choix;
    time_t now;

    afficherBibliotheque(bibliotheque, nbLivres);

    printf("Quel livre voulez-vous emprunter ? (num�ro du livre) : ");
    scanf("%d", &choix);
    choix--;

    if (choix < 0 || choix >= nbLivres) {
        printf("Choix invalide.\n");
        return;
    }

    if (!bibliotheque[choix].disponible) {
        printf("D�sol�, ce livre est d�j� emprunt�.\n");
        return;
    }

    printf("Entrez votre nom : ");
    scanf(" %[^\n]s", emprunt->nom_emprunteur);

    time(&now);
    emprunt->date_emprunt = now;
    emprunt->date_retour_prevue = now + 14 * 24 * 60 * 60; // 2 semaines

    bibliotheque[choix].disponible = 0;
    bibliotheque[choix].date_emprunt = emprunt->date_emprunt;

    printf("Livre emprunt� avec succ�s !\n");
}

// Fonction pour rendre un livre
void rendreLivre(Livre bibliotheque[], int nbLivres, Emprunt *emprunt, char* nomFichier) {
    int choix;
    time_t now;

    afficherBibliotheque(bibliotheque, nbLivres);

    printf("Quel livre voulez-vous rendre ? (num�ro du livre) : ");
    scanf("%d", &choix);
    choix--;

    if (choix < 0 || choix >= nbLivres) {
        printf("Choix invalide.\n");
        return;
    }

    if (bibliotheque[choix].disponible) {
        printf("Ce livre n'est pas emprunt�.\n");
        return;
    }

    time(&now);
    bibliotheque[choix].disponible = 1;
    bibliotheque[choix].date_emprunt = 0;

    printf("Livre rendu avec succ�s !\n");
    sauvegarderBibliotheque(bibliotheque, nbLivres, nomFichier);

}

// Fonction pour sauvegarder la biblioth�que dans un fichier texte
void sauvegarderBibliotheque(Livre bibliotheque[], int nbLivres, char* nomFichier) {
    FILE* fp = fopen(nomFichier, "w");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fp, "%d\n", nbLivres);
    for (int i = 0; i < nbLivres; i++) {
        fprintf(fp, "%s\n%s\n%d\n%d\n%ld\n",
                bibliotheque[i].titre,
                bibliotheque[i].auteur,
                bibliotheque[i].annee,
                bibliotheque[i].disponible,
                bibliotheque[i].date_emprunt);
    }

    fclose(fp);
    printf("Biblioth�que sauvegard�e avec succ�s dans le fichier '%s'.\n", nomFichier);
}

// Fonction pour charger la biblioth�que � partir d'un fichier texte
void chargerBibliotheque(Livre bibliotheque[], int* nbLivres, char* nomFichier) {
    FILE* fp = fopen(nomFichier, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fscanf(fp, "%d", nbLivres);
    for (int i = 0; i < *nbLivres; i++) {
        fscanf(fp, " %[^\n]s", bibliotheque[i].titre);
        fscanf(fp, " %[^\n]s", bibliotheque[i].auteur);
        fscanf(fp, "%d", &bibliotheque[i].annee);
        fscanf(fp, "%d", &bibliotheque[i].disponible);
        fscanf(fp, "%ld", &bibliotheque[i].date_emprunt);
    }

    fclose(fp);
    printf("Bibliotheque charg�e avec succ�s � partir du fichier '%s'.\n", nomFichier);
}

int main() {
    Livre bibliotheque[100];
    int nbLivres = 0;
    int choix;
    Emprunt emprunt;
    char nomFichier[] = "bibliotheque.txt";

    chargerBibliotheque(bibliotheque, &nbLivres, nomFichier);

    while (1) {
        printf("hey Que voulez-vous faire ?\n");
        printf("1. Ajouter un livre\n");
        printf("2. Afficher la biblioth�que\n");
        printf("3. Emprunter un livre\n");
        printf("4. Rendre un livre\n");
        printf("5. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

    switch (choix) {
            case 1:
                ajouterLivre(bibliotheque, &nbLivres, nomFichier);
                break;
            case 2:
                afficherBibliotheque(bibliotheque, nbLivres);
                break;
            case 3:
                emprunterLivre(bibliotheque, nbLivres, &emprunt);
                sauvegarderBibliotheque(bibliotheque, nbLivres, nomFichier);
                break;
            case 4:
                rendreLivre(bibliotheque, nbLivres, &emprunt, nomFichier);
                break;
            case 5:
                printf("Au revoir !\n");
                sauvegarderBibliotheque(bibliotheque, nbLivres, nomFichier);
                return 0;
            default:
                printf("Choix invalide. Veuillez r�essayer.\n");
        }

           }

    return 0;
}
