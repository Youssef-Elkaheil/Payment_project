#include "server.h"
#include "string.h"
#include "../card/card.h"

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
	ST_accountsDB_t *accountRefrence;

	if(isValidAccount(transData->cardHolderData,accountRefrence) == ACCOUNT_NOT_FOUND)
	{
		return FRAUD_CARD;
	}

	if(isBlockedAccount(transData->cardHolderData,accountRefrence) == BLOCKED_ACCOUNT)
	{
		return DECLINED_STOLEN_CARD;
	}

	if(isAmountAvailable(transData->cardHolderData,accountRefrence) == LOW_BALANCE)
	{
		return DECLINED_INSUFFECIENT_FUND;
	}

	if(saveTransaction(transData) == SAVING_FAILED)
	{
		return INTERNAL_SERVER_ERROR
	}
	else if(saveTransaction(transData) == SERVER_OK)
	{	
		return APPROVED;
	}
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
	/*search for the cardPAN*/
	for (int i = 0; i < 255; i++)
	{
		if(strcmp(cardData->primaryAccountNumber, account_database[i].primaryAccountNumber) == 0)
		{
			accountRefrence = & account_database[i];
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
	if(termData->transAmount <= accountRefrence->balance)
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
	for (int i = 0; i <= Global_SequanceNumber; i++)
	{
		printf("#########################\n");
		printf("Transaction Sequence Number: %d\n", transaction_database[i].transactionSequenceNumber);
		printf("Transaction Date: %s\n", transaction_database[i].terminalData.transactionDate);
		printf("Transaction Amount: %f\n", transaction_database[i].terminalData.transAmount);
		printf("Transaction State: %d \n", transaction_database[i].transState);
		printf("Terminal Max Amount: %f\n", transaction_database[i].terminalData.maxTransAmount);
		printf("Cardholder Name: %s\n", transaction_database[i].cardHolderData.cardHolderName);
		printf("PAN:\n %s",transaction_database[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s\n",transaction_database[i].cardHolderData.cardExpirationDate);
		printf("#########################\n");
		
	}
	
}
