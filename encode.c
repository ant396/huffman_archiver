/**
 * @file	Encode
 * @brief	Compress input file
 *
 * Creat archive file and write encode data into it
 * @author	 Anton Svechnikov
 * @license	 BSD
 *
 */

#include "encode.h"

/**
 * @brief Compress function
 *
 * Compress function
 *
 * @param session Pointer on structer with all input 
 * options and parameters
 * @param stat Pointer on statistic array
 * @param code Pointer on array with huffman codes
 *
 * @return 1 in case of error
 *
 */
int compress(global_opt *session, unsigned long *stat, char *code[])
{
	session->ofile_p = fopen(session->ofile, "wb");
	
	fwrite(stat, sizeof(unsigned long), MAXCHARS, session->ofile_p);

	encode(session, code);

	return 0;
}

/**
 * @brief Encode input data
 *
 * Encode data form input file
 *
 * @param session Pointer on structer with all input 
 * options and parameters
 * @param code Pointer on array with huffman codes
 *
 */
int encode(global_opt *session, char **code)
{
	size_t current_size;
	
	tools buff = {0};

	buff.input = calloc(BUFF_SIZE, sizeof(unsigned char));
	buff.output = calloc(BUFF_SIZE, sizeof(char));
	buff.bit_count = BITS_LEN;

	fseek(session->ifile_p, 0L, SEEK_SET);
	
	while ((current_size = fread(buff.input, sizeof(unsigned char), BUFF_SIZE, session->ifile_p))) {
		for ( ; buff.i_count < current_size; buff.i_count++) {
			bit_coding(&buff, session, code[(unsigned char) buff.input[buff.i_count]]);
		}
		buff.i_count = 0;
	}

	if (buff.bit_count < BITS_LEN || buff.o_count > 0) {
		buff.output[buff.o_count++] = buff.bit;
		fwrite(buff.output, sizeof(char), buff.o_count, session->ofile_p);
	}

	free(buff.input);
	free(buff.output);
	
	return 0;
}

/**
 * @brief Code bits
 *
 * Code bits for further encoding
 *
 * @param p_buff Pointer in structer with information about 
 * input and output buffers
 * @param session Pointer on structer with all input 
 * options and parameters
 * @param code Pointer on array with huffman codes
 *
 */
int bit_coding(tools *p_buff, global_opt *session, char *code)
{
	int index;
	size_t code_len = strlen(code);
	
	for (index = 0; index < code_len; index++) {
		check_buff(p_buff, session);
		if (code[index] == '1') {
			p_buff->bit |= (1 << (p_buff->bit_count - 1));
		}
		p_buff->bit_count--;
	}

	return 0;
}

/**
 * @brief Check buffers
 *
 * Check, write and clean output buffer and raw bit
 *
 * @param p_buff Pointer in structer with information about 
 * input and output buffers
 * @param session Pointer on structer with all input 
 * options and parameters
 *
 * @return 1 in case of error
 *
 */
int check_buff(tools *buff, global_opt *session)
{
	if (buff->o_count >= BUFF_SIZE) {
		fwrite(buff->output, sizeof(char), buff->o_count, session->ofile_p);
		memset(buff->output, 0, BUFF_SIZE);
		buff->o_count = 0;
	}

	if (buff->bit_count <= 0) {
		buff->output[buff->o_count++] = buff->bit;
		buff->bit = 0;
		buff->bit_count = BITS_LEN;
	}

	return 0;
}
