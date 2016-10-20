#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct{
	int row,col;
}pt;
typedef struct{
	pt head,tail;
}snake;
#define		SNAKE_HEAD     '+'
#define		SNAKE_TAIL     '-'
#define 	APPLE		   '@'
#define     SIZE			30
enum{NOOVERLAP,OVERLAP};
enum{UP,DOWN,LEFT,RIGHT};

//设置蛇的开始位置
void init_snake(snake *p_snake){
	p_snake->head.row=rand()%(SIZE-2)+2;
	p_snake->head.col=rand()%(SIZE-2)+2;
	switch(rand()%4){
		case UP:
			p_snake->tail.row=p_snake->head.row-1;
			p_snake->tail.col=p_snake->head.col;
			break;
		case DOWN:
			p_snake->tail.row=p_snake->head.row+1;
			p_snake->tail.col=p_snake->head.col;
			break;
		case LEFT:
			p_snake->tail.row=p_snake->head.row;
			p_snake->tail.col=p_snake->head.col-1;
			break;
		case RIGHT:
			p_snake->tail.row=p_snake->head.row;
			p_snake->tail.col=p_snake->head.col+1;
			break;
	}

//设置苹果开始位置
}
void init_apple(pt *p_apple){
	p_apple->row=rand()%SIZE+1;
	p_apple->col=rand()%SIZE+1;
}
//显示所有内容
void show_map(const pt *p_apple,const snake *p_snake){
	int row=0,col=0;//想想为什么不需用i，j来循环	
		for(row=0;row<=SIZE;row++){
			for(col=0;col<=SIZE;col++){
				if(row==p_snake->head.row&&col==p_snake->head.col){
					printf("%c",SNAKE_HEAD);
				}else if(row==p_snake->tail.row&&col==p_snake->tail.col){
					printf("%c",SNAKE_TAIL);
				}else if(row==p_apple->row&&col==p_apple->col){
					printf("%c",APPLE);
				}else{
					printf(" ");
				}
		
			}
			printf("\n");
		}
}
//把蛇移动一步
void move_snake(snake *p_snake){
	int direction=0;
	printf("请选择一个方向?%d代表上，%d代表下，%d代表左，%d代表右",UP,DOWN,LEFT,RIGHT);
	scanf("%d",&direction);
	pt tmp=p_snake->head;
	switch(direction){
		case UP:
			tmp.row--;
			break;
		case DOWN:
			tmp.row++;
			break;
		case LEFT:
			tmp.col--;
			break;
		case RIGHT:
			tmp.col++;
			break;
	}
	if(tmp.row<1||tmp.row>SIZE||tmp.col<1||tmp.col>SIZE){
		return;
	}
	if(tmp.row==p_snake->tail.row&&tmp.col==p_snake->tail.col){
		return;
	}
	p_snake->tail=p_snake->head;
	p_snake->head=tmp;

	
}
//判断蛇和苹果的位置是否重叠
int overlap(const pt *p_apple,const snake *p_snake){
	if(p_apple->row==p_snake->head.row&&p_apple->col==p_snake->head.col){	
		return OVERLAP;
	}
	if(p_apple->row==p_snake->tail.row&&p_apple->col==p_snake->tail.col){
		return OVERLAP;	
	}
	return NOOVERLAP;
}

int main(){
	srand(time(0));
	snake snake1={0};
	pt apple={0};
	init_apple(&apple);
	init_snake(&snake1);
	while(1){
		if(overlap(&apple,&snake1)==NOOVERLAP){
			break;
		}		
		init_apple(&apple);
	}
	show_map(&apple,&snake1);
	while(1){
		move_snake(&snake1);
		while(1){
			if(overlap(&apple,&snake1)==NOOVERLAP){
				break;
			}
			init_apple(&apple);
		}
		show_map(&apple,&snake1);
	}
	




	return 0;
}










