// rasterGraphic.c
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC	// need this to get the line identification
//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF); // in main, after local declarations
//NB must be in debug build

#include <crtdbg.h>
#include <stdio.h>
#include <time.h>

typedef enum { FALSE = 0, TRUE } BOOL;

struct GraphicElement {
	char* fileName;
	struct GraphicElement* pNext;
};

struct RasterGraphic {
	struct GraphicElement* GraphicElements;
};

// Forward declarations
void InitRasterGraphic(struct RasterGraphic*);
void InsertGraphicElement(struct RasterGraphic*);
void DeleteGraphicElement(struct RasterGraphic*);
void ReportRasterGraphic(struct RasterGraphic*);
void CleanUp(struct RasterGraphic*);

int main(void)
{
	char response;
	BOOL RUNNING = TRUE;
	struct RasterGraphic RG;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	InitRasterGraphic(&RG);
	while (RUNNING)
	{
		printf("MENU\n 1. Insert a GraphicElement\n 2. Delete the first GraphicElement\n 3. Report the RasterGraphic\n 4. Quit\n");
		scanf("%c", &response);
		switch (response)
		{
		case '1':InsertGraphicElement(&RG); break;
		case '2':DeleteGraphicElement(&RG); break;
		case '3':ReportRasterGraphic(&RG); break;
		case '4':RUNNING = FALSE; CleanUp(&RG); break;

		default:printf("Please enter a valid option\n");
		}
		printf("\n");
		while ((response = getchar()) != '\n' && response != EOF);// clear input buffer
	}
	return 0;
}
/******************
Function name: InitRasterGraphic
Purpose: Allocate memory for the GraphicElements structure of size struct GraphicElement
Function In parameters: struct RasterGraphic* pA is passed in as a parameter that contains a list of GraphicElements.
Each individual GraphicElement stores a pointer to a string of characters, and each GraphicElement may point to another GraphicElement or a nullptr.
Function Out parameters: None
Version: 1.02
******************/
void InitRasterGraphic(struct RasterGraphic* pA) {
	/* do not allocate memory for the node. Node initially is null */
	pA->GraphicElements = NULL;
	/*
	cannot explicitly set to NULL if GraphicElements is NULL
	pA->GraphicElements->fileName = NULL;
	pA->GraphicElements->pNext = NULL;
	*/
}

/******************
Function name: InsertGraphicElement
Purpose: Inserts a node at a position chosen by the user.
A new node is created temporarily to avoid directly altering the real node.
Cycle through every node until user's position reached, append fileName, point to the next node, and have the previous node point to the user's node.
Function In parameters: struct RasterGraphic* pA is passed in as a parameter that contains a list of GraphicElements at a memory location.
Function Out parameters: None
Version: 1.20
******************/
void InsertGraphicElement(struct RasterGraphic* pA) {
	/* static array pointer to char. Stores file name by user input for the GraphicElement */
	char* fileName[100];
	/* stores the position where the node will be inserted. Default: 0, because conditional statement throws error at compilation */
	int pos = 0;
	/* temporary/new node for the GraphicElement that is used to store the fileName of the GraphicElement, and the pointer to the next node */
	struct GraphicElement* newNode = (struct GraphicElement*)malloc(sizeof(struct GraphicElement));
	/* temporary node holding the real GraphicElement node.head of the RasterGraphic */
	struct GraphicElement* tempNode = pA->GraphicElements;
	/* a counter that holds the number of nodes there are in the linked list */
	int numOfNodes = 0;
	/* holds node of the current index of the GraphicElement list */
	struct GraphicElement* currentNode;
	/* holds the previous node of the GraphicElement list */
	struct GraphicElement* prevNode;
	/* node index begins from the head */
	int currentIndex = 0;
	printf("Insert a GraphicElement in the RasterGraphic\n");
	/* get and store user input for fileName */
	printf("Please enter the GraphicElement filename: ");
	scanf("%s", &fileName);

	/* dynamic memory allocation for newNode, string length of user input */
	newNode->fileName = (char *)malloc((strlen(fileName) + 1));
	/* copy pointer of char in fileName into newNode->fileName as a string */
	strcpy(newNode->fileName, fileName);

	/* check if any nodes exist, if not i still = 0. if nodes exist, count them. */
	if (tempNode) {
		/* cycle through the linked list to determine the length of the linked list */
		while (tempNode) {
			numOfNodes++;
			tempNode = tempNode->pNext;
		}
	}

	/* set current to head/first node to start insertion where the iteration begins from the head of pA->GraphicElements*/
	currentNode = pA->GraphicElements;

	/* check the length of the linked list to determine which node number is currently being appended to the linked list */
	if (numOfNodes > 0) {
		/* print the number of nodes in the list */
		printf("There are %d GraphicElement(s) in the list. Please specify the position (<= %d) to insert at : ", numOfNodes, numOfNodes);
		/* get node insertion position from user input */
		scanf("%d", &pos);

		/* insertion is before the head */
		if (pos == 0) {
			/* new node is before head of linked list, so point next of 'newNode' to full linked list */
			newNode->pNext = currentNode;
			/* 'newNode' contains user input + rest of linked list. Assign that to pointer to GraphicElement */
			pA->GraphicElements = newNode;
			return;
		}
		/* insertion is after the tail */
		/*
		else if (pos == numOfNodes) {

		return;
		}
		*/
		/* insertion is in between the head and tail of the linked list */
		else {

			/* cycle through linked list from head (currentIndex = 0) to index pos (pos = scanf... get user input) */
			for (currentIndex; currentIndex < pos; currentIndex++) {
				/* assign the current node as the previous node before the current node increases its index*/
				prevNode = currentNode;

				/* here, between the previous node and the current node is where the insertion would happen  */

				/* the current node is the next node after that */
				currentNode = currentNode->pNext;
			}
			prevNode->pNext = newNode;
			newNode->pNext = currentNode;
			return;
		}
	}
	else if (numOfNodes == 0) {
		/* print that there are no other nodes in the list before this node */
		printf("This is the first GraphicElement in the list.\n");
		/* next node is a nullptr until a graphic is entered after this node */
		newNode->pNext = pA->GraphicElements;
		pA->GraphicElements = newNode;
		return;
	}
}

