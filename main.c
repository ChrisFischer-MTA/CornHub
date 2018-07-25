#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define __STRINGLENGTH__ 100
#define MAX_CORN_SUPPORTED 100


// Credit to Stack Overflow Q #1921539
typedef int bool;
#define true 1
#define false 0


// Logic. Corn structure definition and global array.
struct Corn 
{
    char color;
    int priceInCents;
    int age; // Anything over 30 days is assumed to be bad.
    int lengthInCM;
};

// It's not lost on me - this is the same word when referencing a SQL transaction. This would be bad practice if the program
// had any practical purpose. Given it's purely academic, this is permissable.
struct transaction
{
	int dayOfMonth;
	int month;
	int year;
	int priceInCents;
	int customerID;
};
struct Corn corns[MAX_CORN_SUPPORTED];
int currentCorns = 0;

/*
Operations
*/

int getChoice(int minimum, int maximum, int attempt){
	int choice;
	printf("Enter any number between %d and %d.\n", minimum, maximum);
	scanf("%d", &choice);
	if(attempt >= 5){
		printf("Error! User unable to input the correct choice. Layer 8 issue. Exiting");
		exit(1);
	}
	if(choice < minimum || choice > maximum){
		printf("Invalid choice!");
		return getChoice(minimum, maximum, attempt+1);
	}
	return choice;
}

void readFileToMem(){
	// Does our file exist?
	FILE* cornStorage;
	int piecesOfCorn;
	// an izisulu is a place to store maize on a farm.
	cornStorage = fopen("izisulu.bin", "rb");
	if(cornStorage == NULL){
		printf("Error! File does not exist!\n");
		cornStorage = fopen("izisulu.bin", "wb");
		printf("Creating, please re-preform this operation.\n");
	}else{
		fscanf(cornStorage, "%d\n", &piecesOfCorn);
	}
	currentCorns = piecesOfCorn;
	for(piecesOfCorn=0; piecesOfCorn<currentCorns; piecesOfCorn++){
		fscanf(cornStorage, "%c %d %d %d\n", &corns[piecesOfCorn].color, &corns[piecesOfCorn].priceInCents, &corns[piecesOfCorn].age, &corns[piecesOfCorn].lengthInCM);
	}
	fclose(cornStorage);
}

void writeSpecificCorn(int i, char color, int price, int age, int length){
	FILE* cornStorage;
	// an izisulu is a place to store maize on a farm.
	cornStorage = fopen("izisulu.bin", "ab");
	// fprintf(cornStorage, "%d\n", currentCorns);
	printf("(Printing) %c %d %d %d\n", corns[i].color, corns[i].priceInCents, corns[i].age, corns[i].lengthInCM);
	fprintf(cornStorage, "%c %d %d %d\n", corns[i].color, corns[i].priceInCents, corns[i].age, corns[i].lengthInCM);
	fclose(cornStorage);
}

void writeToFile(){
	int i;
	FILE* cornStorage;
	cornStorage = fopen("izisulu.bin", "wb");
	
	fprintf(cornStorage, "%d\n", currentCorns);
	fclose(cornStorage);
	
	for(i = 0; i < currentCorns; i++){
		writeSpecificCorn(i,corns[i].color, corns[i].priceInCents, corns[i].age, corns[i].lengthInCM);
	}
}

void pause(){
	system("pause");
}

/*
Drawing Related.
draw - writes a string to screen, pads with standard spacing.
draw - writes a character to screen the amount of standard spacing.
drawMiddle - writes a string to middle, pads with standard spacing on either side.
*/
void draw(char* string, int length){
	int i, j;
	// Print out each character in string before index 100.
	for(i = 0; i < length && j < __STRINGLENGTH__; i++, j++){
		printf("%c", string[i]);	
	}
	// We could accomplish this with printflags, but the printf standard can/will vary by console.
	for(i = length; i < __STRINGLENGTH__; i++){
		printf(" ");
	}	
	printf("\n");
}

