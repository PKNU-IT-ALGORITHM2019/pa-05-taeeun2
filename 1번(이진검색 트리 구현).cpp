#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#define CAPACITY 200000
#define BUFFER_SIZE 1000


typedef struct NodeStruct
{
	char *word;
	char *part;
	char *meaning;

	struct NodeStruct* leftChild;
	struct NodeStruct* rightChild;
}Node;

Node* root = NULL;
int n;

Node* BST_insert(Node* root, char *word, char*part, char*meaning)
{
	if (root == NULL)
	{
		root = (Node*)malloc(sizeof(Node));
		root->leftChild = root->rightChild = NULL;
		root->word = strdup(word);
		root->part = strdup(part);
		root->meaning = strdup(meaning);
		n++;
		return root;
	}
	else
	{
		if (stricmp(root->word, word) > 0)
			root->leftChild = BST_insert(root->leftChild, word, part, meaning);
		else if(stricmp(root->word, word) < 0)
			root->rightChild = BST_insert(root->rightChild, word, part, meaning);
	}
	return root;
}
Node* successor(Node* root)
{
	if (root->leftChild == NULL)
		return root;
	else return successor(root->leftChild);
}
Node* BST_delete(Node* root, char *word)
{
	Node* tNode = NULL;
	if (root == NULL)
		return NULL;
	
	if (stricmp(root->word, word) > 0)
			root->leftChild = BST_delete(root->leftChild, word);
	else if (stricmp(root->word, word) < 0)
			root->rightChild = BST_delete(root->rightChild, word);
	else
		{
			// 자식 노드가 둘 다 있을 경우
			if (root->rightChild != NULL && root->leftChild != NULL)
			{
				tNode = successor(root->rightChild);
				root->word = tNode->word;
				root->rightChild = BST_delete(root->rightChild, tNode->word);
			}
			else
			{
				tNode = (root->leftChild == NULL) ? root->rightChild : root->leftChild;
				free(root);
				return tNode;
			}
	}
		return root;

}
Node* BST_search(Node* root, char *word)
{
	if (root == NULL)
		return NULL;

	if (stricmp(root->word, word) == 0)
		return root;
	else if (root->word == NULL)
		return NULL;
	else if (stricmp(root->word, word) > 0)
		return BST_search(root->leftChild, word);
	else
		return BST_search(root->rightChild, word);
}

void read() {

	char buffer[BUFFER_SIZE];
	char *word, *part, *meaning;
	int state = 0;
	FILE *fp = fopen("shuffled_dict.txt", "r");

	if (fp == NULL) {
		printf("Open failed.\n");
		return;
	}

	while (!feof(fp))
	{
		fgets(buffer, BUFFER_SIZE, fp);

		word = strtok(buffer, "(");
		word[strlen(word) - 1] = NULL;
		part = strtok(NULL, ") ");
		if (part == NULL)
			strcpy(part, NULL);
		meaning = strtok(NULL, "\n");
		root = BST_insert(root, word, part, meaning);
		//삽입할 때 중복된 단어는 넣지 않는다.
		
	}
	
	fclose(fp);



}


int main()
{
	read();
	char command[CAPACITY];

	while (1) {
		printf("$ ");
		scanf("%s", command);
		char word[CAPACITY];

		if (strcmp(command, "size") == 0)
			printf("%d\n", n);
		else if (strcmp(command, "find") == 0)
		{
			scanf("%s", word);
			if (BST_search(root, word) == NULL)
				printf("Not found\n");
			else
				printf("meaning  :  %s\n", BST_search(root, word)->meaning);
			
		}
		else if (strcmp(command, "add") == 0)
		{

			char part2 = ' ';
			char part[2];
			char  meaning[CAPACITY];

			printf("word : ");
			scanf("%s", word);
			getchar();
			printf("class : ");
			scanf("%c", &part2);
			printf("meaning : ");
			scanf("%c", &part2);//gets_s함수가 엔터를 인식하지 않도록 하기 위해
			gets_s(meaning, CAPACITY);

			if (part2 == '\n')
				part[0] = ' ';
			else
				part[0] = part2;

			part[1] = NULL;

			root = BST_insert(root, word, part, meaning);

		}
		else if (strcmp(command, "delete") == 0)
		{
			scanf("%s", word);
			if (BST_search(root, word) == NULL)
				printf("Not Found\n");
			else
			{
				printf("Deleted successfully.\n");
				root = BST_delete(root, word);
			}
		}
		else if (strcmp(command, "deleteall") == 0)
		{
			char fileName[BUFFER_SIZE];
			char buffer[BUFFER_SIZE];
			char *word;
			int num = 0;
			scanf("%s", fileName);

			FILE *fp = fopen(fileName, "r");

			if (fp == NULL) {
				printf("Open failed.\n");
			}

			while (!feof(fp))
			{
				fgets(buffer, BUFFER_SIZE, fp);

				word = strdup(buffer);
				word[strlen(word) - 1] = NULL;
				root = BST_delete(root, word);
				num++;
			}
			fclose(fp);

			printf("%d words were deleted successfully\n",num);

		}


		else if (strcmp(command, "exit") == 0)
			break;

	}

}