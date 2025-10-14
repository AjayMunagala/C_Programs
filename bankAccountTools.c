void pause()
{
	system("pause");
}

void clear()
{
	system("cls");
}

void underline(char *text)
{
	int length = printf("%s", text);
	printf("\n");
	for (int counter = 0; counter <= length; counter++) 
	{
		printf("_");
	}
	printf("\n");
}


void readText (char *text, int size)
{
	fgets(text, size, stdin);
	text[strcspn(text, "\n")] = 0;
	
}

void readNumber(float *number)
{
	scanf("%f", number);	
}

