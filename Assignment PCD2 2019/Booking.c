#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Header.h"
#pragma warning(disable:4996)

// Declaration of self-defined function.

// Function of adding a booking.
void addingBook(char tempID[]);

// Function of delete student own booking.
void deleteBookStud(char tempID[]);

// Function of delete any booking for staff.
void deleteBookStaff(char selfID[]);

// Function of displaying own booking.
void displayBook(char tempID[]);

// Struct for booking.
typedef struct {
	char bookID[6], facID[7], userID[11];
	int bookStart, bookEnd;
	Date dateBooked, dateWhenBook;
} Book;


int main(void)
{
	char selection[2], user = 'S';
	char tempID[] = "S0001";

	printf("-------------------------------------------\n");
	printf("%14sBOOKING MODULE\n", "");
	printf("-------------------------------------------\n\n");
	while (1)
	{
		printf("%19sMENU\n", "");
		printf("-------------------------------------------\n");
		printf("| [1] Adding a booking.                   |\n");
		if (user == 'S')
		{
			printf("| [2] Delete a booking.                   |\n");
			printf("| [3] Checking specific user's booking.   |\n");
			printf("| [4] Checking facility availability.     |\n");
			printf("| [5] Search booking by booking ID.       |\n");
			printf("| [6] Display your booking.               |\n");
			printf("| [7] Exit.                               |\n");
		}
		else
		{
			printf("| [2] Delete your booking.                |\n");
			printf("| [3] Checking facility availability.     |\n");
			printf("| [4] Display your booking.               |\n");
			printf("| [5] Exit.                               |\n");
		}
		printf("-------------------------------------------\n");
		printf("Please choose a selection: ");
		for (int i = 0;;)
		{
			selection[i] = getch();
			if (i == 1)
			{
				if (selection[i] == '\n' || selection[i] == '\r')
					break;
				else if (selection[i] == '\b')
				{
					printf("\b \b");
					i--;
				}
			}
			else if ((selection[i] == '6' || selection[i] == '7') && user != 'S')
				continue;
			else if (selection[i] != '1' && selection[i] != '2' &&
				selection[i] != '3' && selection[i] != '4' &&
				selection[i] != '5' && selection[i] != '6' && selection[i] != '7')
				continue;
			else
			{
				printf("%c", selection[i]);
				i++;
			}
		}
		printf("\n\n\n");
		switch (selection[0])
		{
		case '1':
			addingBook(tempID); break;
		case '2':
			if (user == 'S')
				deleteBookStaff(tempID);
			else
				deleteBookStud(tempID);
			break;
		case '3':
			break;
		case '4':
			if (user == 'S');
			else
				displayBook(tempID);
			break;
		case '5':
			if (user == 'S');
			else
				return 0;
			break;
		case '6':
			displayBook(tempID);
			break;
		case '7':
			return 0;
		}
		printf("\n\n");
	}
}

