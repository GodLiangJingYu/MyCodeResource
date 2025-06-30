/*#include<stdio.h>
#include<string.h>
#define  MAX 1024

int main(){
	int i;
	scanf("%d",&i);
	printf("%d",i);
}*/
/*//����ջ 
typedef struct SequenStack{
	char data[MAX];
	int top;
}Seq;
//��ʼ��ջ 
Seq *Init_Seq(){
	Seq *S;
	S=(Seq*)malloc(sizeof(Seq));
	if(S==NULL)
		return S;
	S->top=-1;
	return S;
}
//��ջ�ײ���һ��Ԫ��
void Push_Seq(Seq *S,char ch){
	if(S->top>=MAX-1)
		exit(0);
	S->top++;
	S->data[S->top]=ch;
}
//��ջ 
void Pop_Seq(Seq *S){
	if(S->top==-1)
		exit(0);
		else
			S->top--;
}
//�ж�ջ�Ƿ�Ϊ�� 
int Empty_Seq(Seq *S){
	if(S->top==-1)
		return 1;
		else
		return 0;
}

int main(){
	char a[MAX];
	gets(a);
	int i;
	Seq *S;
	S=Init_Seq();
	for(i=0;i<MAX&&a[i]!='\0';i++)
	{
		if(a[i]=='('||a[i]=='['||a[i]=='{')
			Push_Seq(S,a[i]);
			else if((a[i]==')'&&S->data[S->top]=='(')||(a[i]==']'&&S->data[S->top]=='[')||(a[i]=='}'&&S->data[S->top]=='{'))
				Pop_Seq(S);
			else if((a[i]==')'&&S->data[S->top]!='(')||(a[i]==']'&&S->data[S->top]!='[')||(a[i]=='}'&&S->data[S->top]!='{'))
			{
				printf("no");
				return 0;
			}
			else continue;
	}
	if(Empty_Seq(S))
		printf("yes");
		else
		printf("no");
	return 0;
}


/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

typedef struct SequenStack {
    char data[MAX];
    int top;
} Seq;

Seq* Init_Seq() {
    Seq* S;
    S = (Seq*)malloc(sizeof(Seq));
    if (S == NULL)
        return S;
    S->top = -1;
    return S;
}

void Push_Seq(Seq* S, char ch) {
    if (S->top >= MAX - 1)
        exit(0);
    S->top++;
    S->data[S->top] = ch;
}

void Pop_Seq(Seq* S) {
    if (S->top == -1)
        exit(0);
    else
        S->top--;
}

int Empty_Seq(Seq* S) {
    if (S->top == -1)
        return 1;
    else
        return 0;
}

int main() {
    char a[MAX];
    fgets(a, MAX, stdin);
    int i;
    Seq* S;
    S = Init_Seq();
    for (i = 0; i < MAX && a[i] != '\0'; i++) {
        if (a[i] == '(' || a[i] == '[' || a[i] == '{')
            Push_Seq(S, a[i]);
        else if ((a[i] == ')' && S->data[S->top] == '(') || (a[i] == ']' && S->data[S->top] == '[') || (a[i] == '}' && S->data[S->top] == '{'))
            Pop_Seq(S);
        else if ((a[i] == ')' && S->data[S->top] != '(') || (a[i] == ']' && S->data[S->top] != '[') || (a[i] == '}' && S->data[S->top] != '{')) {
            printf("no\n");
            return 0;
        }
        else continue;
    }
    if (Empty_Seq(S))
        printf("yes\n");
    else
        printf("no\n");

    free(S); // �ͷŶ�̬������ڴ�
    return 0;
}*/
#include <stdio.h>  
#include <stdlib.h>  

typedef struct {  
    int a; // 系数  
    int b; // 指数  
} A;  

typedef struct Lnode {  
    A data;  
    struct Lnode *next; // 指针指向一个结构体  
} Node;  

Node* CreatList(void) {  
    Node* Head = (Node*)malloc(sizeof(Node));  
    Head->next = NULL;  
    Node* p = Head;  
    int i = 0, j = 0;  

    printf("请输入系数和指数（输入 0 0 结束）：\n");  
    while (1) {  
        scanf("%d %d", &i, &j);  
        if (i == 0 && j == 0) {  
            break; // 结束输入  
        }  

        Node *q = (Node*)malloc(sizeof(Node));  
        q->data.a = i;  
        q->data.b = j;  
        q->next = NULL;  
        p->next = q; // 将新节点链接到链表  
        p = q; // 移动指针到新节点  
    }  
    return Head;  
}  

void derivation(Node *Head) {  
    Node* q = Head->next; // 从第一个实际节点开始  
    printf("导数结果：\n");  
    while (q) {  
        if (q->data.b != 0) { // 指数不为0才有导数  
            int new_a = q->data.a * q->data.b; // 计算新系数  
            int new_b = q->data.b - 1; // 计算新指数  
            printf("%d x^%d ", new_a, new_b); // 输出导数  
            if (q->next) {  
                printf("+ "); // 如果之后还有更多项，打印 +  
            }  
        }  
        q = q->next; // 移动到下一个节点  
    }  
    printf("\n");  
}  

void freeList(Node* Head) {  
    Node* q = Head;  
    while (q) {  
        Node* temp = q;  
        q = q->next;  
        free(temp); // 释放当前节点  
    }  
}  

int main(void) {  
    Node* Head = CreatList();  
    derivation(Head);  
    freeList(Head); // 释放链表内存  
    return 0;  
}