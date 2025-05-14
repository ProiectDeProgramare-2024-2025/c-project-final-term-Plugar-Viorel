#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BBLUE    "\x1b[94m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define ORANGE   "\x1b[38;5;208m"
#define RESET   "\x1b[0m"
#define PINK1    "\x1b[38;5;13m"
#define BLACK   "\x1b[30m"
#define YELLOW  "\x1b[33m"

#define MAX_PLAYLISTS 100
#define MAX_SONGS 100

const int MAIN_OPTIONS = 4;
const int SUB_OPTIONS = 3;  


struct Playlist {
    char name[50];
    float rating;
    int numSongs;
    char songs[MAX_SONGS][100];
};


struct Playlist playlists[MAX_PLAYLISTS];
int totalPlaylists = 0;


void load_data(); //citeste datele din fisier
void save_data(); // punem/scriem datele in fisier, intr-o singura coloana
void display_menu(int highlight); //afiseaza meniul principal
void handling_menu(int choice); // se ocupa de optiuniile din program
void meniu_creare(); //submeniu pentru creearea unui playlist sau adaugarea unei melodii la un playlist
void delete_display(); //submeniu pentru stergerea unui playlist sau stergerea unei melodii dintr-un playlist
void music(); // afisarea datelor din fisier
void creeare_playlist();
void adauga_melodie();
void stergere_playlist();
void stergere_melodie();
void sortPlaylists();//sortarea playlist-urilor dupa rating


int main() {
    int choice = 0;
    int key;
    
   
    load_data();
    
    while (1) {
        display_menu(choice);
        key = _getch();

        if (key == 224) {  
            key = _getch();
            switch (key) {
                case 72: choice = (choice - 1 + MAIN_OPTIONS) % MAIN_OPTIONS; break; // Sus
                case 80: choice = (choice + 1) % MAIN_OPTIONS; break;  // Jos
            }
        } else if (key == 13) {  // Enter
            handling_menu(choice);
        }
    }
    return 0;
}


void save_data() {
    FILE *file = fopen("Muzica.txt", "w");
    if (file == NULL) {
        printf("Eroare la deschiderea fisierului pentru scriere!\n");
        return;
    }

    fprintf(file, "%d\n", totalPlaylists);
    for (int i = 0; i < totalPlaylists; i++) {
        fprintf(file, "%s\n", playlists[i].name);
        fprintf(file, "%.2f\n", playlists[i].rating);
        fprintf(file, "%d\n", playlists[i].numSongs);
        for (int j = 0; j < playlists[i].numSongs; j++) {
            fprintf(file, "%s\n", playlists[i].songs[j]);
        }
    }
    fclose(file);
}


void load_data() {
    FILE *file = fopen("Muzica.txt", "r");
    if (file == NULL) {
        
        totalPlaylists = 0;
        return;
    }
    
    fscanf(file, "%d\n", &totalPlaylists);
    for (int i = 0; i < totalPlaylists; i++) {
        
        fgets(playlists[i].name, sizeof(playlists[i].name), file);
        playlists[i].name[strcspn(playlists[i].name, "\n")] = '\0';
        
        fscanf(file, "%f\n", &playlists[i].rating);
        
        fscanf(file, "%d\n", &playlists[i].numSongs);
        for (int j = 0; j < playlists[i].numSongs; j++) {
            fgets(playlists[i].songs[j], sizeof(playlists[i].songs[j]), file);
            playlists[i].songs[j][strcspn(playlists[i].songs[j], "\n")] = '\0';
        }
    }
    fclose(file);
}


void display_menu(int highlight) {
    const char *menu_items[] = {
        BLUE"Optiunea"RESET PINK1" 1 - "RESET BLUE"Creeaza un playlist sau adauga o melodie"RESET,
        BLUE"Optiunea"RESET PINK1" 2 - "RESET BLUE"Afiseaza playlisturile"RESET,
        BLUE"Optiunea"RESET PINK1" 3 - "RESET BLUE"Sterge un playlist sau o melodie"RESET,
        BLUE"Optiunea"RESET PINK1" 4 - "RESET BLUE"Iesire din program"RESET
    }; 

    system("cls");
    printf(PINK1"        ---"      RESET
           BLUE" Meniu principal " RESET
           PINK1"---\n\n"          RESET);
    for (int i = 0; i < MAIN_OPTIONS; i++) {
        if (i == highlight) {
            printf(YELLOW "-> %s\n"RESET, menu_items[i]);  
        } else {
            printf("   %s\n", menu_items[i]);
        }
    }
}


