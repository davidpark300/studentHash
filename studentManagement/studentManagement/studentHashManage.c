#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 2599999
#define NAME_SIZE 100
#define DATA_SIZE 256
#define LATE_SIZE 10
int data_numbers = 0;

typedef struct {
	int key;
	char name[NAME_SIZE];
	int phone_number;
	char late[LATE_SIZE];
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
			fprintf(stderr, "�̹� Ž��Ű�� ����Ǿ� ����\n");
			return;
		}
	}
	ptr = (struct list*)malloc(sizeof(struct list));
	ptr->item.key = item.key;
	strcpy(ptr->item.name, item.name);
	ptr->item.phone_number = item.phone_number;
	strcpy(ptr->item.late, item.late);

	ptr->link = NULL;

	if (node_before)
		node_before->link = ptr;
	else
		ht[hash_value] = ptr;
}

// ü�ι��� �̿��Ͽ� ���̺� ����� Ű�� Ž��
void hash_chain_search(int key) {
	struct list* node;
	int hash_value = hash_function(key);
	printf("\n=======================================\n");
	if (hash_table[hash_value] == NULL) {
		printf("��ϵ� �й��� �����ϴ�.\n");
		return;
	}
	for (node = hash_table[hash_value]; node; node = node->link) {
		printf("%d, %s, 0%d", node->item.key, node->item.name, node->item.phone_number);
	}
	printf("\n");
	printf("=======================================\n");
}

void csv_data_write(int key) {
	FILE* fpw = NULL;
	if ((fpw = fopen("studentData.csv", "w")) == NULL) {
		printf("���� ���� ����\n");
		fclose(fpw);
		return;
	}
	char lateYes = 'T';
	char lateNo = 'F';
	struct list* node;
	int hash_value = hash_function(key);
	for (node = hash_table[hash_value]; node; node = node->link) {
		fprintf(fpw, "%d, %s, 0%d, %c", node->item.key, node->item.name, node->item.phone_number, lateYes);
	}
	fclose(fpw);
}

// csv ���� �о�ͼ� �ؽÿ� ����� �� �ֵ��� �� �����ϴ� �Լ�
void csv_data_read() {
	FILE* fpr;
	char str[DATA_SIZE];
	element items;

	if ((fpr = fopen("studentData.csv", "r+")) == NULL) {
		printf("���� �б� ����\n");
		fclose(fpr);
		return;
	}

	fgets(str, DATA_SIZE, fpr);	// ���Ͽ��� ���ڿ��� �������� �Լ�

	char* sub = strtok(str, ",");

	while (1) {
		fgets(str, DATA_SIZE, fpr);
		sub = strtok(str, ",");
		items.key = atoi(sub);	// csv ������ �й� ����
		//printf("%d || ", atoi(sub));

		sub = strtok(NULL, ",");
		if (sub == NULL) {
			items.key = 0;
			//printf("������ ����\n");
			return;
		}
		strcpy(items.name, sub);	// csv ������ �̸� ����
		//printf("%s || ", sub);

		sub = strtok(NULL, ",");
		items.phone_number = atoi(sub);	// csv ������ �ڵ��� ��ȣ ����
		//printf("%d || ", atoi(sub));
		//printf("\n");

		sub = strtok(NULL, ",");
		strcpy(items.late, sub);

		data_numbers++;
		hash_chain_add(items, hash_table);
	}
	fclose(fpr);
}



int main() {
	int choice;
	int student_num;

	csv_data_read();

	while (1) {
		printf("������ : 1. �л���ȸ 2. ���� 3. (�̿ϼ�)���� ������Ʈ=> ");
		scanf_s("%d", &choice);
		if (choice == 1) {
			printf("�й� �Է� => ");
			scanf_s("%d", &student_num);
			hash_chain_search(student_num);
		}
		else if (choice == 2)
			break;
		else if (choice == 3) {
			printf("�й� �Է� => ");
			scanf_s("%d", &student_num);
			csv_data_write(student_num);
		}
		else
			printf("�ٽ� �Է��Ͻʽÿ�\n\n");
	}

	return 0;
}