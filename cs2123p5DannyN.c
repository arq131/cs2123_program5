/* cs2123p5DannyN.c

File created by Danny Nguyen. 
Includes:
	-processCommand(Tree tree, QuoteSelection quote, char szInputBuffer)
	-defineValue(Tree tree, char szToken, NodeT *pNew)
	-setRoot(Tree, tree)
	-defineCommand(char szToken[])
	-allocateNodeT(Element element)
	-findParent(NodeT *pParent, NodeT *p, NodeT *pKid)
	-printPriceMenu(NodeT *p, int iIndent)
	-insertPriceMenu(Tree tree, szParentId)
	-findId(NodeT *p, char szId[])
	-printOne(Tree tree, char szId[])
Purpose: 
	Contains files created by team-Danny for the driver to run.
	The creator of the function is noted in the header of the functions. 

	-processCommand: Reads the input data and adds them into the tree.
	-defineValue: determines the cases for DEFINE and inserts into the tree.
	-setRoot: sets the root of the tree.
	-defineCommand: returns an int value depending on the token passed in, used for switch statements
	-allocateNodeT: allocates memory to a new node.
	-findParent: finds the parent of an ID
	-printPriceMenu: prints out the enter price menu when PRINT ALL is called.
	-insertPriceMenu: inserts the new nodes into the tree.
	-findId: finds a certain id that is passed in.
	-printOne: prints out the information for one ID.
*********************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p5.h"



/*********** setRoot *************
NodeT setRoot(Tree tree)
Purpose:
	Creates the root of the tree so that the first options inserted
	into the tree know where the root of the tree is.
Parameters:
	I 	Tree tree 				The tree to insert the root into.
Returns: None.
Notes: 
	[11/8/2015 11:12PM]: No notes as of right now. Might add some later.
	Function created by Danny Nguyen.
*********************************/
void setRoot(Tree tree)
{
	NodeT pNew;
	// set the values for the root element.
	strcpy(pNew.element.szId, "ROOT");			// unique ID named ROOT so that ID's being put into
											// the tree can find the root.
	strcpy(pNew.element.szTitle, "Price Menu");	// Name of the tree to print what the tree is for.
	pNew.element.dCost = 0.0;
	pNew.element.cCostInd = 'N';
	pNew.element.cNodeType = 'O';
	// set both pointers to NULL.
	pNew.pChild = NULL;
	pNew.pSibling = NULL;
	// inserts into the tree. Passes in the "ROOT" search to see if one exists already.
	insertPriceMenu(tree, pNew.element, "ROOT");
}

/********** defineCommand ************
int defineCommand(char szToken[])
Purpose:
	Defines a token and assigns it an int value for switch statement.
Parameters:
	I char szToken[] 			token to check
Returns:
	Returns an integer according to the token passed in.
Notes:
	Created by Danny Nguyen.
*************************************/
int defineCommand(char szToken[])
{

	if (strcmp(szToken, "DEFINE") == 0)
		return DEFINE;
	else if (strcmp(szToken, "QUOTE") == 0)
		return QUOTE;
	else if (strcmp(szToken, "PRINT") == 0)
		return PRINT;
	else if (strcmp(szToken, "DELETE") == 0)
		return DELETE;
	else
	{
		printf("Error: Command not found (in defineCommand)\n");
	}
}
/********* allocateNodeT *********
NodeT *allocateNodeT(Element value)
Purpose:
	This will allocate a new node and return the node with the values.
Parameters:
	I Element value 		value to insert into node.
Returns:
	Returns the newly allocated node.
Notes: 
	(11/5/2015 2:00PM): Possibly move this function into the main driver so that
	everyone who needs to allocate a new node may use this function.
*********************************/
NodeT *allocateNodeT(Element value)
{
	NodeT *pNew = (NodeT *)malloc(sizeof(NodeT));
	pNew->element = value;
	pNew->pChild = NULL;
	pNew->pSibling = NULL;
	return pNew;
}

