#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include "bankAccountTools.c"

#define DATAFILE "accountsData.dat"
#define ACCOUNTNUMBERLENGTH 15
#define NAMELENGTH 20


typedef struct 
{
	char accountNumber[ACCOUNTNUMBERLENGTH];
	char fullName[NAMELENGTH];
	float balance;
} Bank;

typedef struct node
{
 	Bank account;
 	struct node *next;

} Node;


void displayMenu();
void create();
void show();
void update(Node*);
void delete(Node*);
void readAccountDetails(Bank*, int);
void addAccountsToList(Bank);
void pointCurrentToNull();
void loadAccountsToList();
int checkFileOpen(FILE*);
int checkListEmpty();
Node *searchRecord();

Node *current = NULL;
Node *head = NULL;
FILE *fpaccounts;

int main()
{
	loadAccountsToList();
	displayMenu();
}

void displayMenu()
{
	char choice;
	do
	{
		clear();
		underline("APGB");
		printf("\n1.Create\n"
			"2.Show\n"
			"3.Update\n"
			"4.Delete\n"
			"5.Exit\n"
			"==================\n"
			"Enter your choice: ");
		scanf(" %c", &choice);
		getchar();

		switch(choice)
		{
		case '1': create(); break;
		case '2': show();   break;
		case '3': 
			{
				Node *recordToUpdate = searchRecord();
    			update(recordToUpdate);
				break;
			}
			
		case '4': 
			{
				Node *recordToDelete = searchRecord();
    			delete(recordToDelete);
				break;
			}
			
		case '5': exit(0);
		default : printf("please enter a avalid choice..!");
		}
		pause();
	}while (choice != '5');
}

void readAccountDetails(Bank *account, int readAccountNumber)
{
	if (readAccountNumber)
	{
		printf("Enter account number: ");
		readText(account -> accountNumber, ACCOUNTNUMBERLENGTH);	
	}

	printf("Enter full name: ");
	readText(account -> fullName, NAMELENGTH);

	printf("Enter balance: ");
	readNumber(&account -> balance);

}

int checkFileOpen(FILE *filePointer)
{
    if (!filePointer)
    {
        printf("File with name: %s not found..! \n", DATAFILE);
        pause();
        return 0; 
    }
    return 1;  
}


void loadAccountsToList()
{
	Bank account;
	fpaccounts = fopen(DATAFILE, "rb");
	if (!checkFileOpen(fpaccounts))
	{
		return;	
	} 
	while (fread(&account, sizeof(Bank), 1 , fpaccounts) == 1)
	{
		Node *newNode = malloc(sizeof(Node));
		newNode -> account = account;
		newNode -> next = NULL;
		if (head == NULL)
		{
			head = newNode;
		}
		else
		{
			current -> next = newNode;
		}
		current = newNode;
	}
	fclose(fpaccounts);

}

void saveAccountsToDataBase()
{
	fpaccounts = fopen(DATAFILE, "wb");
	if (!checkFileOpen(fpaccounts))
	{
		return;	
	} 
	current = head;
	if (current == NULL)
	{
		printf("No Records Found..!");
	}
	else
	{
		while (current != NULL)
		{
			fwrite (&current -> account, sizeof(Bank), 1, fpaccounts);
			current = current -> next;
		}
	}
	fclose(fpaccounts);
}

void appendAccountsToList(Bank paccount)
{
	Node *newNode = malloc(sizeof(Node));
	newNode -> next = NULL;
	newNode -> account = paccount;
	if (head == NULL)
    {
        head = newNode;
    }
    else
    {
		pointCurrentToNull();
		current -> next = newNode;
	}
	current = newNode;
}

void pointCurrentToNull()
{
	current = head;
	if (current == NULL)
	{
		return;
	}
	while (current -> next != NULL)
	{
		current = current -> next;
	}
}

void create()
{
	Bank account;
	readAccountDetails(&account, 1);
	appendAccountsToList(account);
	saveAccountsToDataBase();
}

void printRecord(Bank *account)
{
	printf("Account Number: %s\n"
		"Full Name: %s\n"
		"Balance: %.2f\n"
		"================\n",
		account -> accountNumber, account -> fullName, account -> balance);
		
}

int checkListEmpty()
{
    if (head == NULL)
    {
        printf("Data file is Empty..!\n");
        return 1;
    }
    return 0;
}

void show()
{
	if (checkListEmpty())
	{
		return;
	}
	current = head;
	while (current != NULL)
	{
		printRecord(&(current -> account));
		current = current -> next;
			
	}	
}

Node* searchRecord()
{
	char accountId[ACCOUNTNUMBERLENGTH];
	printf("Enter accountId: ");
	readText(accountId, ACCOUNTNUMBERLENGTH);
	printf("\n");
	if (checkListEmpty())
	{
		return NULL;
	}
	current = head;
	while (current != NULL)
	{
		if (strcmp(current -> account.accountNumber, accountId) == 0)
		{
			return current;
		}
		current = current -> next;
	}
	return NULL;

}


void update(Node *recordToUpdate)
{
	//Node *recordToUpdate  = searchRecord();
	if (recordToUpdate  != NULL)
	{
		underline("Current Details:");
		printRecord(&(recordToUpdate  -> account));
		underline("\nEnter updated Details:");
		readAccountDetails(&(recordToUpdate  -> account), 0);
		saveAccountsToDataBase();
		printf("Account updated Successfully..!\n");
	}
	else
	{
		printf("No record found with the given account number.\n");
	}
	
}
	

void delete(Node *recordToDelete)
{
	//Node *recordDelete  = searchRecord();
	Node **temporaryNode = &head;
	if (recordToDelete == NULL)
	{
		printf("No record found.\n");
		return;
	}

	char choice;
    printf("Are you sure you want to delete this account? (Y/N): ");
    scanf(" %c", &choice); 
    if (choice != 'Y' && choice != 'y')
    {
        printf("Deletion cancelled.\n");
        pause();
        return;
    }
    while (*temporaryNode != NULL && *temporaryNode != recordToDelete)
    {
        temporaryNode = &((*temporaryNode)->next);
    }

    *temporaryNode = recordToDelete->next;
    free(recordToDelete);
    saveAccountsToDataBase();
    printf("Account deleted successfully.\n");
}


