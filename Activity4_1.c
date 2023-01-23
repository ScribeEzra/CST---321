#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
	for (int i = 0; i < 32; i++){
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

int main(int argc, const char *argv[])
{
	int num;
	char bits[32] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
'0', '0', '0', '0', '0', '0'};
	bool improper = true;
	
	while (improper){
		printf("Enter a number from 0 to 1000...\n");
		scanf("%d", &num);
		if (num < 0 || num > 1000){
			printf("The entered number isn't between 0 and 1000\n");
		}
		else{
			improper = false;
		}
	}
	
	//Binary and Hexidecimal
	printf("The number converted to binary: ");
	convertAndPrintBinary(num, &bits[0]);
	printf("The number converted to hexidecimal: 0x%08X\n", num);
	
	//Shift 10 bits to the left
	int shifted = ((num << 10) & 0xFFFFFc00) | 0x03FF;
	printf("The number has been shifted 10 bits to the left\n");
	
	printf("The number converted to decimal: %d\n", shifted);
	printf("The number converted to hexadecimal: 0x%08X\n", shifted);
	printf("The number converted to binary:");
	convertAndPrintBinary(shifted, &bits[0]);
}
