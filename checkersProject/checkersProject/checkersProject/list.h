#pragma once
#include "tree.h"

typedef struct _SingleSourceMovesListCell {
	checkersPos *position;
	unsigned short captures;
	struct _SingleSourceMovesListCell *next;
} SingleSourceMovesListCell;

typedef struct _SingleSourceMovesList {
	SingleSourceMovesListCell *head;
	SingleSourceMovesListCell *tail;
} SingleSourceMovesList;

typedef struct _multipleSourceMovesListCell {
	SingleSourceMovesList *single_source_moves_list;
	struct _multipleSourceMovesListCell *next;
} MultipleSourceMovesListCell;

typedef struct _multipleSourceMovesList {
	MultipleSourceMovesListCell *head;
	MultipleSourceMovesListCell *tail;
} MultipleSourceMovesList;

//general list declarations
SingleSourceMovesList *FindSingleSourceOptimalMove(SingleSourceMoveTree *moves_tree);
MultipleSourceMovesList *FindAllPossiblePlayerMoves(Board board, Player player);
int checkRowEvenOdd(int rowIndex);
void treeToList(SingleSourceMovesTreeNode *source, SingleSourceMovesList *list);

//multiple move list declarations
MultipleSourceMovesListCell *createMultipleMoveListCell(SingleSourceMovesList *singleMoveList, MultipleSourceMovesListCell *next);
void newMultipleMoveCellToEndList(MultipleSourceMovesList* multipleMoveList, SingleSourceMovesList *singleMoveList);
void insertMultipleMoveNodeToTail(MultipleSourceMovesList *multipleMoveList, MultipleSourceMovesListCell *newTail);
void makeEmptyMultiList(MultipleSourceMovesList *list);
void freeMultiList(MultipleSourceMovesList *list);
int isEmptyMultiList(MultipleSourceMovesList lst);

//single move list declarations
SingleSourceMovesListCell *createSingleMoveListCell(checkersPos *pos, unsigned short caps, SingleSourceMovesListCell *next);
SingleSourceMovesList *findOptimalMove(Board board, checkersPos *pos);
int isEmptySingleList(SingleSourceMovesList lst);
void makeEmptySingleList(SingleSourceMovesList *list);
void freeSingleList(SingleSourceMovesList *list);
void insertNodeToTail(SingleSourceMovesList *ls, SingleSourceMovesListCell *newTail);
void newCellToEndList(SingleSourceMovesList* lst, checkersPos *pos, unsigned short caps);
