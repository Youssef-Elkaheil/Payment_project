
#include "card.h"
#include <string.h>


EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	if (strlen(cardData->cardHolderName) <= 24 && strlen(cardData->cardHolderName) >= 20)
	{
		return OK;
	}
	else
		return WRONG_NAME;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	char month, year;
	if(cardData->cardExpirationDate[2] == '/')
	{
		switch (strlen(cardData->cardExpirationDate))
		{
		case 5: /* format "MM/YY" */
				/* convert string into number*/
				month = (cardData->cardExpirationDate[0] -'0')*10 + (cardData->cardExpirationDate[1] -'0') ;
				if(month<13 && month>0)
				{
					return OK;
				}else
				{
					return WRONG_EXP_DATE;
				}
			break;

		default:
			return WRONG_EXP_DATE;
			break;
		}
	}else
	{
		return WRONG_EXP_DATE;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	if (strlen(cardData->primaryAccountNumber) < 20 && strlen(cardData->primaryAccountNumber) > 15)
	{
		return OK;
	}
	else
		return WRONG_PAN;
}


#include <stdio.h>

void getCardHolderNametest(void)
{
	ST_cardData_t card;
	printf("test Function: getCardHolderName \n\n");
	/* input <20 char*/
	strcpy(card.cardHolderName, "ayamahmoudsalama");
	printf("test case 1: \n");
	printf("input data: %d char\n",strlen(card.cardHolderName));
	printf("expected result: %d\n",WRONG_NAME);
	printf("Actual result: %d\n\n",getCardHolderName(&card));

	/* input =20 char*/
	strcpy(card.cardHolderName, "aya mahmoud salama12");
	printf("test case 2: \n");
	printf("input data: %d char\n",strlen(card.cardHolderName));
	printf("expected result: %d\n",OK);
	printf("Actual result: %d\n\n",getCardHolderName(&card));

	/* 20< input <24 char*/
	strcpy(card.cardHolderName, "aya mahmoud salama1234");
	printf("test case 3: \n");
	printf("input data: %d char\n",strlen(card.cardHolderName));
	printf("expected result: %d\n",OK);
	printf("Actual result: %d\n\n",getCardHolderName(&card));

	/*input =24 char*/
	strcpy(card.cardHolderName, "aya mahmoud salama123456");
	printf("test case 4: \n");
	printf("input data: %d char\n",strlen(card.cardHolderName));
	printf("expected result: %d\n",OK);
	printf("Actual result: %d\n\n",getCardHolderName(&card));

	/*input >24 char*/
	strcpy(card.cardHolderName, "aya mahmoud salama 12345678");
	printf("test case 5: \n");
	printf("input data: %d char\n",strlen(card.cardHolderName));
	printf("expected result: %d\n",WRONG_NAME);
	printf("Actual result: %d\n\n",getCardHolderName(&card));
}

void getCardExpiryDatetest(void)
{
	ST_cardData_t card;
	printf("test Function: getCardExpiryDate \n\n");
	/* input <5 char*/
	strcpy(card.cardExpirationDate, "12/5");
	printf("test case 1: \n");
	printf("input data: %s\n",card.cardExpirationDate);
	printf("expected result: %d\n",WRONG_EXP_DATE);
	printf("Actual result: %d\n\n",getCardExpiryDate(&card));

	/* input >5 char*/
	strcpy(card.cardExpirationDate, "009/025");
	printf("test case 2: \n");
	printf("input data: %s\n",card.cardExpirationDate);
	printf("expected result: %d\n",WRONG_EXP_DATE);
	printf("Actual result: %d\n\n",getCardExpiryDate(&card));

	/* cardExpirationDate[2] != '/' */
	strcpy(card.cardExpirationDate, "9/025");
	printf("test case 3: \n");
	printf("input data: %s\n",card.cardExpirationDate);
	printf("expected result: %d\n",WRONG_EXP_DATE);
	printf("Actual result: %d\n\n",getCardExpiryDate(&card));

	/*months = 12 */
	strcpy(card.cardExpirationDate, "12/24");
	printf("test case 4: \n");
	printf("input data: %s\n",card.cardExpirationDate);
	printf("expected result: %d\n",OK);
	printf("Actual result: %d\n\n",getCardExpiryDate(&card));

	/*months > 12 */
	strcpy(card.cardExpirationDate, "30/24");
	printf("test case 5: \n");
	printf("input data: %s\n",card.cardExpirationDate);
	printf("expected result: %d\n",WRONG_EXP_DATE);
	printf("Actual result: %d\n\n",getCardExpiryDate(&card));

	/*month = 00*/
	strcpy(card.cardExpirationDate, "00/24");
	printf("test case 6: \n");
	printf("input data: %s\n",card.cardExpirationDate);
	printf("expected result: %d\n",WRONG_EXP_DATE);
	printf("Actual result: %d\n\n",getCardExpiryDate(&card));

}

void getCardPANtest(void)
{
		ST_cardData_t card;
    printf("test Function: getCardPAN \n\n");
	/* input <16 char*/
	strcpy(card.primaryAccountNumber, "0123456789");
	printf("test case 1: \n");
	printf("input data: %d char\n",strlen(card.primaryAccountNumber));
	printf("expected result: %d\n",WRONG_PAN);
	printf("Actual result: %d\n\n",getCardPAN(&card));

	/* input = 16 char*/
	strcpy(card.primaryAccountNumber, "0123456789____16");
	printf("test case 2: \n");
	printf("input data: %d char\n",strlen(card.primaryAccountNumber));
	printf("expected result: %d\n",OK);
	printf("Actual result: %d\n\n",getCardPAN(&card));

	/* 16< input <19 char*/
	strcpy(card.primaryAccountNumber, "0123456789_____17");
	printf("test case 3: \n");
	printf("input data: %d char\n",strlen(card.primaryAccountNumber));
	printf("expected result: %d\n",OK);
	printf("Actual result: %d\n\n",getCardPAN(&card));

	/*input =19 char*/
	strcpy(card.primaryAccountNumber, "0123456789_______19");
	printf("test case 4: \n");
	printf("input data: %d char\n",strlen(card.primaryAccountNumber));
	printf("expected result: %d\n",OK);
	printf("Actual result: %d\n\n",getCardPAN(&card));

	/*input >19 char*/
	strcpy(card.primaryAccountNumber, "013245679_____________24");
	printf("test case 5: \n");
	printf("input data: %d char\n",strlen(card.primaryAccountNumber));
	printf("expected result: %d\n",WRONG_PAN);
	printf("Actual result: %d\n\n",getCardPAN(&card));
}
