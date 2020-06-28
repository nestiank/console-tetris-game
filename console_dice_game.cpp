#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<time.h>

#define w 119
#define W 87
#define a 97
#define A 65
#define s 115
#define S 83
#define d 100
#define D 68
#define f 102
#define F 70

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define m 109
#define M 77

#define p 112
#define P 80
#define ESC 27

#define FIRST 49
#define SECOND 50
#define THIRD 51
#define FOURTH 52

#define ACTIVE_NOW_BLOCK -5
#define PLAYER_1_BLOCK -2
#define CEILLING -1
#define EMPTY 0
#define WALL 1
#define PLAYER_2_BLOCK 2

#define MAIN_X 11
#define MAIN_Y 23
#define MAIN_X_ADJ 3
#define MAIN_Y_ADJ 1
#define STATUS_X_ADJ 15 // MAIN_X_ADJ + MAIN_X + 1

int blocks[7][4][4][4] = {
	{ { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 } },
	{ { 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 } },
	{ { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0 } },
	{ { 0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0 },{ 0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0 },{ 0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0 } } };

int bx, by, b_type, b_rotation, b_type_next, b_itemApply = 0;
int main_org[MAIN_Y][MAIN_X], main_cpy[MAIN_Y][MAIN_X];
int key, speed, score1, score2, item1G, item1U, item2G, item2U, turnPlayer;
int turnTimes, gameTimes = 0;
int new_block_on, crush_on, item_on = 0;
time_t startTimestamp;

FILE *resultOut = fopen("result.txt", "w");

void showMenu();
void reset();
void reset_main();
void reset_main_cpy();
void draw_map();
void draw_main();
void new_block();
void check_key();
int check_crush(int bx, int by, int rotation);
void drop_block();
void move_block(int dir);
void check_line();
void check_game_over();
void pause();
void print_game_result();
void activateItem();

