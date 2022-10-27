#include <stdio.h>
#include <string.h>
#include "../card/card.h"
#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	char day,month;
	int year;
	/* format DD/MM/YYY */

	if (strlen(termData->transactionDate) == 10 && (termData->transactionDate[2] == '/' && termData->transactionDate[5] == '/'))
	{
		day = (termData->transactionDate[0] -'0')*10 + (termData->transactionDate[1] -'0') ;
		month = (termData->transactionDate[3] -'0')*10 + (termData->transactionDate[4] -'0') ;
		year =    (termData->transactionDate[6] -'0')*1000  + (termData->transactionDate[7] -'0')*100
				+ (termData->transactionDate[8] -'0')*10 + (termData->transactionDate[9] -'0');


        /*months with 31 days */
        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12 )
        {
            if(day <=31 && day >0)
            {return TERMINAL_OK;}
            else {return WRONG_DATE;}

        }else if (month == 4 || month == 6 || month == 9 || month == 11 ) /*months with 30 days*/
        {
            if(day <=30 && day >0)
            {return TERMINAL_OK;}
            else {return WRONG_DATE;}
        }else if (month == 2)  /*February */
        {
            /* leap year*/
            if(year%4 == 0)
            {
                if(day <=29 && day >0)
                {return TERMINAL_OK;}
                else {return WRONG_DATE;}
            }else
            {
                if(day <=28 && day >0)
                {return TERMINAL_OK;}
                else {return WRONG_DATE;}
            }
        }else{return WRONG_DATE;}


	}
	else
	{
		return WRONG_DATE;
	}
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t *termData)
{
	char cardMonth ,cardYear, termMonth, termYear;
    EN_cardError_t carderror = getCardExpiryDate(cardData);
    EN_terminalError_t termerror = getTransactionDate(termData);

    if(carderror == OK && termerror == TERMINAL_OK)
    {
        cardMonth = (cardData->cardExpirationDate[0] -'0')*10 + (cardData->cardExpirationDate[1] -'0') ;
        cardYear = (cardData->cardExpirationDate[3] -'0')*10 + (cardData->cardExpirationDate[4] -'0') ;

        termMonth = (termData->transactionDate[3] -'0')*10 + (termData->transactionDate[4] -'0') ;
        termYear = (termData->transactionDate[8] -'0')*10 + (termData->transactionDate[9] -'0') ;
        if (termYear < cardYear)
        {
            return TERMINAL_OK;
        }else if(termYear == cardYear)
        {
            if (termMonth < cardMonth)
            {
                return TERMINAL_OK;
            }else
                {return EXPIRED_CARD;}

        }
        else
            {return EXPIRED_CARD;}
    }
    else
    {
        return WRONG_DATE;
    }
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	if(termData->transAmount >0)
	{
		return TERMINAL_OK;
	}else
	{
		return INVALID_AMOUNT;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{

    if (termData->maxTransAmount >= termData->transAmount)
    {
        return TERMINAL_OK;
    }
    else
        return EXCEED_MAX_AMOUNT;


}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	if (termData->maxTransAmount > 0)
	{
		return TERMINAL_OK;
	}
	else
		return INVALID_MAX_AMOUNT;
}

void getTransactionDateTest(void)
{
    ST_terminalData_t testTerminal;
    printf("test Function: getTransactionDate \n\n");
	/* input <10 char*/
	strcpy(testTerminal.transactionDate, "25/24/20");
	printf("test case 1: \n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));

    /* input >10 char*/
	strcpy(testTerminal.transactionDate, "025/024/2022");
	printf("test case 2: input >10 char\n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));

    /* input char[2] != '/' */
	strcpy(testTerminal.transactionDate, "025/4/2022");
	printf("test case 3: input char[2] != '/'\n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));

	    /* input char[5] != '/' */
	strcpy(testTerminal.transactionDate, "02/004/2022");
	printf("test case 4: input char[5] != '/'\n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));


	 /* months input 32 day */
	strcpy(testTerminal.transactionDate, "32/01/2022");
	printf("test case 5: months input 32 day\n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));

	 /* months with 30 days input 31*/
	strcpy(testTerminal.transactionDate, "31/04/2022");
	printf("test case 6: months with 30 days input 31\n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));

	/* February input 30*/
	strcpy(testTerminal.transactionDate, "30/02/2022");
	printf("test case 7: February input 30\n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));

	/* February input 29 in normal year*/
	strcpy(testTerminal.transactionDate, "29/02/2022");
	printf("test case 8: February input 29 in normal year\n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));

	/* February input 29 in leap year*/
	strcpy(testTerminal.transactionDate, "29/02/2020");
	printf("test case 9: February input 29 in leap year\n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",TERMINAL_OK);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));

    /* right format */
	strcpy(testTerminal.transactionDate, "30/04/2022");
	printf("test case 10: right format\n");
	printf("input data: %s\n",testTerminal.transactionDate);
	printf("expected result: %d\n",TERMINAL_OK);
	printf("Actual result: %d\n\n",getTransactionDate(&testTerminal));

}

void isCardExpiredTest(void)
{
    ST_terminalData_t testTerminal;
    ST_cardData_t testCard;

    printf("test Function: isCardExpired \n\n");
	/* wrong transaction date format*/
	strcpy(testTerminal.transactionDate, "25/24/20");
	strcpy(testCard.cardExpirationDate, "25/24");
	printf("test case 1: wrong transaction date format\n");
	printf("input data: %s , %s\n",testCard.cardExpirationDate, testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",isCardExpired(&testCard,&testTerminal));

	/* wrong card Exp. date format*/
	strcpy(testTerminal.transactionDate, "25/24/20");
	strcpy(testCard.cardExpirationDate, "25/2004");
	printf("test case 2: wrong card Exp. date format\n");
	printf("input data: %s , %s\n",testCard.cardExpirationDate, testTerminal.transactionDate);
	printf("expected result: %d\n",WRONG_DATE);
	printf("Actual result: %d\n\n",isCardExpired(&testCard,&testTerminal));

    /* transaction date > card Exp. date */
	strcpy(testTerminal.transactionDate, "25/04/2022");
	strcpy(testCard.cardExpirationDate, "12/20");
	printf("test case 3: transaction date > card Exp. date\n");
	printf("input data: %s , %s\n",testCard.cardExpirationDate, testTerminal.transactionDate);
	printf("expected result: %d\n",EXPIRED_CARD);
	printf("Actual result: %d\n\n",isCardExpired(&testCard,&testTerminal));

	/* transaction date = card Exp. date */
	strcpy(testTerminal.transactionDate, "25/04/2022");
	strcpy(testCard.cardExpirationDate, "04/22");
	printf("test case 4: transaction date = card Exp. date\n");
	printf("input data: %s , %s\n",testCard.cardExpirationDate, testTerminal.transactionDate);
	printf("expected result: %d\n",EXPIRED_CARD);
	printf("Actual result: %d\n\n",isCardExpired(&testCard,&testTerminal));


    /* transaction date < card Exp. date */
	strcpy(testTerminal.transactionDate, "25/04/2022");
	strcpy(testCard.cardExpirationDate, "12/24");
	printf("test case 5: months input 32 day\n");
	printf("input data: %s , %s\n",testCard.cardExpirationDate, testTerminal.transactionDate);
	printf("expected result: %d\n",TERMINAL_OK);
	printf("Actual result: %d\n\n",isCardExpired(&testCard,&testTerminal));

}

void getTransactionAmountTest(void)
{
    ST_terminalData_t testTerminal;
    printf("test Function: getTransactionAmountTest \n\n");

	/* amount < 0 */
    testTerminal.transAmount = -500.0;
	printf("test case 1: amount < 0 \n");
	printf("input data: %f\n",testTerminal.transAmount);
	printf("expected result: %d\n",INVALID_AMOUNT);
	printf("Actual result: %d\n\n",getTransactionAmount(&testTerminal));

    /* amount = 0 */
    testTerminal.transAmount = 0.0;
	printf("test case 2: amount = 0 \n");
	printf("input data: %f\n",testTerminal.transAmount);
	printf("expected result: %d\n",INVALID_AMOUNT);
	printf("Actual result: %d\n\n",getTransactionAmount(&testTerminal));

    /* amount > 0  */
    testTerminal.transAmount = 500.0;
	printf("test case 3: amount > 0 \n");
	printf("input data: %f\n",testTerminal.transAmount);
	printf("expected result: %d\n",TERMINAL_OK);
	printf("Actual result: %d\n\n",getTransactionAmount(&testTerminal));
}

void isBelowMaxAmountTest(void)
{
    ST_terminalData_t testTerminal;
    printf("test Function: isBelowMaxAmount \n\n");
    testTerminal.maxTransAmount = 500.0;
    printf("max: %f\n",testTerminal.maxTransAmount);
	/* amount < max */
    testTerminal.transAmount = 499.5;
	printf("test case 1: amount < max \n");
	printf("input data: %f\n",testTerminal.transAmount);
	printf("expected result: %d\n",TERMINAL_OK);
	printf("Actual result: %d\n\n",isBelowMaxAmount(&testTerminal));

    /* amount = max */
    testTerminal.transAmount = 500.0;
	printf("test case 2: amount = max \n");
	printf("input data: %f\n",testTerminal.transAmount);
	printf("expected result: %d\n",TERMINAL_OK);
	printf("Actual result: %d\n\n",isBelowMaxAmount(&testTerminal));

    /* amount > max  */
    testTerminal.transAmount = 500.5;
	printf("test case 3: amount > 0 \n");
	printf("input data: %f\n",testTerminal.transAmount);
	printf("expected result: %d\n",EXCEED_MAX_AMOUNT);
	printf("Actual result: %d\n\n",isBelowMaxAmount(&testTerminal));
}

void setMaxAmountTest(void)
{
ST_terminalData_t testTerminal;
    printf("test Function: setMaxAmount \n\n");

	/* amount < 0 */
    testTerminal.maxTransAmount = -500.0;
	printf("test case 1: amount < 0 \n");
	printf("input data: %f\n",testTerminal.maxTransAmount);
	printf("expected result: %d\n",INVALID_MAX_AMOUNT);
	printf("Actual result: %d\n\n",setMaxAmount(&testTerminal));

    /* amount = 0 */
    testTerminal.maxTransAmount = 0.0;
	printf("test case 2: amount = 0 \n");
	printf("input data: %f\n",testTerminal.maxTransAmount);
	printf("expected result: %d\n",INVALID_MAX_AMOUNT);
	printf("Actual result: %d\n\n",setMaxAmount(&testTerminal));

    /* amount > 0  */
    testTerminal.maxTransAmount = 500.0;
	printf("test case 3: amount > 0 \n");
	printf("input data: %f\n",testTerminal.maxTransAmount);
	printf("expected result: %d\n",TERMINAL_OK);
	printf("Actual result: %d\n\n",setMaxAmount(&testTerminal));
}
