#ifndef SCHERMO_H
#define SCHERMO_H
#include "./utilities.h"
#include "./proiettile.h"
#include "./collisione.h"
#include "./rana.h"
#include "./nemico.h"
#include "./proiettileNemico.h"
#include "./coccodrillo.h"

void aggiorna(GameData* gameData);

void cancellaOggetto(GameData* gameData, PipeData *old_pos, TipoSprite tipoSprite);

void aggiornaOggetto(GameData* gameData, PipeData* old_pos, TipoSprite tipoSprite);

void stampaSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite,GameData* gameData);

void pulisciSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite, GameData* gameData);

void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]);

void stampaRefreshMatrice( ScreenCell (*screenMatrix)[WIDTH]);

void stampaTanaChiusa(Tana tana, GameData* gameData);

void aggiornaTempo(GameData* gameData);

void normalUpdate(GameData* gameData);

void printPianta(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite);

void handleCoccodrilloMovement(GameData* gameData);

void aggiornaOggettoCoccodrillo(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite,CocodrileControl* controlloCoccodrillo);

void stampaCoccodrilloInMatrice(PipeData *datiNuovi, Sprite *sprite, GameData *gameData,CocodrileControl* controlloCoccodrillo);

void cancellaOggettoDaMatrice(GameData* gameData, PipeData oggetto ,PipeData* old_pos, TipoSprite tipoSprite);

void aggiornaOggettoNew(GameData *gameData,PipeData new_pos, PipeData *old_pos, TipoSprite tipoSprite);

void stampaRanaOnCoccodrillo(GameData* gameData);

int determinaSpriteCoccodrillo(CocodrileControl *controlloCoccodrillo);

#endif