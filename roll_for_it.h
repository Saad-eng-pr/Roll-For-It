#ifndef ROLL_FOR_IT_H
#define ROLL_FOR_IT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "time.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define NB_JOUEUR 2
#define NB_DES 6
#define NB_CARTES 15  
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600
#define POSSIBLE_STATES 6


typedef struct {
    int num; //peut étre 1 ou 2
    int score; //le score obtenu
    int des[NB_DES][2]; //son résultat lors du lancé du dés;
} joueur;

typedef struct {
    int points; 
    int id;
    int taille_combinaison;
    int combinaison[6]; //6 places au max 
} carte;

typedef struct{
    carte paquet_de_cartes[NB_CARTES];
    carte cards[3];
    int count;
    int score_;
    int de[6][2];
    joueur * player; // 0 : ia et 1 : un joueur (homme)
    joueur * player1;
}GameState;

typedef struct Node{
    GameState state;
    int visits;
    double value;
    struct Node *parent;
    struct Node **children;
    int num_children;
}Node;


//fonction du jeu roll_for_it.c
void initialiser_carte(carte cards[NB_CARTES]);
void initialiser_paquet(carte paquet_de_cartes[NB_CARTES]);
void afficher_cartes(carte cartes[3]);
void initialiser_table(carte paquet_de_cartes[NB_CARTES], carte table[3], int *count);
joueur creer_joueur(int numero);
void lancer_des(joueur * player);
void afficher_des(joueur * player);
void place_dice(carte card[3], joueur * player);
int verify(joueur * player, carte card);
int take_card(joueur * player1, carte table[3], carte paquet_de_cartes[NB_CARTES],int *count, int *id);
void return_card_dice_to_players(int id, joueur * player1);
void return_dice(joueur * player);
int Is_winner(joueur * player);

//fonction de l'interface graphique SDL.c
SDL_Texture* load_texture_from_image(const char* file_image_name, SDL_Window* window, SDL_Renderer* renderer);
void end_sdl(char ok, const char* msg, SDL_Window* window, SDL_Renderer* renderer);
int afficherFenetreDemarrage(SDL_Renderer *renderer, SDL_Window* window);
int afficherSelectionMode(SDL_Renderer * renderer);
int EndGame(SDL_Renderer *renderer, SDL_Window* window);
void place_dice_sdl(carte table[3], joueur * player, int dice_index, int card_index);

//mcts
Node* create_node(GameState state, Node *parent);
void free_node(Node *node);
int is_game_over(GameState state);
Node* select_best_child(Node *node);
void expand_node(Node *node);
GameState simulate_random_game(GameState state);
void backpropagate(Node *node, GameState result);
GameState* generate_possible_states(GameState state);
double evaluate_result(GameState result);
GameState mcts(GameState game);


#endif