void addingBook(char tempID[])
{
	FILE *fBook, *fNewBook;
	Book getBook, tempBook;
	char checkValid, tempBookID[6], temptoN[60], selection[2];
	int i, checkBookID;

	// Set date when booking as today's date.
	getBook.dateWhenBook.day = todayDate("day");
	getBook.dateWhenBook.month = todayDate("month");
	getBook.dateWhenBook.year = todayDate("year");

	printf("ADDING A BOOKING\n");
	printf("----------------\n");

	fBook = fopen("BookingLog.txt", "r");
	if (fBook == NULL)
		fBook = fopen("BookingLog.txt", "w");
	// Generate booking ID.
	for (i = 1; !feof(fBook); i++)
	{
		fscanf(fBook, "B%d|%[^\n]\n", &checkBookID, temptoN);
		if (i != checkBookID)
			break;
	}
	sprintf(tempBookID, "B%04d", i);
	strcpy(getBook.bookID, tempBookID);
	printf("Booking ID: %s\n", tempBookID);

	// Get input of facility ID.
	printf("Facility ID (e.g. F0001) (-1 to cancel): ");
	rewind(stdin);
	scanf("%6s", getBook.facID);
	// Cancel and return to menu when user enter -1.
	if (getBook.facID[0] == '-' && getBook.facID[1] == '1')
	{
		printf("Cancelled.\n");
		printf("----------");
		fclose(fBook);
		return;
	}
	while (strlen(getBook.facID) > 5)
	{
		printf("Invalid! booking ID should contain only 5 characters.\n");
		printf("Facility ID (e.g. B1116): ");
		rewind(stdin);
		scanf("%6s", getBook.facID);
	}
	getBook.facID[0] = toupper(getBook.facID[0]);
	// 要check有没有这个facility.

	// Get input of booking date.
	do
	{
		printf("Booking Date (DD/MM/YYYY): ");
		rewind(stdin);
		scanf("%d/%d/%d%c", &getBook.dateBooked.day, &getBook.dateBooked.month, &getBook.dateBooked.year, &checkValid);
		if (checkValid != '\n')
			printf("Invalid input! Please enter again.\n");
		else if (getBook.dateBooked.day <= todayDate("day") + 1 || getBook.dateBooked.month < todayDate("month")
			|| getBook.dateBooked.year < todayDate("year"))
			printf("Invalid input! The Booking Date should at least one day in advance!\n");
	} while (checkValid != '\n' || getBook.dateBooked.day <= todayDate("day") + 1
		|| getBook.dateBooked.month < todayDate("month")
		|| getBook.dateBooked.year < todayDate("year"));


	// Get input of booking time.
	do
	{
		getBook.bookEnd = getBook.bookStart = -2;
		printf("Booking Time (In 24 hours format):\n");
		printf("\tEnter -1 to cancel the booking.\n");
		printf("\tFrom: ");
		rewind(stdin);
		scanf("%d", &getBook.bookStart);
		// 要加check 时间 facility file.
		if (getBook.bookStart == -1)
		{
			printf("Cancelled.\n");
			printf("----------");
			return;
		}
		while (checkTimeValid(getBook.bookStart) == 0)
		{
			printf("\tInvalid time input!\n\n");
			printf("\tFrom: ");
			rewind(stdin);
			scanf("%d", &getBook.bookStart);
			if (getBook.bookStart == -1)
			{
				printf("Cancelled.\n");
				printf("----------");
				return;
			}
		}
		printf("\tTo  : ");
		rewind(stdin);
		scanf("%d", &getBook.bookEnd);
		// 要加check 时间file.
		if (checkTimeValid(getBook.bookEnd) == 0 || getBook.bookStart >= getBook.bookEnd)
			printf("\tInvalid time input!\n");
		else
		{
			rewind(fBook);
			while (!feof(fBook))
			{
				fscanf(fBook, "%[^|]|%d/%d/%d|%d/%d/%d|%d-%d|%[^|]|%s\n", tempBook.bookID,
					&tempBook.dateWhenBook.day, &tempBook.dateWhenBook.month, &tempBook.dateWhenBook.year,
					&tempBook.dateBooked.day, &tempBook.dateBooked.month, &tempBook.dateBooked.year,
					&tempBook.bookStart, &tempBook.bookEnd, tempBook.userID, tempBook.facID);
				if (!((getBook.bookStart < tempBook.bookStart && getBook.bookEnd <= tempBook.bookStart
					&& getBook.bookStart < tempBook.bookEnd && getBook.bookEnd < tempBook.bookEnd) ||
					(getBook.bookStart > tempBook.bookStart && getBook.bookEnd > tempBook.bookStart
						&& getBook.bookStart >= tempBook.bookEnd && getBook.bookEnd > tempBook.bookEnd)) && strcmp(tempBook.facID, getBook.facID) == 0 &&
					getBook.dateBooked.day == tempBook.dateBooked.day && getBook.dateBooked.month == tempBook.dateBooked.month &&
					getBook.dateBooked.year == tempBook.dateBooked.year)
					printf("\tInvalid input! The booking time %04d-%04d has been booked.\n\n", tempBook.bookStart, tempBook.bookEnd);
				break;
			}
		}
	} while (checkTimeValid(getBook.bookEnd) == 0 || getBook.bookStart >= getBook.bookEnd || 
		(!((getBook.bookStart < tempBook.bookStart && getBook.bookEnd <= tempBook.bookStart
			&& getBook.bookStart < tempBook.bookEnd && getBook.bookEnd < tempBook.bookEnd) ||
			(getBook.bookStart > tempBook.bookStart && getBook.bookEnd > tempBook.bookStart
				&& getBook.bookStart >= tempBook.bookEnd && getBook.bookEnd > tempBook.bookEnd)) && strcmp(tempBook.facID, getBook.facID) == 0 &&
			getBook.dateBooked.day == tempBook.dateBooked.day && getBook.dateBooked.month == tempBook.dateBooked.month &&
			getBook.dateBooked.year == tempBook.dateBooked.year));

	printf("----------------\n\n");
	printf("%6sYOUR BOOKING\n", "");
	printf("------------------------\n");
	printf("Booking ID  : %s\n", getBook.bookID);
	printf("Facility ID : %s\n", getBook.facID);
	printf("Booking Date: %02d/%02d/%d\n", getBook.dateBooked.day, getBook.dateBooked.month, getBook.dateBooked.year);
	printf("Booking Time: %04d-%04d\n", getBook.bookStart, getBook.bookEnd);
	printf("------------------------\n");
	printf("Confirm? (Type Y for yes and N for no): ");
	// To let user only input Y and N.
	for (int i = 0;;)
	{
		selection[i] = getch();
		if (i == 1)
		{
			if (selection[i] == '\n' || selection[i] == '\r')
				break;
			else if (selection[i] == '\b')
			{
				printf("\b \b");
				i--;
			}
		}
		else if (selection[i] != 'y' && selection[i] != 'Y' && selection[i] != 'n' && selection[i] != 'N')
			continue;
		else
		{
			printf("%c", selection[i]);
			i++;
		}
	}
	if (toupper(selection[0]) == 'Y')
	{
		printf("\nAdding...\n");
		fNewBook = fopen("tempBook.txt", "w");
		rewind(fBook);
		for (int x = 1; x < i; x++)
		{
			fscanf(fBook, "B%d|%[^\n]\n", &checkBookID, temptoN);
			fprintf(fNewBook, "B%04d|%s\n", checkBookID, temptoN);
		}
		fprintf(fNewBook, "%s|%02d/%02d/%d|%02d/%02d/%d|%04d-%04d|%s|%s\n", getBook.bookID,
			getBook.dateWhenBook.day, getBook.dateWhenBook.month, getBook.dateWhenBook.year,
			getBook.dateBooked.day, getBook.dateBooked.month, getBook.dateBooked.year,
			getBook.bookStart, getBook.bookEnd, tempID, getBook.facID);
		while (fscanf(fBook, "B%d|%[^\n]\n", &checkBookID, temptoN) != EOF)
		{
			fprintf(fNewBook, "B%04d|%s\n", checkBookID, temptoN);
		}

		// Close the file and rename the temp file into booking log file.
		fclose(fBook);
		fclose(fNewBook);
		system("del /f BookingLog.txt");
		system("ren tempBook.txt BookingLog.txt");

		printf("Added Successfully.\n");
		printf("-------------------");
	}
	else
	{
		printf("\nCancelled.\n");
		printf("----------");
	}
}

