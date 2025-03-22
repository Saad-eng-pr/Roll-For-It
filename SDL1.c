#include "roll_for_it.h"

#define DICE_SIZE 75

void end_sdl(char ok, const char* msg, SDL_Window* window, SDL_Renderer* renderer) {
    char msg_formated[255];
    int l;

    if (!ok) {
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");

        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();

    if (!ok) {
        exit(EXIT_FAILURE);
    }
}

SDL_Texture* load_texture_from_image(const char* file_image_name, SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Surface* my_image = IMG_Load(file_image_name);
    if (my_image == NULL) {
        end_sdl(0, "Chargement de l'image impossible", window, renderer);
    }

    SDL_Texture* my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);
    if (my_texture == NULL) {
        end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);
    }

    return my_texture;
}

int afficherFenetreDemarrage(SDL_Renderer *renderer, SDL_Window* window) {
    SDL_bool continuer = SDL_TRUE;
    SDL_Event evenement;

    // Charger une texture pour l'arrière-plan
    SDL_Texture* Start_Background = load_texture_from_image("SDL_image/background.jpg", window, renderer);
    if (!Start_Background) {
        fprintf(stderr, "Erreur lors du chargement de la texture de l'arrière-plan: %s\n", SDL_GetError());
        return -1;
    }

    while (continuer) {
        while (SDL_PollEvent(&evenement)) {
            switch (evenement.type) {
                case SDL_QUIT:
                    continuer = SDL_FALSE;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    continuer = SDL_FALSE;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, Start_Background, NULL, NULL); // Ajouter l'arrière-plan
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(Start_Background);
    return 0;
}

// Fonction pour afficher la page de sélection de mode de jeu
int afficherSelectionMode(SDL_Renderer * renderer) {
    SDL_bool continuer = SDL_TRUE;
    SDL_Event event;
    int Mode; // Mode par défaut


    // Centrer les boutons sur l'écran
    int boutonWidth = 350;
    int boutonHeight = 250;
    int boutonSpacing = 10; // Espace entre les boutons
    int totalHeight = (2 * boutonHeight) + boutonSpacing;
    int startY = (WINDOW_HEIGHT - totalHeight) / 2;

    SDL_Rect boutonPvP = {(WINDOW_WIDTH - boutonWidth) / 2, startY, boutonWidth, boutonHeight};
    SDL_Rect boutonPvC = {(WINDOW_WIDTH - boutonWidth) / 2, startY + boutonHeight + boutonSpacing, boutonWidth, boutonHeight};


    // Charger les textures pour les boutons
    SDL_Texture* text_PvP = IMG_LoadTexture(renderer, "SDL_image/PvP.jpg");
    SDL_Texture* text_PvC = IMG_LoadTexture(renderer, "SDL_image/PvC.jpg");

    // Vérifier si les textures ont été chargées correctement
    if (!text_PvP || !text_PvC) {
        fprintf(stderr, "Erreur lors du chargement des textures des boutons: %s\n", SDL_GetError());
        return -1;
    }

    while (continuer) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    continuer = SDL_FALSE;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.x >= boutonPvP.x && event.button.x <= (boutonPvP.x + boutonPvP.w) &&
                        event.button.y >= boutonPvP.y && event.button.y <= (boutonPvP.y + boutonPvP.h)) {
                        Mode= 1; //PvP
                        continuer = SDL_FALSE;
                    } else if (event.button.x >= boutonPvC.x && event.button.x <= (boutonPvC.x + boutonPvC.w) &&
                               event.button.y >= boutonPvC.y && event.button.y <= (boutonPvC.y + boutonPvC.h)) {
                        Mode= 0; //PvC
                        continuer = SDL_FALSE;
                    }
                    break;
            }
        }


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // White background
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, text_PvP, NULL, &boutonPvP);
        SDL_RenderCopy(renderer, text_PvC, NULL, &boutonPvC);
        SDL_RenderPresent(renderer);
    }

    // Libérer les textures
    SDL_DestroyTexture(text_PvP);
    SDL_DestroyTexture(text_PvC);

    return Mode;
}

