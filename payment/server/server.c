#include <stdio.h>
#include <string.h>
#include "../card/card.h"
#include "../terminal/terminal.h"
#include "server.h"

ST_accountsDB_t account_database[255] = {{1000.0, RUNNING, "0132456789____01"}, {2000.0, BLOCKED, "0132456789____02"},
										 {3000.0, RUNNING, "0132456789____03"}, {4000.0, BLOCKED, "0132456789____04"},
										 {5000.0, RUNNING, "0132456789____05"}, {6000.0, BLOCKED, "0132456789____06"},
										 {7000.0, RUNNING, "0132456789____07"}, {8000.0, BLOCKED, "0132456789____08"},
									     {9000.0, RUNNING, "0132456789____09"},{10000.0, BLOCKED, "0132456789____010"}
										};

ST_transaction_t transaction_database[255] = {0};

static unsigned long int Global_SequanceNumber = 0;


EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    ST_accountsDB_t *accountRefrence ;
    EN_serverError_t serverError = isValidAccount(&transData->cardHolderData,&accountRefrence);
	if(serverError == ACCOUNT_NOT_FOUND)
	{
		transData->transState = FRAUD_CARD;
		return FRAUD_CARD;

	}
		printf("%s \n",accountRefrence->primaryAccountNumber);

	if(isBlockedAccount(accountRefrence) == BLOCKED_ACCOUNT)
	{
		transData->transState =  DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}

	if(isAmountAvailable(&transData->terminalData,accountRefrence) == LOW_BALANCE)
	{
		transData->transState =  DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}

    /* set max amount globally by a macro */
    transData->terminalData.maxTransAmount = MAX_AMOUNT;
    if(setMaxAmount(&transData->terminalData) == INVALID_MAX_AMOUNT)
    {
        return INTERNAL_SERVER_ERROR;
    }

	if(saveTransaction(transData) == SERVER_OK)
	{
		transData->transState =  APPROVED;
		accountRefrence->balance = accountRefrence->balance - transData->terminalData.transAmount;
		return APPROVED;
	}

    transData->transState =  INTERNAL_SERVER_ERROR;

    return INTERNAL_SERVER_ERROR;

}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t **accountRefrence)
{
	/*search for the cardPAN*/
	for (int i = 0; i < 10; i++)
	{
		if(strcmp(cardData->primaryAccountNumber, account_database[i].primaryAccountNumber) == 0)
		{
			*accountRefrence =  &account_database[i];
			return SERVER_OK;
		}

	}
	accountRefrence = 0;
	return ACCOUNT_NOT_FOUND ;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
	if(accountRefrence->state == BLOCKED)
	{
		return BLOCKED_ACCOUNT;
	}
	else if(accountRefrence->state == RUNNING)
	{
		return SERVER_OK;
	}
	else
	{
		return BLOCKED_ACCOUNT;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
	if(termData->transAmount <= accountRefrence->balance && termData->transAmount <= termData->maxTransAmount)
	{
		return SERVER_OK;

	}
	else
	{
		return LOW_BALANCE;
	}
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
	Global_SequanceNumber++;

		if(Global_SequanceNumber <255)
		{
			transaction_database[Global_SequanceNumber] = *transData;
			transaction_database[Global_SequanceNumber].transactionSequenceNumber = Global_SequanceNumber;
			return SERVER_OK;
		}

	/* database full */
	return SAVING_FAILED;
}

void listSavedTransactions(void)
{
    if(Global_SequanceNumber ==0)
    {
        printf("List Empty\n\n");
        return;
    }
	for (int i = 0; i < Global_SequanceNumber; i++)
	{
		printf("#########################\n");
		printf("Transaction Sequence Number: %d\n", transaction_database[i].transactionSequenceNumber);
		printf("Transaction Date: %s\n", transaction_database[i].terminalData.transactionDate);
		printf("Transaction Amount: %f\n", transaction_database[i].terminalData.transAmount);
		printf("Transaction State: %d \n", transaction_database[i].transState);
		printf("Terminal Max Amount: %f\n", transaction_database[i].terminalData.maxTransAmount);
		printf("Card holder Name: %s\n", transaction_database[i].cardHolderData.cardHolderName);
		printf("PAN:\n %s",transaction_database[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s\n",transaction_database[i].cardHolderData.cardExpirationDate);
		printf("#########################\n\n");

	}
}


void recieveTransactionDataTest(void)
{

    ST_transaction_t testTransaction;
    printf("test Function: recieveTransactionData \n\n");

	/* FRAUD_CARD */

    strcpy(testTransaction.cardHolderData.primaryAccountNumber, "0132456789____001");
	printf("test case 1: account not found\n");
	printf("input data: %s\n",testTransaction.cardHolderData.primaryAccountNumber);
	printf("expected result: %d\n",FRAUD_CARD);
	printf("Actual result: %d\n\n",recieveTransactionData(&testTransaction));

    /* DECLINED_STOLEN_CARD */
	strcpy(testTransaction.cardHolderData.primaryAccountNumber, "0132456789____02");
	printf("test case 2: DECLINED_STOLEN_CARD\n");
	printf("input data: %s\n",testTransaction.cardHolderData.primaryAccountNumber);
	printf("expected result: %d\n",DECLINED_STOLEN_CARD);
	printf("Actual result: %d\n\n",recieveTransactionData(&testTransaction));

    /* DECLINED_INSUFFECIENT_FUND */
	strcpy(testTransaction.cardHolderData.primaryAccountNumber, "0132456789____01");
	printf("test case 3: DECLINED_INSUFFECIENT_FUND\n");
	printf("input data: %s\n",testTransaction.cardHolderData.primaryAccountNumber);
	printf("expected result: %d\n",DECLINED_INSUFFECIENT_FUND);
	printf("Actual result: %d\n\n",recieveTransactionData(&testTransaction));

	    /* INTERNAL_SERVER_ERROR*/
	strcpy(testTransaction.cardHolderData.primaryAccountNumber, "0132456789____01");
	printf("test case 4: INTERNAL_SERVER_ERROR\n");
	printf("input data: %s\n",testTransaction.cardHolderData.primaryAccountNumber);
	printf("expected result: %d\n",INTERNAL_SERVER_ERROR);
	printf("Actual result: %d\n\n",recieveTransactionData(&testTransaction));


	 /* APPROVED */
	strcpy(testTransaction.cardHolderData.primaryAccountNumber, "0132456789____01");
	printf("test case 5: APPROVED\n");
	printf("input data: %s\n",testTransaction.cardHolderData.primaryAccountNumber);
	printf("expected result: %d\n",APPROVED);
	printf("Actual result: %d\n\n",recieveTransactionData(&testTransaction));

}
