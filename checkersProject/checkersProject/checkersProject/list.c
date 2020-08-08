#include "list.h"

//returns move list with most captures of received move tree
SingleSourceMovesList *FindSingleSourceOptimalMove(SingleSourceMoveTree *moves_tree) {

	//empty tile
	if (moves_tree == NULL)
		return NULL;

	//no moves available
	if (moves_tree->source->next_move[LEFT] == NULL && moves_tree->source->next_move[RIGHT] == NULL)
		return NULL;

	//single move list that will be returned
	SingleSourceMovesList *moveList = (SingleSourceMovesList *)malloc(sizeof(SingleSourceMovesList));
	checkAllocation(moveList);
	makeEmptySingleList(moveList);								//initialize list head and tail

	newCellToEndList(moveList, moves_tree->source->pos, 0);		//set source to head of the list
	treeToList(moves_tree->source, moveList);					//create optimal move list from tree

	return moveList;											//returns the created move list
}

//make a list out of received tree node
void treeToList(SingleSourceMovesTreeNode *source, SingleSourceMovesList *list) {

	if (height_tree(source) == 1)	//if current node is leaf, return
		return;
	//if current node has 0 captures and has two children (root of tree)
	if (source->total_captures_So_far == 0 && source->next_move[LEFT] != NULL && source->next_move[RIGHT] != NULL)
	{
		//if left is not capture, and right is capture
		if (source->next_move[LEFT]->total_captures_So_far == 0 && source->next_move[RIGHT]->total_captures_So_far == 1) {
			newCellToEndList(list, source->next_move[RIGHT]->pos, source->next_move[RIGHT]->total_captures_So_far);	//add right to list
			treeToList(source->next_move[RIGHT], list);	//call function to create list from right side
			return;
		}
		//if left is capture, and right is not capture
		else if (source->next_move[LEFT]->total_captures_So_far == 1 && source->next_move[RIGHT]->total_captures_So_far == 0) {
			newCellToEndList(list, source->next_move[LEFT]->pos, source->next_move[LEFT]->total_captures_So_far);	//add left to list
			treeToList(source->next_move[LEFT], list);	//call function to create list from left side
			return;
		}
	}
	//if left side height is bigger than right side height
	if (height_tree(source->next_move[LEFT]) > height_tree(source->next_move[RIGHT])) {
		newCellToEndList(list, source->next_move[LEFT]->pos, source->next_move[LEFT]->total_captures_So_far);	//add left to list
		treeToList(source->next_move[LEFT], list);	//call function to create list from left side
	}
	//right side height is bigger/equal than left side height
	else if (height_tree(source->next_move[LEFT]) < height_tree(source->next_move[RIGHT])) {
		newCellToEndList(list, source->next_move[RIGHT]->pos, source->next_move[RIGHT]->total_captures_So_far);	//add right to list
		treeToList(source->next_move[RIGHT], list);	//call function to create list from right side
	}
	//same height on both sides, randomly select a direction to move
	else {
		int r = rand() % 2;	//random number 0 or 1
		if (r) {
			newCellToEndList(list, source->next_move[LEFT]->pos, source->next_move[LEFT]->total_captures_So_far);	//add left to list
			treeToList(source->next_move[LEFT], list);	//call function to create list from left side
		}
		else {
			newCellToEndList(list, source->next_move[RIGHT]->pos, source->next_move[RIGHT]->total_captures_So_far);	//add right to list
			treeToList(source->next_move[RIGHT], list);	//call function to create list from right side
		}
	}
}

//creates a SingleSourceMovesListCell
SingleSourceMovesListCell *createSingleMoveListCell(checkersPos *pos, unsigned short caps, SingleSourceMovesListCell *next) {

	SingleSourceMovesListCell *node;

	node = (SingleSourceMovesListCell *)malloc(sizeof(SingleSourceMovesListCell));	//allocate memory for new node
	checkAllocation(node);															//memory allocation check

	node->position = (checkersPos*)malloc(sizeof(checkersPos));						//allocate memory for new checker position
	checkAllocation(node->position);												//memory allocation check

	node->position->row = pos->row;		//set position row
	node->position->col = pos->col;		//set position column
	node->captures = caps;				//set total captures
	node->next = next;					//set next cell

	return node;
}

//create list of all possible optimal moves for received player
MultipleSourceMovesList *FindAllPossiblePlayerMoves(Board board, Player player) {

	SingleSourceMovesList *singleMoveList;											//single move list

	MultipleSourceMovesList *multipleMoveList = (MultipleSourceMovesList *)malloc(sizeof(MultipleSourceMovesList));
	checkAllocation(multipleMoveList);
	makeEmptyMultiList(multipleMoveList);											//initialize list head and tail

	checkersPos *pos = (checkersPos *)malloc(sizeof(checkersPos));
	checkAllocation(pos);

	int rows, cols;																	//index for rows and columns

	for (rows = 0; rows < BOARD_SIZE; rows++) {
		cols = checkRowEvenOdd(rows);												//checks if row is even or odd
		for (cols; cols < BOARD_SIZE; cols += 2) {
			if (board[rows][cols] == player) {
				pos->row = 'A' + rows;												//set position row
				pos->col = '1' + cols;												//set position column
				singleMoveList = findOptimalMove(board, pos);						//create single move list for current position
				if (singleMoveList != NULL)
					newMultipleMoveCellToEndList(multipleMoveList, singleMoveList);	//add created list to multiple move list
			}
		}
	}

	free(pos);
	return multipleMoveList;
}

