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
QuoteSelection quoteBegin()
{
	QuoteSelection *newQuote = (QuoteSelection *) malloc(sizeof(QuoteSelectionImp));
	newQuote.iQuoteItemCnt = 0;
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
*******************************************/
void quoteOption(char *pszRemainingTxt, QuoteSelection quoteSelection)
{


}

/************** determineQuote *************
*******************************************/

QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection)
{


}