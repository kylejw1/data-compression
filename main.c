#include <stdio.h>
#include <stdint.h>

#define REPEAT_FLAG 0x80

#define DEBUG 1
#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

uint64_t byte_compress(uint8_t *data, uint64_t data_length);
uint8_t count_repeats(uint8_t *data, uint64_t data_length);
void print_byte_array(uint8_t *data, uint64_t array_length);

/*

NOTES:

1. This strategy works by utilizing the low MSB on the input data.  If the data is repeated, the 
compressed data indicates this by setting (high MSB | number of repeats) before the value to be repeated.
If the value is not repeated, no extra byte is added before the value.  This means that the compress
algorithm is worst-case 100% of the original size, assuming no repeated values.

2. If there were no assumption was made that data is commonly duplicated sequentially, a hybrid solution
could be implemented where if there is not enough duplication, compression would be done via stripping
off the MSB of each byte and packeing them together

3. It would be more memory efficient to read and write directly to a file, which would also allow 
compression of data sets larger than available RAM.  In a language with memory stream support this 
solution could be generic and work with both file and memory equivalently.  

4. Since the decompressed_length is not written to the buffer, a decompress function would have to 
first iterate through the data set and calculate the final length in order to allocate a buffer.
This could be avoided by appending the decompressed_length to the compressed data if the compressed data
could be returned in a new buffer (to avoid possible buffer overflow)

*/

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

	if (compressed_length < 0) {
		printf("Compression failed");
	} else {
		printf("Compressed length = %lu\n", compressed_length);
		print_byte_array(data, compressed_length);
	}
}

uint64_t byte_compress(uint8_t *data_ptr, uint64_t data_length) 
{
	uint64_t read_index;
	uint64_t write_index = 0;
	uint8_t repeats;

	if (NULL == data_ptr) {
		printf("Invalid input.  data_ptr == NULL\n");
		return -1;
	}
	
	for (read_index = 0; read_index < data_length; read_index += repeats) {
		uint8_t value = data_ptr[read_index];
		repeats = count_repeats(&data_ptr[read_index], data_length - read_index);

		if (value & REPEAT_FLAG) {
			/* Handle illegal input data (MSB set).  This is okay as long as the value 
			is repeated (meaning there is room for a repeat indicator byte).  Decompression 
			will look at the repeat indicator byte and does not care that the MSB is set in 
			the value byte */
			
			debug_print("WARN: The current byte has the MSB set: %#x", value);
			
			if (repeats == 1) {
				printf("ERROR: Non-repeated byte has MSB set.  Cannot continue.");
				return -1;
			}
		}

		if (repeats > 1) 
		{
			// Signal the number of repeats by setting the REPEAT_FLAG
			data_ptr[write_index++] = repeats | REPEAT_FLAG;
		}
		data_ptr[write_index++] = value;
	}

	return write_index;
}

uint8_t count_repeats(uint8_t *data_ptr, uint64_t data_length) 
{
	static const uint8_t repeat_max_value = UINT8_MAX & ~REPEAT_FLAG;

	uint8_t initial_value = data_ptr[0];
	uint8_t i = 0;
	while(data_ptr[i] == initial_value) {
		if (i >= repeat_max_value) {
			// Not an error condition, just cant represent any more repeats in this byte.  
			// Next iteration will pick up where we left off.
			debug_print("Repeats of %#x exceeded max allowed, exiting at %u.  Will continue on the next iteration\n", initial_value, repeat_max_value);
			return repeat_max_value;
		}
		i++;
	}

	debug_print("Counted %u repeats of %#x\n", i, initial_value);
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
