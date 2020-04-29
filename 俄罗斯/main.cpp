#include <stdio.h>
#include <time.h>
#include <graphics.h>
#include <conio.h>

int score = 0;    // �ܷ�
int rank = 0;      //�ȼ�

#define BLOCK_COUNT    5      //5�ַ���
#define BLOCK_WIDTH     5      //������
#define BLOCK_HEIDHT    5      //���� ��
#define UNIT_SIZE           20     //����Ĵ�С

#define START_X    130
#define START_Y    30

#define KEY_UP           72
#define KEY_RIGHT     77
#define KEY_DOWN    80
#define KEY_LEFT        75
#define KEY_SPACE     32

int speed = 500;   //0.5��
int minX = 30;
int minY = 30;

//����ĳ���
typedef enum{
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWN, 
	BLOCK_LEFT
}block_dir_t;
//������ƶ�
typedef enum{
	MOVE_DOWN,     //�½�
	MOVE_LEFT,         //����
	MOVE_RIGHT       //����
}move_dir_t;

int NextIndex = -1;   //��һ������
int BlockIndex = -1;  //��ǰ���������

int color[BLOCK_COUNT] = {
	GREEN, CYAN, MAGENTA, BROWN, YELLOW
};

int visit[30][15];  //��������
int markColor[30][15]; //��ʾ��Ӧλ�õ���ɫ

