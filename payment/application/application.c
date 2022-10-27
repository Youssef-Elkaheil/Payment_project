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
		printf("DECLINED_INSUFFECIENT_FUND\n");
		break;

		case DECLINED_STOLEN_CARD:
		printf("DECLINED_INSUFFECIENT_FUND\n");
		break;

		case FRAUD_CARD:
		printf("FRAUD_CARD\n");
		break;

		case INTERNAL_SERVER_ERROR:
		printf("INTERNAL_SERVER_ERROR\n");
		break;


	default:
		printf("Undefined States\n");
		break;
	}
}

void appStart(void)
{
	ST_transaction_t newTransaction;
	EN_terminalError_t termerror;
	EN_cardError_t carderror;
	EN_transState_t transState;
    char option;
	while (1)
	{
	    printf("Options:\n");
	    printf("1- Add transaction.\n");
	    printf("2- Print all transactions.\n");
	    printf("Enter your choice :");
	    scanf("%s",&option);
	    printf("\n");
	    if(option == '1')
        {
            printf("ENTER Holder name:  ");
            scanf("%s",newTransaction.cardHolderData.cardHolderName);
            carderror = getCardHolderName(&newTransaction.cardHolderData);
            if (carderror == OK)
            {
                printf("ENTER Card Expiration date:  ");
                scanf("%s",newTransaction.cardHolderData.cardExpirationDate);
                carderror = getCardExpiryDate(&newTransaction.cardHolderData);
                if (carderror == OK)
                {
                    printf("ENTER primary account number:  ");
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
                            if (termerror == TERMINAL_OK)
                            {
                                printf("ENTER transaction amount:  ");
                                scanf("%f",&newTransaction.terminalData.transAmount);
                                transState = recieveTransactionData(&newTransaction);
                                printTransactionState(transState);
                            }else
                            {
                                printf("EXPIRED_CARD\n");

                            }
                        }else
                        {
                            printf("Wrong Date format\n\n");
                        }
                    }else
                    {
                        printf("Wring PAN\n\n");
                    }
                }else
                {
                    printf("Wrong Expiration Date format\n\n");
                }
            }else
            {
                printf("Wrong Name format\n\n");
            }
        }
        else if( option == '2')
            {listSavedTransactions();}
        else
            {printf("Invalid option\n\n");}
	}
}