int EndGame(SDL_Renderer *renderer, SDL_Window* window) {
    SDL_bool continuer = SDL_TRUE;
    SDL_Event event;
    
    // Charger une texture pour l'arrière-plan

    SDL_Texture* You_win = load_texture_from_image("SDL_image/you_win.jpg", window, renderer);
    if (!You_win) {
        fprintf(stderr, "Erreur lors du chargement de la texture de l'arrière-plan: %s\n", SDL_GetError());
        return -1;
    }

    while (continuer) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                continuer = SDL_FALSE;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, You_win, NULL, NULL); // Ajouter l'arrière-plan
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(You_win);

    return 0;
}

int choose_dice(int x_dice, int y_dice){
    int dice_index = -1; // Initialize to -1 for invalid index

    // Define dice areas and check if the click is within any of them
    SDL_Rect dice_areas[6] = {
        {120, 437, DICE_SIZE, DICE_SIZE},
        {237, 437, DICE_SIZE, DICE_SIZE},
        {354, 437, DICE_SIZE, DICE_SIZE},
        {471, 437, DICE_SIZE, DICE_SIZE},
        {588, 437, DICE_SIZE, DICE_SIZE},
        {705, 437, DICE_SIZE, DICE_SIZE},
    };

    for (int i = 0; i < 6; ++i) {
        if (x_dice > dice_areas[i].x && x_dice < dice_areas[i].x + DICE_SIZE &&
            y_dice > dice_areas[i].y && y_dice < dice_areas[i].y + DICE_SIZE) {
            dice_index = i;
            break;
        }
    }

    if (x_dice > 800 && x_dice < 900 && y_dice > 520 && y_dice < 600) {
        dice_index = 6; // Pass turn button
    }
    if (x_dice > 0 && x_dice < 100 && y_dice > 520 && y_dice < 600) {
        dice_index = 7;
    }

    return dice_index;
}

int choose_card(int x_card, int y_card) {
    int card_index = -1; // Initialize to -1 for invalid index

    if (x_card > 100 && x_card < 300 && y_card > 75 && y_card < 425) {
        card_index = 0;
    } else if (x_card > 350 && x_card < 550 && y_card > 75 && y_card < 425) {
        card_index = 1;
    } else if (x_card > 600 && x_card < 800 && y_card > 75 && y_card < 425) {
        card_index = 2;
    }

    return card_index;
}

void place_dice_sdl(carte card[3], joueur *player, int dice_index, int card_index) {
    if(dice_index == -1 || card_index == -1)
    {
        return;
    }
    if (player->des[dice_index][1] != -1) {
        printf("Le dé est déjà mis sur une carte\n"); // To be replaced with textures
        return;
    }

    int matched = 0;
    for (int j = 0; j < card[card_index].taille_combinaison; j++) {
        if (player->des[dice_index][0] == card[card_index].combinaison[j]) {
            player->des[dice_index][1] = card[card_index].id;
            matched = 1;
            break;
        }
    }

    if (!matched) {
        printf("Le dé ne correspond pas à la combinaison de la carte.\n"); // To be replaced with textures
    }
}


