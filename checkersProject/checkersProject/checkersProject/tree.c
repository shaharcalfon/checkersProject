#include "tree.h"

//returns a tree that holds all possible moves for player in received position
SingleSourceMoveTree *FindSingleSourceMoves(Board board, checkersPos *src) {

	//received checker position is empty
	if (board[(src->row - 'A')][(src->col - '1')] != PLAYER_T &&
		board[(src->row - 'A')][(src->col - '1')] != PLAYER_B)
		return NULL;

	SingleSourceMoveTree *moveTree = (SingleSourceMoveTree*)malloc(sizeof(SingleSourceMoveTree));
	checkAllocation(moveTree);

	moveTree->source = createNode(board, src, 0, NULL, NULL);			//initialize source node

	if (board[(src->row - 'A')][(src->col - '1')] == PLAYER_T)
		FindSingleSourceMovesHelperT(board, src, moveTree->source);		//send to T helper function
	else if (board[(src->row - 'A')][(src->col - '1')] == PLAYER_B)
		FindSingleSourceMovesHelperB(board, src, moveTree->source);		//send to B helper function

	return moveTree;
}

//recursive function that returns all possible moves for player T in a tree
void FindSingleSourceMovesHelperT(Board board, checkersPos *src, SingleSourceMovesTreeNode *source) {

	int leftMove = leftValidT(board, src);							//calculate possible move left
	int rightMove = rightValidT(board, src);						//calculate possible move right

	if (source->total_captures_So_far > 0) {						//if capture counter is bigger than 0
		if (leftMove != 2 && rightMove != 2)						//if capture is not possible in either direction
			return;
		if (leftMove == 2) {										//left capture is possible
			moveLeftT(board, src, source, leftMove);				//create node for left capture and create+update board
			//call recursive function to calculate "next" nodes for the left side
			FindSingleSourceMovesHelperT(source->next_move[LEFT]->board, source->next_move[LEFT]->pos, source->next_move[LEFT]);
		}
		if (rightMove == 2) {										//right capture is possible
			moveRightT(board, src, source, rightMove);				//create node for right capture and create+update board
			//call recursive function to calculate "next" nodes for the right side
			FindSingleSourceMovesHelperT(source->next_move[RIGHT]->board, source->next_move[RIGHT]->pos, source->next_move[RIGHT]);
		}
		return;
	}
	if (leftMove == 1)										//if left move is possible
		moveLeftT(board, src, source, leftMove);			//create node for left move and create+update board
	else if (leftMove == 2) {								//if left capture is possible
		moveLeftT(board, src, source, leftMove);			//create node for left capture and create + update board
		//call recursive function to calculate "next" nodes for the left side
		FindSingleSourceMovesHelperT(source->next_move[LEFT]->board, source->next_move[LEFT]->pos, source->next_move[LEFT]);
	}
	if (rightMove == 1)										//if right move is possible
		moveRightT(board, src, source, rightMove);			//create node for right move and create+update board
	else if (rightMove == 2) {								//if right capture is possible
		moveRightT(board, src, source, rightMove);			//create node for right move and create+update board
		//call recursive function to calculate "next" nodes for the right side
		FindSingleSourceMovesHelperT(source->next_move[RIGHT]->board, source->next_move[RIGHT]->pos, source->next_move[RIGHT]);
	}
}