void deleteBookStud(char tempID[])
{
	FILE *fBook, *fNewBook;
	Book readBook;
	int counter;
	char tempBookID[7], selection[2];
		
	printf("DELETE YOUR BOOKING\n");
	printf("-------------------\n\n");

	// Open the booking log file and check whether it is exist.
	fBook = fopen("BookingLog.txt", "r");
	if (fBook == NULL)
	{
		printf("The file cannot open successfully! Perhaps no one was booking before?\n");
		return;
	}

	printf("%-15s%-20s%-15s%-15s%s\n", "Booking ID", "Date of Booking", "Date Booked", "   Time  ", "Facility ID");
	printf("%-15s%-20s%-15s%-15s%s\n", "----------", "---------------", "-----------", "---------", "-----------");
	counter = 0;
	while (!feof(fBook))
	{
		fscanf(fBook, "%[^|]|%d/%d/%d|%d/%d/%d|%d-%d|%[^|]|%s\n", readBook.bookID,
			&readBook.dateWhenBook.day, &readBook.dateWhenBook.month, &readBook.dateWhenBook.year,
			&readBook.dateBooked.day, &readBook.dateBooked.month, &readBook.dateBooked.year,
			&readBook.bookStart, &readBook.bookEnd, readBook.userID, readBook.facID);
		if (strcmp(readBook.userID, tempID) == 0)
		{
			printf("%-15s%02d/%02d/%d%10s%02d/%02d/%d%5s%04d-%04d%9s%s\n", readBook.bookID,
				readBook.dateWhenBook.day, readBook.dateWhenBook.month, readBook.dateWhenBook.year,
				"", readBook.dateBooked.day, readBook.dateBooked.month, readBook.dateBooked.year,
				"", readBook.bookStart, readBook.bookEnd, "", readBook.facID);
			counter++;
		}
	}
	printf("-----\n%d record(s) found.\n", counter);

	printf("Please enter booking ID you wanted to delete (-1 to cancel): ");
	do
	{
		do
		{
			rewind(stdin);
			scanf("%6s", tempBookID);
			tempBookID[0] = toupper(tempBookID[0]);
			if (tempBookID[0] == '-' && tempBookID[1] == '1')
			{
				printf("Cancelled.\n");
				printf("----------");
				return;
			}
			if (strlen(tempBookID) != 5)
				printf("Invalid input! Please enter again: ");
		} while (strlen(tempBookID) != 5);

		// Open a new file to fill in the data except that needed to delete.
		fNewBook = fopen("tempBook.txt", "w");
		rewind(fBook);
		while (!feof(fBook))
		{
			fscanf(fBook, "%[^|]|%d/%d/%d|%d/%d/%d|%d-%d|%[^|]|%s\n", readBook.bookID,
				&readBook.dateWhenBook.day, &readBook.dateWhenBook.month, &readBook.dateWhenBook.year,
				&readBook.dateBooked.day, &readBook.dateBooked.month, &readBook.dateBooked.year,
				&readBook.bookStart, &readBook.bookEnd, readBook.userID, readBook.facID);
			if (strcmp(tempBookID, readBook.bookID) == 0 && strcmp(tempID, readBook.userID) == 0)
				break;
			else
				fprintf(fNewBook, "%s|%02d/%02d/%d|%02d/%02d/%d|%04d-%04d|%s|%s\n", readBook.bookID,
					readBook.dateWhenBook.day, readBook.dateWhenBook.month, readBook.dateWhenBook.year,
					readBook.dateBooked.day, readBook.dateBooked.month, readBook.dateBooked.year,
					readBook.bookStart, readBook.bookEnd, readBook.userID, readBook.facID);
		}
		if (feof(fBook) && strcmp(tempBookID, readBook.bookID) != 0)
		{
			printf("Invalid input! The Booking ID %s is not found in the record.\n", tempBookID);
			fclose(fNewBook);
			printf("Please type again: ");
		}
		else
		{
			printf("Confirm? (Type Y for yes and N for no): ");
			// To let user only input Y and N.
			for (int i = 0;;)
			{
				selection[i] = getch();
				if (i == 1)
				{
					if (selection[i] == '\n' || selection[i] == '\r')
						break;
					else if (selection[i] == '\b')
					{
						printf("\b \b");
						i--;
					}
				}
				else if (selection[i] != 'y' && selection[i] != 'Y' && selection[i] != 'n' && selection[i] != 'N')
					continue;
				else
				{
					printf("%c", selection[i]);
					i++;
				}
			}
		}
		if (toupper(selection[0]) == 'N')
		{
			fclose(fNewBook);
			system("del /f tempBook.txt");
			printf("\nCancelled.\n");
			printf("----------");
			return;
		}
	} while (feof(fBook) && strcmp(tempBookID, readBook.bookID) != 0);
	printf("\nDeleting...");
	while (!feof(fBook))
	{
		fscanf(fBook, "%[^|]|%d/%d/%d|%d/%d/%d|%d-%d|%[^|]|%s\n", readBook.bookID,
			&readBook.dateWhenBook.day, &readBook.dateWhenBook.month, &readBook.dateWhenBook.year,
			&readBook.dateBooked.day, &readBook.dateBooked.month, &readBook.dateBooked.year,
			&readBook.bookStart, &readBook.bookEnd, readBook.userID, readBook.facID);
		fprintf(fNewBook, "%s|%02d/%02d/%d|%02d/%02d/%d|%04d-%04d|%s|%s\n", readBook.bookID,
			readBook.dateWhenBook.day, readBook.dateWhenBook.month, readBook.dateWhenBook.year,
			readBook.dateBooked.day, readBook.dateBooked.month, readBook.dateBooked.year,
			readBook.bookStart, readBook.bookEnd, readBook.userID, readBook.facID);
	}

	// Close the file and rename the temp file into booking log file.
	fclose(fBook);
	fclose(fNewBook);
	system("del /f BookingLog.txt");
	system("ren tempBook.txt BookingLog.txt");

	printf("\nDelete successfully.");
	printf("\n--------------------");
}

