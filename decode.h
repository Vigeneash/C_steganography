#ifndef DECODE_H
#define DECODE_H
#include "common.h"
#include "types.h"

#define DECODE_FILE_EXTN_SIZE 32
#define DECODE_FILE_SIZE 32

typedef struct _DecodeInfo
{

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    uint image_data_size;
    char image_data[MAX_IMAGE_BUF_SIZE];
    
     /* Text File Info */
    char *output_file_name;
    FILE *fptr_output_file;
    char extn_output_file[MAX_FILE_SUFFIX];
    char decode_data[MAX_SECRET_BUF_SIZE];
    char *magst;
    uint magst_size;
    
} DecodeInfo;

/* Decoding funciton prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* file pointer for input and output files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decoding the file */
Status do_decoding(DecodeInfo *decInfo);

/* Magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Decoding file extension size*/
Status decode_output_file_extn_size(DecodeInfo *decInfo);

/* Decoding file extension */
Status decode_output_file_extn(uint extn_size, DecodeInfo *decInfo);

/* Decoding file size */
Status decode_file_size(DecodeInfo *decInfo);

/* Decoding file data */
Status decode_data_to_file(DecodeInfo *decInfo);

/* Decoding an array of LSB into byte */
Status decode_lsb_to_byte(char *decode_data, char *image_data);

#endif