/************ findParent **************
NodeT *findParent(NodeT *pParent, NodeT *p, NodeT *pKid)
Purpose:
Returns the parent of a given node. Returns NULL if not found or doesn't have a parent.
Parameters:
I/O     NodeT   *pParent                        //current parent level
I/O     NodeT   *p                                      //tree of options
I/O     NodeT   *pKid                           //child used to find the parent
I       NodeT   *pCheck                         //checks to see if pKid exists in tree
I       NodeT   *pSiblingCheck          //recursively searches through siblings Nodes
I       NodeT   *pChildCheck            //recursively searches through children Nodes
Returns:
NULL    - Not found, No Parent
pParent - Returns the found parent held in pParent
p               - Returns the current Node in p if it is the parent.
Notes: Created by J'hon.
******************************************/
NodeT *findParent(NodeT *pParent, NodeT *p, NodeT *pkid)
{
        //Node checkers
        NodeT *pCheck, *pSiblingCheck, *pChildCheck;
        //checks for NULL p
        if (p == NULL)
                return;
        //checks to see if a pkid is at the root of the tree
        if (strcmp(p->element.szId, pkid->element.szId) == 0)
                return NULL;
        //checks for pkid in tree
        pCheck = findId(p, pkid->element.szId);
        //continue checking
        if (pCheck != NULL)
        {
                //checks if pParent has been found
                if (pParent != NULL && strcmp(p->element.szId, pkid->element.szId) == 0)
                        return pParent;
                //recursively search siblings
                pSiblingCheck = findParent(pParent, p->pSibling, pkid);
                //recursively search children
                pChildCheck = findParent(p, p->pChild, pkid);
                //checks for NULL sibling checks
                if (pSiblingCheck != NULL)
                        return pSiblingCheck;
                //checks for NULL child checks
                if (pChildCheck != NULL)
                        return pChildCheck;
 
        }
        //else return NULL
        return NULL;
 
}
/********** defineValue ************
void defineValue(Tree tree, char szToken, NodeT *pNew, char pszRemainingTxt)
Purpose:
	This will check to see what the second token is after the DEFINE token is detected. 
	If the second token is OPTION:
		The input line will contain:
		DEFINE OPTION szId szSubordinateToId szTitle
		where:
			szId:
				This is the ID of the new node. 
			szSubordinateToId:
				This is the ID of the node that is its parent (via a pChild Pointer).
				Note: If we are inserting at the root, then the szSubordinateToId is going
				to be "ROOT".
			szTitle:
				This is the title for the node. (e.g. "Model").

		DEFINE VALUE szId szOptionId cCostInd dCost szTitle
		where:
			szId:
				This is the ID of the new node.
			szOptionId:
				This is the ID of the option which is its parent (via a pChild pointer).
			cCostInd:
				This is a character that represents if it has a cost or not. 
				Note: 'Y' indicates that it has a value. 'N' indicates that it does not have a value.
			dCost:
				This is the cost of the ID, which may be a positive or a negative number.
				Note: This is assuming that the cCostInd is equal to 'Y'.
			szTitle:
				This is the title for the node. (e.g. "Racing White").
Parameters:
	I 	Tree tree 					This is the tree that the information is going to be inserted into.
	I 	char szToken 				This is the token that getToken will insert the token that it is
									is grabbing into.
	I   NodeT *pNew 				This is the newly created node. (Possibly edited to make this a local
									value).
	I 	pszRemainingTxt				This is the remaining text after grabbing the input line.

Returns: None.
Notes: May add some extra notes later. Currently (11/5/2015 1:59PM) no notes need to be added.
	Function created by Danny Nguyen.
***********************************/
void defineValue(Tree tree, char szToken[], char *pszRemainingTxt)
{
	char szSubordinateToId[MAX_ID_SIZE]; 		// used to store the subordinate ID. (DEFINE OPTION _____ xxxxx _____) <-- Where the xxxxx is located.
	Element element;
	int iStringLen;
	// if the command line of DEFINE is "OPTION", enter.
	if (strcmp(szToken, "OPTION") == 0)
	{
			// set the cNodeType of the element to O, indicating that it is an OPTION
			element.cNodeType = 'O';

			// grab the next token, and insert it into the ID of the new element. 
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(element.szId, szToken);

			// this will grab the subordinate ID from the input line.
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(szSubordinateToId, szToken);
			
			// This will grab the title name of the input line.
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(element.szTitle, szToken);

			// Insert the new node into the tree. 
			insertPriceMenu(tree, element, szSubordinateToId);
	}
	// else if the command line of DEFINE is "VALUE"
	else if (strcmp(szToken, "VALUE") == 0)
	{
			// set the cNodeType of the element to V, indicating that it is a VALUE
			element.cNodeType = 'V';

			// Grabs the ID of the value
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(element.szId, szToken);

			// Grabs the subordinate ID to know where it is inserted into.
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(szSubordinateToId, szToken);

			// Grabs the cost index to determine if there is a value.
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			element.cCostInd = szToken[0];

			// grabs the cost of the value.
			if (szToken[0] == 'Y')
			{
				pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
				element.dCost = atoi(szToken);
			}

			// the rest of the text remaining will be the title that is printed.
			if (pszRemainingTxt != NULL)
			{
					strcpy(element.szTitle, pszRemainingTxt);
					iStringLen = strlen(element.szTitle);
					// if there is a /r within the sztitle, remove it and change to a null terminate.
					// this is needed so that the print lines do not have any printing errors in both the terminal and
					// the output file.
					if (element.szTitle[iStringLen - 2] == '\r')
					{
						element.szTitle[iStringLen - 2] = '\0';
					}
			}

			// if there is no title, print an error then break.
			else
			{
				printf("Error: No title identified.\n");
				return;
			}

			// insert the information into the Price Menu.
			insertPriceMenu(tree, element, szSubordinateToId);
	}
	// else if the command is unrecognize, print out an error message, but doesn't exit out of the function.
	else
		printf("Define value not recognized, received: %s\n", szToken);
}