//recursive function that returns all possible moves for player B in a tree
void FindSingleSourceMovesHelperB(Board board, checkersPos *src, SingleSourceMovesTreeNode *source) {

	int leftMove = leftValidB(board, src);							//calculate possible move for left
	int rightMove = rightValidB(board, src);						//calculate possible move for right

	if (source->total_captures_So_far > 0) {	 					//if capture counter is bigger than 0
		if (leftMove != 2 && rightMove != 2)						//if capture is not possible in either direction
			return;
		if (leftMove == 2) {										//left capture is possible
			moveLeftB(board, src, source, leftMove);				//create node for left capture and create+update board
			//call recursive function to calculate "next" nodes for the left side
			FindSingleSourceMovesHelperB(source->next_move[LEFT]->board, source->next_move[LEFT]->pos, source->next_move[LEFT]);
		}
		if (rightMove == 2) {										//right capture is possible
			moveRightB(board, src, source, rightMove);				//create node for right capture and create+update board
			//call recursive function to calculate "next" nodes for the right side
			FindSingleSourceMovesHelperB(source->next_move[RIGHT]->board, source->next_move[RIGHT]->pos, source->next_move[RIGHT]);
		}
		return;
	}
	if (leftMove == 1)												//if left move is possible
		moveLeftB(board, src, source, leftMove);					//create node for left move and create+update board
	else if (leftMove == 2) {										//if left capture is possible
		moveLeftB(board, src, source, leftMove);					//create node for left capture and create+update board
		//call recursive function to calculate "next" nodes for the left side
		FindSingleSourceMovesHelperB(source->next_move[LEFT]->board, source->next_move[LEFT]->pos, source->next_move[LEFT]);
	}
	if (rightMove == 1)												//if right move is possible
		moveRightB(board, src, source, rightMove);					//create node for right move and create+update board
	else if (rightMove == 2) {										//if right capture is possible
		moveRightB(board, src, source, rightMove);					//create node for right move and create+update board
		//call recursive function to calculate "next" nodes for the right side
		FindSingleSourceMovesHelperB(source->next_move[RIGHT]->board, source->next_move[RIGHT]->pos, source->next_move[RIGHT]);
	}
}

//create next left move node of receieved SingleSourceMovesTreeNode for player T
void moveLeftT(Board board, checkersPos *src, SingleSourceMovesTreeNode *source, int moves) {

	checkersPos *dest = (checkersPos*)malloc(sizeof(checkersPos));	//allocate memory for destination tile

	if (moves == 1) {				//if single move left
		dest->row = src->row + 1;	//calculate destination row
		dest->col = src->col - 1;	//calculate destination column
		//create the destination node
		source->next_move[LEFT] = createNode(board, dest, 0, NULL, NULL);
		updateBoard(source->next_move[LEFT]->board, *src, *dest);	//update board without changing original
	}
	else {							//if capture left
		dest->row = src->row + 2;	//calculate destination row
		dest->col = src->col - 2;	//calculate destination column
		//create the destination node
		source->next_move[LEFT] = createNode(board, dest, source->total_captures_So_far + 1, NULL, NULL);
		updateBoard(source->next_move[LEFT]->board, *src, *dest);	//update board without changing original
	}

	free(dest);		//free memory allocation for temporary destination tile
}

//create next right move node of receieved SingleSourceMovesTreeNode for player T
void moveRightT(Board board, checkersPos *src, SingleSourceMovesTreeNode *source, int moves) {

	checkersPos *dest = (checkersPos*)malloc(sizeof(checkersPos));	//allocate memory for destination tile

	if (moves == 1) {				//if single move right
		dest->row = src->row + 1;	//calculate destination row
		dest->col = src->col + 1;	//calculate destination column
		//create the destination node
		source->next_move[RIGHT] = createNode(board, dest, 0, NULL, NULL);
		updateBoard(source->next_move[RIGHT]->board, *src, *dest);			//update board without changing original
	}
	else {							//if capture right
		dest->row = src->row + 2;	//calculate destination row
		dest->col = src->col + 2;	//calculate destination column
		//create the destination node
		source->next_move[RIGHT] = createNode(board, dest, source->total_captures_So_far + 1, NULL, NULL);
		updateBoard(source->next_move[RIGHT]->board, *src, *dest);			//update board without changing original
	}

	free(dest);
}

