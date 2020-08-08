#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int BOOL;
typedef unsigned char BYTE;

#define BOARD_SIZE 8	
#define EMPTY ' '		
#define PLAYER_T 'T'
#define PLAYER_B 'B'
#define LEFT 0
#define RIGHT 1
#define EVEN 1
#define ODD 0
#define ROW_A 0
#define ROW_H 7

#define Max(a,b)  ((a) > (b) ? (a) : (b))	//macro for calculating maximum of two numbers

typedef struct _checkerPos {
	char row, col;
} checkersPos;

typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

typedef unsigned char Player;

typedef struct _SingleSourceMovesTreeNode {
	Board board;
	checkersPos *pos;
	unsigned short total_captures_So_far;
	struct _SingleSourceMovesTreeNode *next_move[2];
} SingleSourceMovesTreeNode;

typedef struct _SingleSourceMovesTree {
	SingleSourceMovesTreeNode *source;
} SingleSourceMoveTree;

SingleSourceMoveTree *FindSingleSourceMoves(Board board, checkersPos *src);
SingleSourceMovesTreeNode *createNode(Board board, checkersPos *pos, unsigned short caps,
	SingleSourceMovesTreeNode *leftMove, SingleSourceMovesTreeNode *rightMove);
int leftValidT(Board board, checkersPos *position);
int rightValidT(Board board, checkersPos *position);
int leftValidB(Board board, checkersPos *position);
int rightValidB(Board board, checkersPos *position);
int height_tree(SingleSourceMovesTreeNode *tNode);
void FindSingleSourceMovesHelperT(Board board, checkersPos *src, SingleSourceMovesTreeNode *source);
void FindSingleSourceMovesHelperB(Board board, checkersPos *src, SingleSourceMovesTreeNode *source);
void moveLeftT(Board board, checkersPos *src, SingleSourceMovesTreeNode *source, int moves);
void moveRightT(Board board, checkersPos *src, SingleSourceMovesTreeNode *source, int moves);
void moveLeftB(Board board, checkersPos *src, SingleSourceMovesTreeNode *source, int moves);
void moveRightB(Board board, checkersPos *src, SingleSourceMovesTreeNode *source, int moves);
void checkAllocation(void *p);
void freeTree(SingleSourceMovesTreeNode *root);