/************ printPriceMenu **************
void printPriceMenu(NodeT *p, int iIndent)
Purpose:
	Prints out the information inside the tree.
Parameters:
	I NodeT *p 				Top node of the tree.
	I int iIndent 			Used for nicely indenting the information
Returns: None.
Notes: Function created by Danny Nguyen.
******************************************/
void printPriceMenu(NodeT *p, int iIndent)
{
	int i;
	char szTitle[MAX_TOKEN_SIZE];

	// if the node is empty, return.
	if (p == NULL)
		return;

	// print out a tab for the start.
	printf("   ");

	// print out indentions for child pointers.
	for (i = 0; i < iIndent; i++)
		printf("   ");

	// if the ID has a cost, print out the title and the cost.
	if (p->element.cCostInd == 'Y')
		printf("%-30s%50.2lf\n", p->element.szTitle, p->element.dCost);
	// else if the title doesnt have a cost, just print out the title.
	else
		printf("%-s\n", p->element.szTitle);

	// if the child is not null, go into the child of the pointer.
	if (p->pChild != NULL)
		printPriceMenu(p->pChild, iIndent + 1);

	// if the sibling is not null, go into the sibling of the pointer and set the next pointer to sibling.
	if (p->pSibling != NULL)
		printPriceMenu(p->pSibling, iIndent);
}

