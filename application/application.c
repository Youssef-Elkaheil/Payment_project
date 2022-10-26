#include<stdio.h>
#include "../card/card.h"
#include "../terminal/terminal.h"
#include "../server/server.h"
#include "application.h"

static void printTransactionState(EN_transState_t state)
{
	switch (state)
	{
	case APPROVED:
		printf("APPROVED\n");
		break;

		case DECLINED_INSUFFECIENT_FUND:
		printf("\n");
		break;

		case DECLINED_STOLEN_CARD:
		printf("DECLINED_INSUFFECIENT_FUND\n");
		break;

		case FRAUD_CARD:
		printf("\n");
		break;

		case INTERNAL_SERVER_ERROR:
		printf("FRAUD_CARD\n");
		break;


	default:
		printf("Undefined States\n");
		break;
	}
}

void appStart(void)
{
	ST_transaction_t newTransaction;
	ST_accountsDB_t accountrefrance;
	EN_terminalError_t termerror;
	EN_cardError_t carderror;
	EN_transState_t transState;
	while (1)
	{	
		printf("ENTER Holder name:  ");
		scanf("%s",newTransaction.cardHolderData.cardHolderName);
		carderror = getCardHolderName(&newTransaction.cardHolderData);
		if (carderror == OK)
		{
			printf("valid name\nENTER Card Expiration date:  "); 
			scanf("%s",newTransaction.cardHolderData.cardExpirationDate);
			carderror = getCardExpiryDate(&newTransaction.cardHolderData);
			if (carderror == OK)
			{
				printf("valid Card Expiration date\nENTER primary account number:  ");
				scanf("%s",newTransaction.cardHolderData.primaryAccountNumber);
				carderror = getCardPAN(&newTransaction.cardHolderData);
				if (carderror == OK)
				{
					printf("ENTER transaction date in format  DD/MM/YYYY, e.g 25/06/2022.:  ");
					scanf("%s",newTransaction.terminalData.transactionDate);
					termerror = getTransactionDate(&newTransaction.terminalData);
					if (termerror == TERMINAL_OK)
					{
						termerror = isCardExpired(&newTransaction.cardHolderData, &newTransaction.terminalData);
						if (termerror == EXPIRED_CARD)
						{
							printf("EXPIRED_CARD");
							continue;
						}
						else if (termerror == TERMINAL_OK)
						{
							transState = recieveTransactionData(&newTransaction);
							printTransactionState(transState);
						}else
						{
							printf("EXCEED_MAX_AMOUNT");
							continue;
						}
					}
				}
			}
		}
	}
}