void handling_menu(int choice) {
    switch (choice) {
        case 0:
            meniu_creare();
            break;
        case 1:
            music();
            break;
        case 2:
            delete_display();
            break;
        case 3:
            printf("Iesire din program...\n");
            exit(0);
    }
}


void meniu_creare() {
    const char *menu_items[] = {
        ORANGE"1."RESET GREEN" Adaugati un playlist",RESET
        ORANGE"2."RESET GREEN" Adaugati o melodie la un playlist",RESET
        ORANGE"3."RESET GREEN" Mergi inapoi"RESET
    };

    int key, choice = 0;
    while (1) {
        system("cls");
        printf(ORANGE"        ---"RESET GREEN" Meniu modificare playlisturi"RESET ORANGE" ---\n\n"RESET);
        for (int i = 0; i < SUB_OPTIONS; i++) {
            if (i == choice) {
                printf(YELLOW"-> %s\n"RESET, menu_items[i]);
            } else {
                printf("   %s\n", menu_items[i]);
            }
        }
        key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
                case 72: choice = (choice - 1 + SUB_OPTIONS) % SUB_OPTIONS; break;
                case 80: choice = (choice + 1) % SUB_OPTIONS; break;
            }
        } else if (key == 13) {
            if (choice == 0) {
                creeare_playlist();
            } else if (choice == 1) {
                adauga_melodie();
            } else if (choice == 2) {
                return;
            }
        }
    }
}


void delete_display() {
    const char *menu_items[] = {
        "Stergeti un playlist",
        "Stergeti o melodie",
        "Mergi inapoi"
    };

    int key, choice = 0;
    while (1) {
        system("cls");
        printf(YELLOW"        ---"RESET BLUE" Meniu stergere "RESET YELLOW"---\n\n"RESET);
        for (int i = 0; i < SUB_OPTIONS; i++) {
            if (i == choice) {
                printf(YELLOW"-> %s\n"RESET, menu_items[i]);
            } else {
                printf("   %s\n", menu_items[i]);
            }
        }
        key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
                case 72: choice = (choice - 1 + SUB_OPTIONS) % SUB_OPTIONS; break;
                case 80: choice = (choice + 1) % SUB_OPTIONS; break;
            }
        } else if (key == 13) {
            if (choice == 0) {
                stergere_playlist();
            } else if (choice == 1) {
                stergere_melodie();
            } else if (choice == 2) {
                return;
            }
        }
    }
}


void music() {
    sortPlaylists();
    system("cls");
    printf(
        GREEN "\n-" RESET RED "-" RESET GREEN "- " ORANGE "Playlisturi " RESET
        RED "sortate dupa" RESET ORANGE " rating " RESET GREEN "-" RESET RED "-" RESET GREEN "-\n\n" RESET
    );

    for (int i = 0; i < totalPlaylists; i++) {
        // 1) numarul playlistului colorat in galben
        // 2) numele playlistului colorat in cyan
        // 3) rating-ul colorat in magenta
        printf(
            YELLOW "%2d. "          // numar
            BLUE "%s "              // nume playlist
            GREEN "("
            MAGENTA "Rating: "
            RED"%.2f"  // rating
            GREEN ")\n",            // revenire la normal
            i + 1,
            playlists[i].name,
            playlists[i].rating,
            RESET
        );

        // afisez numarul de melodii
        printf(GREEN"    Numar melodii: "RED"%d\n"RESET, playlists[i].numSongs);

        // 4) pentru fiecare melodie, alternanta albastru/mov si virgula rosie
        printf("    ");
        for (int j = 0; j < playlists[i].numSongs; j++) {
            const char *color = (j % 2 == 0 ? BLUE : MAGENTA);
            
            printf("%s%s%s", color, playlists[i].songs[j], RESET);

            if (j + 1 < playlists[i].numSongs) {
                // virgula rosie intre melodii
                printf("%s,%s ", GREEN, RESET);
            }
        }
        printf("\n\n");
    }

    printf(YELLOW"Apasati orice tasta pentru a reveni la meniu..."RESET);
    _getch();
}



