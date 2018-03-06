#include <stdio.h>

unsigned long byte_compress(char *data, unsigned long data_length);

int main() 
{
	char data[] = { 
		0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
		0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 
	};
	unsigned long data_length = sizeof(data) / sizeof(data[0]);

	printf("Compressing data of length = %lu\n", data_length);

	unsigned long new_length = byte_compress(data, data_length);

	printf("New length = %lu\n", new_length);
}

unsigned long byte_compress(char *data, unsigned long data_length) {
	
}