/************* insertPriceMenu ****************
void insertPriceMenu(Tree tree, Element element, char szParentId[])
Purpose:
	Inserts the information into the tree. 
Parameters:
	I Tree tree 				The tree the info is being inserted into
	I Element element 			The information that is being inserted into the tree
	I char szParentId[] 		The parent that needs to be found. If it isn't, then it'll be inserted as
								a new child.
Returns: None.
Notes: Function created by Joey Drew.
**********************************************/
void insertPriceMenu(Tree tree, Element element, char szParentId[])
{
	NodeT *pNew = allocateNodeT(element);
	NodeT *pHead = tree->pRoot;
	NodeT *p = findId(pHead, szParentId);

	// if the tree is empty, insert it into the beginning of the tree.
	if (pHead == NULL)
	{
		// when we call setRoot, this will be the only time it enters this if statement.
		tree->pRoot = pNew;
		return;
	}

	// if findID finds the correct ID, insert it accordingly.
	if (p != NULL)
	{
		// checks to see if the ID of the element already exists.
		NodeT *pCheck = findId(pHead, element.szId);

		// if it already exists, print out an error message and return. 
		if (pCheck != NULL)
		{
			printf("Error: %s already exists within the menu.\n", element.szId);
			return;
		}

		// checks if we are inserting a value into a value.
		if (element.cNodeType == 'V' && p->element.cNodeType == 'V')
		{
				printf("Error: Inserting a value into a value. Received (Node inserting): %s; Received (Node found): %s\n", pNew->element.szId, p->element.szId);
				return;	
		}
		// if the child is empty, insert it into the child and return.
		if (p->pChild == NULL)
		{
			p->pChild = pNew;
			return;
		}

		// else if the child is not empty, go into the child and check if it's sibling is NULL.
		p = p->pChild;

		// while there is still a sibling, continue till sibling hits a null. 
		while (p->pSibling != NULL)
		{
			// if the type that is being inserted does not match the type of the siblings, print out an error message and return.
			if (p->element.cNodeType != pNew->element.cNodeType)
			{
				printf("Error: Inserting the wrong selection type (value or option) into the wrong node.\n");
				return;
			}
			p = p->pSibling;
		}
		// the null sibling will be where we insert the node.
		if (p->pSibling == NULL)
			p->pSibling = pNew;

	}
	// if the ID is not returned through findID, print out an error message, but does not exit the program.
	else
		printf("Error, ID not found: %s\n", szParentId);

	return;
}
/************ findId **************
NodeT *findId(NodeT *p, char szId[])
Purpose:
	Searches for an ID in the tree. if not found, returns nothing.
Parameters:
	I NodeT *p 			The starting node to start searching
	I char szId[] 		The ID to search for
Returns:
	Returns the node inside the tree that it is looking for.
Notes: Function created by Lung Joe.
**********************************/

NodeT *findId(NodeT *p, char szId[])
{
	NodeT *pFind = NULL;
	// If tree empty return null;
	if (p == NULL)
		return NULL;

	//If leaf contains the passed parent value the boolean becomes true.
	if (strcmp(p->element.szId, szId) == 0) 
		return p;

	// if Sibling is not null, go into the sibling
	if (p->pSibling != NULL)
		pFind = findId(p->pSibling, szId);
	
	// if id is not found in sibling, check the childs
	if (pFind == NULL)
		pFind = findId(p->pChild, szId);

	// return the node if found. 
	return pFind;
}