void drawLetter(char string){
	int i = 0;
	for(i = 0; i < __STRINGLENGTH__; i++)
		printf("%c",string);
	printf("\n");
}

void drawMiddle(char* string, int length, char padding){
	int i;
	for(i = 0; i < (__STRINGLENGTH__/2) - (length/2); i++){
		printf("%c", padding);
	}
	printf("%s", string);
	for(i = 0; i < (__STRINGLENGTH__/2) - (length/2) && length%2==0; i++){
		printf("%c", padding);
	}
	for(i = 0; i < (__STRINGLENGTH__/2) - (length/2)-1 && length%2==1; i++){
		printf("%c", padding);
	}
	printf("\n");
}

/*
Draws and Logic.
Prints each menu and submenu, taking user feedback as appropriate.
*/

void drawInventory(){
	// Ensure reading is updated.
	readFileToMem();
	// Iterate through the stored corn.
	int i;
	for(i = 0; i < currentCorns; i++){
		printf("%08d. 	%c 		$%d.%02d 		%d cm 		%d days old.\n", i+1, corns[i].color, corns[i].priceInCents/100, corns[i].priceInCents%100, corns[i].lengthInCM, corns[i].age);
	}
	pause();
}

void addInventory(){
	struct Corn tempCorn;
	// First, let's make sure everything is current.
	readFileToMem();
	currentCorns++;
	// Are we going to surpass our maximum amount of corn?
	if(MAX_CORN_SUPPORTED <= currentCorns){
		// We've eclipsed supported storage.
		printf("(Warning) Can not add corn due to maximum corn limit reached.\n");
		pause();
		return;
	}
	// Great, now let's add our new corn.
	// Let's make a temporary corn!
	printf("Please enter the corn's color code.\n");
	scanf(" %c", &tempCorn.color);
	printf("Please enter the corn's price in cents.\n");
	scanf("%d", &tempCorn.priceInCents);
	printf("Please enter the corn's age in days.\n");
	scanf("%d", &tempCorn.age);
	printf("Please enter the corn's length in CM.\n");
	scanf("%d", &tempCorn.lengthInCM);
	corns[currentCorns-1] = tempCorn;
	pause();
	// Flush
	writeToFile();
}

void delInventory(){
	bool passed = false;
	int i = 0, choice;
	drawInventory();
	printf("Please enter the SKU# to delete.\n");
	// Use the choice function.
	choice = getChoice(1, currentCorns, 1);
	choice--;
	// We now have the index of what to delete. Let's preform a swap of it and the last one.
	if(choice!=currentCorns-1){
		corns[choice] = corns[currentCorns-1];
	}
	currentCorns--;
	writeToFile();
	
}

void modInventory(){
	drawInventory();
	printf("Enter 1 to add inventory. Enter 2 to delete inventory.\n");
	if(getChoice(1,2,1)-1){
		// Delete
		delInventory();
	}else{
		addInventory();
	}
}

void skuLookup(){
	int i, sku;
	readFileToMem();
	printf("Please enter desired SKU.\n");
	scanf("%d", &sku);
	printf("SKU    			COLOR         PRICE		AGE  	     LENGTH\n");
	for(i = 0; i < currentCorns; i++){
		if(i==sku){
			printf("%08d		%c	 	%d		 %d		 %d\n", sku, corns[i].color, corns[i].priceInCents, corns[i].age, corns[i].lengthInCM);
		}
	}
	system("pause");
}

void drawPrice(){
	int i, j;
	readFileToMem();
	/* I'll put this here.
	There's a special place in hell for me because I did it this way.
	Could I have done a merge sort or insertion sort? Sure!
	But my possible values were so small, who cares?
	This is a demonstration of creative programming.
	*/
	for(i = 0; i < INT_MAX; i++){
		for(j = 0; j < currentCorns; j++){
			if(corns[j].priceInCents == i){
				printf("%08d. 	%c 		$%d.%02d 		%d cm 		%d days old.\n", j+1, corns[j].color, corns[j].priceInCents/100, corns[j].priceInCents%100, corns[j].lengthInCM, corns[j].age);
			}
		}
	}
	system("pause");
}