void draw_dice(SDL_Renderer *renderer, SDL_Texture *textures[], SDL_Rect rect[6], SDL_Rect rect_top[6], joueur *player, int id) {
    for (int i = 0; i < NB_DES; i++) {
        if (player->des[i][1] == -1) {
            SDL_RenderCopy(renderer, textures[player->des[i][0] - 1], NULL, &rect[i]);
        }
        else if (player->des[i][1] == id){
            SDL_RenderCopy(renderer, textures[player->des[i][0] - 1], NULL, &rect_top[i]);
        }
    }
}
/*
void move_dice(SDL_Rect * rectf, SDL_Rect *recti){
    
    recti.x = rectf.x;
    recti.y = rectf.y;
    recti.w = rectf.w;
    recti.h = rectf.h;

}
*/


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Roll For It!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    afficherFenetreDemarrage(renderer, window);
    
    SDL_Texture* red_dice[] = {
        load_texture_from_image("SDL_image/dice/R1.png", window, renderer),
        load_texture_from_image("SDL_image/dice/R2.png", window, renderer),
        load_texture_from_image("SDL_image/dice/R3.png", window, renderer),
        load_texture_from_image("SDL_image/dice/R4.png", window, renderer),
        load_texture_from_image("SDL_image/dice/R5.png", window, renderer),
        load_texture_from_image("SDL_image/dice/R6.png", window, renderer)
    };
    
    SDL_Texture* white_dice[] = {
        load_texture_from_image("SDL_image/dice/B1.png", window, renderer),
        load_texture_from_image("SDL_image/dice/B2.png", window, renderer),
        load_texture_from_image("SDL_image/dice/B3.png", window, renderer),
        load_texture_from_image("SDL_image/dice/B4.png", window, renderer),
        load_texture_from_image("SDL_image/dice/B5.png", window, renderer),
        load_texture_from_image("SDL_image/dice/B6.png", window, renderer),
    };
    
    SDL_Texture* CARDS[] = {
        load_texture_from_image("SDL_image/cards/card15/0.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card15/1.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card15/2.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card10/3.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card10/4.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card10/5.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card10/6.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/7.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/8.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/9.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/10.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/11.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/12.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/13.png", window, renderer),
        load_texture_from_image("SDL_image/cards/card5/14.png", window, renderer)
    };

    SDL_Texture* roll_dice = load_texture_from_image("SDL_image/roll.png", window, renderer);
    SDL_Texture* pass_turn = load_texture_from_image("SDL_image/pass_turn.png", window, renderer);
    SDL_Texture* return_dice_txt = load_texture_from_image("SDL_image/return.jpg", window, renderer);

    SDL_Texture* table_txt = load_texture_from_image("SDL_image/table2.jpg", window, renderer);

    SDL_Rect card0 = {100, 75, 200, 350};
            SDL_Rect card1 = {350, 75, 200, 350};
            SDL_Rect card2 = {600, 75, 200, 350};

            SDL_Rect dice_rect[] = {
                {120, 437, 75, 75},
                {237, 437, 75, 75},
                {354, 437, 75, 75},
                {471, 437, 75, 75},
                {588, 437, 75, 75},
                {705, 437, 75, 75}
            };
            

            SDL_Rect card_top[3][6] = {{{100,40,25,25},
                                    {130,40,25,25},
                                    {160,40,25,25},
                                    {100,70,25,25},
                                    {130,70,25,25},
                                    {160,70,25,25}},
                                    {{350,40,25,25},
                                    {380,40,25,25},
                                    {410,40,25,25},
                                    {350,70,25,25},
                                    {380,70,25,25},
                                    {410,70,25,25}},
                                    {{600,40,25,25},
                                    {630,40,25,25},
                                    {660,40,25,25},
                                    {600,70,25,25},
                                    {630,70,25,25},
                                    {660,70,25,25}}};

            SDL_Rect roll_rect = {400, 520, 100, 80};
            SDL_Rect return_rect = {0, 520, 100, 80};
            SDL_Rect pass_rect = {800, 520, 100, 80};
    
    srand(time(NULL));
    // Initialize cards
    carte cards[NB_CARTES];
    initialiser_carte(cards);
    initialiser_paquet(cards);
    
    // Initialize players
    joueur joueur_W = creer_joueur(1);
    joueur joueur_R = creer_joueur(2);
    
    // Initialize the table
    int count = 0;
    carte table[3];
    initialiser_table(cards, table, &count);
    
    int player = 1;
    int running = 1;
    SDL_Event event;
    int dice_index, card_index;
    int k = 0;
    int roll = 0;
    int v = 0;
    int id;
    int mode = afficherSelectionMode(renderer);
    if (mode == 1){
        while (running) {
            if (player == 1){
                
            while (SDL_PollEvent(&event)) {         
                    
                if (event.type == SDL_QUIT) {
                    running = 0;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_SPACE && roll == 0) {
                        lancer_des(&joueur_W);
                        roll = 1;
                    
                    }

                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    printf("%d %d\n", x ,y);
                    if ((k % 2 == 0 && roll == 1)){
                        dice_index = choose_dice(x,y);
                        printf("%d\n", dice_index);
                        if (dice_index == 7 && roll == 0) {
                            return_dice(&joueur_W);
                            k = 1;
                        }
                        else if (dice_index == 6) {
                            player = 2;
                            k=0;
                            roll = 0;
                            break;
                        }
                        k++;
                    } else if ((k % 2 == 1) && dice_index != 7) {
                        card_index = choose_card(x,y);
                        printf("%d\n", card_index);
                        place_dice_sdl(table, &joueur_W, dice_index, card_index);
                        afficher_des(&joueur_W);
                        v = take_card(&joueur_W, table, cards, &count, &id);
                        printf("le score du joueur_W est %d\n", joueur_W.score);
                        if (Is_winner(&joueur_W)){
                            EndGame(renderer, window);
                            running = 0;
                            break;
                        } 
                        afficher_cartes(table);
                        k++;
                    }
                    
                }
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // White background
            SDL_RenderClear(renderer);

            

            SDL_RenderCopy(renderer, table_txt, NULL, NULL);
            
            // Render cards on the table
            SDL_RenderCopy(renderer, CARDS[table[0].id], NULL, &card0);
            SDL_RenderCopy(renderer, CARDS[table[1].id], NULL, &card1);
            SDL_RenderCopy(renderer, CARDS[table[2].id], NULL, &card2);
        
            // Render dice
            draw_dice(renderer, white_dice, dice_rect, card_top[0], &joueur_W,table[0].id);
            draw_dice(renderer, white_dice, dice_rect, card_top[1], &joueur_W,table[1].id);
            draw_dice(renderer, white_dice, dice_rect, card_top[2], &joueur_W,table[2].id);
        
            // Render buttons
            SDL_RenderCopy(renderer, pass_turn, NULL, &pass_rect);
            SDL_RenderCopy(renderer, return_dice_txt, NULL, &return_rect);
            SDL_RenderCopy(renderer, roll_dice, NULL, &roll_rect);
        

            SDL_RenderPresent(renderer);
            if(v && player == 2){
                            return_card_dice_to_players(id, &joueur_W);
                            return_card_dice_to_players(id, &joueur_R);
                        }
            }
            
            else {
                        
                while (SDL_PollEvent(&event)) {   
                             
                    if (event.type == SDL_QUIT) {
                        running = 0;
                    }  else if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_SPACE && roll == 0) {
                            lancer_des(&joueur_R);
                            roll = 1;
                            
                        }
                    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        printf("%d %d\n", x ,y);
                        if ((k % 2 == 0 && roll == 1)){
                            dice_index = choose_dice(x,y);
                            printf("%d\n", dice_index);
                            if (dice_index == 7 && roll == 0) {
                                return_dice(&joueur_R);
                                k = 1;
                            }
                            else if (dice_index == 6) {
                                player = 1;
                                k = 0;
                                roll = 0;
                                break;
                            }
                            k++;
                        } else if ((k % 2 == 1) && dice_index != 7 ){
                            card_index = choose_card(x,y);
                            printf("%d\n", card_index);
                            place_dice_sdl(table, &joueur_R, dice_index, card_index);
                            afficher_des(&joueur_R);
                            v = take_card(&joueur_R, table, cards, &count, &id);
                            printf("le score du joueur_R est %d\n", joueur_R.score);
                            if (Is_winner(&joueur_R)){
                                EndGame(renderer, window);
                                running = 0;
                                break;
                            } 
                            afficher_cartes(table);
                            k++;
                        }
                        
                    }
        
                }

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // White background
                SDL_RenderClear(renderer);

                SDL_Rect card0 = {100, 75, 200, 350};
                SDL_Rect card1 = {350, 75, 200, 350};
                SDL_Rect card2 = {600, 75, 200, 350};

                SDL_Rect dice_rect[] = {
                    {120, 437, 75, 75},
                    {237, 437, 75, 75},
                    {354, 437, 75, 75},
                    {471, 437, 75, 75},
                    {588, 437, 75, 75},
                    {705, 437, 75, 75}
                };

                SDL_Rect roll_rect = {400, 520, 100, 80};
                SDL_Rect return_rect = {0, 520, 100, 80};
                SDL_Rect pass_rect = {800, 520, 100, 80};

                SDL_RenderCopy(renderer, table_txt, NULL, NULL);
                // Render cards on the table
                SDL_RenderCopy(renderer, CARDS[table[0].id], NULL, &card0);
                SDL_RenderCopy(renderer, CARDS[table[1].id], NULL, &card1);
                SDL_RenderCopy(renderer, CARDS[table[2].id], NULL, &card2);
        
                // Render dice
                draw_dice(renderer, red_dice, dice_rect, card_top[0], &joueur_R,table[0].id);
                draw_dice(renderer, red_dice, dice_rect, card_top[1], &joueur_R,table[1].id);
                draw_dice(renderer, red_dice, dice_rect, card_top[2], &joueur_R,table[2].id);
        
                // Render buttons
                SDL_RenderCopy(renderer, pass_turn, NULL, &pass_rect);
                SDL_RenderCopy(renderer, return_dice_txt, NULL, &return_rect);
                SDL_RenderCopy(renderer, roll_dice, NULL, &roll_rect);

                SDL_RenderPresent(renderer);
                if(v && player == 1){
                            return_card_dice_to_players(id, &joueur_W);
                            return_card_dice_to_players(id, &joueur_R);
                        }
            }
        }
    }
    else{
        while (running) {
            if (player == 1){
                while (SDL_PollEvent(&event)) { 
                             
                    if (event.type == SDL_QUIT) {
                            running = 0;
                    } else if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_SPACE && roll == 0) {
                            lancer_des(&joueur_W);
                            roll = 1;
                            
                        }
                    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        printf("%d %d\n", x ,y);
                        if ((k % 2 == 0) ){
                            dice_index = choose_dice(x,y);
                            printf("%d\n", dice_index);
                            if (dice_index == 7 && roll == 0) {
                                return_dice(&joueur_W);
                                k = 1;
                            }
                            else if (dice_index == 6) {
                                player = 2;
                                k=0;
                                roll = 0;
                                break;
                            }
                            k++;
                        } else if ((k % 2 == 1) && dice_index != 7) {
                            card_index = choose_card(x,y);
                            printf("%d\n", card_index);
                            place_dice_sdl(table, &joueur_W, dice_index, card_index);
                            afficher_des(&joueur_W);
                            v = take_card(&joueur_W, table, cards, &count, &id);
                            printf("le score du joueur_W est %d\n", joueur_W.score);
                            if (Is_winner(&joueur_W)){
                                EndGame(renderer, window);
                                running = 0;
                                break;
                            } 
                            afficher_cartes(table);
                            k++;
                        }
                    }
        
                }

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // White background
                SDL_RenderClear(renderer);

                SDL_Rect card0 = {100, 75, 200, 350};
                SDL_Rect card1 = {350, 75, 200, 350};
                SDL_Rect card2 = {600, 75, 200, 350};

                SDL_Rect dice_rect[] = {
                    {120, 437, 75, 75},
                    {237, 437, 75, 75},
                    {354, 437, 75, 75},
                    {471, 437, 75, 75},
                    {588, 437, 75, 75},
                    {705, 437, 75, 75}
                };

                SDL_Rect roll_rect = {400, 520, 100, 80};
                SDL_Rect return_rect = {0, 520, 100, 80};
                SDL_Rect pass_rect = {800, 520, 100, 80};

                SDL_RenderCopy(renderer, table_txt, NULL, NULL);
                // Render cards on the table
                SDL_RenderCopy(renderer, CARDS[table[0].id], NULL, &card0);
                SDL_RenderCopy(renderer, CARDS[table[1].id], NULL, &card1);
                SDL_RenderCopy(renderer, CARDS[table[2].id], NULL, &card2);
        
                // Render dice
                 draw_dice(renderer, white_dice, dice_rect, card_top[0], &joueur_W,table[0].id);
                draw_dice(renderer, white_dice, dice_rect, card_top[1], &joueur_W,table[1].id);
                draw_dice(renderer, white_dice, dice_rect, card_top[2], &joueur_W,table[2].id);
        
                // Render buttons
                SDL_RenderCopy(renderer, pass_turn, NULL, &pass_rect);
                SDL_RenderCopy(renderer, return_dice_txt, NULL, &return_rect);
                SDL_RenderCopy(renderer, roll_dice, NULL, &roll_rect);
        

                SDL_RenderPresent(renderer);
                if(v && player == 2){
                    return_card_dice_to_players(id, &joueur_W);
                    return_card_dice_to_players(id, &joueur_R);
                }
            }

            else {
                afficher_cartes(table);
                printf("C'est le tour du joueur 2:\n");
                int c = 0;
                for (int i=0; i<6; i++)
                {
                    if (joueur_R.des[i][1] == -1)
                    {
                        c++;
                    }
                }
                if (c <= 1)
                {
                    return_dice(&joueur_R);
                }
                lancer_des(&joueur_R);
                //créer une copie de l'état de jeu pour l'IA
                GameState state;
                memcpy(state.paquet_de_cartes, cards, sizeof(cards));
                memcpy(state.cards, table, sizeof(table));
                state.count = count;
                state.score_ = joueur_R.score;
                memcpy(state.de, joueur_R.des, sizeof(joueur_R.des));
                state.player = &joueur_R;
                state.player1 = &joueur_W;
            

                GameState new_state = mcts(state);
                //afficher_des(new_state.player);
                printf("%d %d\n", new_state.player->score, new_state.score_);
                //appliquer les changements de l'IA à l'état de jeu réel
                memcpy(table, new_state.cards, sizeof(table));
                memcpy(joueur_R.des, new_state.de, sizeof(joueur_R.des));
                joueur_R.score = new_state.score_;
                count = new_state.count;

                afficher_des(&joueur_R);
                v = take_card(&joueur_R, table, cards, &count, &id);
                if (Is_winner(&joueur_R)) {
                    printf("Le joueur 2 a gagné !!!!!!!\n");
                    break;
                }
                afficher_cartes(table);
                player = 1;
                if(v && player == 1){
                    return_card_dice_to_players(id, &joueur_W);
                    return_card_dice_to_players(id, &joueur_R);
                }
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // White background
                SDL_RenderClear(renderer);

                SDL_Rect card0 = {100, 75, 200, 350};
                SDL_Rect card1 = {350, 75, 200, 350};
                SDL_Rect card2 = {600, 75, 200, 350};

                SDL_Rect dice_rect[] = {
                    {120, 437, 75, 75},
                    {237, 437, 75, 75},
                    {354, 437, 75, 75},
                    {471, 437, 75, 75},
                    {588, 437, 75, 75},
                    {705, 437, 75, 75}
                };

                SDL_Rect roll_rect = {400, 520, 100, 80};
                SDL_Rect return_rect = {0, 520, 100, 80};
                SDL_Rect pass_rect = {800, 520, 100, 80};

                SDL_RenderCopy(renderer, table_txt, NULL, NULL);
                // Render cards on the table
                SDL_RenderCopy(renderer, CARDS[table[0].id], NULL, &card0);
                SDL_RenderCopy(renderer, CARDS[table[1].id], NULL, &card1);
                SDL_RenderCopy(renderer, CARDS[table[2].id], NULL, &card2);
        
                // Render dice
                draw_dice(renderer, red_dice, dice_rect, card_top[0], &joueur_R,table[0].id);
                draw_dice(renderer, red_dice, dice_rect, card_top[1], &joueur_R,table[1].id);
                draw_dice(renderer, red_dice, dice_rect, card_top[2], &joueur_R,table[2].id);
        
                // Render buttons
                SDL_RenderCopy(renderer, pass_turn, NULL, &pass_rect);
                SDL_RenderCopy(renderer, return_dice_txt, NULL, &return_rect);
                SDL_RenderCopy(renderer, roll_dice, NULL, &roll_rect);

                SDL_RenderPresent(renderer);

                SDL_Delay(4000);
            }
        }
        SDL_DestroyTexture(pass_turn);
        SDL_DestroyTexture(return_dice_txt);
        SDL_DestroyTexture(roll_dice);
        SDL_DestroyTexture(table_txt);
    
        for (int i = 0; i < 6; ++i) {
            SDL_DestroyTexture(white_dice[i]);
            SDL_DestroyTexture(red_dice[i]);
        }
        for (int i = 0; i < 15; ++i) {
            SDL_DestroyTexture(CARDS[i]);
        }
    }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 0;
}
