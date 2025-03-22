#include "roll_for_it.h"

int main(){
    carte cards[NB_CARTES];
    srand(time(NULL));
    initialiser_carte(cards);
    joueur  joueur_1 = creer_joueur(1);
    joueur  joueur_2 = creer_joueur(2);
    initialiser_paquet(cards);
    int count = 0;
    carte table[3];
    initialiser_table(cards,table,&count);
    while(1){
        int placer_des1;
        afficher_cartes(table);
        int b;
        printf("C'est le tour du joueur 1:\n");
        printf("voulez vous retourner vos des? si oui taper 1, 0 sinon\n");
        scanf("%d", &b);
        if(b == 1)
        {
            return_dice(&joueur_1);
        }
        lancer_des(&joueur_1);
        printf("Voulez vous placer un dés? si oui taper 1, 0 sinon\n");
        scanf("%d",&placer_des1);    
        while (placer_des1){
            place_dice(table, &joueur_1);
            afficher_des(&joueur_1);
            take_card(&joueur_1, &joueur_2, table, cards, &count);
            if(Is_winner(&joueur_1)){
                printf("Le joueur 1 a gagne !!!!!!!");
                break;
            }
            printf("Voulez vous placer un autre dés? si oui taper 1, 0 sinon\n");
            scanf("%d",&placer_des1);
        }
        if(Is_winner(&joueur_1)){
            break;
        }

        int placer_des2;
        afficher_cartes(table);
        int b1;
        printf("C'est le tour du joueur 2:\n");
        printf("voulez vous retourner vos des? si oui taper 1, 0 sinon\n");
        scanf("%d", &b1);
        if(b1 == 1)
        {
            return_dice(&joueur_2);
        }
        lancer_des(&joueur_2);
        printf("Voulez vous passer un dés? si oui taper 1, 0 sinon\n");
        scanf("%d",&placer_des2);    
        while (placer_des2){
            place_dice(table, &joueur_2);
            afficher_des(&joueur_2);
            take_card(&joueur_2, &joueur_1, table, cards, &count);
            if(Is_winner(&joueur_2)){
                printf("Le joueur 2 a gagne !!!!!!!");
                break;
            }
            printf("Voulez vous passer un autre dés? si oui taper 1, 0 sinon\n");
            scanf("%d",&placer_des2);
        }
        if(Is_winner(&joueur_2)){
            break;
        }
    }
    return 0;
}