//create next left move node of receieved SingleSourceMovesTreeNode for player B
void moveLeftB(Board board, checkersPos *src, SingleSourceMovesTreeNode *source, int moves) {

	checkersPos *dest = (checkersPos*)malloc(sizeof(checkersPos));	//allocate memory for destination tile

	if (moves == 1) {				//if single move left
		dest->row = src->row - 1;	//calculate destination row
		dest->col = src->col - 1;	//calculate destination column
		//create the destination node
		source->next_move[LEFT] = createNode(board, dest, 0, NULL, NULL);
		updateBoard(source->next_move[LEFT]->board, *src, *dest);			//update board without changing original
	}
	else {							//if capture left
		dest->row = src->row - 2;	//calculate destination row
		dest->col = src->col - 2;	//calculate destination column
		//create the destination node
		source->next_move[LEFT] = createNode(board, dest, source->total_captures_So_far + 1, NULL, NULL);
		updateBoard(source->next_move[LEFT]->board, *src, *dest);			//update board without changing original
	}

	free(dest);
}

//create next right move node of receieved SingleSourceMovesTreeNode for player B
void moveRightB(Board board, checkersPos *src, SingleSourceMovesTreeNode *source, int moves) {

	checkersPos *dest = (checkersPos*)malloc(sizeof(checkersPos));	//allocate memory for destination tile

	if (moves == 1) {				//if single move right
		dest->row = src->row - 1;	//calculate destination row
		dest->col = src->col + 1;	//calculate destination column
		//create the destination node
		source->next_move[RIGHT] = createNode(board, dest, 0, NULL, NULL);
		updateBoard(source->next_move[RIGHT]->board, *src, *dest);			//update board without changing original
	}
	else {							//if capture right
		dest->row = src->row - 2;	//calculate destination row
		dest->col = src->col + 2;	//calculate destination column
		//create the destination node
		source->next_move[RIGHT] = createNode(board, dest, source->total_captures_So_far + 1, NULL, NULL);
		updateBoard(source->next_move[RIGHT]->board, *src, *dest);			//update board without changing original
	}

	free(dest);
}

//check which right move is valid for player T (0 = no move, 1 = single move, 2 = capture)
int rightValidT(Board board, checkersPos *position) {	//check if next left move is valid for player T

	//in cases: last row, edge of board, next tile = T >> return 0
	if ((board[(position->row + 1) - 'A'][(position->col) - '0'] == PLAYER_T)
		|| (position->row == 'H') || (position->col == '8'))
		return 0;
	//in case: next tile = B, not on G or 7 >> check if skip tile is empty
	else if (board[(position->row + 1) - 'A'][(position->col) - '0'] == PLAYER_B
		&& (position->row != 'G') && (position->col != '7')) {
		//return 2 if empty, else 0
		return (board[(position->row + 2) - 'A'][(position->col + 1) - '0'] == EMPTY) ? 2 : 0;
	}
	//if next tile is B, skip is not available >> return 0 else regular advancement is possible >> return 1
	else
		return (board[(position->row + 1) - 'A'][(position->col) - '0'] == PLAYER_B) ? 0 : 1;
}

//check which left move is valid for player T (0 = no move, 1 = single move, 2 = capture)
int leftValidT(Board board, checkersPos *position) {	//check if next right move is valid for player T

	//in cases: last row, edge of board, next tile = T >> return 0
	if ((board[(position->row + 1) - 'A'][(position->col - 2) - '0'] == PLAYER_T)
		|| (position->row == 'H') || (position->col == '1'))
		return 0;
	//in case: next tile = B, not on G or 2 >> check if skip tile is empty
	else if (board[(position->row + 1) - 'A'][(position->col - 2) - '0'] == PLAYER_B
		&& (position->row != 'G') && (position->col != '2')) {
		//return 2 if empty, else 0
		return (board[(position->row + 2) - 'A'][(position->col - 3) - '0'] == EMPTY) ? 2 : 0;
	}
	//if next tile is B, skip is not available >> return 0 else regular advancement is possible >> return 1
	else
		return (board[(position->row + 1) - 'A'][(position->col - 2) - '0'] == PLAYER_B) ? 0 : 1;
}