void gotoxy(int x, int y)
{
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void setcursortype(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;
	switch(c) {
		case NOCURSOR:
			CurInfo.dwSize = 1;
			CurInfo.bVisible = FALSE;
			break;
		case SOLIDCURSOR:
			CurInfo.dwSize = 100;
			CurInfo.bVisible = TRUE;
			break;
		case NORMALCURSOR:
			CurInfo.dwSize = 20;
			CurInfo.bVisible = TRUE;
			break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void showMenu()
{
	gotoxy(5, 4); printf("¦£¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤");
	gotoxy(5, 5); printf("¦¢                          ¦¢");
	gotoxy(5, 6); printf("¦¢                          ¦¢");
	gotoxy(5, 7); printf("¦¢                          ¦¢");
	gotoxy(5, 8); printf("¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥");
	
	gotoxy(10, 6); printf("T E T R I S");
	gotoxy(5, 11); printf("Please Enter Any Key to Start..");
	
	gotoxy(5, 13); printf("¦¢Keys For Player #1");
	gotoxy(5, 14); printf("¦¢  W   : Rotate");
	gotoxy(5, 15); printf("¦¢A   D : Left / Right");
	gotoxy(5, 16); printf("¦¢  S   : Drop");
	gotoxy(5, 17); printf("¦¢  F   : Use Item");
	
	gotoxy(5, 19); printf("¦¢Keys For Player #2");
	gotoxy(5, 20); printf("¦¢  ¡â   : Rotate");
	gotoxy(5, 21); printf("¦¢¢·  ¢¹ : Left / Right");
	gotoxy(5, 22); printf("¦¢  ¡ä   : Drop");
	gotoxy(5, 23); printf("¦¢   M   : Use Item");
	
	gotoxy(18, 13); printf("¦¢Common Keys");
	gotoxy(18, 14); printf("¦¢P : Pause");
	gotoxy(18, 15); printf("¦¢ESC : Quit");

	while(1) {
		if (kbhit()) break;
	}
	while(kbhit()) getch();
}

void reset()
{
	startTimestamp = time(NULL);
	score1 = 0;
	score2 = 0;
	item1G = 0;
	item1U = 0;
	item2G = 0;
	item2U = 0;
	turnPlayer = 1;
	turnTimes = 0;
	key = 0;
	crush_on = 0;
	speed = 100;
	
	system("cls");
	reset_main();
	draw_map();
	draw_main();
	b_type_next = rand() % 7;
	new_block();
}
void reset_main()
{
	for (int i = 0; i < MAIN_Y; i++) {
		for (int j = 0; j < MAIN_X; j++) {
			main_org[i][j] = 0;
			main_cpy[i][j] = 100;
		}
	}
	for (int j = 1; j < MAIN_X; j++) {
		main_org[3][j] = CEILLING;
	}
	for (int i = 1; i < MAIN_Y - 1; i++) {
		main_org[i][0] = WALL;
		main_org[i][MAIN_X - 1] = WALL;
	}
	for (int j = 0; j < MAIN_X; j++) {
		main_org[MAIN_Y - 1][j] = WALL;
	}
}
void reset_main_cpy()
{
	for (int i = 0; i < MAIN_Y; i++) {
		for (int j = 0; j < MAIN_X; j++) {
			main_cpy[i][j] = 100;
		}
	}
}
void draw_map()
{
	gotoxy(STATUS_X_ADJ, 5); printf("+-- N E X T --+ ");
	gotoxy(STATUS_X_ADJ, 6); printf("|             | ");
	gotoxy(STATUS_X_ADJ, 7); printf("|             | ");
	gotoxy(STATUS_X_ADJ, 8); printf("|             | ");
	gotoxy(STATUS_X_ADJ, 9); printf("|             | ");
	gotoxy(STATUS_X_ADJ, 10); printf("+-------------+ ");
	
	gotoxy(STATUS_X_ADJ, 12); printf("¦¢Player #1");
	gotoxy(STATUS_X_ADJ, 13); printf("¦¢Score : %03d", score1);
	gotoxy(STATUS_X_ADJ, 14); printf("¦¢Items : %03d", item1G - item1U);
	gotoxy(STATUS_X_ADJ + 8, 12); printf("¦¢Player #2");
	gotoxy(STATUS_X_ADJ + 8, 13); printf("¦¢Score : %03d", score2);
	gotoxy(STATUS_X_ADJ + 8, 14); printf("¦¢Items : %03d", item2G - item2U);
	
	if (turnPlayer) {
		gotoxy(STATUS_X_ADJ, 17); printf("Now Turn : Player #1");
		gotoxy(STATUS_X_ADJ, 19); printf("   W   : Rotate       ¦¢  F  : Item");
		gotoxy(STATUS_X_ADJ, 20); printf(" A   D : Left / Right ¦¢  P  : Pause");
		gotoxy(STATUS_X_ADJ, 21); printf("   S   : Drop         ¦¢ ESC : Quit");
	}
	else {
		gotoxy(STATUS_X_ADJ, 17); printf("Now Turn : Player #2");
		gotoxy(STATUS_X_ADJ, 19); printf("  ¡â   : Rotate       ¦¢  M  : Item");
		gotoxy(STATUS_X_ADJ, 20); printf("¢·  ¢¹ : Left / Right ¦¢  P  : Pause");
		gotoxy(STATUS_X_ADJ, 21); printf("  ¡ä   : Drop         ¦¢ ESC : Quit");
	}
}
void draw_main() {
	for (int j = 1; j < MAIN_X - 1; j++) {
		if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
	}
	for (int i = 0; i < MAIN_Y; i++) {
		for (int j = 0; j < MAIN_X; j++) {
			if (main_cpy[i][j] != main_org[i][j]) {
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch(main_org[i][j]) {
					case EMPTY:
						printf("  ");
						break;
					case CEILLING:
						printf(". ");
						break;
					case WALL:
						printf("¢Ì");
						break;
					case PLAYER_1_BLOCK:
						printf("¡á");
						break;
					case PLAYER_2_BLOCK:
						printf("¡à");
						break;
					case ACTIVE_NOW_BLOCK:
						if (turnPlayer) printf("¡á");
						else printf("¡à");
						break;
				}
			}
		}
	}
	for (int i = 0; i < MAIN_Y; i++) {
		for (int j = 0; j < MAIN_X; j++) {
			main_cpy[i][j] = main_org[i][j];
		}
	}
}
void new_block()
{
	bx = (MAIN_X / 2) - 1;
	by = 0;
	if (b_itemApply) {
		b_type = b_itemApply;
		b_itemApply = 0;
	}
	else {
		b_type = b_type_next;
		b_type_next = rand() % 7;
	}
	b_rotation = 0;
	new_block_on = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[b_type][b_rotation][i][j]) {
				main_org[by + i][bx + j] = ACTIVE_NOW_BLOCK;
			}
		}
	}
	for (int i = 1; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j]) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				if (!turnPlayer) printf("¡á");
				else printf("¡à");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}
