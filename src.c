#include <stdio.h>
#include <stdlib.h> //malloc, rand
#include <string.h> //strcpy, strcmp
#include <time.h> 
#include <unistd.h> //sleep


//A. Node Func
//0. struct Node
struct Node {
	char eng[15];
	char kor[3][30];
	struct Node* next;
	struct Node* prev;
	int correctcheck, use, randuse;
};
//1. create a node
struct Node* createnode() {
	struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));
	newnode->randuse=0;
	return newnode;
}
//2. count nodes
int countnode(int date, struct Node* head[]) {
	int cnt = 0;
	struct Node* horse = head[date]->next;
	while (horse != NULL) {
		cnt++;
		horse = horse->next;
	}
	return cnt;
}
//3. get node at
struct Node* getnodeat(int date, struct Node* head[], int index) {
	struct Node* horse = head[date]->next;
	int cnt = 0;
	while (horse != NULL) {
		if (cnt == (index - 1))
			return horse;
		horse = horse->next;
		cnt++;
	}
	return NULL;
}


//B. Before Func
//1. file date
int filedate() {
	int date;
	printf("파일명(일차): ");
	scanf("%d", &date);
	return date;
}
//2. print the menu
int printMenu(char k) {
	if (k == 'A') {
		printf("\n\n-----------------------------------------\n 영어 단어 암기 프로그램 \n-----------------------------------------\n");
		printf("1) 단어 맞히기\n2) 단어 재시험\n3) 플래쉬카드\n4) 단어장 관리\n5) 프로그램 종료\n\n");
	}
	else {
		printf("\n\n>> 영어 단어 암기 프로그램: 단어장 관리 <<\n");
		printf("1. 새 파일 추가하기 \n2. 새 단어 추가하기\n3. 단어장 보기\n4. 단어 파일 목록보기\n5. 단어장 관리 종료\n\n");
	}
	int number;
	printf("번호를 선택하세요: ");
	scanf("%d", &number);
	return number;
}
//3. fileopen && upload data
void upload(int date, struct Node* head[]) {
	int r = 1;
	char buffer[10], lines[50], tokens[4][30];
	sprintf(buffer, "%d.dic", date);
	FILE* fp = fopen(buffer, "r");
	struct Node* tmpnode = createnode();
	struct Node* ordnode = createnode();
	while (fgets(lines, 50, fp)) {
		lines[strlen(lines) - 1] = '\0';
		tmpnode->next = createnode();
		char* ptr = strtok(lines, " ");
		int t = 0, s = 1;
		while (ptr != NULL) {
			strcpy(tokens[t++], ptr);
			ptr = strtok(NULL, " ");
		}
		strcpy(tmpnode->eng, tokens[0]);
		while (s < t) {
			strcpy(tmpnode->kor[s - 1], tokens[s]);
			s++;
		}
		if (r == 1)
			ordnode = tmpnode;
		tmpnode = tmpnode->next;
		r++;
	}
	head[date]->next = ordnode;
	ordnode->prev = head[date];
	fclose(fp);
}
//4. swap data of node
void swap(struct Node* node1, struct Node* node2) {
	char eng_tmp[15];
	strcpy(eng_tmp, node1->eng);
	strcpy(node1->eng, node2->eng);
	strcpy(node2->eng, eng_tmp);
	int i = 0;
	while (i < 3) {
		char kor_tmp[30];
		strcpy(kor_tmp, node1->kor[i]);
		strcpy(node1->kor[i], node2->kor[i]);
		strcpy(node2->kor[i], kor_tmp);
		i++;
	}
}
//5. sort nodes
void sortnode(int date, struct Node* head[]) {
	int len = countnode(date, head)-1;
	for (int i = 0; i < len ; i++) {
		struct Node* horse = head[date]->next;
		for (int j = 0; j < len - i; j++) {
			if (strcmp(horse->eng, horse->next->eng) > 0)
				swap(horse, horse->next);
			horse = horse->next;
		}
	}
}


