#include "bytes.h"

//converts game board to binary file
void StoreBoard(Board board, char *filename) {

	FILE *fp = fopen(filename, "wb");		//open file in "write binary" mode

	if (!fp) {								//check if file opened successfully
		fprintf(stderr, "Error opening binary file in write mode\n");
		exit(1);
	}

	int rows, cols, bitsLeft = 8;			//"bitsLeft" - number of bits free in buffer
	BYTE mask = 0xC0, buffer = 0x00;		//mask and buffer for bit operations
	BYTE B = 0x80, T = 0x40, E = 0x00;		//two bit representation of game board tile

	for (rows = 0; rows < BOARD_SIZE; rows++) {
		for (cols = 0; cols < BOARD_SIZE; cols++) {

			if (!bitsLeft) { 				//bitsLeft = 0, write buffer to file, reset buffer & bitsLeft
				fwrite(&buffer, sizeof(BYTE), 1, fp);
				buffer = 0x00;
				bitsLeft = 8;
			}

			switch (board[rows][cols]) { 	//check piece in tile & insert bits to buffer
			case PLAYER_B:
				buffer = buffer | ((mask & B) >> 8 - bitsLeft);
				break;
			case PLAYER_T:
				buffer = buffer | ((mask & T) >> 8 - bitsLeft);
				break;
			case EMPTY:
				buffer = buffer | ((mask & E) >> 8 - bitsLeft);
				break;
			}

			if (cols == 7 && rows == 7)		//write last 4 tiles to file
				fwrite(&buffer, sizeof(BYTE), 1, fp);

			bitsLeft -= 2;					//reduce number of bits left in buffer
		}
	}
	fclose(fp);								//close file
}

//converts binary file to game board
void LoadBoard(char *filename, Board board) {

	FILE *fp = fopen(filename, "rb");	//open file in "read binary" mode

	if (!fp) {							//check if file opened successfully
		fprintf(stderr, "Error opening binary file in read mode\n");
		exit(1);
	}

	int rows, cols;
	BYTE mask = 0xC0, fourTiles[2], tile;

	for (rows = 0; rows < BOARD_SIZE; rows++) {

		fread(fourTiles, sizeof(BYTE), 2, fp);	//read two bytes from file (one row)

		for (cols = 0; cols < BOARD_SIZE; cols++) {

			//finds correct byte in the two-byte array (cols/4)
			//moves mask according to each tile in byte (2 * cols%4)
			tile = (mask >> (2 * (cols % 4))) & (*(fourTiles + (cols / 4)));

			tile <<= (2 * (cols % 4));	//shift extracted bits left to most significant bits

			switch (tile) {
			case 0x80:	//B = 10000000
				board[rows][cols] = PLAYER_B;
				break;
			case 0x40:	//T = 01000000
				board[rows][cols] = PLAYER_T;
				break;
			case 0x00:	//Empty = 00000000
				board[rows][cols] = EMPTY;
				break;
			}
		}
	}

	fclose(fp);
}