void creeare_playlist() {
    system("cls");
    if (totalPlaylists >= MAX_PLAYLISTS) {
        printf("Capacitate maxima atinsa!\n");
        _getch();
        return;
    }
    struct Playlist newPlaylist;

    // Nume playlist
    printf(GREEN "Introduceti" 
           ORANGE " numele " 
           GREEN "playlistului: " RESET);
    fgets(newPlaylist.name, sizeof(newPlaylist.name), stdin);
    newPlaylist.name[strcspn(newPlaylist.name, "\n")] = '\0';

    // Rating — validare input
    int valid;
    do {
        printf(ORANGE "Introduceti" 
               GREEN " ratingul (" 
               RED "0-5" 
               GREEN "): " RESET);
        valid = scanf("%f", &newPlaylist.rating);
        while (getchar() != '\n');  // golim bufferul

        if (valid != 1 || newPlaylist.rating < 0.0f || newPlaylist.rating > 5.0f) {
            printf(RED "Eroare: Trebuie sa introduci un numar intre 0 si 5.\n" RESET);
        }
    } while (valid != 1 || newPlaylist.rating < 0.0f || newPlaylist.rating > 5.0f);

    // Numar de melodii
    do {
        printf(GREEN "Introduceti" 
               ORANGE " numarul " 
               GREEN "de melodii: " RESET);
        valid = scanf("%d", &newPlaylist.numSongs);
        while (getchar() != '\n');
        if (valid != 1 || newPlaylist.numSongs < 0 || newPlaylist.numSongs > MAX_SONGS) {
            printf(RED "Eroare: Trebuie sa introduci un numar valid de melodii.\n" RESET);
        }
    } while (valid != 1 || newPlaylist.numSongs < 0 || newPlaylist.numSongs > MAX_SONGS);

    // Citire titluri melodii
    for (int i = 0; i < newPlaylist.numSongs; i++) {
        printf(GREEN "Melodia " ORANGE "%d: " RESET, i + 1);
        fgets(newPlaylist.songs[i], sizeof(newPlaylist.songs[i]), stdin);
        newPlaylist.songs[i][strcspn(newPlaylist.songs[i], "\n")] = '\0';
    }

    // Salvare
    playlists[totalPlaylists++] = newPlaylist;
    save_data();

    // Confirmare
    printf(GREEN "Playlistul " ORANGE "%s" GREEN " a fost creat cu succes!\n" RESET, newPlaylist.name);
    printf(GREEN "Apasati orice tasta pentru a reveni la meniu..." RESET);
    _getch();
}



void adauga_melodie() {
    char playlistName[50];
    int found = 0;
    int playlistIndex = -1;

    // Loop pana cand gasim playlist-ul
    do {
        system("cls");
        printf(GREEN"Introduceti numele playlistului unde doriti sa adaugati o "ORANGE"melodie: "RESET);
        fgets(playlistName, sizeof(playlistName), stdin);
        playlistName[strcspn(playlistName, "\n")] = '\0';

        // Cutam playlist-ul
        found = 0;
        for (int i = 0; i < totalPlaylists; i++) {
            if (strcmp(playlists[i].name, playlistName) == 0) {
                found = 1;
                playlistIndex = i;
                break;
            }
        }

        if (!found) {
            // Eroare colorata in rosu
            printf(RED"Eroare: Playlist-ul \"%s\" nu a fost gasit. Mai incearca.\n"RESET, playlistName);
            printf(GREEN"Apasati orice tasta pentru a incerca din nou..."RESET);
            _getch();
        }
    } while (!found);

    // La acest punct, playlistIndex e valid
    // Verificam daca mai incapem melodii
    if (playlists[playlistIndex].numSongs >= MAX_SONGS) {
        printf(RED"Numarul maxim de melodii pentru acest playlist a fost atins!\n"RESET);
        printf(GREEN"Apasati orice tasta pentru a reveni la meniu..."RESET);
        _getch();
        return;
    }

    // Citim melodia de adaugat
    char newSong[100];
    system("cls");
    printf(GREEN"Introduceti "ORANGE"numele melodiei " GREEN"de adaugat: "RESET);
    fgets(newSong, sizeof(newSong), stdin);
    newSong[strcspn(newSong, "\n")] = '\0';

    // Adaugam si salvam
    strcpy(playlists[playlistIndex].songs[playlists[playlistIndex].numSongs], newSong);
    playlists[playlistIndex].numSongs++;
    save_data();

    // Confirmare
    printf(GREEN"Melodia " ORANGE"%s" GREEN" a fost adaugata cu succes in playlist-ul " ORANGE"%s"GREEN"!\n"RESET,
           newSong, playlists[playlistIndex].name);
    printf(GREEN"Apasati orice tasta pentru a reveni la meniu..."RESET);
    _getch();
}



