/********** cs2123p5Quote
TEAM_DANNY

file containing the functions for QUOTE

functions:
	QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection);

file input example:
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
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p5.h"

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
	QuoteSelectionItem quoteItem;

	if (pszRemainingTxt == NULL)
	{
		printf("Error: command OPTION has no options listed.\n");
		return;
	}

	pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
	quoteItem.iLevel = atoi(szToken);

	pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
	strcpy(quoteItem.szOptionId, szToken);

	pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
	quoteItem.iSelection = atoi(szToken);

	return quoteItem;

}

/************** determineQuote *************
*******************************************/

QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection)
{
	int i;
	int iCount;
	int iCountLevels = 0;
	int bPartialCheck = FALSE;
	int iItemCount = quoteSelection->iQuoteItemCnt;
	NodeT *pHead = tree->pRoot;

	NodeT *pSibling;
	NodeT *pOption;
	NodeT *pParent;
	QuoteResult quoteResult;
	quoteResult.dTotalCost = 0;

	// runs a loop for the number of options selected. 
	for (iCount = 0; iCount < iItemCount; iCount++)
	{
		NodeT *pChild;
		char szOptionId[MAX_ID_SIZE];
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

		// counter to see if it is a complete quote or a partial quote.
		if (quoteSelection->quoteItemM[iCount].iLevel == 1)
			iCountLevels++;

		// if the ID is found, set the pChild to its child.
		if (p->pChild != NULL)
			pChild = p->pChild;

		// while the child isn't null, traverse the child's
		if (p != NULL)
		{
			if (quoteSelection->quoteItemM[iCount].iLevel == 1)
			{
				if (pOption != NULL)
				{
					if (strcmp(pOption->element.szId, szOptionId) == 0)
					{
						pSibling = pOption->pChild;
						pOption = pOption->pSibling;
					}
					else
					{
						quoteResult.returnCode = QUOTE_PARTIAL;
						quoteResult.error.iLevel = 1;
						strcpy(quoteResult.error.szOptionId, pOption->element.szId);
						quoteResult.error.iSelection = 0;
						bPartialCheck = TRUE;
					}

				}
				else
					pSibling = pChild;
			}
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

			quoteSelection->quoteItemM[iCount].dCost = pSibling->element.dCost;
			quoteResult.dTotalCost += pSibling->element.dCost;
			//printf("%-20s%50s%15.2lf\n", p->element.szTitle, pSibling->element.szTitle, quoteSelection->quoteItemM[iCount].dCost);
		}
	}
	// if less/more than 3 options were selected, only a partial quote will return. It should also return which portion is missing.
	if ((pOption != NULL) || (bPartialCheck) )
	{
		quoteResult.returnCode = QUOTE_PARTIAL;
		quoteResult.error.iLevel = 1;
		strcpy(quoteResult.error.szOptionId, pOption->element.szId);
		quoteResult.error.iSelection = 0;
		return quoteResult;
	}
	// else it has successfully determined the quote and returns it as normal.
	else
	{
		quoteResult.returnCode = QUOTE_NORMAL;
		return quoteResult;
	}
}

/******************** determineMissingOption ************
********************************************************/
NodeT *pSelection(int iSelection, char *pszOptionId, NodeT *pHead)
{
	int i;
	NodeT *p = findId(pHead, pszOptionId);
	NodeT *pChild;
	NodeT *pSibling;
	if (p->pChild != NULL)
		pSibling = p->pChild;

	for (i = 0; i < iSelection - 1; i++)
	{
		if (pSibling->pSibling != NULL)
				pSibling = pSibling->pSibling;
	}
	return pSibling;
}

/******************** determineResult **************
***************************************************/
void printQuote(Tree tree, QuoteSelection quote, QuoteResult quoteResult)
{
	NodeT *pHead = tree->pRoot;
	int i;
	int iItemCount = quote->iQuoteItemCnt;

	switch(quoteResult.returnCode)
	{
		case QUOTE_NORMAL:
			for (i = 0; i < iItemCount; i++)
			{
				NodeT *pSelect = pSelection(quote->quoteItemM[i].iSelection, quote->quoteItemM[i].szOptionId, pHead);
				NodeT *pParent = findId(pHead, quote->quoteItemM[i].szOptionId);
				printf("%-20s%50s%15.2lf\n"
					, pParent->element.szTitle
					, pSelect->element.szTitle
					, quote->quoteItemM[i].dCost);				
			}
			printf("Total cost: %20.2lf\n", quoteResult.dTotalCost);
			break;

		case QUOTE_PARTIAL:
			for (i = 0; i < iItemCount; i++)
			{
				NodeT *pSelect = pSelection(quote->quoteItemM[i].iSelection, quote->quoteItemM[i].szOptionId, pHead);
				NodeT *pParent = findId(pHead, quote->quoteItemM[i].szOptionId);
				printf("%-20s%50s%15.2lf\n"
					, pParent->element.szTitle
					, pSelect->element.szTitle
					, quote->quoteItemM[i].dCost);
			}
			printf("Total cost: %20.2lf\n", quoteResult.dTotalCost);
			printf("This is a partial quote. The following item was missing from your quote: %s option.\n", quoteResult.error.szOptionId);
			break;

		case QUOTE_BAD_SELECTION:
			printf("This quote has a bad value option: QUOTE OPTION %d %s %d\n"
				, quoteResult.error.iLevel
				, quoteResult.error.szOptionId
				, quoteResult.error.iSelection);
			break;

		case QUOTE_BAD_OPTION:
			printf("This quote had selected a bad option (an option that was not found): QUOTE OPTION %d %s %d\n"
				, quoteResult.error.iLevel
				, quoteResult.error.szOptionId
				, quoteResult.error.iSelection);
			break;
	}
}