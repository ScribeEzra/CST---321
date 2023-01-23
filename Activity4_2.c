#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int virtualPage1[0];
int virtualPage2[0];
int virtualPage3[0];
int physicalPage1[0];
int physicalPage2[0];

void binaryFromDecimal(int decimal, char* bits)
{
	int i = 0;
	int result = decimal;
	while (result != 0)
	{
		int remainder = result % 2;
		
		if (remainder > 0){
			bits[i] = '1';
		}
		else{
			bits[i] = '0';
		}
		result = result / 2;
		i++;
	}
}
void printBinary(char* bits)
{
	for (int i = 0; i < 16; i++){
		printf("%s", &bits[32-i]);
	}
	printf("\n");
}

void clearBits(char* bits)
{
	for (int i = 0; i < 32; i++){
		bits[i] = '0';
	}
}

void convertAndPrintBinary(int number, char* bits)
{
	clearBits(&bits[0]);
	binaryFromDecimal(number, &bits[0]);
	printBinary(&bits[0]);
}

void createPages(int num)
{

	virtualPage1[num];
	virtualPage2[num];
	virtualPage3[num];
	physicalPage1[num];
	physicalPage2[num];

	for (int i = 0; i < num; i++)
	{
		physicalPage1[i] = num - i;//each entry is a decending number
	}
	for (int i = 0; i < num; i++)
	{
		physicalPage2[i] = i;//each entry is an ascending number
	}
	for (int i = 0; i < num; i++)
	{
		virtualPage1[i] = num - i;//each entry is a decending number
	}
	for (int i = 0; i < num; i++)
	{
		virtualPage2[i] = i;//each entry is an ascending number
	}
	for (int i = 0; i < num; i++)
	{
		virtualPage3[i] = i;//each entry is an ascending number
	}
	
}

int TLB(int virtualPage, int offset)
{
	int data;
	//Two pages saved in this TLB
	//But it would normally be around 64
	if (virtualPage == 1){
		printf("Virtual page 1 translated to Physical Page 2 with offset of %d\n", offset);
		data = physicalPage2[offset];
	}
	if (virtualPage == 2){
		printf("Virtual page 2 translated to Physical Page 1 with offset of %d\n", offset);
		data = physicalPage1[offset];
	}
	else{
		printf("Data not found in TLB. Must Search physical address...");
		data = 0;
		//This is where physical addresses would be searched.
	}
	
	return data;
}

int main(int argc, const char *argv[])
{
	char bits[16] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
'0', '0', '0', '0', '0', '0'};
	bool improper = true;
	
	//Get Page Size
	printf("Enter a page size (4095 or 8191 reccomended)\n");
	int num;
	scanf("%d", &num);
	
	//Converted to Binary
	printf("Page size of %d converted to binary: \n", num);
	convertAndPrintBinary(num, &bits[0]);
	
	//Converted to Hexidecimal
	printf("Page size converted to hexidecimal: 0x%08X\n", num);
	
	//Create pages of "num" space
	//2 physical and 2 virtual
	createPages(num);
	printf("Pages of %d size created\n", num);
	
	//Convert virtual to physical addresses
	
	printf("Searching for virtual page 1, offset 15...\n");
	printf("Data: %d\n", TLB(1, 15));
	
	printf("Searching for virtual page 1, offset 12...\n");
	printf("Data: %d\n", TLB(1, 12));
	
	printf("Searching for virtual page 2, offset 5...\n");
	printf("Data: %d\n", TLB(2, 55));
	
	printf("Searching for virtual page 2, offset 90...\n");
	printf("Data: %d\n", TLB(2, 90));
	
	printf("Searching for virtual page 1, offset 100...\n");
	printf("Data: %d\n", TLB(1, 100));
	
	//End
	return 0;
}