void stergere_playlist() {
    system("cls");
    char playlistName[50];
    int found = 0;

    // Loop pana cand gasim un playlist cu numele introdus
    do {
        printf(GREEN "Introduceti numele playlistului de sters: " RESET);
        fgets(playlistName, sizeof(playlistName), stdin);
        playlistName[strcspn(playlistName, "\n")] = '\0';

        found = 0;
        for (int i = 0; i < totalPlaylists; i++) {
            if (strcmp(playlists[i].name, playlistName) == 0) {
                found = 1;
                // Shift la stanga pentru a elimina elementul i
                for (int j = i; j < totalPlaylists - 1; j++) {
                    playlists[j] = playlists[j + 1];
                }
                totalPlaylists--;
                save_data();
                printf(YELLOW "Playlistul " RED "%s" YELLOW " a fost sters cu succes!\n" RESET, playlistName);
                break;
            }
        }

        if (!found) {
            printf(RED "Eroare: Playlistul \"%s\" nu a fost gasit. Incearca din nou.\n" RESET, playlistName);
        }
    } while (!found);

    printf(GREEN "Apasati orice tasta pentru a reveni la meniu..." RESET);
    _getch();
}


void stergere_melodie() {
    system("cls");

    char playlistName[50];
    int pIndex = -1;

    // 1) Caut playlistul
    do {
        printf(GREEN "Introduceti numele playlist-ului: " RESET);
        fgets(playlistName, sizeof(playlistName), stdin);
        playlistName[strcspn(playlistName, "\n")] = '\0';

        pIndex = -1;
        for (int i = 0; i < totalPlaylists; i++) {
            if (strcmp(playlists[i].name, playlistName) == 0) {
                pIndex = i;
                printf(GREEN "Playlistul " YELLOW "%s" GREEN " a fost gasit!\n\n" RESET,
                       playlistName);
                break;
            }
        }
        if (pIndex == -1) {
            printf(RED "Eroare: Playlistul \"%s\" nu exista. Incearca din nou.\n\n" RESET,
                   playlistName);
        }
    } while (pIndex == -1);

    // 2) Caut melodia
    char songName[100];
    int sIndex = -1;
    do {
        printf(GREEN "Introduceti titlul melodiei de sters din \"%s\": " RESET,
               playlists[pIndex].name);
        fgets(songName, sizeof(songName), stdin);
        songName[strcspn(songName, "\n")] = '\0';

        sIndex = -1;
        for (int j = 0; j < playlists[pIndex].numSongs; j++) {
            if (strcmp(playlists[pIndex].songs[j], songName) == 0) {
                sIndex = j;
                printf(YELLOW "Melodia " RED "\"%s\"" YELLOW " a fost gasita!\n\n" RESET,
                       songName);
                break;
            }
        }
        if (sIndex == -1) {
            printf(RED "Eroare: Melodia \"%s\" nu se gaseste in playlist. Incearca din nou.\n\n" RESET,
                   songName);
        }
    } while (sIndex == -1);

    // 3) sterg melodia din playlist
    for (int k = sIndex; k < playlists[pIndex].numSongs - 1; k++) {
        strcpy(playlists[pIndex].songs[k], playlists[pIndex].songs[k + 1]);
    }
    playlists[pIndex].numSongs--;

    // 4) salvare si confirmare finala
    save_data();
    printf(GREEN "In playlist-ul " YELLOW "%s" GREEN ", melodia " RED "\"%s\"" GREEN " a fost stearsa cu succes!\n" RESET,
           playlists[pIndex].name, songName);
    printf(GREEN "Apasati orice tasta pentru a reveni la meniu..." RESET);
    _getch();
}



void sortPlaylists() {
    for (int i = 0; i < totalPlaylists - 1; i++) {
        for (int j = i + 1; j < totalPlaylists; j++) {
            if (playlists[i].rating < playlists[j].rating) {
                struct Playlist temp = playlists[i];
                playlists[i] = playlists[j];
                playlists[j] = temp;
            }
        }
    }
}
