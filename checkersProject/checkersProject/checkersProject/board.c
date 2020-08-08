#include "board.h"

//Main Game Function
void PlayGame(Board board, Player starting_player) {

	BOOL game_on = 1;						//flag for game status
	char player_turn = starting_player;
	char current_player[10];				//string for current player output messages

	printBoard(board);

	while (game_on) {

		//print player turns on screen
		if (player_turn == PLAYER_B)
			strcpy(current_player, "BOTTOM_UP");
		else
			strcpy(current_player, "TOP_DOWN");

		printf("Player %s's turn\n", current_player);

		Turn(board, player_turn);			//execute player turn

		if (GameState(board, player_turn))	//check if there is a winner
			game_on = 0;					//0 = game over
		else
			NextTurn(&player_turn);			//change player turn	
	}

	printf("Winner is: Player %s!\n", current_player);
}

//checks if game is finished - 1 finished / 0 not finished
int GameState(Board board, char player) {

	if (player == PLAYER_B)
		return ScanBoardB(board);	//check board for player B
	else
		return ScanBoardT(board);	//check board for player T
}

//scans board for winner (PLAYER B) - reached finish line or no opponent players
int ScanBoardB(Board board) {

	BOOL opponentFound = 0;		//flag for opponent pieces (1 = opponent has pieces, 0 = else)

	int rows, cols;

	for (rows = 0; rows < BOARD_SIZE; rows++) {
		cols = checkRowEvenOdd(rows);								//checks if row is even or odd
		for (cols; cols < BOARD_SIZE; cols += 2) {
			if (rows != ROW_A && opponentFound)						//if opponent found and not on last line game not finished
				return 0;
			if (rows == ROW_A && board[rows][cols] == PLAYER_B) {
				return 1;											//player has reached the finish line
			}
			if (!opponentFound && board[rows][cols] == PLAYER_T) {	//look for opponent player
				opponentFound = 1;
			}
		}
	}

	return (!opponentFound);	//if no opponent found, return 1 - game finished else return 0- game ongoing
}

//scans board for winner (PLAYER T) - reached finish line or no opponent players
int ScanBoardT(Board board) {

	BOOL opponentFound = 0;		//flag for opponent pieces (1 = opponent has pieces, 0 = else)

	int rows, cols;

	for (rows = 0; rows < BOARD_SIZE; rows++) {
		if (opponentFound)											//checks if opponent was found
			rows = ROW_H;											//jump to last row
		cols = checkRowEvenOdd(rows);								//checks if row is even or odd
		for (cols; cols < BOARD_SIZE; cols += 2) {
			if (rows == ROW_H && board[rows][cols] == PLAYER_T) {
				return 1;											//player has reached the finish line
			}
			if (!opponentFound && board[rows][cols] == PLAYER_B) {	//look for opponent player
				opponentFound = 1;
				if (rows != ROW_H)									//if opponent found and not on last line, stop searching
					break;
			}
		}
	}

	return (!opponentFound);	//if no opponent found, return 1 - game finished
}

//changes player turn, stores result inside "player"
void NextTurn(char *player) {

	if (*player == PLAYER_T)
		*player = PLAYER_B;
	else
		*player = PLAYER_T;
}

//receives board and player, executes player's optimal move
void Turn(Board board, Player player) {

	//create list of all moves
	MultipleSourceMovesList *moveList = FindAllPossiblePlayerMoves(board, player);

	//if empty list (no moves available) - game over
	if (moveList->head == NULL) {
		
		free(moveList);										//free memory allocation of multiple move list
		if (player == PLAYER_B) {
			printf("Winner is: Player TOP_DOWN's");
			exit(3);
		}
		else {
			printf("Winner is: Player BOTTOM_UP's");
			exit(3);
		}
	}

	int maxLen = 0, i;

	MultipleSourceMovesListCell *current = moveList->head;	//set pointer to start of multiple move list
	SingleSourceMovesListCell *move;						//pointer to current cell in single move list

	move = selectOptimalMove(current, &maxLen, player);		//function returns head of optimal move list
	printMove(move);										//print player move
	PlayerMove(move, &maxLen, board);						//execute player move

	freeMultiList(moveList);								//free memory allocation of multiple move list
}

//select move from all available optimal moves
SingleSourceMovesListCell *selectOptimalMove(MultipleSourceMovesListCell *head, int *maxLen, Player player) {

	int caps, len, maxCaps = 0;

	SingleSourceMovesList *optimalMove = head->single_source_moves_list;	//initialized to first move list

	while (head) {
		turnHelper(&caps, &len, head->single_source_moves_list);	//calculates number of caps and length of list

		if (len >= *maxLen) {										//compare list length to longest length so far
			*maxLen = len;											//new longest length found
			if (player == PLAYER_T) {
				if (caps >= maxCaps) {								//check number of caps ('>=' ensures most advanced move - lowest on board)
					maxCaps = caps;
					optimalMove = head->single_source_moves_list;	//set pointer to new optimal move list
				}
			}
			else {
				if (caps > maxCaps) {								//check number of caps ('>' ensures most advanced move - highest on board)
					maxCaps = caps;
					optimalMove = head->single_source_moves_list;	//set pointer to new optimal move list
				}
			}
		}

		head = head->next;											//advance list pointer
	}

	return optimalMove->head;										//returns selected optimal move
}

