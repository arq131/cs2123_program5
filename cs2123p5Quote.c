/********** cs2123p5Quote
TEAM_DANNY

file containing the functions for QUOTE

functions:
	void quoteOptions(char *pszRemainingTxt);
	QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection);
	NodeT *pSelection(int iSelection, char *pszOptionId, NodeT *pHead);
	void printQuote(Tree tree, QuoteSelection quote, QuoteResult quoteResult);

This file was created by Danny Nguyen.

file input example for quotes:
********************************** QUOTE ***************
*
* This is a complete quote
*
QUOTE BEGIN
QUOTE OPTION 0 model 1
QUOTE OPTION 1 engine_base 1
QUOTE OPTION 1 color_base 2
QUOTE OPTION 1 audio_base 2
QUOTE OPTION 0 warranty 2
QUOTE END
*
********************************* PARTIAL QUOTE ********
*
QUOTE BEGIN 
QUOTE OPTION 0 model 3
QUOTE OPTION 1 engine_oy 2
QUOTE OPTION 1 color_oy 2
QUOTE OPTION 0 warranty 3
QUOTE END
*
********************************* Bad Option ********
*
QUOTE BEGIN
QUOTE OPTION 0 model 1
QUOTE OPTION 1 engine_base 1
QUOTE OPTION 1 colour_base 2
QUOTE OPTION 1 audio_base 2
QUOTE OPTION 0 warranty 2
QUOTE END
*
********************************* Bad Value ********
*
QUOTE BEGIN
QUOTE OPTION 0 model 1
QUOTE OPTION 1 engine_base 1
QUOTE OPTION 1 color_base 2
QUOTE OPTION 1 audio_base 3
QUOTE OPTION 0 warranty 2
QUOTE END
*
***********/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p5.h"

#define MODEL_ERROR 2
#define ENGINE_ERROR 3
#define COLOR_ERROR 4
#define AUDIO_ERROR 5
#define WARRANTY_ERROR 6

/************** quoteOption ****************
void quoteOption(char *pszRemainingTxt, QuoteSelection quoteSelection)
Purpose:
	Grabs the values from the input text and inserts them into quoteSelection.
Parameters:
	char *pszRemainingTxt 			the input line code
	QuoteSelection quoteSelection 	quoteSelection to store the information into
Return:
	returns information through quoteSelection
Notes:
	Function created by Danny Nguyen
	This function is called in processCommands.
	Information for typedef quoteSelection:

	typedef struct
	{
    	int    iLevel;                      // Outline level for Options:
                                        	//   Level 0 is for options at
                                        	//       the root level.
                                        	//   Level 1 is for options subordinate
                                        	//       to level 0 option values.
                                        	//   Level 2 is for options subordinate
                                        	//       to level 1 option values.
    	char   szOptionId[MAX_ID_SIZE+1];   // Option ID  (this is not the
                                        	// ID for an option value)
    	int    iSelection;                  // Selection index (relative to 1)
    	double  dCost;                      // Cost of this selected item
	} QuoteSelectionItem;

*******************************************/
QuoteSelectionItem quoteOption(char *pszRemainingTxt)
{
	char szToken[MAX_TOKEN_SIZE];
	QuoteSelectionItem quoteItem; 		//quote selection to return.

	// if the text is empty, return an error.
	if (pszRemainingTxt == NULL)
	{
		printf("Error: command OPTION has no options listed.\n");
		return;
	}

	// grabs the first token and insert it into iLevel
	pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
	quoteItem.iLevel = atoi(szToken);

	// grabs the second token and insert it into szOptionId.
	pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
	strcpy(quoteItem.szOptionId, szToken);

	// grabs the last token and inserts it into iSelection
	pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
	quoteItem.iSelection = atoi(szToken);

	// return the selected quote items.
	return quoteItem;

}

int bCheck(int bModel, int bEngine, int bColor, int bAudio, int bWarranty)
{
	if (!bModel)
		return MODEL_ERROR;
	if (!bEngine)
		return ENGINE_ERROR;
	if (!bColor)
		return COLOR_ERROR;
	if (!bAudio)
		return AUDIO_ERROR;
	if (!bWarranty)
		return WARRANTY_ERROR;
	return TRUE;
}
/************** determineQuote *************
QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection)
Purpose:
	Reads each of the quote command lines and finds out which selection ID to choose.
Parameters:
	Tree tree 							the tree to search the ID for
	QuoteSelection quoteSelection 		the quote selection containing the quote information.
Returns: 
	Returns the result of the selections. 
Notes:
	Function created by Danny Nguyen.
	This function is called in processCommands.
*******************************************/

QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection)
{
	int i;
	int iCount;
	int bPartialCheck = FALSE;
	int bModel = FALSE;
	int bEngine = FALSE;
	int bColor = FALSE;
	int bAudio = FALSE;
	int bWarranty = FALSE;
	int iItemCount = quoteSelection->iQuoteItemCnt;
	NodeT *pHead = tree->pRoot;

	NodeT *pSibling;
	NodeT *pOption;
	QuoteResult quoteResult;
	quoteResult.dTotalCost = 0;

	// runs a loop for the number of options selected. 
	for (iCount = 0; iCount < iItemCount; iCount++)
	{
		NodeT *pChild;
		char szOptionId[MAX_ID_SIZE];
		char szOptionTitle[MAX_TOKEN_SIZE];
		strcpy(szOptionId, quoteSelection->quoteItemM[iCount].szOptionId);

		// searches for the ID of the option
		NodeT *p = findId(pHead, szOptionId);

		// if the option is not found, set the return code to QUOTE_BAD_OPTION and add what line caused the error, then returns.
		if (p == NULL)
		{
			quoteResult.returnCode = QUOTE_BAD_OPTION;
			quoteResult.error = quoteSelection->quoteItemM[iCount];
			return quoteResult;
		}

		// if the ID is found, set the pChild to its child.
		if (p->pChild != NULL)
			pChild = p->pChild;

		// while the child isn't null, traverse the child's
		if (p != NULL)
		{
			strcpy(szOptionTitle, p->element.szTitle);
			// these are checks used to see which option has been selected.
			// if its model, set the check to true for model.
			if (strcmp(szOptionTitle, "Model") == 0)
			{
				// if its already true, more than 1 model had been selected.
				if (bModel == TRUE)
				{
					strcpy(quoteResult.error.szOptionId, "Model");
					bModel = FALSE;
				}
				else
					bModel = TRUE;
			}
			// if its warranty, set the check to true for warranty.
			if (strcmp(szOptionTitle, "Warranty") == 0)
			{
				// if its already true, more than 1 warranty had been selected.
				if (bWarranty == TRUE)
				{
					strcpy(quoteResult.error.szOptionId, "Warranty");
					bWarranty = FALSE;
				}
				else
					bWarranty = TRUE;
			}

			// if the level is set to 1 or higher, check the options.
			if (quoteSelection->quoteItemM[iCount].iLevel != 0)
			{
				// if the option is not null (which should not be null if it reached this point)
				if (pOption != NULL)
				{
					// compare the two ID's to make sure that they match. (In this case, ENGINE->COLOR->AUDIO)
					if (strcmp(pOption->element.szId, szOptionId) == 0)
					{
						pSibling = pOption->pChild;
						pOption = pOption->pSibling;
						// these if statements are used to check which option has been selected.

						// if its engine, set the check to true.
						if (strcmp(szOptionTitle, "Engine") == 0)
						{
							// if its already true, then more than one color has been set.
							if (bEngine == TRUE)
							{
								strcpy(quoteResult.error.szOptionId, "Engine");
								bEngine = FALSE;
							}
							else
								bEngine = TRUE;
						}
						
						// if its color, set the check to true.
						if (strcmp(szOptionTitle, "Color") == 0)
						{
							// if its already true, then more than one color has been set.
							if (bColor == TRUE)
							{
								strcpy(quoteResult.error.szOptionId, "Color");
								bColor = FALSE;
							}
							else
								bColor = TRUE;
						}

						// if its audio, set the check to true.
						if (strcmp(szOptionTitle, "Audio") == 0)
						{
							// if its already true, then more than one audio has been set.
							if (bAudio == TRUE)
							{
								strcpy(quoteResult.error.szOptionId, "Audio");
								bAudio = FALSE;
							}
							else
								bAudio = TRUE;
						}

					}

				}
				// else if the option is null, set sibling to the child. This is a backup error check incase it does end up in here
				else
					pSibling = pChild;
			}
			// else if the option level is at the root, set the sibling to the child of the ID thats found. 
			else
				pSibling = p->pChild;


			// select which sibling that is being returned.
			for (i = 0; i < quoteSelection->quoteItemM[iCount].iSelection - 1; i++)
			{
				// if the sibling isn't null, set the sibling to the next one.
				if (pSibling->pSibling != NULL)
					pSibling = pSibling->pSibling;

				// else if there is no more siblings, return an error that has a return code of QUOTE_BAD_SELECTION and add which line
				// caused the error. Then return the result.
				else
				{
					quoteResult.returnCode = QUOTE_BAD_SELECTION;
					quoteResult.error = quoteSelection->quoteItemM[iCount];
					return quoteResult;
				}
			}
		}

		// if it successfully selects the option, set the dCost price into the selection and add it into the total cost.
		if (pSibling != NULL)
		{
			if (quoteSelection->quoteItemM[iCount].iLevel == 0)
			{
				if (pOption == NULL)
				pOption = pSibling->pChild;
			}

			// set the cost of the sibling to the quoteSelection
			quoteSelection->quoteItemM[iCount].dCost = pSibling->element.dCost;
			// add the cost to the total cost result
			quoteResult.dTotalCost += pSibling->element.dCost;
		}
	}

	bPartialCheck = bCheck(bModel, bEngine, bColor, bAudio, bWarranty);
	// if the option is still not null, then not all of the options were selected.
	if ((pOption != NULL) || (bPartialCheck) != TRUE )
	{
		// if the partial check was triggered, returned the error handling that is already inserted when the check was triggered.
		if (bPartialCheck != TRUE)
		{
			// do a switch statement to determine which check was not visited.
			switch(bPartialCheck)
			{
				// if its model, set the option error to model.
				case MODEL_ERROR:
					strcpy(quoteResult.error.szOptionId, "Model");
					break;

				// if its engine, set the option error to engine.
				case ENGINE_ERROR:
					strcpy(quoteResult.error.szOptionId, "Engine");
					break;

				// if its color, set the option error to color.
				case COLOR_ERROR:
					strcpy(quoteResult.error.szOptionId, "Color");
					break;

				// if its audio, set the option error to audio.
				case AUDIO_ERROR:
					strcpy(quoteResult.error.szOptionId, "Audio");
					break;

				// if its warranty, set the option error to warranty.
				case WARRANTY_ERROR:
					strcpy(quoteResult.error.szOptionId, "Warranty");
					break;
			}
			quoteResult.returnCode = QUOTE_PARTIAL;
			quoteResult.error.iLevel = 1;
			quoteResult.error.iSelection = 0;
			return quoteResult;
		}
		// otherwise, set values to the missing option value that was not selected.
		quoteResult.returnCode = QUOTE_PARTIAL;
		quoteResult.error.iLevel = 1;
		strcpy(quoteResult.error.szOptionId, pOption->element.szId);
		quoteResult.error.iSelection = 0;
		// return the result.
		return quoteResult;
	}
	// else it has successfully determined the quote and returns it as normal.
	else
	{
		quoteResult.returnCode = QUOTE_NORMAL;
		return quoteResult;
	}
}