void check_key()
{
	if (kbhit()) {
		key = getch();
		switch(key) {
			case p:
			case P:
				pause();
				break;
			case ESC:
				system("cls");
				fclose(resultOut);
				exit(0);
			case f:
			case F:
				if (!item_on && (item1G - item1U)) item_on = 1;
				break;
			case m:
			case M:
				if (!item_on && (item2G - item2U)) item_on = 2;
				break;
		}
		if (turnPlayer) {
			switch(key) {
				case a:
				case A:
					if (check_crush(bx - 1, by, b_rotation)) move_block(LEFT);
					break;
				case d:
				case D:
					if (check_crush(bx + 1, by, b_rotation)) move_block(RIGHT);
					break;
				case s:
				case S:
					if (check_crush(bx, by + 1, b_rotation)) move_block(DOWN);
					break;
				case w:
				case W:
					if (check_crush(bx, by, (b_rotation + 1) % 4)) move_block(UP);
					else if (crush_on && check_crush(bx, by - 1, (b_rotation + 1) % 4)) move_block(100);
			}
		}
		else if (key == 224) {
			while(key == 224) key = getch();
			switch(key) {
				case LEFT:
					if (check_crush(bx - 1, by, b_rotation)) move_block(LEFT);
					break;
				case RIGHT:
					if (check_crush(bx + 1, by, b_rotation)) move_block(RIGHT);
					break;
				case DOWN:
					if (check_crush(bx, by + 1, b_rotation)) move_block(DOWN);
					break;
				case UP:
					if (check_crush(bx, by, (b_rotation + 1) % 4)) move_block(UP);
					else if (crush_on && check_crush(bx, by - 1, (b_rotation + 1) % 4)) move_block(100);
			}
		}
	}
	while (kbhit()) getch();
}
int check_crush(int bx, int by, int b_rotation)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[b_type][b_rotation][i][j] && (main_org[by + i][bx + j] > 0 || main_org[by + i][bx + j] == -2)) return 0;
		}
	}
	return 1;
}
void drop_block()
{
	if (crush_on) {
		if (!check_crush(bx, by + 1, b_rotation)) {
			for (int i = 0; i < MAIN_Y; i++) { 
				for (int j = 0; j < MAIN_X; j++) {
					if (main_org[i][j] == ACTIVE_NOW_BLOCK) {
						if (turnPlayer) main_org[i][j] = PLAYER_1_BLOCK;
						else main_org[i][j] = PLAYER_2_BLOCK;
					}
				}
			}
			check_line();
			new_block_on++;
		}
		else {
			move_block(DOWN);
		}
		crush_on--;
	}
	else {
		if (check_crush(bx, by + 1, b_rotation)) move_block(DOWN);
		else crush_on++;
	}
}
void move_block(int dir)
{
	switch(dir) {
		case LEFT:
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) main_org[by + i][bx + j] = EMPTY;
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) {
						main_org[by + i][bx + j - 1] = ACTIVE_NOW_BLOCK;
					}
				}
			}
			bx--;
			break;
		case RIGHT:
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) main_org[by + i][bx + j] = EMPTY;
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) {
						main_org[by + i][bx + j + 1] = ACTIVE_NOW_BLOCK;
					}
				}
			}
			bx++;
			break;
		case DOWN:
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) main_org[by + i][bx + j] = EMPTY;
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) {
						main_org[by + i + 1][bx + j] = ACTIVE_NOW_BLOCK;
					}
				}
			}
			by++;
			break;
		case UP:
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) main_org[by + i][bx + j] = EMPTY;
				}
			}
			b_rotation = (b_rotation + 1) % 4;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) {
						main_org[by + i][bx + j] = ACTIVE_NOW_BLOCK;
					}
				}
			}
			break;
		case 100:
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) main_org[by + i][bx + j] = EMPTY;
				}
			}
			b_rotation = (b_rotation + 1) % 4;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (blocks[b_type][b_rotation][i][j]) {
						main_org[by + i - 1][bx + j] = ACTIVE_NOW_BLOCK;
					}
				}
			}
			by--;
			break;
	}
}
void check_line()
{
	int checkItemAccessibility = 0;
	for (int i = MAIN_Y - 2; i > 3;) {
		int block_amount = 0;
		for (int j = 1; j < MAIN_X - 1; j++) {
			if (main_org[i][j] > 0 || main_org[i][j] == -2) block_amount++;
		}
		if (block_amount == MAIN_X - 2) {
			turnPlayer ? score1++ : score2++;
			checkItemAccessibility++;
			for (int j = i; j > 1; j--) {
				for (int k = 1; k < MAIN_X - 1; k++) {
					if (main_org[j - 1][k] == CEILLING) main_org[j][k] = EMPTY;
					else main_org[j][k] = main_org[j - 1][k];
				}
			}
		}
		else i--;
	}
	if (turnPlayer) {
		if (checkItemAccessibility > 1) item1G++;
		gotoxy(STATUS_X_ADJ, 13); printf("¦¢Score : %03d", score1);
		gotoxy(STATUS_X_ADJ, 14); printf("¦¢Items : %03d", item1G - item1U);
		
		gotoxy(STATUS_X_ADJ, 17); printf("Now Turn : Player #2");
		gotoxy(STATUS_X_ADJ, 19); printf("  ¡â   : Rotate       ¦¢  M  : Item");
		gotoxy(STATUS_X_ADJ, 20); printf("¢·  ¢¹ : Left / Right ¦¢  P  : Pause");
		gotoxy(STATUS_X_ADJ, 21); printf("  ¡ä   : Drop         ¦¢ ESC : Quit");
		turnPlayer--;
		turnTimes++;
	}
	else {
		if (checkItemAccessibility > 1) item2G++;
		gotoxy(STATUS_X_ADJ + 8, 13); printf("¦¢Score : %03d", score2);
		gotoxy(STATUS_X_ADJ + 8, 14); printf("¦¢Items : %03d", item2G - item2U);
		
		gotoxy(STATUS_X_ADJ, 17); printf("Now Turn : Player #1");
		gotoxy(STATUS_X_ADJ, 19); printf("   W   : Rotate       ¦¢  F  : Item");
		gotoxy(STATUS_X_ADJ, 20); printf(" A   D : Left / Right ¦¢  P  : Pause");
		gotoxy(STATUS_X_ADJ, 21); printf("   S   : Drop         ¦¢ ESC : Quit");
		turnPlayer++;
		turnTimes++;
	}
	
	if (item_on) activateItem();
}
void check_game_over()
{
	for (int i = 1; i < MAIN_X - 2; i++) {
		if (main_org[3][i] > 0 || main_org[3][i] == -2 || turnTimes == 60) {
			gotoxy(6, 5); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
			gotoxy(6, 6); printf("¢Ç                              ¢Ç");
			gotoxy(6, 7); printf("¢Ç  +-----------------------+   ¢Ç");
			gotoxy(6, 8); printf("¢Ç  |  G A M E  O V E R..   |   ¢Ç");
			gotoxy(6, 9); printf("¢Ç  +-----------------------+   ¢Ç");
			gotoxy(6, 10); printf("¢Ç   Player #1 Score : %03d      ¢Ç", score1);
			gotoxy(6, 11); printf("¢Ç   Player #2 Score : %03d      ¢Ç", score2);
			gotoxy(6, 12); printf("¢Ç                              ¢Ç");
			gotoxy(6, 13); printf("¢Ç  Press any key to restart..  ¢Ç");
			gotoxy(6, 14); printf("¢Ç                              ¢Ç");
			gotoxy(6, 15); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
			print_game_result();
			Sleep(1000);
			while(kbhit()) getch();
			key = getch();
			reset();
		}
	}
}
void print_game_result()
{
	char timeBuffer[30];
	strftime(timeBuffer, 30, "%Y/%m/%d %H:%M:%S KST\n", localtime(&startTimestamp));
	
	fprintf(resultOut, "¦¢Game Result Log #%d\n", ++gameTimes);
	fprintf(resultOut, "¦¢Start Time           : %s", timeBuffer);
	fprintf(resultOut, "¦¢\n");
	fprintf(resultOut, "¦¢Player #1 Score      : %03d\n", score1);
	fprintf(resultOut, "¦¢Player #1 Items Gain : %03d\n", item1G);
	fprintf(resultOut, "¦¢Player #1 Items Used : %03d\n", item1U);
	fprintf(resultOut, "¦¢Player #2 Score      : %03d\n", score2);
	fprintf(resultOut, "¦¢Player #2 Items Gain : %03d\n", item2G);
	fprintf(resultOut, "¦¢Player #2 Items Used : %03d\n", item2U);
	fprintf(resultOut, "¦¢\n");
	if (score1 > score2) fprintf(resultOut, "¦¢Final Winner         : Player #%d\n\n", 1);
	else if (score1 < score2) fprintf(resultOut, "¦¢Final Winner         : Player #%d\n\n", 2);
	else fprintf(resultOut, "¦¢Final Winner         : Draw\n\n");
}
void pause()
{
	gotoxy(6, 5); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
	gotoxy(6, 6); printf("¢Ç                              ¢Ç");
	gotoxy(6, 7); printf("¢Ç  +------------------------+  ¢Ç");
	gotoxy(6, 8); printf("¢Ç  |       P A U S E        |  ¢Ç");
	gotoxy(6, 9); printf("¢Ç  +------------------------+  ¢Ç");
	gotoxy(6, 10); printf("¢Ç   Press any key to resume.   ¢Ç");
	gotoxy(6, 11); printf("¢Ç                              ¢Ç");
	gotoxy(6, 12); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
	
	getch();
	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();
	
	for (int i = 1; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j]) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				if (!turnPlayer) printf("¡á");
				else printf("¡à");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}

