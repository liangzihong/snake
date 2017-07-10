#include<stdio.h>
#include<stdlib.h>
#include"acllib.h"
#include<time.h>

int youcaozuo = 0;
int distance = 5;
int xlimit = 995;
int ylimit = 495;
int foodstate = 0;
int snakepart = 3;
int starthour, startmin, startsec;

typedef struct turnpoint {
	int x;
	int y;
	int olddirect;
	struct turnpoint*nextpoint;
	int newdirect;
}point;

typedef struct fuck {
	int x;
	int y;
	int direct;
	struct fuck * nextsnake;
	point*control;
}snake;

snake*head;
snake*food;

// M   forward  蛇纯前进时的变化数据
snake* forward(snake*p) {

	snake*p1 = p;
	switch (p->direct)
	{
	case 1:
		while (p != NULL) {
			p->y -= distance;
			p = p->nextsnake;
		}
		break;
	case 2:
		while (p != NULL) {
			p->y += distance;
			p = p->nextsnake;
		}
		break;
	case 3:
		while (p != NULL) {
			p->x -= distance;
			p = p->nextsnake;
		}
		break;
	case 4:
		while (p != NULL) {
			p->x += distance;
			p = p->nextsnake;
		}
		break;
	}
	return p1;
}

//  M  create  创造蛇的身体
snake* create(snake*p, int x, int y, int direct)
{
	if (p == NULL) {
		p = (snake*)malloc(sizeof(snake));
		p->x = x, p->y = y, p->direct = direct; p->nextsnake = NULL; p->control = NULL;
		return p;
	}
	snake*p1, *p2;
	p1 = p2 = p;
	while (p != NULL) {
		p2 = p;
		p = p->nextsnake;
	}
	p = (snake*)malloc(sizeof(snake));
	p->x = x, p->y = y, p->direct = direct; p->nextsnake = NULL; p->control = NULL;
	p2->nextsnake = p;
	return p1;
}

//  M   check   检查转完弯没有==看下是不是所有snake链表的control都等于NULL
snake* check(snake*p)
{
	snake*p1 = p;
	while (p != NULL)
	{
		if (p->control != NULL)
		{
			break;
		}
		p = p->nextsnake;
	}
	if (p == NULL)
	{
		youcaozuo = 0;
	}
	return p1;
}

// V   画画
void paint(snake*p1) {
	snake*p = p1;
	beginPaint();
	clearDevice();
	setBrushColor(RED);
	setPenColor(RED);
	while (p != NULL) {
		rectangle(p->x - 5, p->y - 5, p->x + 5, p->y + 5);
		p = p->nextsnake;
	}
	endPaint();
}

// M   shanchu  当蛇的部分到达转折点时，删除一个point
snake* shanchu(snake*p)
{
	p->direct = (p->control)->newdirect;
	p->control = (p->control)->nextpoint;
	return p;
}

//  M   turn 蛇转弯时的数据及其操作
snake* turn(snake*p)
{
	int xmark, ymark;
	if (p->control == NULL) {
		switch (p->direct)
		{
		case 1:p->y -= distance; break;
		case 2:p->y += distance; break;
		case 3:p->x -= distance; break;
		case 4:p->x += distance; break;
		}
	}
	else {
		xmark = (p->control)->x, ymark = (p->control)->y;
		switch (p->direct)
		{
		case 1:
			if (p->y != ymark)
			{
				p->y -= distance;
				if (p->y == ymark)
				{
					p = shanchu(p);               //shanchu的作用是删掉一个point，并修改direct;   
				}
				break;
			}
		case 2:
			if (p->y != ymark)
			{
				p->y += distance;
				if (p->y == ymark)
				{
					p = shanchu(p);               //shanchu的作用是删掉一个point，并修改direct;   
				}
				break;
			}
		case 3:
			if (p->x != xmark)
			{
				p->x -= distance;
				if (p->x == xmark)
				{
					p = shanchu(p);               //shanchu的作用是删掉一个point，并修改direct;   
				}
				break;
			}
		case 4:
			if (p->x != xmark)
			{
				p->x += distance;
				if (p->x == xmark)
				{
					p = shanchu(p);               //shanchu的作用是删掉一个point，并修改direct;   
				}
				break;
			}
		}
	}
	return p;
}

//  M  createpoint 创造转折点，有几个转折点则创造几个链表point，链表point记录面临几个转折点
point* createpoint(point*p, int x, int y, int olddirect, int newdirect)
{
	point* p1 = p;
	if (p == NULL) {
		p = (point*)malloc(sizeof(point));
		p->x = x, p->y = y, p->olddirect = olddirect, p->newdirect = newdirect, p->nextpoint = NULL;
		p1 = p;
	}
	else {
		p->nextpoint = createpoint(p->nextpoint, x, y, olddirect, newdirect);
	}
	return p1;
}

//  M receive  当有上下左右时，snake身上的control就会有对应的链表，控制他们之后的走向
snake* receive(snake*p, int direct)
{
	snake*p1 = p;
	int markx = p->x;
	int marky = p->y;
	int markdirect = p->direct;
	p->direct = direct;
	while ((p = p->nextsnake) != NULL) {
		p->control = createpoint(p->control, markx, marky, markdirect, direct);
	}
	return p1;
}