/******************** pSelection ************
NodeT *pSelection(int Selection, char *pszOptionId, Node *pHead)
Purpose:
	Returns the selected node from the option ID.
Parameter:
	int iSelection					number determine which item to choose
	char *pszOptionId				the option ID (parentID) to search for
	NodeT *pHead					the root of the tree to search for the ID
Returns:
	returns the ID of the option that was selected.
Note:
	Function created by Danny Nguyen
	This function is only used in printQuote
********************************************/
NodeT *pSelection(int iSelection, char *pszOptionId, NodeT *pHead)
{
	int i;
	NodeT *p = findId(pHead, pszOptionId);		// searches for the option ID
	NodeT *pChild;
	NodeT *pSibling;

	// if the ID is not found, return NULL
	if (p == NULL)
		return NULL;

	// if the child is not null, set the first sibling to the child of the ID.
	if (p->pChild != NULL)
		pSibling = p->pChild;

	// for loop determining which selection option to choose.
	for (i = 0; i < iSelection - 1; i++)
	{
		// if the sibling is not null, set it to the next sibling.
		if (pSibling->pSibling != NULL)
			pSibling = pSibling->pSibling;
		// else if the next sibling is null while this loop is still running, return null.
		else
			return NULL;
	}
	// return the sibling selected.
	return pSibling;
}

