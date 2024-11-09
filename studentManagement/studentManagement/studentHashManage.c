#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 2599999
#define NAME_SIZE 100
#define DATA_SIZE 256
#define DATA_TYPES 5

typedef struct {
	int key;
	char name[NAME_SIZE];
	int phone_number;
} element;

struct list {
	element item;
	struct list* link;
};

struct list* hash_table[TABLE_SIZE];

int hash_function(int key) {
	return key /* % TABLE_SIZE*/;
}

void hash_chain_add(element item, struct list* ht[]) {
	int hash_value = hash_function(item.key);
	struct list* ptr;
	struct list* node_before = NULL;
	struct list* node = ht[hash_value];
	for (; node; node_before = node, node = node->link) {
		if (node->item.key == item.key) {
			fprintf(stderr, "이미 탐색키가 저장되어 있음\n");
			return;
		}
	}
	ptr = (struct list*)malloc(sizeof(struct list));
	ptr->item.phone_number = item.phone_number;
	ptr->item.key = item.key;
	strcpy(ptr->item.name, item.name);
	ptr->link = NULL;

	if (node_before)
		node_before->link = ptr;
	else
		ht[hash_value] = ptr;
}

// 체인법을 이용하여 테이블에 저장된 키를 탐색
void hash_chain_search(element item, struct list* ht[]) {
	struct list* node;
	int hash_value = hash_function(item.key);
	for (node = ht[hash_value]; node; node = node->link) {
		if (node->item.key == item.key) {
			fprintf(stderr, "탐색 %d 성공\n", item.key);
			return;
		}
	}
	printf("키를 찾지 못했음\n");
}

void hash_chain_print(int key) {
	struct list* node;
	int hash_value = hash_function(key);
	printf("\n=======================================\n");
	if (hash_table[hash_value] == NULL) {
		printf("등록된 학번이 없습니다.\n");
		return;
	}
	for (node = hash_table[hash_value]; node; node = node->link) {
		printf("%d, %s, 0%d", node->item.key, node->item.name, node->item.phone_number);
	}
	printf("\n");
	printf("=======================================\n");
}

void csv_data_add() {
	FILE* fp;
	char str[DATA_SIZE];
	element items;

	if ((fp = fopen("studentData.csv", "r")) == NULL) {
		printf("파일이 존재하지 않습니다.\n");
		items.key = 0;
		strcpy(items.name, "INVALID");
		items.phone_number = 0;
		fclose(fp);
		return 0;
	}

	fgets(str, DATA_SIZE, fp);	// 파일에서 문자열을 가져오는 함수

	char* sub = strtok(str, ",");

	while(1) {
		fgets(str, DATA_SIZE, fp);
		sub = strtok(str, ",");
		items.key = atoi(sub);
		//printf("%d || ", atoi(sub));

		sub = strtok(NULL, ",");
		if (sub == NULL) {
			items.key = 0;
			printf("데이터 없음\n");
			return 0;
		}
		strcpy(items.name, sub);
		//printf("%s || ", sub);

		sub = strtok(NULL, ",");
		items.phone_number = atoi(sub);
		//printf("%d || ", atoi(sub));
		//printf("\n");

		hash_chain_add(items, hash_table);

	}
}

int main() {
	int choice;
	int student_num;

	csv_data_add();

	while (1) {
		printf("선택지 : 1. 학생조회 2. 종료 => ");
		scanf_s("%d", &choice);
		if (choice == 1) {
			printf("학번 입력 => ");
			scanf_s("%d", &student_num);
			hash_chain_print(student_num);
		}
		else if (choice == 2)
			break;
		else
			printf("다시 입력하십시오\n");
	}
	printf("\n\n");
	return 0;
}