int block[BLOCK_COUNT*4][BLOCK_WIDTH][BLOCK_HEIDHT] = {
	// | �η���
	{ 0,0,0,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,0,0,0,0,
	  0,1,1,1,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,0,0,0,0,
	  0,1,1,1,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	// L �η���
	{ 0,0,0,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,1,1,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,0,0,0,0,
	  0,1,1,1,0,
	  0,1,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,1,1,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,0,0,1,0,
	  0,1,1,1,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	// �� �η���
	{ 0,0,0,0,0,
	  0,1,1,0,0,
	  0,1,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,1,1,0,0,
	  0,1,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,1,1,0,0,
	  0,1,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,1,1,0,0,
	  0,1,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	// T �η���
	{ 0,0,0,0,0,
	  0,1,1,1,0,
	  0,0,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,0,0,1,0,
	  0,0,1,1,0,
	  0,0,0,1,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,0,1,0,0,
	  0,1,1,1,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,1,0,0,0,
	  0,1,1,0,0,
	  0,1,0,0,0,
	  0,0,0,0,0 },

	// Z �η���
	{ 0,0,0,0,0,
	  0,1,1,0,0,
	  0,0,1,1,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,0,1,0,0,
	  0,1,1,0,0,
	  0,1,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,1,1,0,0,
	  0,0,1,1,0,
	  0,0,0,0,0,
	  0,0,0,0,0 },

	{ 0,0,0,0,0,
	  0,0,1,0,0,
	  0,1,1,0,0,
	  0,1,0,0,0,
	  0,0,0,0,0 },
};

//��ӭ����
void welcome(void) {
	//��ʼ������
	initgraph(550, 660);
	//���ô��ڱ���
	HWND window = GetHWnd();  //��ȡ����
	SetWindowText(window, _T("����˹����    ��ͨ��"));    //���ô��ڱ���

	//�����ı���������ʽ
	settextstyle(40, 0, _T("΢���ź�"));
	setcolor(WHITE);     //��ɫ
	outtextxy(205, 200, _T("����˹����"));

	settextstyle(22, 0, _T("����"));
	outtextxy(175, 300, _T("���, �Ӷ���˹���鿪ʼ!"));
	Sleep(3000);  //˯��(��ͣ)3000����    3��
}

//��ʼ����Ϸ����
void initGameScene(void) {
	char str[16];
	 //�����Ļ
	cleardevice();

	rectangle(27, 27, 336, 635);
	rectangle(29, 29, 334, 633);
	rectangle(370, 50, 515, 195);

	settextstyle(24, 0, _T("����"));
	setcolor(LIGHTGRAY);
	outtextxy(405, 215, _T("��һ��"));

	setcolor(RED);
	outtextxy(405, 280, _T("����:"));
	sprintf_s(str, "%d", score);
	outtextxy(415, 310, str);

	outtextxy(405, 375, _T("�ȼ�:"));
	sprintf_s(str, "%d",  rank);
	outtextxy(425, 405, str);

	//����˵��
	setcolor(LIGHTBLUE);
	outtextxy(390, 475, "����˵��:");
	outtextxy(390, 500, "��: ��ת");
	outtextxy(390, 525, "��: �½�");
	outtextxy(390, 550, "��: ����");
	outtextxy(390, 575, "��: ����");
	outtextxy(390, 600, "�ո�: ��ͣ");
}
void clearBlock(void){
	setcolor(BLACK);
	settextstyle(23, 0, "����");

	for(int i=0; i<BLOCK_HEIDHT; i++){
		for(int j=0; j<BLOCK_WIDTH; j++){
			//"��"
			int x = 391 + j * UNIT_SIZE;
			int y = 71 + i * UNIT_SIZE;
			outtextxy(x, y, "��");
		}
	}
}
//���ָ��λ��ָ������ķ���
// ����x:��������Ͻǵ�x����
//����y: ��������Ͻ�����Ϸ�����ڵ�����, ������Ϸ���򶥲��ľ���
void clearBlock(int x, int y, block_dir_t dir){
	setcolor(BLACK);
	int id = BlockIndex * 4 + dir;
	y += START_Y;

	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			if(block[id][i][j] == 1){
				//�����÷���ĵ� i �еĵ� j ��
				outtextxy(x + 20 * j, y + i * 20, "��");
			}
		}
	}
}

//�����Ͻ�������, ������һ������
void drawBlock( int x, int y ){
	setcolor(color[NextIndex]);
	settextstyle(23, 0, "����");

	for(int i=0; i<BLOCK_HEIDHT; i++){
		for(int j=0; j<BLOCK_WIDTH; j++){
			//"��"
			if(block[NextIndex * 4][i][j] == 1){
			   int x2 = x + j * UNIT_SIZE;
			   int y2 = y + i * UNIT_SIZE;
			   outtextxy(x2, y2, "��");
			}
		}
	}
}
//���Ʒ���: ��ָ��λ�û���ָ������ķ���
void drawBlock( int x, int y, int blockIndex, block_dir_t dir){
	setcolor(color[blockIndex]);
	settextstyle(23, 0, "����");
	int id = blockIndex * 4 + dir;

	for(int i=0; i<BLOCK_HEIDHT; i++){
		for(int j=0; j<BLOCK_WIDTH; j++){
			//"��"
			if(block[id][i][j] == 1){
			   int x2 = x + j * UNIT_SIZE;
			   int y2 = y + i * UNIT_SIZE;
			   outtextxy(x2, y2, "��");
			}
		}
	}
}

void nextblock(void){
	clearBlock();//������ϽǷ���

	//���ѡ��һ������
	srand((unsigned int)time(NULL));//ʹ��ʱ�亯���ķ���ֵ, ����Ϊ�������
	NextIndex = rand() % BLOCK_COUNT;

	drawBlock(391, 71);
}
//�����ָ��λ�ÿ�����ָ�������ƶ�, �ͷ���1, ����ͷ���0
int moveable(int x0, int y0, move_dir_t moveDir, block_dir_t blockDir){
	//���㵱ǰ��������Ͻ���30*15����Ϸ����λ��(�ڶ�����, �ڶ�����)
	int x = (y0 - minY) / UNIT_SIZE;
	int y = (x0 - minX) / UNIT_SIZE;
	int id = BlockIndex * 4 + blockDir;
	int ret = 1;

	if(moveDir == MOVE_DOWN){
		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){
				if(block[id][i][j] == 1 && (x + i + 1 >=30 || visit[x + i + 1][y + j] == 1)){
					ret = 0;
				}
			}
		}
	}else if(moveDir == MOVE_LEFT){
		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){
				if(block[id][i][j] == 1 && (y + j ==0 || visit[ x + i][y + j - 1] == 1)){
					ret = 0;
				}
			}
		}
	}else if(moveDir == MOVE_RIGHT){
		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){
				if(block[id][i][j] == 1 && (y + j +1 >= 15 || visit[ x + i][y + j + 1] == 1)){
					ret = 0;
				}
			}
		}
	}
	return ret;
}

//�����Ϸ�Ƿ����
void failCheck(){
	if(!moveable(START_X, START_Y, MOVE_DOWN, BLOCK_UP)) {
		setcolor(WHITE);
		settextstyle(45, 0, _T("����"));
		outtextxy(75, 300, "��Ϸ����!");
		Sleep(1000);
		system("pause");
		closegraph();
		exit(0);
	}
};
//ʱ��ͣ��
void wait(int interval){
	int count = interval / 10;
	for(int i=0; i<count; i++){
		Sleep(10);
		if(_kbhit()){
			return;
		}
	}
}

//�жϵ�ǰ�����Ƿ����ת��ָ������
//ע��: ��ʱ��δת��
int rotatable(int x, int y, block_dir_t dir){
    int id = BlockIndex * 4 + dir; 
	int xIndex = (y - minY) / 20;
	int yIndex = (x - minX) / 20;

	if(!moveable(x, y, MOVE_DOWN, dir)){
		return 0;
	}
	 
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			if(block[id][i][j] == 1 && (yIndex + j < 0 || yIndex + j>= 15 || visit[xIndex + i][yIndex + j] == 1)){
				return 0;
			}
		}
	}
	return 1;
}
//�̻�����
void mark(int x, int y, int blockIndex, block_dir_t dir){
	int id = blockIndex * 4 +dir; 
	int x2 = (y - minY) / 20;
	int y2 = (x - minX) / 20;

	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			if(block[id][i][j] == 1){
				visit[x2 + i][y2 + j] = 1;
				markColor[x2 + i][y2 + j] = color[blockIndex];
			}
		}
	}
}

