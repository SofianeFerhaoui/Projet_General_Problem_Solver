#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
typedef char string[80]; /* un string est un  tableau de 80 char */
 
string start[5];   /* faits initiaux */
int nbstart;
string finish[5];  /* buts  */
int nbfin;
 
/* une regle structures a champs preconditions, faits a ajouter, faits a supprimer */
struct action {
    string nom_action;
    string preconds[5];
    int nbpr;
    string add[5];
    int nbadd;
    string delet[5];
    int nbdel;
};
 
struct action regles [20]; /*  les regles du probleme */
int nbaction;
 
/* un etat est liste des faits vrais et la regle  */
struct etat {
    string current[20];
    int nb_current;
    int nb_regle;
};
 
struct etat tab_etat [50]; /*  etats explores */

/* extrai les elements separes par des virgules apres le : */
int parseLine(char source[], string cible[]){
    int i=0, n=0;
    while(source[i]!=':') i++;
    i++;
    int j=i;
    while(source[i]!='\n'){
        if(source[i]==','){
            memcpy(&cible[n], &source[j], i-j);
            cible[n][i-j] = '\0';
            n++;
            j=i+1;
        }
        i++;
    }
    return n;
}
 
/* lit le fichier et stocke dans  start, finish et  les regles */
void enregistrer (char nom_fichier[]) {
    FILE* monflux = fopen(nom_fichier,"r");
    char c[300];
    bool debut_done=false;
    bool fin_done=false;
    int i;
    int current_regl=-1; /* -1 car incremente avant de lire la regle */
    while (fgets(c,300,monflux)!=NULL) {
        if (strncmp("start",c,5)==0 && debut_done==false){
            nbstart=parseLine(c,start);
            debut_done=true;
        } else if (strncmp("finish",c,6)==0 && fin_done==false){
                nbfin=parseLine(c,finish);
                fin_done=true;
                }
                else if (strncmp("action",c,6)==0){
                    string t[1];
                    parseLine(c,t);
                    strcpy(regles[current_regl].nom_action,t[0]);
                }
                else if (strncmp("preconds", c, 8) == 0) {
                    regles[current_regl].nbpr=parseLine(c, regles[current_regl].preconds);
                }
                else if (strncmp("add", c, 3) == 0) {
                    regles[current_regl].nbadd=parseLine(c, regles[current_regl].add);
                }
                else if (strncmp("delete", c, 6) == 0) {
                    regles[current_regl].nbdel=parseLine(c, regles[current_regl].delet);
                }
                else if (strncmp("****", c, 4) == 0) {
                    current_regl++;
                }
    }
    fclose(monflux);
    nbaction=current_regl+1;
}
 
/* affiche les faits initiaux, les buts et  les regles */
void afficher(){
    printf("START FACTS:\n");
    for(int i=0; i<nbstart; i++){
        printf("  start[%d] = %s\n", i+1, start[i]);
    }
    printf("\nFINISH GOALS:\n");
    for(int i=0; i<nbfin; i++){
        printf("  finish[%d] = %s\n", i+1, finish[i]);
    }
    printf("\nRULES:\n");
    for(int i=0; i<nbaction; i++){
        printf("  action: %s\n", regles[i].nom_action);
        printf("  preconds:\n");
        for(int j=0; j<regles[i].nbpr; j++){
            printf("    %s\n", regles[i].preconds[j]);
        }
        printf("  add:\n");
        for(int j=0; j<regles[i].nbadd; j++){
            printf("    %s\n", regles[i].add[j]);
        }
        printf("  delete:\n");
        for(int j=0; j<regles[i].nbdel; j++){
            printf("    %s\n", regles[i].delet[j]);
        }
        printf("  ----\n");
    }
    printf("nombre des regles : %d\n", nbaction);
}
 
/* retourne vrai si toutes les preconditions de pre[] sont presentes dans current[] s = nb preconditions, s2 = nb faits courants */
bool applicable(string pre[5],string current[],int s,int s2) {
    int i,j,n;
    bool app=false;
    i=0;
    n=0;
    while (i<s){
        for (j=0;j<s2;j++){
            if (strcmp(current[j],pre[i])==0){
                j=s2;
                n=n+1;
            }
        }
    i++;
    }
    if (n==s) {return true;} else {return false;};
}
 
/* cherche la premiere regle applicable a partir de l'indice IR */
int trouver_regle(struct action regles[], string current[], int n, int n2, int IR) {
    int i = IR;
    while (i < n) {
        bool app = applicable(regles[i].preconds, current, regles[i].nbpr, n2);
        if (app == true) {
            return i;
        }
        i++;
    }
    return -1;
}
 
/* supprime le fait c dans le tableaupar decalage des elements */
void supprimer (string c,string from[],int n){
    int i=0;
    bool found=false;
    while (found == false && i<n){
        if (strcmp(c,from[i])==0){
            found=true;
            for (int j=i;j+1<n;j++){
                strcpy(from[j],from[j+1]);
            }
            from[n-1][0]='\0';
        }i++;
    }
}
 
/* ajoute c dans la table s'il n'y est pas deja, retourne false si il ya deja*/
bool ajouter(string c, string to[], int n){ //reout dun boolean pour verifier si la fait etait ajouter
    for(int i = 0; i < n; i++){
        if(strcmp(to[i], c) == 0) return false;
    }
    strcpy(to[n], c);
    return true;
}
 
