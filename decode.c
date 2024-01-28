#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(argv[2] == NULL || strstr(argv[2], ".bmp") == NULL)
	return e_failure;
    if(strcmp(strchr(argv[2], '.'), ".bmp") == 0)
	decInfo -> stego_image_fname = argv[2];


    if(argv[3] == NULL)
	decInfo -> output_file_name = "decode.txt";
    else
	decInfo -> output_file_name = argv[3];
	
    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{

    	decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "r");
	
	if(decInfo -> fptr_stego_image == NULL)
	{
	    	perror("fopen");
		fprintf(stderr,"Error: Unable to open file %s\n", decInfo -> stego_image_fname);
		return e_failure;
	}

	else
	{
	    	printf("Opened file: %s\n", decInfo -> stego_image_fname);
	}

	decInfo -> fptr_output_file = fopen(decInfo -> output_file_name, "w");

	if(decInfo -> fptr_output_file == NULL)
	{
	    	perror("fopen");
		fprintf(stderr,"Error: Unable to open file %s\n", decInfo -> output_file_name);
		return e_failure;
	}

	else
	{
	    	printf("Opened file: %s\n", decInfo -> output_file_name);
	}

	return e_success;
}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    size_t magic_string_length = strlen(magic_string);

    for (size_t i = 0; i < magic_string_length; i++)
    {
        fread(&decInfo->image_data, sizeof(char), 8, decInfo->fptr_stego_image);
        decode_lsb_to_byte(&decInfo->decode_data[i], decInfo->image_data);

        if (decInfo->decode_data[i] != magic_string[i])
        {
            return e_failure;
        }
    }

    return e_success;
}

Status decode_output_file_extn_size(DecodeInfo *decInfo)
{
	char file_size[DECODE_FILE_EXTN_SIZE];
	fread(file_size, sizeof(char), DECODE_FILE_EXTN_SIZE, decInfo -> fptr_stego_image);

	decInfo -> image_data_size = 0;
	for (uint i = 0; i < DECODE_FILE_EXTN_SIZE; i++)
	{
		decInfo -> image_data_size <<= 1;
		decInfo -> image_data_size |= (uint) (file_size[i] & 0x01);
	}
	
	return e_success;
}

Status decode_output_file_extn(uint extn_size, DecodeInfo *decInfo)
{
	for (uint i = 0; i < extn_size; i++)
	{
		fread(decInfo -> image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, decInfo -> fptr_stego_image);
		
		decode_lsb_to_byte(decInfo -> decode_data, decInfo -> image_data);
		
		decInfo -> extn_output_file[i] = decInfo -> decode_data[i];
	}
	return e_success;
}

Status decode_file_size(DecodeInfo *decInfo)
{
	char file_size[DECODE_FILE_SIZE];
	
	fread(file_size, sizeof(char), DECODE_FILE_SIZE, decInfo -> fptr_stego_image);
	
	decInfo -> image_data_size = 0;
	for (uint i = 0; i < DECODE_FILE_SIZE; i++)
	{
		decInfo -> image_data_size <<= 1;
		
		decInfo -> image_data_size |= (uint) (file_size[i] & 0x01);
	}
	return e_success;
}

Status decode_data_to_file(DecodeInfo *decInfo)
{
	for (uint i = 0; i < decInfo -> image_data_size; i++)
	{
		fread(decInfo -> image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, decInfo -> fptr_stego_image);
		
		decode_lsb_to_byte(decInfo -> decode_data, decInfo -> image_data);
		
		fwrite(decInfo -> decode_data, sizeof(char), 1, decInfo -> fptr_output_file);
	}
	return e_success;
}

Status decode_lsb_to_byte(char *decode_data, char *image_data)
{
	decode_data[0] = 0;
	for (uint i = 0; i < MAX_IMAGE_BUF_SIZE; i++)
	{
		decode_data[0] <<= 1;
		decode_data[0] |=  (image_data[i] & 0x01);
	}
	return e_success;
}



Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_success)
    {
        printf("Opened files successfully\n");

        uint raster_data;
        fseek(decInfo->fptr_stego_image, 0, SEEK_SET);
        fread(&raster_data, sizeof(int), 1, decInfo->fptr_stego_image);
        fseek(decInfo->fptr_stego_image, raster_data, SEEK_SET);

        if (decode_magic_string(MAGIC_STRING, decInfo) == e_success)
        {
            printf("Decoded magic string successfully\n");

            if (decode_output_file_extn_size(decInfo) == e_success)
            {
                printf("Decoded Output file extension size successfully\n");

                if (decode_output_file_extn(decInfo->image_data_size, decInfo) == e_success)
                {
                    printf("Decoded Output file extension successfully\n");

                    if (decode_file_size(decInfo) == e_success)
                    {
                        printf("Decoded file size successfully\n");

                        if (decode_data_to_file(decInfo) == e_failure)
                        {
                            fprintf(stderr, "Error: decode data to file() is failure\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Error: decode file size() is failure\n");
                        return e_failure;
                    }
                }
                else
                {
                    fprintf(stderr, "Error: decode output file extn() is failure\n");
                    return e_failure;
                }
            }
            else
            {
                fprintf(stderr, "Error: decode output file extn size() is failure\n");
                return e_failure;
            }
        }
        else
        {
            fprintf(stderr, "Error: Failed to decode magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open files\n");
        return e_failure;
    }

    return e_success;
}

