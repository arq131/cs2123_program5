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
/************** quoteBegin *******************
void quoteBegin();
Purpose: 
	Allocates memory for a quoteSelection.
Parameters:
	None.
Returns: newly allocated quote selection
Notes:
	Function created by Danny Nguyen
*********************************************/
QuoteSelection *quoteBegin()
{
	QuoteSelection *newQuote = (QuoteSelection *) malloc(sizeof(QuoteSelectionImp));
	return newQuote;
}

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
void quoteOption(char *pszRemainingTxt, QuoteSelectionItem quoteSelectionItem)
{
	char szToken[MAX_TOKEN_SIZE];

	if (pszRemainingTxt == NULL)
	{
		printf("Error: command OPTION has no options listed.\n");
		return;
	}

	pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
	quoteSelectionItem.iLevel = atoi(szToken);

	pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
	strcpy(quoteSelectionItem.szOptionId, szToken);

	pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
	quoteSelectionItem.iSelection = atoi(szToken);



}

/************** determineQuote *************
*******************************************/

QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection)
{
	int i;
	int iCount;
	int iCountLevels;
	int iItemCount = quoteSelection->iQuoteItemCnt;
	NodeT *pHead = tree->pRoot;
	NodeT *pChild;
	NodeT *pSibling;
	QuoteResult quoteResult;


	for (iCount = 0; iCount < iItemCount; iCount++)
	{
		NodeT *p = findId(pHead, quoteSelection->quoteItemM[iCount].szOptionId);
		if (p == NULL)
		{
			quoteResult.returnCode = QUOTE_BAD_OPTION;
			quoteResult.error = quoteSelection->quoteItemM[iCount];
			return quoteResult;
		}

		if (quoteSelection->quoteItemM[iCount].iLevel == 1)
			iCountLevels++;

		if (p->pChild != NULL)
			pChild = p->pChild;

		if (pChild != NULL)
		{
			pSibling = pChild;

			for (i = 0; i < quoteSelection->quoteItemM[iCount].iSelection - 1; i++)
			{
				if (pSibling->pSibling != NULL)
					pSibling = pSibling->pSibling;
				else
				{
					quoteResult.returnCode = QUOTE_BAD_SELECTION;
					quoteResult.error = quoteSelection->quoteItemM[iCount];
					return quoteResult;
				}
			}
		}

		if (pSibling != NULL)
		{
			quoteSelection->quoteItemM[iCount].dCost = pSibling->element.dCost;
			quoteResult.dTotalCost += pSibling->element.dCost;
			printf("%20s%20s%15.2lf\n", p->element.szTitle, pSibling->element.szTitle, quoteSelection->quoteItemM[iCount].dCost);
		}
	}
	if (iCountLevels != 3)
	{
		quoteResult.returnCode = QUOTE_PARTIAL;
		return quoteResult;
	}
	else
	{
		quoteResult.returnCode = QUOTE_NORMAL;
		return quoteResult;
	}
}

/******************** determineResult **************
***************************************************/
/*void determineResult(QuoteResult quoteResult)
{
	int iSwitchCode = quoteResult->returnCode;

	switch(iSwitchCode)
	{
		case QUOTE_NORMAL:
			for ()


	}

}*/