/*#include<stdio.h>
#include<stdlib.h>
#define MAX 1024
//定义队列 
typedef struct Quene{
	int data[MAX];
	int front;
	int rear;
	int length;
}Quene;
//初始化 
Quene *Init_Quene(){
	Quene *Q;
	Q=(Quene*)malloc(sizeof(Quene));
	if(Q!=NULL)
	{
		Q->front=0;
		Q->rear=0;
		Q->length=0;
	}
	return Q; 
}
//入队
int Enter_Quene(Quene *Q,int x){
	int i=0;
	if(Full_Quene(Q))
		exit(0);
	Q->data[Q->rear]=x;
	Q->rear=(Q->rear+1)%Q->length;
}
//出队
void Delete_Quene(Quene *Q,int *x){
	if(Q->front=Q->rear)
		exit(0);
		else
		{
			*x=Q->data[Q->front];
			Q->front=(Q->front+1)%Q->length;
		}
}
//取头数据 
int Frontout_Quene(Quene *Q){
	int x;
	if(Q->front==Q->rear)
		exit(0);
		else
			x=Q->data[Q->front];
	return x;
}

int main(){
	int i,j,m,n,num=0;
	char ch;
	scanf("%d",&m);//m为队列长度
	Quene Q;
	Q=Init_Quene();
	Q->length=m;
	j=0;
	while(ch!='\n');
	{
		scanf("%d",&i);
		ch=getchar();
		Enter_Quene(Q,&i);
		j++;
	}
	scanf("%d",&n);
	if(j==Q->length)//判断是否满 
		printf("yes\n");
		else
		printf("no\n");
	i=Frontout_Quene(Q);
	while(i!=n){
		Delete_Quene(Q,&i);
		j--;
	}
	for(i=0;i<j;i++)
	{
		printf("%d",Q->data[i]);
		if(i!=j-1)
			printf(" ");
	}
	printf("\n");
	i=Frontout_Quene(Q);
	printf("%d",i);
} */
#include <stdio.h>
#include <stdlib.h>

#define MAX 1024

// 定义循环队列
typedef struct Queue {
    int data[MAX];
    int front;
    int rear;
    int length;
} Queue;

// 初始化循环队列
Queue *Init_Queue() {
    Queue *Q;
    Q = (Queue*)malloc(sizeof(Queue));
    if (Q != NULL) {
        Q->front = 0;
        Q->rear = 0;
        Q->length = 0;
    }
    return Q;
}

// 判断循环队列是否满
int Full_Queue(Queue *Q) {
    return (Q->length == MAX);
}

// 入队
void Enter_Queue(Queue *Q, int x) {
    if (Full_Queue(Q)) {
        printf("yes\n");
        return;
    }
    Q->data[Q->rear] = x;
    Q->rear = (Q->rear + 1) % MAX;
    Q->length++;
}

// 出队
int Delete_Queue(Queue *Q) {
    int x;
    if (Q->front == Q->rear) {
        exit(0);
    } else {
        x = Q->data[Q->front];
        Q->front = (Q->front + 1) % MAX;
        Q->length--;
    }
    return x;
}

int main() {
    int m, n, num;
    char ch;
    scanf("%d", &m); // m为队列长度

    Queue *Q = Init_Queue();
    Q->length = m;

    int j = 0;
    do {
        scanf("%d", &num);
        ch = getchar();
        Enter_Queue(Q, num);
        j++;
    } while (ch != '\n');

    scanf("%d", &n);

    if (Full_Queue(Q)) {
        printf("yes\n");
    } else {
        printf("no\n");
    }

    int i = Delete_Queue(Q);
    while (i != n) {
        printf("%d ", i);
        i = Delete_Queue(Q);
        j--;
    }

    printf("\n");
int k;
    for ( k = 0; k < j; k++) {
        printf("%d", Q->data[k]);
        if (k != j - 1) {
            printf(" ");
        }
    }
    printf("\n");

    i = Delete_Queue(Q);
    printf("%d", i);

    free(Q);
    return 0;
}
