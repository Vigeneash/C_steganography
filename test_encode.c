#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
    if(check_operation_type(argv) == e_encode)
    {
	printf("Selected encoding\n");
	EncodeInfo encInfo;
	if(read_and_validate_encode_args(argv, &encInfo) == e_success)
	{
	    printf("Read and validate encode arguments is a success\n");

	    if(do_encoding(&encInfo) == e_success)
	    {
		printf("Encoding is completed successfully\n");
	    }
	    else
	    {
		printf("Encoding is not completed successfully");
		return 1;
	    }
	}
	else
	{
	    printf("Read and validate encode arguments is a failure\n");
	    return 1;
	}
    }
    else if(check_operation_type(argv) == e_decode)
    {
	printf("Selected decoding\n");
	DecodeInfo decInfo;
	if(read_and_validate_decode_args(argv, &decInfo) == e_success)
	{
	    printf("Read and validate decode arguments is a success\n");
	    
	    if(do_decoding(&decInfo) == e_success)
	    {
		printf("Decoding is completed successfully\n");
	    }
	    else
	    {
		printf("Decoding is not completed successfully\n");
		return 1;
	    }
	}
	else
	{
	    printf("Read and validate decode arguments is a failure\n");
	    return 1;
	}
    }
    else
	printf("Invalid input!!\nUsage for encoding:\n./a.out -e beautiful.bmp secret.txt [stego.bmp]\nUsage for decoding:\n./a.out -d stego.bmp [decode.txt]\n");
    }
    else
	printf("Usage for encoding:\n./a.out -e beautiful.bmp secret.txt [stego.bmp]\nUsage for decoding:\n./a.out -d stego.bmp [decode.txt]\n");
    return 0;
}