//C. Main Func
//1. vocabulary test
double test(int date, struct Node* head[], int trt) {
	int mode, ek, rd;
	double count = 0, correct = 0;
	printf("출력방식(알파벳순:1, 무작위:2): ");
	scanf("%d", &mode);
	if (mode == 1) {
		sortnode(date, head);
		struct Node* horse = head[date]->next;
		horse = horse->next;
		while (horse != NULL) {
			if (trt==1){
				if (horse->correctcheck==0){
					horse=horse->next;
					continue;
				}
			}
			char answer[30];
			for(int j=0; horse->kor[j][0]!='\0';j++)
				printf("%s ", horse->kor[j]);
			printf("-> ");
			scanf("%s", answer);
			if (strcmp(answer, ".quit") == 0) {
				horse->correctcheck = 0;
				break;
			}
			else if (strcmp(answer, horse->eng) == 0) {
				printf("correct!\n");
				correct++;
				horse->correctcheck = 0;
			}
			else {
				printf("incorrect!\n");
				horse->correctcheck = 1;
			}
			count++;
			horse = horse->next;
		}
	}
	else if (mode == 2) {
		srand(time(NULL));
		int nodecnt = countnode(date, head)-1, i = 0;
		while (i < nodecnt) {
			ek = rand() % 2;
			rd = rand() % nodecnt + 1;
			char answer[30];
			struct Node* horse = getnodeat(date, head, rd);
			if (horse->randuse==1)
				continue;
			horse->randuse=1;
			if (trt==1){
				i++;
				if (horse->correctcheck==0)
					continue;
			}
			if (ek == 0) {
				printf("%s -> ", horse->eng);
				scanf("%s", answer);
				if (strcmp(answer, ".quit") == 0) {
					horse->correctcheck = 0;
					break;
				}
				else if ((strcmp(answer, horse->kor[0]) == 0)||(strcmp(answer, horse->kor[1]) == 0)||(strcmp(answer, horse->kor[2]) == 0)) { 
					printf("correct!\n");
					correct++;
					horse->correctcheck = 0;
				}
				else {
					printf("incorrect!\n");
					horse->correctcheck = 1;
				}
			}
			else {
				printf("%s %s %s-> ", horse->kor[0],horse->kor[1],horse->kor[2]);
				scanf("%s", answer);
				if (strcmp(answer, ".quit") == 0) {
					horse->correctcheck = 0;
					break;
				}
				else if (strcmp(answer, horse->eng) == 0) {
					printf("correct!\n");
					correct++;
					horse->correctcheck = 0;
				}
				else {
					printf("incorrect!\n");
					horse->correctcheck = 1;
				}
			}
			count++;
			i++;
		}
	}
	if (count != 0)
		return ((correct / count)*100);
	else{
		if (trt==1)
			printf("틀린 문제가 없습니다.");
		return 0;
	}
	getchar();
}
//2. retest
void retest(struct Node* head[]) {
	int date = filedate();
	printf("\n\n>> 영어 단어 암기 프로그램: 단어 재시험 <<\n");
	printf("당신의 점수는 %.2f 점입니다.\n\n", test(date,head,1));
	getchar();
}
//3. flash card
void flashcard(struct Node* head[]) {
	int date = filedate(), sec, mode, rd;
	printf("속도(초): ");
	scanf("%d", &sec);
	printf("출력 방식(알파벳 순서대로==1, 무작위==2): ");
	scanf("%d", &mode);
	upload(date, head);
	if (mode == 1) {
		struct Node* horse = head[date]->next;
		sortnode(date, head);
		while (horse != NULL) {
			printf("\x1b[H\x1b[J");
			printf("\n\n>> 영어 단어 암기 프로그램: 플래시카드 <<\n\n");
			printf("\t\t%s\n", horse->eng);
			sleep(sec);
			printf("\x1b[H\x1b[J");
			printf("\n\n>> 영어 단어 암기 프로그램: 플래시카드 <<\n\n");
			printf("\t\t%s %s %s\n", horse->kor[0],horse->kor[1],horse->kor[2]);
			sleep(sec);
			horse = horse->next;
		}
	}
	else if (mode == 2) {
		srand(time(NULL));
		int rcnt = 0, nodecnt = countnode(date,head);
		while (rcnt < nodecnt) {
			rd = rand() % nodecnt + 1;
			struct Node* horse = getnodeat(date, head, rd);
			if (horse->use != 1) {
				printf("\x1b[H\x1b[J");
				printf("\n\n>> 영어 단어 암기 프로그램: 플래시카드 <<\n\n");
				printf("\t\t%s\n", horse->eng);
				sleep(sec);
				printf("\x1b[H\x1b[J");
				printf("\n\n>> 영어 단어 암기 프로그램: 플래시카드 <<\n\n");
				printf("\t\t%s %s %s\n", horse->kor[0],horse->kor[1],horse->kor[2]);
				sleep(sec);
				horse->use = 1;
				rcnt++;
			}
		}
	}
}
//4-0. dic list 개수
int countdic() {
	int count = 0;
	FILE* fp = fopen("dic.list", "r");
	while (1) {
		if (feof(fp)) break;
		char str[10];
		fgets(str, 10, fp);
		count++;
	}
	fclose(fp);
	return count;
}
//4-1. 새 파일 추가하기
void newfile() {
	char buffer[10];
	sprintf(buffer, "%d.dic", countdic());
	FILE* fp = fopen(buffer, "w+");
	FILE* dl = fopen("dic.list", "a+");
	fprintf(dl, "%s\n", buffer);
	printf("새 파일이 추가되었습니다.\n");
	fclose(dl);
	fclose(fp);
}
//4-2. 새 단어 추가하기
void newwords() {
	int m = 0, date = filedate();
	char buffer[10], lines[50];
	sprintf(buffer, "%d.dic", date);
	FILE* fp = fopen(buffer, "a+");
	getchar();
	printf("---------------------추가할 단어와 뜻을 입력하세요.(ex)notice 알아차리다 주목하다)\n");
	while (m++ < 20) {
		char w[50];
		fgets(w, sizeof(w), stdin);
		w[strlen(w) - 1] = '\0';
		if (strcmp(w, ".add") == 0) {
			fclose(fp);
			break;
		}
		else {
			printf("추가한 단어:%s\n", w);
			fputs(w, fp);
			fputs("\n", fp);
		}
	}
	printf("-----------------------\n단어가 추가되었습니다.\n");
}
//4-3. 단어장 보기
void readlist() {
	char buffer[10], str[50];
	int date = filedate();
	sprintf(buffer, "%d.dic", date);
	FILE* fp = fopen(buffer, "r+");
	printf("\n\n -------- 단어장 --------\n");
	while (fgets(str, 50, fp)) {
		printf("%s", str);
	}
	printf("-----------------------\n");
	fclose(fp);
}
//4-4. 단어 파일 목록 보기
void readdiclist() {
	FILE* fp = fopen("dic.list", "r");
	printf("\n-----단어 파일 목록-----\n");
	char str[10];
	while (fgets(str, 10, fp)) {
		printf("%s\n", str);
	}
	fclose(fp);
}
//4-M. manage vocabulary list
void manage() {
	int e = 1;
	while (e) {
		int number = printMenu('B');
		switch (number) {
		case 1:
			printf("\n\n>> 영어 단어 암기 프로그램: 단어장 관리: 새 파일 추가하기 <<\n");
			newfile();
			break;
		case 2:
			printf("\n\n>> 영어 단어 암기 프로그램: 단어장 관리: 새 단어 추가하기 <<\n");
			newwords();
			break;
		case 3:
			printf("\n\n>> 영어 단어 암기 프로그램: 단어장 관리: 단어장 보기 <<\n");
			readlist();
			break;
		case 4:
			printf("\n\n>> 영어 단어 암기 프로그램: 단어장 관리: 단어 파일 목록보기 <<\n");
			readdiclist();
			break;
		case 5:
			printf("단어장 관리 종료\n\n");
			e = 0;
		}
	}
}


//MAIN Func
int main() {
	struct Node* head[7];
	for (int i = 0; i < 7; i++) {
		head[i] = createnode();
	}
	while (1) {
		int number = printMenu('A');
		switch (number) {
		case 1:
		{
			int date = filedate();
			upload(date, head);
			printf("\n\n>> 영어 단어 암기 프로그램: 단어 맞추기 <<\n");
			printf("당신의 점수는 %.2f 점입니다.\n\n", test(date, head,0));
			break;
		}
		case 2:
			retest(head);
			break;
		case 3:
			flashcard(head);
			break;
		case 4:
			manage();
			break;
		case 5:
			printf("프로그램 종료\n\n");
			for (int i = 0; i < 7; i++)
				free(head[i]);
			exit(1);
		}
	}
}