void deleteBookStaff(char selfID[])
{
	FILE *fBook, *fNewBook;
	Book readBook;
	int counter;
	char tempBookID[7], selection[2], tempID[12];

	printf("DELETE A BOOKING\n");
	printf("----------------\n");

	do
	{
		printf("Please enter user ID (Enter SELF for your own ID): ");
		rewind(stdin);
		scanf("%11s", tempID);
		if (strlen(tempID) > 10)
			printf("Invalid input! The user ID should contain within 10 characters!\n");
	} while (strlen(tempID) > 10);

	// Change ID to own ID if the user type "SELF".
	for (int i = 0; i < strlen(tempID); i++)
		tempID[i] = toupper(tempID[i]);
	if (strcmp("SELF", tempID) == 0)
		strcpy(tempID, selfID);

	// Open the booking log file and check whether it is exist.
	fBook = fopen("BookingLog.txt", "r");
	if (fBook == NULL)
	{
		printf("The file cannot open successfully! Perhaps no one was booking before?\n");
		return;
	}

	printf("\n%-15s%-20s%-15s%-15s%s\n", "Booking ID", "Date of Booking", "Date Booked", "   Time  ", "Facility ID");
	printf("%-15s%-20s%-15s%-15s%s\n", "----------", "---------------", "-----------", "----------", "-----------");
	counter = 0;
	while (!feof(fBook))
	{
		fscanf(fBook, "%[^|]|%d/%d/%d|%d/%d/%d|%d-%d|%[^|]|%s\n", readBook.bookID,
			&readBook.dateWhenBook.day, &readBook.dateWhenBook.month, &readBook.dateWhenBook.year,
			&readBook.dateBooked.day, &readBook.dateBooked.month, &readBook.dateBooked.year,
			&readBook.bookStart, &readBook.bookEnd, readBook.userID, readBook.facID);
		if (strcmp(readBook.userID, tempID) == 0)
		{
			printf("%-15s%02d/%02d/%d%10s%02d/%02d/%d%5s%04d-%04d%9s%s\n", readBook.bookID,
				readBook.dateWhenBook.day, readBook.dateWhenBook.month, readBook.dateWhenBook.year,
				"", readBook.dateBooked.day, readBook.dateBooked.month, readBook.dateBooked.year,
				"", readBook.bookStart, readBook.bookEnd, "", readBook.facID);
			counter++;
		}
	}
	printf("-----\n%d record(s) found.\n", counter);

	if (counter == 0)
	{
		printf("Process terminated because no records found.\n");
		system("pause");
		return;
	}

	printf("Please enter booking ID you wanted to delete (-1 to cancel): ");
	do
	{
		do
		{
			rewind(stdin);
			scanf("%6s", tempBookID);
			tempBookID[0] = toupper(tempBookID[0]);
			if (tempBookID[0] == '-' && tempBookID[1] == '1')
			{
				printf("\nCancelled.\n");
				printf("----------");
				return;
			}
			if (strlen(tempBookID) != 5)
				printf("Invalid input! Please enter again: ");
		} while (strlen(tempBookID) != 5);

		// Open a new file to fill in the data except that needed to delete.
		fNewBook = fopen("tempBook.txt", "w");
		rewind(fBook);
		while (!feof(fBook))
		{
			fscanf(fBook, "%[^|]|%d/%d/%d|%d/%d/%d|%d-%d|%[^|]|%s\n", readBook.bookID,
				&readBook.dateWhenBook.day, &readBook.dateWhenBook.month, &readBook.dateWhenBook.year,
				&readBook.dateBooked.day, &readBook.dateBooked.month, &readBook.dateBooked.year,
				&readBook.bookStart, &readBook.bookEnd, readBook.userID, readBook.facID);
			if (strcmp(tempBookID, readBook.bookID) == 0 && strcmp(tempID, readBook.userID) == 0)
				break;
			else
				fprintf(fNewBook, "%s|%02d/%02d/%d|%02d/%02d/%d|%04d-%04d|%s|%s\n", readBook.bookID,
					readBook.dateWhenBook.day, readBook.dateWhenBook.month, readBook.dateWhenBook.year,
					readBook.dateBooked.day, readBook.dateBooked.month, readBook.dateBooked.year,
					readBook.bookStart, readBook.bookEnd, readBook.userID, readBook.facID);
		}
		if (feof(fBook) && strcmp(tempBookID, readBook.bookID) != 0)
		{
			printf("Invalid input! The Booking ID %s is not found in the record.\n", tempBookID);
			fclose(fNewBook);
			printf("Please type again: ");
		}
		else
		{
			printf("Confirm? (Type Y for yes and N for no): ");
			// To let user only input Y and N.
			for (int i = 0;;)
			{
				selection[i] = getch();
				if (i == 1)
				{
					if (selection[i] == '\n' || selection[i] == '\r')
						break;
					else if (selection[i] == '\b')
					{
						printf("\b \b");
						i--;
					}
				}
				else if (selection[i] != 'y' && selection[i] != 'Y' && selection[i] != 'n' && selection[i] != 'N')
					continue;
				else
				{
					printf("%c", selection[i]);
					i++;
				}
			}
		}
		if (toupper(selection[0]) == 'N')
		{
			fclose(fNewBook);
			system("del /f tempBook.txt");
			printf("\nCancelled.\n");
			printf("----------");
			return;
		}
	} while (feof(fBook) && strcmp(tempBookID, readBook.bookID) != 0);
	printf("\nDeleting...");
	while (!feof(fBook))
	{
		fscanf(fBook, "%[^|]|%d/%d/%d|%d/%d/%d|%d-%d|%[^|]|%s\n", readBook.bookID,
			&readBook.dateWhenBook.day, &readBook.dateWhenBook.month, &readBook.dateWhenBook.year,
			&readBook.dateBooked.day, &readBook.dateBooked.month, &readBook.dateBooked.year,
			&readBook.bookStart, &readBook.bookEnd, readBook.userID, readBook.facID);
		fprintf(fNewBook, "%s|%02d/%02d/%d|%02d/%02d/%d|%04d-%04d|%s|%s\n", readBook.bookID,
			readBook.dateWhenBook.day, readBook.dateWhenBook.month, readBook.dateWhenBook.year,
			readBook.dateBooked.day, readBook.dateBooked.month, readBook.dateBooked.year,
			readBook.bookStart, readBook.bookEnd, readBook.userID, readBook.facID);
	}

	// Close the file and rename the temp file into booking log file.
	fclose(fBook);
	fclose(fNewBook);
	system("del /f BookingLog.txt");
	system("ren tempBook.txt BookingLog.txt");

	printf("\nDelete successfully.");
	printf("\n--------------------");
}