void drawAge(){
	readFileToMem();
	// Insertion Sort!
	int i, j, smallest, smallestIndex;
	struct Corn sortedCorns[MAX_CORN_SUPPORTED];
	// For the purposes of my program, I'm going to copy corns into sortedCorns and play with that.
	for(i = 0; i < currentCorns; i++){
		sortedCorns[i] = corns[i];
	}
	struct Corn tempCorn;

	for(i = 0; i < currentCorns; i++){
		smallest = sortedCorns[i].age;
		smallestIndex = i;
		for(j = i; j < currentCorns; j++){
			if(sortedCorns[j].age < smallest){
				smallestIndex = j;
				smallest = sortedCorns[j].age;
			}
		}
		//so we've found the smallest.
		//swap index i and the smallest.
		if(smallestIndex!=i){
			tempCorn = sortedCorns[i];
			sortedCorns[i] = sortedCorns[smallestIndex];
			sortedCorns[smallestIndex] = tempCorn;
		}
	}
	for(i = 0; i < currentCorns; i++){
		printf("%c 		$%d.%02d 		%d cm 		%d days old.\n", sortedCorns[i].color, sortedCorns[i].priceInCents/100, sortedCorns[i].priceInCents%100, sortedCorns[i].lengthInCM, sortedCorns[i].age);
	}
	system("pause");
}

void drawLen(){
	readFileToMem();
	// Insertion Sort!
	int i, j, smallest, smallestIndex;
	struct Corn sortedCorns[MAX_CORN_SUPPORTED];
	// For the purposes of my program, I'm going to copy corns into sortedCorns and play with that.
	for(i = 0; i < currentCorns; i++){
		sortedCorns[i] = corns[i];
	}
	struct Corn tempCorn;

	for(i = 0; i < currentCorns; i++){
		smallest = sortedCorns[i].lengthInCM;
		smallestIndex = i;
		for(j = i; j < currentCorns; j++){
			if(sortedCorns[j].lengthInCM < smallest){
				smallestIndex = j;
				smallest = sortedCorns[j].lengthInCM;
			}
		}
		//so we've found the smallest.
		//swap index i and the smallest.
		if(smallestIndex!=i){
			tempCorn = sortedCorns[i];
			sortedCorns[i] = sortedCorns[smallestIndex];
			sortedCorns[smallestIndex] = tempCorn;
		}
	}
	for(i = 0; i < currentCorns; i++){
		printf("%c 		$%d.%02d 		%d cm 		%d days old.\n", sortedCorns[i].color, sortedCorns[i].priceInCents/100, sortedCorns[i].priceInCents%100, sortedCorns[i].lengthInCM, sortedCorns[i].age);
	}
	system("pause");
}

void salesHistory(){
	/*
	First we open the file.
	*/
	FILE* transactionStorage;
	int numOfTransactions = 0, i;
	int dayOfMonth, month, year, priceInCents, customerID;
	
	transactionStorage = fopen("history.bin", "rb");
	if(transactionStorage == NULL){
		
		printf("Error! File does not exist!\n");
		transactionStorage = fopen("history.bin", "wb");
		printf("Creating, please re-preform this operation.\n");
		
	}
	while((fscanf(transactionStorage, "%d %d %d %d %d\n", &dayOfMonth, &month, &year, &priceInCents, &customerID)==5)) {// Read correctly.
		printf("%d/%d/%d $%d ID:%d \n",dayOfMonth, 1+month, 1900+year, priceInCents, customerID);
	}
	fclose(transactionStorage);
	system("pause");
}