//find and create list of optimal move for received board position
SingleSourceMovesList *findOptimalMove(Board board, checkersPos *pos) {

	SingleSourceMoveTree *movesTree = FindSingleSourceMoves(board, pos);		//create tree of possible moves
	SingleSourceMovesList *movesList = FindSingleSourceOptimalMove(movesTree);	//create optimal move list out of tree

	if (movesTree != NULL) {
		freeTree(movesTree->source);	//free memory allocation of created tree
		free(movesTree);
	}

	return movesList;
}

//create new node from receieved parameters and insert to tail of list
void newMultipleMoveCellToEndList(MultipleSourceMovesList* multipleMoveList, SingleSourceMovesList *singleMoveList) {

	MultipleSourceMovesListCell *newTail;

	newTail = createMultipleMoveListCell(singleMoveList, NULL);	//creates new multiple move list node
	insertMultipleMoveNodeToTail(multipleMoveList, newTail);	//inserts node to received list
}

//inserts node to tail of list
void insertMultipleMoveNodeToTail(MultipleSourceMovesList *multipleMoveList, MultipleSourceMovesListCell *newTail) {

	if (isEmptyMultiList(*multipleMoveList))		//if list is empty, set new node to head and tail
		multipleMoveList->head = multipleMoveList->tail = newTail;
	else {
		multipleMoveList->tail->next = newTail;		//point last node at new node
		multipleMoveList->tail = newTail;			//update list tail
	}
}

//initialize head and tail of MultipleSourceMovesList
void makeEmptyMultiList(MultipleSourceMovesList *list) {

	list->head = list->tail = NULL;
}

//free memory allocation of MultipleSourceMovesList
void freeMultiList(MultipleSourceMovesList *list) {

	MultipleSourceMovesListCell *tmp, *curr;

	curr = list->head;									//pointer to head of list

	while (curr) {
		tmp = curr->next;								//set tmp to next node
		freeSingleList(curr->single_source_moves_list);	//free single move list
		free(curr->single_source_moves_list);			//free single move list field
		free(curr);										//free current node
		curr = tmp;										//advance current node pointer
	}

	free(list);											//free the list itself
}

//check if multiple move list is empty, 1 = empty, 0 = not empty
int isEmptyMultiList(MultipleSourceMovesList lst) {

	return(lst.head == NULL);
}

//checks if row is even or odd
int checkRowEvenOdd(int rowIndex) {

	return (rowIndex % 2 == 0) ? EVEN : ODD;
}

//creates a MultipleSourceMovesListCell
MultipleSourceMovesListCell *createMultipleMoveListCell(SingleSourceMovesList *singleMoveList, MultipleSourceMovesListCell *next) {

	MultipleSourceMovesListCell *node;

	node = (MultipleSourceMovesListCell *)malloc(sizeof(MultipleSourceMovesListCell));	//allocate memory for new node
	checkAllocation(node);																//memory allocation check

	node->single_source_moves_list = singleMoveList;	//set single move list
	node->next = next;									//set next cell

	return node;
}

//create new node from receieved parameters and insert to tail of list
void newCellToEndList(SingleSourceMovesList* lst, checkersPos *pos, unsigned short caps)
{
	SingleSourceMovesListCell *newTail;

	newTail = createSingleMoveListCell(pos, caps, NULL);	//create new single move list node
	insertNodeToTail(lst, newTail);							//inserts node to tail of list
}

//inserts node to tail of list
void insertNodeToTail(SingleSourceMovesList *ls, SingleSourceMovesListCell *newTail)
{
	if (isEmptySingleList(*ls))		//if list is empty, set new node to head and tail
		ls->head = ls->tail = newTail;
	else {
		ls->tail->next = newTail;	//point last node at new node
		ls->tail = newTail;			//update list tail
	}
}

//initialize head and tail of SingleSourceMovesList
void makeEmptySingleList(SingleSourceMovesList *list) {
	list->head = list->tail = NULL;
}

//free memory allocation of SingleSourceMovesList
void freeSingleList(SingleSourceMovesList *list) {

	SingleSourceMovesListCell *tmp, *curr;

	curr = list->head;			//pointer to head of list

	while (curr) {
		tmp = curr->next;		//set tmp to next node
		free(curr->position);	//free position field
		free(curr);				//free list node
		curr = tmp;				//advance current node pointer
	}
}

//check if single move list is empty, 1 = empty, 0 = not empty
int isEmptySingleList(SingleSourceMovesList lst)
{
	return(lst.head == NULL);
}