/******************
Function name: DeleteGraphicElement
Purpose: Deletes the first node in the linked list.
Creates a temporary linked list, reassigns head of temporary linked list to next node.
Assigns temporary linked list with deleted first node to original linked list.
Function In parameters: struct RasterGraphic* pA is passed in as a parameter that contains a list of GraphicElements at a memory location.
Function Out parameters: None
Version: 1.0
******************/
void DeleteGraphicElement(struct RasterGraphic* pA) {
	/* assign real linked list to temporary linked list */
	struct GraphicElement* tempNode = pA->GraphicElements;
	/* check if linked list is empty */
	if (tempNode) {
		/* remove first node in the list by assigning the node temp node as the contents of the next node */
		tempNode = tempNode->pNext;
		/* assign temp list of nodes to original node list */
		pA->GraphicElements = tempNode;
		return;
	}
	/* if empty do nothing */
	else {
		return;
	}
}

/******************
Function name: CleanUp
Purpose: Deletes nodes and nodes pointing to filename
Function In parameters: struct RasterGraphic* pA is passed in as a parameter that contains a list of GraphicElements at a memory location.
Function Out parameters: None
Version: 1.0
******************/
void CleanUp(struct RasterGraphic* pA) {
	/*
	List of Dyanmic Memory allocated:
	pA->GraphicElements->fileName
	*/
	/* check linked list is not NULL before freeing */
	if (pA->GraphicElements) {
		/* loop while linked list is valid */
		while (pA->GraphicElements) {
			/* free the filename because it was allocated through dyanmic memory in InsertGraphicElement */
			free(pA->GraphicElements->fileName);
			/* set original linked list to the next node */
			pA->GraphicElements = pA->GraphicElements->pNext;
		}
		return;
	}
	/* linked list is NULL, do not free or else possible errors occur */
	else {
		return;
	}
}


void ReportRasterGraphic(struct RasterGraphic* pA)
{
	int counter = 0;
	time_t startsec, oldsec, newsec;
	struct GraphicElement* iterator = pA->GraphicElements;

	if (pA->GraphicElements == 0)
	{
		printf("No GraphicElements in the RasterGraphic\n");
		return;
	}

	printf("Report the RasterGraphic\n");
	startsec = oldsec = time(NULL);
	while (iterator)
	{
		newsec = time(NULL);
		if (newsec > oldsec)
		{
			printf("GraphicElement #%d, time = %I64d sec\n", counter++, newsec - startsec);
			printf("Image file name = %s\n", iterator->fileName);
			iterator = iterator->pNext;
			oldsec = newsec;
		}
	}
}