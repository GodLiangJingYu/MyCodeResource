/*#include<stdio.h>
#define MAX 100
//定义栈 
typedef struct SequenStack{
	char data[MAX];
	int top;
}Seq;
//初始化栈 
Seq *Init_Seq(){
	Seq *S;
	S=(Seq*)malloc(sizeof(Seq));
	if(S==NULL)
		return S;
	S->top=-1;
	return S;
}
//在栈底插入一个元素
void Push_Seq(Seq *S,char ch){
	if(S->top>=MAX-1)
		exit(0);
	S->top++;
	S->data[S->top]=ch;
}
//出栈 
void Pop_Seq(Seq *S,char *ch){
	if(S->top==-1)
		exit(0);
		else
		{
			S->top--;
			*ch=S->data[S->top];
			}	
}
//取取栈顶元素 
void Get_Seq(Seq *S,char *ch){
	if(S->top==-1)
	 *ch=' ';
	 else	*ch=S->data[S->top];
}
//定义优先级
int priority(char op) {
    if (op == '(' || op == ')') {
        return 0;
    } else if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/') {
        return 2;
    } else {
        return -1; // Invalid operator
    }
}
 
int main(){
	char a[MAX],b[MAX],ch;
	gets(a);
	Seq S,T;
	S=Init_Seq();
	T=Init_Seq();
	int i=0,j=0;
	while(a[i]!='\0'&&i<MAX)
	{
		if(a[i]=='(')
		{
			i++;
			while(a[i]!=')')
			{
				
			}
			}	
		
	}
}*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 1024

typedef struct Queue {
    int data[MAX];
    int front;
    int rear;
    int length;
} Queue;

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

int Full_Queue(Queue *Q) {
    return (--Q->length == MAX);
}

void Enter_Queue(Queue *Q, int x) {
    if (Full_Queue(Q)) {
        return;
    }
    Q->data[Q->rear] = x;
    Q->rear = (Q->rear + 1) % MAX;
    Q->length++;
}

void Delete_Queue(Queue *Q, int *x) {
    if (Q->front == Q->rear) {
        exit(0);
    } else {
        *x = Q->data[Q->front];
        Q->front = (Q->front + 1) % MAX;
        Q->length--;
    }
}

int Frontout_Queue(Queue *Q, int *x) {
    if (Q->front == Q->rear) {
        exit(0);
    } else {
        *x = Q->data[Q->front];
        return 1;
    }
}

int main() {
    Queue *Q = Init_Queue();

    int N,i, num;
    scanf("%d", &N);

    for ( i = 0; i < N; i++) {
        scanf("%d", &num);
        Enter_Queue(Q, num);
    }
	
    if (Full_Queue(Q)) {
        printf("yes\n");
    } else {
        printf("no\n");
    }

    int dequeueElement;
    scanf("%d", &dequeueElement);

    int frontElement;
    Frontout_Queue(Q, &frontElement);
    printf("%d\n", frontElement);

    Delete_Queue(Q, &dequeueElement);

    for ( i = 0; i < Q->length; i++) {
        if (i == Q->length - 1) {
            printf("%d\n", Q->data[(Q->front + i) % MAX]);
        } else {
            printf("%d ", Q->data[(Q->front + i) % MAX]);
        }
    }

    free(Q);
    return 0;
}