/* applique la regle t sur current[] ajouter et supprimer */
int appliquer(struct action regles[], string current[], int t, int n2){
    for(int i=0; i<regles[t].nbadd; i++){
        if(ajouter(regles[t].add[i], current, n2)){
            n2++;  //on incriment si le fait est ajouter
        }
    }
    for(int i=0; i<regles[t].nbdel; i++){
        supprimer(regles[t].delet[i], current, n2);
        n2--;
    }
    return n2;
}
 
/* retourne vrai si tous les buts sont presents dans l'etat courant */
bool but_atteint(string fin[], string curr[], int a, int b){
    int n = 0;
    for(int i=0; i<a; i++){
        int j = 0;
        bool found = false;
        while(found == false && j < b){
            if(strcmp(fin[i], curr[j]) == 0){
                found = true;
                n++;
            }
            j++;
        }
    }
    return (n == a);
}
 
/* affiche les faits de l'etat courant */
void afficher_courants(string current[], int nb_current){
    printf("Current facts:\n");
    for(int i=0; i<nb_current; i++){
        printf("  - %s\n", current[i]);
    }
    printf("\n");
}
 
/* affiche les etats de la solution avec la regle appliquee a chaque etape */
void afficher_etats(struct etat tab_etat[], int nb_etats){
    for(int i=0; i<nb_etats; i++){
        printf("etat %d:\n", i);
        for(int j=0; j<tab_etat[i].nb_current; j++){
            printf("  - %s\n", tab_etat[i].current[j]);
        }
        if(i==0){
            printf("  (etat initial)\n");
        } else {
            printf("  regle appliquee: %s\n", regles[tab_etat[i].nb_regle].nom_action);
        }
        printf("\n");
    }
}
 
/* retourne true si new_state[] est identique a un etat deja dans tabetat  */
bool etat_deja_vu(struct etat tab_etat[], int nb_etats, string new_state[], int nb_new){
    for(int i=0; i<nb_etats; i++){
        if(tab_etat[i].nb_current != nb_new) continue;
        bool identical = true;
        for(int j=0; j<nb_new && identical; j++){
            bool found = false;
            for(int k=0; k<tab_etat[i].nb_current && !found; k++){
                if(strcmp(new_state[j], tab_etat[i].current[k])==0) found = true;
            }
            if(!found) identical = false;
        }
        if(identical) return true;
    }
    return false;
}
 
/* recherche avec backtracking et detection de cycles */
int solve_prblm(struct action regles[], struct etat tab_etat[]){
    tab_etat[0].nb_current = nbstart;
    tab_etat[0].nb_regle = -1;
    bool goal = false;
    for(int i=0; i<nbstart; i++){
        strcpy(tab_etat[0].current[i], start[i]);
    }
    printf("Initial facts:\n");
    afficher_courants(tab_etat[0].current, tab_etat[0].nb_current);
    int j=0;
    int IR=0;
    bool possible=true;
    while(goal == false && possible == true){
        int t = trouver_regle(regles, tab_etat[j].current, nbaction, tab_etat[j].nb_current, IR);
        if (t != -1) {
            // compte le nouveau etat tomporairement
            string temp_state[20];
            int temp_nb = tab_etat[j].nb_current;
            for(int k=0; k<temp_nb; k++){
                strcpy(temp_state[k], tab_etat[j].current[k]);
            }
            temp_nb = appliquer(regles, temp_state, t, temp_nb);
 
            // on continue si on a jamais vu cet etat
            if(etat_deja_vu(tab_etat, j+1, temp_state, temp_nb)){
                //si letat est dija visiter on passe a la procaine regle
                IR = t + 1;
                printf("IR was updated to %d, cycle detected\n", IR);
            } else {
                for(int k=0; k<temp_nb; k++){
                    strcpy(tab_etat[j+1].current[k], temp_state[k]);
                }
                tab_etat[j+1].nb_current = temp_nb;
                tab_etat[j+1].nb_regle = t;
                printf("Applying rule: %s\n", regles[t].nom_action);
                afficher_courants(tab_etat[j+1].current, tab_etat[j+1].nb_current);
                IR = 0;
                j++;
                goal = but_atteint(finish, tab_etat[j].current, nbfin, tab_etat[j].nb_current);
            }
        }
        else if (j > 0){
            int failed_rule = tab_etat[j].nb_regle;
            IR = failed_rule + 1;
            printf("IR was updated to %d, backtracking\n", IR);
            j--;
        }
        else {
            possible = false;
        }
    }
    if (goal) {
        printf("But atteint!\n");
        afficher_etats(tab_etat, j+1);
    } else {
        printf("But impossible\n");
    }
    return(j+1);
}
 
int main() {
    enregistrer("monkeys.txt");
    int c = -1;
    while (c != 0) {
        printf("\n=== Menu GPS ===\n");
        printf("1. Partie 1 - Chargement des donnees\n");
        printf("2. Partie 2 - Moteur simple\n");
        printf("3. Partie 3 -  backtracking\n");
        printf("4. Partie 4 - Le chou, la chevre et le loup BURGER\n");
        printf("0. SORTIR\n");
        printf("Votre choix : ");
        scanf("%d", &c);

        if (c == 1) {
            afficher();
        } else if (c == 2) {
            solve_prblm(regles, tab_etat);
        } else if (c == 3) {
            solve_prblm(regles, tab_etat);
        } else if (c == 4) {
            enregistrer("chou.txt");
            solve_prblm(regles, tab_etat);
        } else if (c != 0) {
            printf("Choix invalide.\n");
        }
    }

    printf("FIN\n");
    return 0;
}