//execute player move and update board
void PlayerMove(SingleSourceMovesListCell *move, int *moveLen, Board board) {

	checkersPos *start = (checkersPos *)malloc(sizeof(checkersPos));	//move start tile pointer
	checkAllocation(start);
	checkersPos *end = (checkersPos *)malloc(sizeof(checkersPos));		//move end tile pointer
	checkAllocation(end);

	int i;

	//execute optimal move and update board
	for (i = 0; i < *moveLen - 1; i++) {
		start->row = move->position->row;		//set start row
		start->col = move->position->col;		//set start column
		end->row = move->next->position->row;	//set end row
		end->col = move->next->position->col;	//set end column
		updateBoard(board, *start, *end);		//update board according to start and end locations
		move = move->next;						//advance list pointer
	}

	free(start);								//free memory allocation of source position
	free(end);									//free memory allocation of destination position
}

//print player move
void printMove(SingleSourceMovesListCell *currCell) {

	//print current tile
	printf("%c%c", currCell->position->row, currCell->position->col);

	//print the next tile/s in format: "->C4..."
	while (currCell->next) {
		printf("->%c%c", currCell->next->position->row, currCell->next->position->col);
		currCell = currCell->next;	//advance pointer to next move in list
	}

	printf("\n");
}

//calculates number of captures and length of single move list
void turnHelper(int *caps, int *len, SingleSourceMovesList *lst) {

	SingleSourceMovesListCell *current = lst->head;	//pointer to head of list

	*len = 0;										//initialize list length

	while (current) {
		*len = *len + 1;							//increase list size for every node
		if (current == lst->tail)					//if current node is tail
			*caps = current->captures;				//save number of captures
		current = current->next;
	}
}

//copy one board to another
void copyBoard(Board from, Board to)
{
	int rows, cols;

	//copy each element in the board one by one
	for (rows = 0; rows < BOARD_SIZE; rows++) {
		for (cols = 0; cols < BOARD_SIZE; cols++)
			to[rows][cols] = from[rows][cols];
	}
}

//receives a checker position and destination, updates board accordingly
void updateBoard(Board board, checkersPos pos, checkersPos dest) {

	char player = board[pos.row - 'A'][(pos.col - 1) - '0'];		//player T or B

	if (player == PLAYER_T) {	//player T
		if ((dest.row - pos.row) == 1) {							//move without capture
			board[pos.row - 'A'][(pos.col - 1) - '0'] = EMPTY;		//empty current tile
			board[dest.row - 'A'][(dest.col - 1) - '0'] = player;	//update destination tile
		}
		else if ((dest.col - pos.col) > 0) {						//move right with capture - empty current tile and captured tile
			board[pos.row - 'A'][(pos.col - 1) - '0'] = board[(pos.row - 'A') + 1][pos.col - '0'] = EMPTY;
			board[dest.row - 'A'][(dest.col - 1) - '0'] = player;	//update destination tile
		}
		else {														//move left with capture empty current tile and captured tile
			board[pos.row - 'A'][(pos.col - 1) - '0'] = board[(pos.row - 'A') + 1][(pos.col - 2) - '0'] = EMPTY;
			board[dest.row - 'A'][(dest.col - 1) - '0'] = player;	//update destination tile
		}
	}
	else {	//player B
		if ((pos.row - dest.row) == 1) {							//move without capture
			board[pos.row - 'A'][(pos.col - 1) - '0'] = EMPTY;		//empty current tile
			board[dest.row - 'A'][(dest.col - 1) - '0'] = player;	//update destination tile
		}
		else if ((dest.col - pos.col) > 0) {						//move right with capture - empty current tile and captured tile
			board[pos.row - 'A'][(pos.col - 1) - '0'] = board[(pos.row - 'A') - 1][pos.col - '0'] = EMPTY;
			board[dest.row - 'A'][(dest.col - 1) - '0'] = player;	//update destination tile
		}
		else {														//move left with capture - empty current tile and captured tile
			board[pos.row - 'A'][(pos.col - 1) - '0'] = board[(pos.row - 'A') - 1][(pos.col - 2) - '0'] = EMPTY;
			board[dest.row - 'A'][(dest.col - 1) - '0'] = player;	//update destination tile
		}
	}
}

//prints game board
void printBoard(Board board)
{
	int i, j;

	printf("+-+-+-+-+-+-+-+-+-+\n");
	printf("+ |1|2|3|4|5|6|7|8|\n");
	printf("+-+-+-+-+-+-+-+-+-+\n");

	for (i = 0; i < BOARD_SIZE; i++) {
		printf("|%c|", 'A' + i);
		for (j = 0; j < BOARD_SIZE; j++)
		{
			printf("%c|", board[i][j]);	//print each element one by one
		}
		printf("\n");
		printf("+-+-+-+-+-+-+-+-+-+\n");
	}
}