/********* printOne *************
void printOne(Tree tree, char szId[])
Purpose:
	Prints out the information of the ID that is passed in. If the ID has
	child's, then it will print out all of the childs and their prices. If the
	ID does not have childs, then just print out the name and price of the ID.
Parameters:
	I Tree tree 				the tree to find the id from
	I char szId[] 				The id that being searched for.
Returns: None.
Notes: Function created by Danny Nguyen.
********************************/
void printOne(Tree tree, char szId[])
{
	int i;
	NodeT *pHead = tree->pRoot;
	// findId is called to search for the ID.
	NodeT *p = findId(pHead, szId);

	// if no ID is found, print out a warning indicating that the item is not found.
	if (p == NULL)
	{
		printf("Warning: Item ID '%s' not found.\n", szId);
	}

	// if the pointer is found, and it has a child, go into the child and print out all of the childs of the node
	if (p->pChild != NULL)
	{
		// print out the ID 
		printf("%-10s\n", p->element.szTitle);
		p = p->pChild;
		// print out the child's of the ID.
		while (p->pSibling != NULL)
		{
			printf("%-30s%-10.2lf\n", p->element.szTitle, p->element.dCost);
			p = p->pSibling;
		}
	}
	// else if the node does not have any child, just print out the item ID and price.
	else
	{
		printf("Item: %s; Price: %.2lf\n", p->element.szTitle, p->element.dCost);
	}
}
/******** processCommand *********
void processCommand(Tree tree, QuoteSelection quote, char szInputBuffer)
Purpose:
	Processes the command for the main file. 
Parameters:
	I Tree tree 						tree to insert information into
	I QuoteSelection quote 				quote function use. ???
	I char szInputBuffer 				used to read input file lines.
Returns: None
Notes: 
	defineCommand will return an int code to determine which case the switch statement will enter.
	Function created by Danny Nguyen.
*********************************/
void processCommand(Tree tree, QuoteSelection quote, char szInputBuffer[])
{
	char szToken[MAX_TOKEN_SIZE];
	char *pszRemainingTxt;
	int iSwitchCode;
	QuoteResult quoteResult;

	// grabs the first token of the input line
	pszRemainingTxt = getToken(szInputBuffer, szToken, MAX_TOKEN_SIZE - 1);

	// grabs a switch code from defineCommand for a switch statement.
	iSwitchCode = defineCommand(szToken);

	// determines what to do from first command line
	switch(iSwitchCode)
	{
		// if case is DEFINE
		case DEFINE:
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			defineValue(tree, szToken, pszRemainingTxt);
			break;

		// if case is QUOTE
		case QUOTE:
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);

			if (strcmp(szToken, "BEGIN") == 0)
			{
				quote->iQuoteItemCnt = 0;
			}
			else if (strcmp(szToken, "OPTION") == 0)
			{		
				quote->quoteItemM[quote->iQuoteItemCnt] = quoteOption(pszRemainingTxt);
				quote->iQuoteItemCnt += 1;
			}
			else if (strcmp(szToken, "END") == 0)
			{
				quoteResult = determineQuote(tree, quote);
				printQuote(tree, quote, quoteResult);
			}
			else
				printf("Error: Quote command not recognized.\n");
			break;

		// if case if DELETE	
		// NOTE: [11/20/2015; 12:22PM: Current, DELETE does not do anything. The command for this statement will be 
		// added by the next due date.]	
		case DELETE:
			//pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			//deleteItem(tree, szToken);
			break;

		// if case is PRINT
		case PRINT:
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			
			// if the option is "PRINT ALL", print all information of the tree.
			if (strcmp(szToken, "ALL") == 0)
			{
				printf("%-s\n", tree->pRoot->element.szTitle);
				printPriceMenu(tree->pRoot->pChild, 0);
			}
			// else if the option is "PRINT ONE", print the information of the ID given.
			else if (strcmp(szToken, "ONE") == 0)
			{
				pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
				printOne(tree, szToken);
			}
			// else if the option is unrecognize, print out an error message, but continue to run the program.
			else
				printf("PRINT command not recognized, found: %s\n", szToken);

			break;

		// if case is unrecognized, return an error. 
		default:
			printf("Command not recognized, found: %s\n", szToken);
	}

}

/*************** freeTree *******************
void freeTree(Tree tree)
Purpose:
	Frees the tree memory.
Parameters:
	I Tree tree 		the tree to be freed
Returns: None
Notes: Created by Lung Joe.
********************************************/
void freeTree(Tree tree)
{
	// freeSubTree called to free all of the subtrees recursively
	freeSubTree(tree->pRoot);

	// free the main tree.
	free(tree);
}

/**************** freeSubTree ***************
void freeSubTree(NodeT *p)
Purpose:
	frees the subtree of the main tree
Parameter:
	I NodeT *p 		pointer to head of tree
Returns: None
Notes: Created by Lung Joe.
********************************************/
void freeSubTree(NodeT *p)
{
	// if the pointer is null, return.
	if (p == NULL)
		return;

	// if the child isn't null, proceed into the child.
	if (p->pChild != NULL)
		freeSubTree(p->pChild);

	// if the sibling isn't null, proceed into the sibling
	if (p->pSibling != NULL)
		freeSubTree(p->pSibling);

	// free the node
	free(p);
}