// C 接受上下左右数据
void keyboard(int key, int event)
{
	static int direct = 4;
	if (key == 38 && event == 1)
	{
		if (direct != 2 && direct != 1)
		{
			direct = 1;
			head = receive(head, direct), youcaozuo = 1;
		}
	}
	else if (key == 40 && event == 1)
	{
		if (direct != 1 && direct != 2)
		{
			direct = 2;
			head = receive(head, direct), youcaozuo = 1;
		}
	}
	else if (key == 37 && event == 1)
	{
		if (direct != 4 && direct != 3)
		{
			direct = 3;
			head = receive(head, direct), youcaozuo = 1;
		}
	}
	else if (key == 39 && event == 1)
	{
		if (direct != 3 && direct != 4)
		{
			direct = 4;
			head = receive(head, direct), youcaozuo = 1;
		}
	}
}

//  M judge 判断随机产生的点与snake的身体有无重合
int judge(int x1, int y1, int x2, int y2)
{
	int a, b;
	for (a = x2 - distance; a <= x2 + distance; a++)
	{
		for (b = y2 - distance; b <= y2 + distance; b++)
		{
			if (x1 - distance == a&&y1 - distance == b || x1 - distance == a&&y1 + distance == b ||
				x1 + distance == a &&y1 + distance == b || x1 + distance == a &&y1 - distance == b)
			{
				return 0;
			}
		}
	}
	return 1;
}

//  M random 产生随机数
snake* random(snake*p)
{
	snake*p1 = NULL;
	int r1, r2;
	r1 = rand() % (xlimit - 5 + 1) + 5;
	r2 = rand() % (ylimit - 5 + 1) + 5;
	while (p)
	{
		if (judge(r1, r2, p->x, p->y) == 0)
		{
			break;
		}
		p = p->nextsnake;
	}
	if (p == NULL) {
		p1 = create(p1, r1, r2, 0);
	}
	else {
		p1 = random(head);
	}
	return p1;
}

//  V  drawfood 在适当的时候画出随机数
void drawfood(snake*p)
{
	beginPaint();
	setBrushColor(BLUE);
	setPenColor(BLUE);
	rectangle(p->x - 5, p->y - 5, p->x + 5, p->y + 5);
	endPaint();
}

//   M  eatfood吃掉食物时snake身体的变化（增长），并且food的变化
void eatfood(void)
{
	int x = head->x, y = head->y;
	if (judge(food->x, food->y, x, y) == 0)       //吃到了
	{
		switch (head->direct)
		{
		case 1:food->x = x, food->y = y - distance, food->direct = head->direct, food->nextsnake = head; break;
		case 2:food->x = x, food->y = y + distance, food->direct = head->direct, food->nextsnake = head; break;
		case 3:food->y = y, food->x = x - distance, food->direct = head->direct, food->nextsnake = head; break;
		case 4:food->y = y, food->x = x + distance, food->direct = head->direct, food->nextsnake = head; break;
		}
		head = food;
		foodstate = 0;
		snakepart++;
	}
}

//  M  suicide检查有没自杀
int suicide(snake*p)
{
	p = (p->nextsnake)->nextsnake;
	int xx = head->x, yy = head->y;
	while (p != NULL)
	{
		switch (head->direct)
		{
		case 1:
			if (yy - distance == p->y + distance && yy>p->y && (p->direct == 3 || p->direct == 4)&&xx==p->x)
			{
				return 1;
			}
			break;

		case 2:
			if (yy + distance == p->y - distance && (p->direct == 3 || p->direct == 4) && xx == p->x)
			{
				return 1;
			}break;
		case 3:
			if (xx - distance == p->x + distance&&xx>p->x && (p->direct == 1 || p->direct == 2) && yy == p->y)
			{
				printf("head->direct=%d\n", head->direct);
				printf("xx - distance=%d,p->x + distance=%d\n", xx - distance, p->x + distance);
				return 1;
			}break;
		case 4:
			if (xx + distance == p->x - distance && (p->direct == 1 || p->direct == 2)&&yy==p->y)
			{
				return 1;
			}break;
		}
		p = p->nextsnake;
	}
	return 0;
}

//  V画画
void timework(int id)
{
	//产生食物的画画

	if (!foodstate)
	{
		food = random(head);
		foodstate = 1;
	}

	//对转弯或不转弯时的画画	 
	if (!youcaozuo)
	{
		head = forward(head);
		paint(head);
		drawfood(food);
	}
	else {
		snake*p = head;
		while (p != NULL) {
			p = turn(p);
			p = p->nextsnake;
		}
		paint(head);
		drawfood(food);
		head = check(head);
	}
	eatfood();

	//死掉的画画
	if (head->x + (distance - 1) >= 1000 || head->x - (distance - 1) <= 0 || head->y + (distance - 1) >= 500 || head->y - (distance - 1) <= 0 || suicide(head) == 1)
	{
		char chengji[30];
		
		sprintf(chengji, "你的成绩是%d", snakepart - 3);

		beginPaint();
		setTextSize(30);
		paintText(300, 100, "gameover");
		paintText(300, 150, chengji);
		endPaint();
		cancelTimer(0);
	}







}


int Setup()
{
	initWindow("snack", DEFAULT, DEFAULT, 1000, 500);
	head = NULL;
	head = create(head, 25, 5, 4);
	head = create(head, 15, 5, 4);
	head = create(head, 5, 5, 4);
	paint(head);

	/*time_t timer = time(NULL);
	struct tm*timel1 = localtime(&timer);

	starthour = timel1->tm_hour;
	startmin = timel1->tm_min;
	startsec = timel1->tm_sec;*/


	startTimer(0, 20);
	registerKeyboardEvent(keyboard);
	registerTimerEvent(timework);

	return 0;
}