void writeToHistoryFile(int customerID, int total){
	time_t t = time(NULL);
	struct tm timeMacro = *localtime(&t);
	
	
	FILE* transactionStorage;
	int numOfTransactions = 0, i;
	int dayOfMonth, month, year;
	
	transactionStorage = fopen("history.bin", "ab");
	if(transactionStorage == NULL){
		printf("Error! File does not exist!\n");
		transactionStorage = fopen("history.bin", "wb");
		printf("Creating, please re-preform this operation.\n");
		}else{
			
	}	
	fprintf(transactionStorage, "%d %d %d %d %d \n", timeMacro.tm_mday, timeMacro.tm_mon, timeMacro.tm_year, total, customerID);
	printf("(Debug!)%d %d %d %d %d \n", timeMacro.tm_mday, timeMacro.tm_mon, timeMacro.tm_year, total, customerID);
	fclose(transactionStorage);
	system("pause");
}

void checkPerson(){
	
	int total, choice, i, cID;
	drawMiddle("", 0, '#');
	drawMiddle("      Welcome to checkout!    ", 30, '#');
	drawMiddle("", 0, '#');
	drawInventory();
	struct Corn tempCorn;
	printf("Enter the SKU #. \n");
	choice = getChoice(1,currentCorns,1);
	choice--;
	for(i=0; i < currentCorns; i++){
		if(i==choice-1){
		printf("Your selected corn: \n");	
		printf("%08d. 	%c 		$%d.%02d 		%d cm 		%d days old.\n", choice-1, corns[choice-1].color, corns[choice-1].priceInCents/100, corns[choice-1].priceInCents%100, corns[choice-1].lengthInCM, corns[choice-1].age);
		}
	}
	printf("Your total is: \t $%d.%02d", corns[choice].priceInCents/100, corns[choice].priceInCents%100);
	total = corns[choice].priceInCents;
	printf("\nPlease enter your customer ID, or type zero to use the store's customer ID.\n");
	scanf("%d", &cID);
	if(choice!=currentCorns-1){
			corns[choice] = corns[currentCorns-1];
			currentCorns--;
			writeToFile();
		}
	else{
		currentCorns--;
		writeToFile();
	}
	writeToHistoryFile(cID, total);
	system("pause");
}

void specificSearch(){
	printf("Your store must upgrade to the premium package in order to access this feature!\n");
	system("pause");
}


void drawMainMenu(){
	system("cls");
	// Let's make a greeting.
	drawMiddle("", 0, '#');
	drawMiddle("   Welcome to the Corn Hub!   ", 30, '#');
	drawMiddle("", 0, '#');
	// Present User with their options
	printf("\n");
	drawMiddle("", 0, '#');
	drawMiddle("                     1. View Inventory                           ", 65, '#');
	drawMiddle("                     2. Modify Invetory                          ", 65, '#');
	drawMiddle("                     3. Lookup by SKU                            ", 65, '#');
	drawMiddle("                     4. View Sales History                       ", 65, '#');
	drawMiddle("                     5. Checkout Person                          ", 65, '#');
	drawMiddle("                     6. Lookup by Artifact                       ", 65, '#');
	drawMiddle("                     7. Exit The Program                         ", 65, '#');
	drawMiddle("", 0, '#');
	int choice = getChoice(1,7,1);
	switch(choice){
		case 1:
			printf("Please choose one of the following:\n1 Sorted by price.\n2 Sorted by age.\n3 Sorted by length.\n4 Sorted by SKU number.\n");
			choice = getChoice(1,4,1);
			switch(choice){
				case 1:
					drawPrice();
					break;
				case 2:
					drawAge();
					break;
				case 3:
					drawLen();
					break;
				case 4:
					drawInventory();
					break;
			}
			break;
		case 2:
			modInventory();
			break;
		case 3:
			skuLookup();
			break;
		case 4:
			salesHistory();
			break;
		case 5:
			checkPerson();
			break;
		case 6: 
			specificSearch();
			break;
		case 7:
			exit(0);				
	}
	return;
}


int main(int argc, char *argv[]) {

	while(1){
		drawMainMenu();
	}
}

