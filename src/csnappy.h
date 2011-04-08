#ifndef __CSNAPPY_H__
#define __CSNAPPY_H__
/*
File modified for the Linux Kernel by
Zeev Tarantov <zeev.tarantov@gmail.com>
*/

#define CSNAPPY_VERSION	1

#define SNAPPY_WORKMEM_BYTES_POWER_OF_TWO 15
#define SNAPPY_WORKMEM_BYTES (1 << SNAPPY_WORKMEM_BYTES_POWER_OF_TWO)

/*
 * Returns the maximal size of the compressed representation of
 * input data that is "source_len" bytes in length;
 */
uint32_t
snappy_max_compressed_length(uint32_t source_len) __attribute__((const));

/*
 * Flat array compression that does not emit the "uncompressed length"
 * prefix. Compresses "input" string to the "*op" buffer.
 *
 * REQUIRES: "input" is at most "kBlockSize" bytes long.
 * REQUIRES: "op" points to an array of memory that is at least
 * "snappy_max_compressed_length(input.size())" in size.
 * REQUIRES: working_memory has (1 << workmem_bytes_power_of_two) bytes.
 * REQUIRES: 9 <= workmem_bytes_power_of_two <= 15.
 *
 * Returns an "end" pointer into "op" buffer.
 * "end - op" is the compressed size of "input".
 */
char*
snappy_compress_fragment(
	const char* const input,
	const uint32_t input_size,
	char *op,
	void *working_memory,
	const int workmem_bytes_power_of_two);

/*
 * REQUIRES: "compressed" must point to an area of memory that is at
 * least "snappy_max_compressed_length(input_length)" bytes in length.
 * REQUIRES: working_memory has (1 << workmem_bytes_power_of_two) bytes.
 * REQUIRES: 9 <= workmem_bytes_power_of_two <= 15.
 *
 * Takes the data stored in "input[0..input_length]" and stores
 * it in the array pointed to by "compressed".
 *
 * "*compressed_length" is set to the length of the compressed output.
 */
void
snappy_compress(
	const char *input,
	uint32_t input_length,
	char *compressed,
	uint32_t *compressed_length,
	void *working_memory,
	const int workmem_bytes_power_of_two);

/*
 * Reads header of compressed data to get stored length of uncompressed data.
 * REQUIRES: start points to compressed data.
 * REQUIRES: n is length of available compressed data.
 * 
 * Returns SNAPPY_E_OK iff was able to decode length.
 * Stores decoded length into *result.
 */
int
snappy_get_uncompressed_length(const char *start, uint32_t n, uint32_t *result);

/*
 * Safely decompresses all data from array "src" of length "src_len" containing
 * entire compressed stream (with header) into array "dst" of size "dst_len".
 * REQUIRES: dst_len is at least snappy_get_uncompressed_length(...).
 * 
 * Iff sucessful, returns SNAPPY_E_OK.
 * If recorded length in header is greater than dst_len, returns
 *  SNAPPY_E_OUTPUT_INSUF.
 * If compressed data is malformed, does not write more than dst_len into dst.
 */
int
snappy_decompress(const char *src, uint32_t src_len, char *dst, uint32_t dst_len);

/*
 * Return values (< 0 = Error)
 */
#define SNAPPY_E_OK			0
#define SNAPPY_E_HEADER_BAD		(-1)
#define SNAPPY_E_OUTPUT_INSUF		(-2)
#define SNAPPY_E_OUTPUT_OVERRUN		(-3)
#define SNAPPY_E_INPUT_NOT_CONSUMED	(-4)
#define SNAPPY_E_UNEXPECTED_OUTPUT_LEN	(-5)
#define SNAPPY_E_DATA_MALFORMED		(-6)

#endif