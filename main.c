#include <stdio.h>
#include <stdint.h>

// Gotchas:
// Output length bigger than input - buffer overflow
// sorted Linked list for duplicate data?
// Invalid data

#define REPEAT_FLAG 0x80
#define READ_UINT64(data_ptr) (*data_ptr++ & 0xFF | *data_ptr++ << 8 | *data_ptr++ << 16)

uint64_t byte_decompress(uint8_t *data, uint64_t data_length);
uint64_t byte_compress(uint8_t *data, uint64_t data_length);
uint64_t calculate_decompressed_length(uint8_t* compressed_data, uint64_t compressed_data_length);
uint8_t count_repeats(uint8_t *data, uint64_t data_length);
void print_byte_array(uint8_t *data, uint64_t array_length);

int main() 
{
	uint8_t data[] = { 
		0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
		0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 
	};
	uint64_t data_length = sizeof(data) / sizeof(data[0]);

	printf("Original length = %lu\n", data_length);
	print_byte_array(data, data_length);

	uint64_t compressed_length = byte_compress(data, data_length);
	printf("Compressed length = %lu\n", compressed_length);
	print_byte_array(data, compressed_length);

	uint64_t decompressed_length = byte_decompress(data, compressed_length);
	printf("Decompressed length = %lu", decompressed_length);
	print_byte_array(data, decompressed_length);
}

uint64_t byte_compress(uint8_t *data, uint64_t data_length) 
{
	uint64_t read_index;
	uint64_t write_index = 0;
	uint8_t repeats;

	if (NULL == data) {
		printf("Invalid input.  data == NULL\n");
		return data_length;
	}
	
	for (read_index = 0; read_index < data_length; read_index += repeats) {
		uint8_t read_value = data[read_index];
		repeats = count_repeats(&data[read_index], data_length - read_index);

		if (repeats > 1) 
		{
			// Signal the number of repeats by setting the REPEAT_FLAG
			data[write_index++] = repeats | REPEAT_FLAG;
		}
		data[write_index++] = read_value;
	}

	return write_index;
}

uint64_t byte_decompress(uint8_t *compressed_data_ptr, uint64_t compressed_data_length) 
{
	uint8_t *compressed_data = compressed_data_ptr;
	uint64_t decompressed_length = calculate_decompressed_length(compressed_data, compressed_data_length);
	uint8_t *decompressed_data = (uint8_t*)malloc(decompressed_length);

	uint64_t read_index;
	uint64_t write_index = 0;
	for(read_index=0; read_index < compressed_data_length; read_index++)
	{
		uint8_t value = compressed_data[i];

		if (value & REPEAT_FLAG) 
		{
			// Remove the repeat flag from the value and add the number of 
			// repeats to the calculated length
			decompressed_length += value & (~REPEAT_FLAG);
			i++;
		} 
		else 
		{
			// Value is not repeated
			decompressed_length++;
		}
	}

	return decompressed_length;
}

uint64_t calculate_decompressed_length(uint8_t* compressed_data, uint64_t compressed_data_length)
{
	uint64_t decompressed_length = 0;
	uint64_t i;

	for(i = 0; i < compressed_data_length; i++) {
		uint8_t value = compressed_data[i];

		if (value & REPEAT_FLAG) 
		{
			// Remove the repeat flag from the value and add the number of 
			// repeats to the calculated length
			decompressed_length += value & (~REPEAT_FLAG);
			i++;
		} 
		else 
		{
			// Value is not repeated
			decompressed_length++;
		}
	}

	return decompressed_length;
}

uint8_t count_repeats(uint8_t *data, uint64_t data_length) 
{
	const uint8_t repeat_max_value = UINT8_MAX & ~REPEAT_FLAG;

	uint8_t initial_value = data[0];
	uint8_t i = 0;
	while(data[i] == initial_value) {
		if (i >= repeat_max_value) {
			printf("Repeats of %#x exceeded max allowed, exiting at %u\n", initial_value, repeat_max_value);
			return repeat_max_value;
		}
		i++;
	}

	printf("Counted %u repeats of %#x\n", i, initial_value);
	return i;
}

void print_byte_array(uint8_t *data, uint64_t array_length)
{
	for (uint8_t i = 0; i < array_length; i++) {
		if (i % 8 == 0) {
			printf("\n");
		}
		printf("0x%02x ", data[i]);
	}
	printf("\n");
}