//check which left move is valid for player B (0 = no move, 1 = single move, 2 = capture)
int leftValidB(Board board, checkersPos *position) {	//check if next left move is valid for player B

	//in cases: last row, edge of board, next tile = B >> return 0
	if ((board[(position->row - 1) - 'A'][(position->col - 2) - '0'] == PLAYER_B)
		|| (position->row == 'A') || (position->col == '1'))
		return 0;
	//in case: next tile = T, not on B or 2 >> check if skip tile is empty
	else if (board[(position->row - 1) - 'A'][(position->col - 2) - '0'] == PLAYER_T
		&& (position->row != 'B') && (position->col != '2')) {
		//return 2 if empty, else 0
		return (board[(position->row - 2) - 'A'][(position->col - 3) - '0'] == EMPTY) ? 2 : 0;
	}
	//if next tile is T, skip is not available >> return 0 else regular advancement is possible >> return 1
	else
		return (board[(position->row - 1) - 'A'][(position->col - 2) - '0'] == PLAYER_T) ? 0 : 1;
}

//check which left move is valid for player B (0 = no move, 1 = single move, 2 = capture)
int rightValidB(Board board, checkersPos *position) {	//check if next right move is valid for player B

	//in cases: last row, edge of board, next tile = B >> return 0
	if ((board[(position->row - 1) - 'A'][(position->col) - '0'] == PLAYER_B)
		|| (position->row == 'A') || (position->col == '8'))
		return 0;
	//in case: next tile = T, not on B or 7 >> check if skip tile is empty
	else if (board[(position->row - 1) - 'A'][(position->col) - '0'] == PLAYER_T
		&& (position->row != 'B') && (position->col != '7')) {
		//return 2 if empty, else 0
		return (board[(position->row - 2) - 'A'][(position->col + 1) - '0'] == EMPTY) ? 2 : 0;
	}
	//if next tile is T, skip is not available >> return 0 else regular advancement is possible >> return 1
	else
		return (board[(position->row - 1) - 'A'][(position->col) - '0'] == PLAYER_T) ? 0 : 1;
}

//recurive function that calculates hight of tree
int height_tree(SingleSourceMovesTreeNode *tNode) {

	if (tNode == NULL)	//if empty tree return height 0
		return 0;

	//call function to check which side of tree is bigger (add 1 for current level in tree)
	return 1 + Max(height_tree(tNode->next_move[LEFT]), height_tree(tNode->next_move[RIGHT]));
}

//creates a SingleSourceMoves tree node
SingleSourceMovesTreeNode *createNode(Board board, checkersPos *pos, unsigned short caps,
	SingleSourceMovesTreeNode *leftMove, SingleSourceMovesTreeNode *rightMove) {

	SingleSourceMovesTreeNode *node;

	node = (SingleSourceMovesTreeNode *)malloc(sizeof(SingleSourceMovesTreeNode));	//allocate memory for new node
	checkAllocation(node);															//memory allocation check

	node->pos = (checkersPos*)malloc(sizeof(checkersPos));		//allocate memory for new checker position
	checkAllocation(node->pos);									//memory allocation check

	node->pos->row = pos->row;				//set checker row
	node->pos->col = pos->col;				//set checker column
	copyBoard(board, node->board);			//call function to set board
	node->total_captures_So_far = caps;		//set total captures
	node->next_move[LEFT] = leftMove;		//set next left move
	node->next_move[RIGHT] = rightMove;		//set next right move

	return node;
}

//free memory allocation of tree
void freeTree(SingleSourceMovesTreeNode *node) {

	if (node == NULL)					//if empty tree return
		return;

	freeTree(node->next_move[LEFT]);	//free left side of tree
	freeTree(node->next_move[RIGHT]);	//free right side of tree
	free(node->pos);					//free position
	free(node);							//free current node
}

//memory allocation check
void checkAllocation(void *p) {

	if (!p) {
		printf("Allocation Error");
		exit(-1);
	}
}