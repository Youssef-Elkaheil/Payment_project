#include "application.h"
#include "../server/server.h"
#include<stdio.h>





//ST_cardData_t customer_card = { "ahmed maged alia ","1234567890qweert","5/12" };
ST_transaction_t transaction_data = { {"ahmed maged alia ","1234567890qweert","5/12"},{300,30000,"15/7/2022"},0,0 };
void appStart(void)
{
	ST_cardData_t customer_card;
	ST_cardData_t* ptr_customer_card = &customer_card;
	ST_terminalData_t customer_terminal;
	ST_terminalData_t* ptr_customer_terminal = &customer_terminal;
	EN_terminalError_t termerror;
	EN_cardError_t carderror;
	printf("ENTER Holder name:  ");
	fgets(ptr_customer_card->cardHolderName, sizeof(ptr_customer_card->cardHolderName),stdin);
	carderror = getCardHolderName(ptr_customer_card);
	if (carderror == OK)
	{
		printf("valid name\nENTER Card expiratory date:  "); 
		fgets(ptr_customer_card->cardExpirationDate, sizeof(ptr_customer_card->cardExpirationDate), stdin);
		carderror = getCardExpiryDate(ptr_customer_card);
		if (carderror == OK)
		{
			printf("valid Card expiratory date\nENTER primary account number:  ");
			fgets(ptr_customer_card->primaryAccountNumber, sizeof(ptr_customer_card->primaryAccountNumber), stdin);
			carderror = getCardPAN(ptr_customer_card);
			if (carderror == OK)
			{
				printf("ENTER transaction date in format  DD/MM/YYYY, e.g 25/06/2022.:  ");
				fgets(ptr_customer_terminal->transactionDate, sizeof(ptr_customer_terminal->transactionDate), stdin);
				termerror = getTransactionDate(ptr_customer_terminal);
				if (termerror == OK_TER)
				{
					termerror = isCardExpired(customer_card, customer_terminal);
					if (termerror == EXPIRED_CARD)
					{
						printf("EXPIRED_CARD");
					}
					else if (termerror == OK_TER)
					{

					}
				}
				
				//for (int i = 0; i < 255; i++)
				//{
					//if(ptr_customer_card->primaryAccountNumber== transaction_database[i].c)
				//}
			}


		}
	}
	
	


	
}