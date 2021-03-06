/*
   Copyright (c) 2015, Digi International Inc.

   Permission to use, copy, modify, and/or distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
/**********************************************************
	icom232.c

	This sample program is used with products such as
	Intellicom Series and Rabbit TCP/IP Development Kits.

	This program demonstrates a simple RS232 loopback
	displayed in STDIO window.

	Serial port C on parallel port C's alternate function
	(TxC) is used.

	Make the following connection on J7.

		Tx <---> Rx

**********************************************************/
#class auto 			// Change default storage class for local variables: on the stack

// serial C buffer size
#define CINBUFSIZE  15
#define COUTBUFSIZE 15

// serial baud rate
#ifndef _232BAUD
#define _232BAUD 115200
#endif



//////////////////////////////////////////////////////////

void main()
{
	int nIn;
	char cOut;

	serCopen(_232BAUD);
	serMode(0);
	serCwrFlush();
	serCrdFlush();

	while (1) {
		for (cOut='a';cOut<='z';++cOut) {			//	Send Classified Sequence
			serCputc (cOut);								//	Send Byte
			while ((nIn=serCgetc()) == -1);			// Wait for echo
			printf ("Upper case %c is %c\n",cOut,toupper(nIn));	//	Display Byte vs. Echo
			}
		}
}
//////////////////////////////////////////////////////////