void activateItem()
{
	item_on == 1 ? item1U++ : item2U++;
	
	for (int i = 5; i > 0; i--) {
		gotoxy(STATUS_X_ADJ, 24); printf("Item Applied! Automatic Pause Lasts for %d seconds", i);
		Sleep(1000);
	}
	
	if (turnPlayer) {
		gotoxy(6, 5); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
		gotoxy(6, 6); printf("¢Ç                              ¢Ç");
		gotoxy(6, 7); printf("¢Ç  +------------------------+  ¢Ç");
		gotoxy(6, 8); printf("¢Ç  |        I T E M         |  ¢Ç");
		gotoxy(6, 9); printf("¢Ç  +------------------------+  ¢Ç");
		gotoxy(6, 10); printf("¢Ç   Press The Block Number..   ¢Ç");
		gotoxy(6, 11); printf("¢Ç                              ¢Ç");
		gotoxy(6, 12); printf("¢Ç     1 ¡á         2 ¡á¡á      ¢Ç");
		gotoxy(6, 13); printf("¢Ç       ¡á           ¡á¡á      ¢Ç");
		gotoxy(6, 14); printf("¢Ç       ¡á                     ¢Ç");
		gotoxy(6, 15); printf("¢Ç       ¡á                     ¢Ç");
		gotoxy(6, 16); printf("¢Ç                              ¢Ç");
		gotoxy(6, 17); printf("¢Ç     3   ¡á       4   ¡á      ¢Ç");
		gotoxy(6, 18); printf("¢Ç       ¡á¡á¡á       ¡á¡á      ¢Ç");
		gotoxy(6, 19); printf("¢Ç                    ¡á        ¢Ç");
		gotoxy(6, 20); printf("¢Ç                              ¢Ç");
		gotoxy(6, 21); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
	}
	else {
		gotoxy(6, 5); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
		gotoxy(6, 6); printf("¢Ç                              ¢Ç");
		gotoxy(6, 7); printf("¢Ç  +------------------------+  ¢Ç");
		gotoxy(6, 8); printf("¢Ç  |        I T E M         |  ¢Ç");
		gotoxy(6, 9); printf("¢Ç  +------------------------+  ¢Ç");
		gotoxy(6, 10); printf("¢Ç   Press The Block Number..   ¢Ç");
		gotoxy(6, 11); printf("¢Ç                              ¢Ç");
		gotoxy(6, 12); printf("¢Ç     1 ¡à         2 ¡à¡à      ¢Ç");
		gotoxy(6, 13); printf("¢Ç       ¡à           ¡à¡à      ¢Ç");
		gotoxy(6, 14); printf("¢Ç       ¡à                     ¢Ç");
		gotoxy(6, 15); printf("¢Ç       ¡à                     ¢Ç");
		gotoxy(6, 16); printf("¢Ç                              ¢Ç");
		gotoxy(6, 17); printf("¢Ç     3   ¡à       4   ¡à      ¢Ç");
		gotoxy(6, 18); printf("¢Ç       ¡à¡à¡à       ¡à¡à      ¢Ç");
		gotoxy(6, 19); printf("¢Ç                    ¡à        ¢Ç");
		gotoxy(6, 20); printf("¢Ç                              ¢Ç");
		gotoxy(6, 21); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
	}
	
	key = getch();
	switch(key) {
		case FIRST:
			b_itemApply = 1;
			break;
		case SECOND:
			b_itemApply = 0;
			break;
		case THIRD:
			b_itemApply = 6;
			break;
		case FOURTH:
			b_itemApply = 2;
			break;
	}
	
	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();
	item_on = 0;
}

int main()
{
	srand((unsigned)time(NULL));
	setcursortype(NOCURSOR);
	
	showMenu();
	reset();
	while(1) {
		for (int i = 0; i < 5; i++) {
			check_key();
			draw_main();
			Sleep(speed);
			if (crush_on && !check_crush(bx, by + 1, b_rotation)) Sleep(100);
		}
		drop_block();
		check_game_over();
		if (new_block_on) new_block();
	}
}