/******************** printQuote **************
void printQuote(Tree tree, QuoteSelection quote, QuoteResult quoteResult)
Purpose:
	Reads the return code and prints out the information accordingly. 
	return Codes:
	NORMAL:
		Prints out the selected options and the total price.
	PARTIAL:
		Prints out the selected options and the total price. This will also print out 
		an error stating that one of the options was not selected.
	BAD_SELECTION:
		Prints out an error message stating which line was a invalid command, as well as stating that it is a bad
		selection.
	BAD_OPTION:
		Prints out an error message stating which line was a invalid command, as well as stating that it is a bad
		option.
Parameters:
	Tree tree 							tree to search the ID's for
	QuoteSelection quote 				quote containing the quote information.
	QuoteResult quoteResult		 		the result of the quote command.
Returns: None.
Notes:
	Function created by Danny Nguyen
	This function is used in processCommands in cs2123p5DannyN.c
**********************************************/
void printQuote(Tree tree, QuoteSelection quote, QuoteResult quoteResult)
{
	NodeT *pHead = tree->pRoot; 					// set the head to the root of the tree
	int i;
	int iItemCount = quote->iQuoteItemCnt;

	// switch statement to determine what to print out.
	switch(quoteResult.returnCode)
	{
		// if the return code is QUOTE_NORMAL
		case QUOTE_NORMAL:
			// traverse through the structured array.
			for (i = 0; i < iItemCount; i++)
			{
				// find the selected ID.
				NodeT *pSelect = pSelection(quote->quoteItemM[i].iSelection, quote->quoteItemM[i].szOptionId, pHead);
				// if the selection is not found, print an error.
				if (pSelect == NULL)
				{
					printf("Error: ID from quote is not found.\n");
					break;
				}

				// find the parent of the ID.
				NodeT *pParent = findId(pHead, quote->quoteItemM[i].szOptionId);
				
				// print out the information.
				printf("%-20s%50s%15.2lf\n"
					, pParent->element.szTitle 				// the option that is it selecting for.
					, pSelect->element.szTitle 				// the selected option
					, quote->quoteItemM[i].dCost);			// the cost of the selected item.	
			}
			// print out the total cost of all of the options.
			printf("Total cost: %20.2lf\n", quoteResult.dTotalCost);
			break;

		// if the return code is QUOTE_PARTIAL
		case QUOTE_PARTIAL:
			// traverse through the structured array.
			for (i = 0; i < iItemCount; i++)
			{
				// find the selected ID.
				NodeT *pSelect = pSelection(quote->quoteItemM[i].iSelection, quote->quoteItemM[i].szOptionId, pHead);
				// if the selection is not found, print an error.
				if (pSelect == NULL)
				{
					printf("Error: ID from quote is not found.\n");
					break;
				}

				// find the parent of the ID.
				NodeT *pParent = findId(pHead, quote->quoteItemM[i].szOptionId);

				// print out the information.
				printf("%-20s%50s%15.2lf\n"
					, pParent->element.szTitle 				// the option that it is looking for.
					, pSelect->element.szTitle 				// the selected option.
					, quote->quoteItemM[i].dCost); 			// the cost of the selected item.
			}
			// print out the total cost of all of the options.
			printf("Total cost: %20.2lf\n", quoteResult.dTotalCost);
			// print out an error message that states that this is a partial quote, and also print out what ID is missing.
			printf("This is a partial quote. The following item had an error with your quote: %s option.\n", quoteResult.error.szOptionId);
			break;

		// if the return code is QUOTE_BAD_SELECTION
		case QUOTE_BAD_SELECTION:
			// print out an error message containing the information of the bad command line.
			printf("This quote has a bad value option: QUOTE OPTION %d %s %d\n"
				, quoteResult.error.iLevel 					// the level of the error.
				, quoteResult.error.szOptionId 				// the option ID of the error.
				, quoteResult.error.iSelection); 			// the selection level of the error.
			break;

		// if the return code is QUOTE_BAD_OPTION
		case QUOTE_BAD_OPTION:
			// print out an error message containing the information of the bad command line.
			printf("This quote had selected a bad option (an option that was not found): QUOTE OPTION %d %s %d\n"
				, quoteResult.error.iLevel 					// the level of the error.
				, quoteResult.error.szOptionId 				// the option ID of the error.
				, quoteResult.error.iSelection); 			// the selection level of the error.
			break;
	}
}