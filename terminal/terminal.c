#include "terminal.h"
#include <string.h>


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	char day,month;
	int year;
	/* format DD/MM/YYY   */
	
	if (strlen(termData->transactionDate) == 10 && (termData->transactionDate[2] == termData->transactionDate[5] == '/'))
	{
		day = (termData->transactionDate[0] -'0')*10 + (termData->transactionDate[1] -'0') ;
		month = (termData->transactionDate[3] -'0')*10 + (termData->transactionDate[4] -'0') ;
		year =    (termData->transactionDate[6] -'0')*1000  + (termData->transactionDate[7] -'0')*100
				+ (termData->transactionDate[8] -'0')*10 + (termData->transactionDate[9] -'0');

		/* assume no transaction were before 2000 */
		/* */
		if (year > 2000 && year< 2022)
		{
			/*months with 31 days*/
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
	}	
	else 
	{
		return WRONG_DATE;
	}
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t *termData)
{
	char cardMonth ,cardYear, termMonth, termYear;
	
	cardMonth = (cardData->cardExpirationDate[0] -'0')*10 + (cardData->cardExpirationDate[1] -'0') ;
	cardYear = (cardData->cardExpirationDate[3] -'0')*10 + (cardData->cardExpirationDate[4] -'0') ;
	
	termMonth = (termData->transactionDate[3] -'0')*10 + (termData->transactionDate[4] -'0') ;
	termYear = (termData->transactionDate[8] -'0')*10 + (termData->transactionDate[9] -'0') ;

	if (termYear <= cardYear)
	{
		if (termMonth < cardMonth)
		{
			return TERMINAL_OK;
		}else{return EXPIRED_CARD;}
	}
	else
		{return EXPIRED_CARD;}
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