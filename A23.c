#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include<string.h>


unsigned char exi = 1, gameend = 1, se = 1, gameover2 = 1,a = 1,gameend2 = 1;
int point;
double ema = 0.0;
struct keya{
	char up;
	char down;
	char left;
	char right;
	char enter;
};
struct keya keyb;


int stage[21][12] = {0};
int block[4][4] = {0};
int nextblock[4][4] = {0};
int field[21][12] = {0};


int block_list[7][4][4]  = 
{{{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}},
{{0,0,0,0},{0,1,1,0},{0,1,0,0},{0,1,0,0}},
{{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
{{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
{{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
{{0,0,0,0},{0,1,1,0},{0,0,1,0},{0,0,1,0}}};

int y = 0;
int x = 4;
int oneline, twoline, threeline, fourline;

int best, score;

void key1(char *m, unsigned char *b, char *f);
void key2(void);
void Gamestart();
void Initialize();
int CreateBlock();
void ShowGameField();
void ControlBlock();
int  CheckOverlap(int, int);
void MoveBlock(int, int);
int TurnBlock();
void DropBlock();
void LockBlock();
void CheckLines();
void InitializeNextBlock();
void gameover();
void put_data(int best);
void readscore();
void checkscore();

int main()
{ 
	int time = 0;
	int score = 0;
	while (exi)
	{
		Gamestart();
		Sleep(100);
		
			while(gameover2!=0)
			{
				Initialize();
				
				while(gameover2!=0)
				{	
					if(kbhit())
					{
						ControlBlock();
						}
				if(time < 10000)
				{
					time++;
					}
				else 
				{
					DropBlock();
					time = 0;
					}
				}
			}
		gameend2 = 1;
	    system("cls");
	    gameover();
	 }
		 
	return 0;
}
void checkscore()
{

if (score > best) {

printf("\n\n-------------------------\n\n-------------最高得点を更新しました!!-------------\n\n-------------------------\n\n");

best = score;
}

put_data(best);

}

void readscore()
{

best = get_data();

score = game();
}
void put_data(int best)
{
FILE *fp;
time_t t = time(NULL);
struct tm *local = localtime(&t);

if ((fp = fopen("SCORE.txt", "w")) == NULL) {
printf("エラー発生\n");
exit(-1);
}

fprintf(fp, "%d %d %d %d %d %d\n", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
fprintf(fp, "%d ", best);

fclose(fp);
}
void key1(char *m, unsigned char *b, char *f){
	static int cnt1 = 0;
	struct keya keyb;
	char c;
	if (_kbhit()){
		c = _getch();
		switch (c){
		case 0x50:
			if (*m<5)*m = *m + 1;
			keyb.down = 1;
			break;
		case 0x48:
			if (*m>0)*m = *m - 1;
			keyb.up = 1;
			break;

		case 0x4D:
			if (*b<8)*b = *b + 1;
			keyb.right = 1;
			break;

		case 0x4B:
			if (*b>0)*b = *b - 1;
			keyb.left = 1;
			break;

		case 5:
			keyb.enter = 1;
			*f = 0;
			point = 0;
			break;

		default:;
		}
	}
}
void key2(void){
	char c;
	if (_kbhit()){
		c = _getch();
		switch (c){
		case 0x50:
			keyb.down = 1;
			break;
		case 0x48:
			keyb.up = 1;
			break;

		case 0x4D:
			keyb.right = 1;
			break;

		case 0x4B:
			keyb.left = 1;
			break;

		case 13:
			keyb.enter = 1;
			break;

		default:;
		}
	}
}

void gameover() {
	int se = 1;
	while(gameend2){
		
		char syuryogamen[36][100] = {"",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■　　　　　　　　　　　　　　　　　　　　　　　　■■■■■",
		"■■■■■　　　　　RETRY or QUIT ? 　　　　　　　　　　　■■■■■",
		"■■■■■　　　　　　　　　　　　　　　　　　　　　　　　■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■【　】　　　RETRY 　　　　■■",
		"■■■■■■■■■■■■■■■■■■■　　　　　　　　　　　　　■■",
		"■■■■■■■■■■■■■■■■■■■【　】　　　QUIT　　　　　■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		};
		key2();
		if (keyb.down == 1){
			if (se == 1){
				se = 2;
			}
			else if (se == 2){
				se = 1;
			}
			keyb.down = 0;
		}
		if (keyb.up == 1){
			if (se == 1){
				se = 2;
			}
			else if (se == 2){
				se = 1;
			}
			keyb.up = 0;
		}
		if (se == 1){
		syuryogamen[12][41] = '■';
		}
		if (se == 2){
		syuryogamen[14][41] = '■';
		}
		if (keyb.enter == 1){
			keyb.enter = 0;
			if (se == 1){
				system("cls");
				gameend2 = 0;
				gameend = 1;
				gameover2 = 1;
				

			}
			if (se == 2){
				system("cls");
				gameend2 = 0;
				exi = 0;
			}
		}
		system("cls");
		printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n"
			"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s",syuryogamen[0], syuryogamen[1], syuryogamen[2], syuryogamen[3], syuryogamen[4],
			syuryogamen[5], syuryogamen[6], syuryogamen[7], syuryogamen[8], syuryogamen[9],
			syuryogamen[10], syuryogamen[11], syuryogamen[12], syuryogamen[13], syuryogamen[14],
			syuryogamen[15], syuryogamen[16], syuryogamen[17], syuryogamen[18], syuryogamen[19],
		    syuryogamen[20], syuryogamen[21], syuryogamen[22], syuryogamen[23], syuryogamen[24],
			syuryogamen[25], syuryogamen[26], syuryogamen[27], syuryogamen[28], syuryogamen[29],
			syuryogamen[30], syuryogamen[31], syuryogamen[32], syuryogamen[33], syuryogamen[34]);
		checkscore();
		printf("YOUR SCORE :　　%d\n",oneline*100 + twoline*300 + threeline*700 + fourline*1500);
		printf("BEST SCORE :　　%d\n", best);
		Sleep(100);
	}
}

void Gamestart()
{       int se = 1;
		exi = 1;
	while(gameend){
		
		char startgamen[36][100] = {"",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■　　　　■■■■■　■■■■■■　■■　■■■　　　　　■■■",
		"■■■■■■■■■■■■　■■■■■■　■■　■■■■■■■　■■■",
		"■　　　　　　　　■■■　　■■■■■　■■　■■■■■■　　■■■",
		"■■■■■　■■■■■■　■　■■■■　■■　■■■　　■　■■■■",
		"■■■■■　■■■■■■　■■　■■■■■■　■■■■■　■■■■■",
		"■■■■■　■■■■■■　■■■■■■■■■　■■■■　■　■■■■",
		"■■■■■　■■■■■■　■■■■■■■■■　■■■　■■■　■■■",
		"■■■■　■■■■■■■　■■■■■■■■　■■■　　■■■■　　■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■　　■■■　　　■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■　　■■■■■　■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■　■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■　　　■■■■■■■　■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■　■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■　　　■■■　■■",
		"■■【 】　　　テトリヌをプレイ　 ■■■■■■■■　■■■■■　■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■【 】　　　テトリスがしたい　　 ■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■　　■■■■■■",
		"■■【 】　　　ゲーム終了　　　　 ■■■■■■■■　　■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■　　■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■　　■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
		};
		key2();
		if (keyb.down == 1){
			if (se == 1){
				se = 2;
			}
			else if (se == 2){
				se = 3;
			}
			else if (se == 3){
				se = 1;
			}
			keyb.down = 0;
		}
		if (keyb.up == 1){
			if (se == 1){
				se = 3;
			}
			else if (se == 2){
				se = 1;
			}
			else if (se == 3){
				se = 2;
			}
			keyb.up = 0;
		}
		if (se == 1){
		startgamen[22][6] = '>';
		}
		if (se == 2){
		startgamen[24][6] = '>';
		}
		if (se == 3){
		startgamen[26][6] = '>';
		}
		if (keyb.enter == 1){
			keyb.enter = 0;
			if (se == 1){
				system("cls");
				gameend = 0;
			}
			if (se == 2){
				system("cls");
				gameend = 0;
			}
			if (se == 3){
				system("cls");
				exi = 0;
				gameend = 0;	
				gameend2 = 0;
				gameover2 = 0;
			}
		}
		system("cls");
		printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n"
			"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s   ",
			startgamen[0], startgamen[1], startgamen[2], startgamen[3], startgamen[4],
			startgamen[5], startgamen[6], startgamen[7], startgamen[8], startgamen[9],
			startgamen[10], startgamen[11], startgamen[12], startgamen[13], startgamen[14],
			startgamen[15], startgamen[16], startgamen[17], startgamen[18], startgamen[19],
		    startgamen[20], startgamen[21], startgamen[22], startgamen[23], startgamen[24],
			startgamen[25], startgamen[26], startgamen[27], startgamen[28], startgamen[29],
			startgamen[30], startgamen[31], startgamen[32], startgamen[33], startgamen[34]);
		readscore();
		Sleep(100);
	}
}


int game(void)
{
int i;

score = oneline*100 + twoline*300 + threeline*700 + fourline*1500;

return (score);
}


int get_data(void)
{
FILE *fp;
int best;

if ((fp = fopen("SCORE.txt", "r")) == NULL) {
best = 0;

printf("BEST SCORE :　　%d\n", best);
} 
else {
int year, month, day, h, m, s;

fscanf(fp, "%d%d%d%d%d%d", &year, &month, &day, &h, &m, &s);

fscanf(fp, "%d", &best);

printf("前回の終了は%04d年%02d月%02d日%02d時%02d分%02d秒でした。  ", year, month, day, h, m, s);

printf("  BEST SCORE :　　%d", best);

fclose(fp);
}

return (best);
}


void Initialize()
{
	int i, j;
	oneline = 0;
	twoline = 0;
	threeline = 0;
	fourline = 0;



	for(i = 0; i <= 20; i++) {
		for(j = 0; j <= 11; j++) {
			if((j == 0) || (j == 11) || (i == 20)) {
				field[i][j] = stage[i][j] = 9;
			}
			else {
				field[i][j] = stage[i][j] = 0;
			}
		}
	}
	InitializeNextBlock();
	CreateBlock();
	ShowGameField();
}
void InitializeNextBlock()
{
	int i, j;
	int block_type;

	srand((unsigned)time(NULL));
	block_type = rand()%7;

	for(i = 0; i<4; i++) {
		for(j= 0; j<4; j++) {
			nextblock[i][j] = block_list[block_type][i][j];
		}
	}
}

int CreateBlock()
{
	int i, j;
	int block_type;

	
	y = 0;
	x = 4;

	
	srand((unsigned)time(NULL));
	block_type = rand() % 7;


	
	for(i = 0; i<4; i++) {
		for(j = 0; j<4; j++) {
			block[i][j] = 0;
			block[i][j] = nextblock[i][j];
		}
	}

	
	for(i = 0; i<4; i++) {
		for(j= 0; j<4; j++) {
			nextblock[i][j] = block_list[block_type][i][j];
		}
	}

	
	for(i = 0; i<4; i++) {
		for(j = 0; j<4; j++) {
			field[i][j+4] = stage[i][j+4] + block[i][j];

			
			if(field[i][j+4] > 1) {
				gameover2 = 0;
				return 1;
			}

		}
	}
	return 0;
}


void ShowGameField()
{
	int i, j, k;

	
	system("cls");


	for(i = 0; i<21; i++) {
		for(j = 0; j < 12; j++) {

			switch(field[i][j]) {
			case 0:
				printf("　");
				break;
			case 9:
				printf("□");
				break;
			default:
				printf("■");
				break;
			}
		}
		if(i == 3) {
			printf("　　　□□□□□□");
		}
		if(i == 4) {
			printf("　　　□");
			for(k = 0; k<4; k++) {
				switch(nextblock[0][k]) {
				case 0:
					printf("　");
					break;
				default:
					printf("■");
					break;
				}
			}
			printf("□");
		}
		if(i == 5) {
			printf("　　　□");
			for(k = 0; k<4; k++) {
				switch(nextblock[1][k]) {
				case 0:
					printf("　");
					break;
				default:
					printf("■");
					break;
				}
			}
			printf("□");
		}
		if(i == 6) {
			printf("　　　□");
			for(k = 0; k<4; k++) {
				switch(nextblock[2][k]) {
				case 0:
					printf("　");
					break;
				default:
					printf("■");
					break;
				}
			}
			printf("□");
		}
		if(i == 7) {
			printf("　　　□");
			for(k = 0; k<4; k++) {
				switch(nextblock[3][k]) {
				case 0:
					printf("　");
					break;
				default:
					printf("■");
					break;
				}
			}
			printf("□");
		}
		if(i == 8) {
			printf("　　　□□□□□□");
		}
		if(i == 13) {
			printf("　　　□□□□□□");
		}
		if(i == 14) {
			printf("　　　　 SCORE");
		}
		if(i == 15) {
			printf("　　　　　%d", oneline*100 + twoline*300 + threeline*700 + fourline*1500);
		}
		if(i == 16){
			printf("　　　　 BEST");
			}
		if(i == 17){
			printf("　　　　　%d", best);
			}
		if(i == 18) {
			printf("　　　□□□□□□");
		}
		printf("\n");
	}

	
	printf("\nシングル：%d回　ダブル：%d回　トリプル：%d回　テトリス：%d回\n", oneline, twoline, threeline, fourline);

}


void ControlBlock()
{
	char key;

	key = getch();

	
	switch(key) {

	case 0x4d:
		if(!CheckOverlap(x+1, y)) {
			MoveBlock(x+1, y);
		}
		break;
	case 0x4b:
		if(!CheckOverlap(x-1, y)) {
			MoveBlock(x-1, y);
		}
		break;
	case 0x50:
		if(!CheckOverlap(x, y+1)) {
			MoveBlock(x, y+1);
		}
		break;
	case 0x48:
		TurnBlock();

	}
}


int CheckOverlap(int x2, int y2)
{
	int i, j;

	
	for(i = 0; i<4; i++) {
		for(j = 0; j<4; j++) {
			if(block[i][j]) {
				if(stage[y2 + i][x2 + j] != 0) {
					return 1;
				}
			}
		}
	}
	return 0;
}


void MoveBlock(int x2, int y2)
{
	int i, j;

	
		for(i = 0; i<4; i++) {
			for(j = 0; j<4; j++) {
				field[y+i][x+j] -= block[i][j];
			}
		}
	
		x = x2;
		y = y2;

	
		for(i = 0; i<4; i++) {
			for(j = 0; j<4; j++) {
				field[y+i][x+j] += block[i][j];
			}
		}

	ShowGameField();
}


int TurnBlock()
{
	int i, j;
	int temp[4][4] = {0};

	
	for(i = 0; i<4; i++) {
		for(j = 0; j<4; j++) {
			temp[i][j] = block[i][j];
		}
	}

	
	for(i = 0; i<4; i++) {
		for(j = 0; j<4; j++) {
			block[i][j] = temp[3-j][i];
		}
	}
	
	if(CheckOverlap(x, y)) {
		for(i = 0; i<4; i++) {
			for(j = 0; j<4; j++) {
				block[i][j] = temp[i][j];
			}
		}
		return 1;
	}

	
	for(i = 0; i<4; i++) {
		for(j = 0; j<4; j++) {
			field[y+i][x+j] -= temp[i][j];
			field[y+i][x+j] += block[i][j];
		}
	}

	ShowGameField();

	return 0;
}


void DropBlock()
{
	
	if(!CheckOverlap(x, y+1)) {
		MoveBlock(x, y+1);
	}
	
	else{
		LockBlock();
		CreateBlock();
		ShowGameField();
	}
}


void LockBlock()
{
	int i, j;

	
	for(i = 0; i<21; i++) {
		for(j = 0; j<12; j++) {
			stage[i][j] = field[i][j];
		}
	}

	CheckLines();

	
	for(i = 0; i<21; i++) {
		for(j = 0; j<12; j++) {
			field[i][j] = stage[i][j];
		}
	}


}

void CheckLines()
{
	int i, j, k;
	int comp;
	int lines = 0;

	while(1) {
		for(i = 0; i<20; i++) {
			comp = 1;

			for(j = 1; j<11; j++) {
				if(stage[i][j] == 0) {
					comp = 0;
				}
			}
			if(comp == 1) break;
		}
		if(comp == 0) break;

		lines++;
	
		for(j = 1; j<11; j++) {
			stage[i][j] = 0;
		}
		
		for(k = i; k>0; k--) {
			for(j = 1; j<11; j++) {
				stage[k][j] = stage[k-1][j];
			}
		}
	}
	switch(lines) {
	case 1:
		oneline++;
		break;
	case 2:
		twoline++;
		break;
	case 3:
		threeline++;
		break;
	case 4:
		fourline++;
		break;
	default:
		break;
	}
}