void move(void){
	int x = START_X;
	int y = START_Y;
	int k = 0;
	block_dir_t blockDir = BLOCK_UP;
	int curSpeed = speed;

	//�����Ϸ�Ƿ����
	failCheck();

	//�������½���
	while(1){
		if(_kbhit()){
			int key = _getch();
			if(key == KEY_SPACE){
				_getch();
			}
		}
		//�����ǰ����
		clearBlock(x, k, blockDir);

		if(_kbhit()){
			int key = _getch();

			if(key == KEY_UP){
				block_dir_t nextDir = (block_dir_t)((blockDir + 1) % 4); 
				if(rotatable(x, y + k, nextDir)){
					blockDir = nextDir;
				}
			}else if(key == KEY_DOWN){
				curSpeed = 50;  //�����½�
			}else if(key ==KEY_LEFT){
				if(moveable(x, y + k + 20, MOVE_LEFT,blockDir)){
				    x -= 20;
				}
			}else if(key == KEY_RIGHT){
				if(moveable(x, y + k + 20, MOVE_RIGHT,blockDir)){
				    x += 20;
				}
			}
		}
		k += 20;
		//���Ƶ�ǰ����
		drawBlock(x, y + k, BlockIndex, blockDir);

		wait(curSpeed);

		//k +=20;
		//�����"�̻�"����
		if(!moveable(x, y + k, MOVE_DOWN, blockDir)){
			mark(x, y + k, BlockIndex, blockDir);
			break;
		}
	}
}
void newblock(){
	//ȷ������ʹ�õķ��������
	BlockIndex = NextIndex;

	//����һ���Ӷ����½��ķ���
	drawBlock(START_X, START_Y);
	//���³��ֵķ�����ͣ, 
	Sleep(100);  //0.1��

	//�����Ͻ�����, ������һ������
	nextblock();

	//���齵��
	move();
}
 //������ x ��, ���������һ�ж�����
void down(int x){
	for(int i=x; i >0; i--){
		//������ i ��, �� j ��
		for(int j=0; j<15; j++){
			if(visit[i - 1][j]){
				visit[i][j] = 1;
				markColor[i][j] = markColor[i - 1][j];
				setcolor(markColor[i][j]);
				outtextxy(20 * j + minX, 20 * i + minY, "��");
			}else{
				visit[i][j] = 0;
				setcolor(BLACK);
				outtextxy(20 * j + minX, 20 * i + minY, "��");
			}
		}
	}
	//�����ϵ���һ��(�����б�Ϊ0����һ��)
	setcolor(BLACK);
	for(int j=0; j<15; j++){
		visit[0][j] = 0;
		outtextxy(20 * j +minX, minY, "��");
	}

}

//���·���, ����lines��ʾ����������
void addScore(int lines){
	char str[32];

	setcolor(RED);
	score += lines * 10;
	sprintf_s(str, "%d", score);
	outtextxy(415, 310, str);
}

void updateGrade(){
	//���µȼ���ʾ
	//�ȼ�: 50��һ��
	rank = score / 50;
	char str[16];

	sprintf_s(str, "%d", score);
	outtextxy(425, 405, str);

	//�����ٶ�, �ȼ�Խ��, �ٶ�Խ��, speedԽС
	//����: 500, ���:100
	speed = 500 - rank * 100;
	if(speed <=100){
		speed = 100;
	}

}

void check(void){
	int i, j;
	int clearLines = 0;

	for(i=29; i >= 0; i--){
		//���� i ����û����
		for(j=0; j<15 && visit[i][j]; j++);
		//ִ�е���ʱ, ���������
		//1. �� i ��û����, �п�λ, ��ʱ j < 15
		//2. �� i ������, ��ʱ j >=15
		if(j >= 15){
			//��ʱ, �� i ���Ѿ�����, ����Ҫ������ i ��
			down(i);  //������ i ��, ���������һ�ж�����
			i++;   //��Ϊ������ѭ������ i--, ����������i++, ʹ���´�ѭ��, �ٰ� ��һ�м��һ��
			clearLines++;
		}
	}

	//���·���
	addScore(clearLines);

	//���µȼ�(���µȼ���ʾ, �����ٶ�)
	updateGrade();
}

int main(void) {
	welcome();
	initGameScene();

	//���Ͻǲ����·���
	nextblock();
	Sleep(500);

	//��ʼ����������
	memset(visit, 0, sizeof(visit));

	//����Ľ���
	while(1){
		//��Ҫ�Ĳ����·���
		newblock();

		//��������, �����·������ٶ�
		check();
	}

	system("pause");
	closegraph();
	return 0;
}