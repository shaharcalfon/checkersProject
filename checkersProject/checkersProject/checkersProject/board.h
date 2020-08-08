#pragma once
#include "list.h"

void Turn(Board board, Player player);
void printMove(SingleSourceMovesListCell *currCell);
void turnHelper(int *caps, int *len, SingleSourceMovesList *lst);
void NextTurn(char *player);
void PlayGame(Board board, Player starting_player);
void PlayerMove(SingleSourceMovesListCell *move, int *moveLen, Board board);
void printBoard(Board board);
void copyBoard(Board from, Board to);
void updateBoard(Board board, checkersPos pos, checkersPos dest);
int GameState(Board board, char player);
int ScanBoardB(Board board);
int ScanBoardT(Board board);
SingleSourceMovesListCell *selectOptimalMove(MultipleSourceMovesListCell *head, int *maxLen, Player player);