void displayBook(char tempID[])
{
	FILE *fBook;
	Book readBook;
	int counter = 0;

	printf("DISPLAYING YOUR BOOKING\n");
	printf("-----------------------\n\n");

	// Open the booking log file and check whether it is exist.
	fBook = fopen("BookingLog.txt", "r");
	if (fBook == NULL)
	{
		printf("The file cannot open successfully! Perhaps no one was booking before?\n");
		return;
	}

	printf("%-15s%-20s%-15s%-15s%s\n", "Booking ID", "Date of Booking", "Date Booked", "   Time   ", "Facility ID");
	printf("%-15s%-20s%-15s%-15s%s\n", "----------", "---------------", "-----------", "----------", "-----------");
	while (!feof(fBook))
	{
		fscanf(fBook, "%[^|]|%d/%d/%d|%d/%d/%d|%d-%d|%[^|]|%s\n", readBook.bookID,
			&readBook.dateWhenBook.day, &readBook.dateWhenBook.month, &readBook.dateWhenBook.year,
			&readBook.dateBooked.day, &readBook.dateBooked.month, &readBook.dateBooked.year,
			&readBook.bookStart, &readBook.bookEnd, readBook.userID, readBook.facID);
		if (strcmp(readBook.userID, tempID) == 0)
		{
			printf("%-15s%02d/%02d/%d%10s%02d/%02d/%d%5s%04d-%04d%9s%s\n", readBook.bookID,
				readBook.dateWhenBook.day, readBook.dateWhenBook.month, readBook.dateWhenBook.year,
				"", readBook.dateBooked.day, readBook.dateBooked.month, readBook.dateBooked.year,
				"", readBook.bookStart, readBook.bookEnd, "", readBook.facID);
			counter++;
		}
	}
	printf("-----\n%d record(s) found.\n", counter);
	fclose(fBook);
	system("pause");
	return;
}