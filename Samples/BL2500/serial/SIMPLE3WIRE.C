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
	simple3wire.c
	
	This program is used with BL2500 series controllers.
	
	Description
	===========
	This program demonstrates basic initialization for a
	simple RS232 3-wire loopback displayed in STDIO window.
	
	Proto-board Connections
	=======================
	For this simple demonstration, make the following
	connections:
	
		TxE <---> RxF
		RxE <---> TxF

	Instructions
	============
	1.  Compile and run this program.
	2.  Lower case characters are sent by TxE. RxF receives
	    the character.  TxF sends the converted uppercase
	    character to RxE and displays in STDIO.

**********************************************************/
#class auto

///////
// change serial buffer name and size here
// for example:  CINBUFSIZE to DINBUFSIZE
///////
#define EINBUFSIZE  15
#define EOUTBUFSIZE 15

#define FINBUFSIZE  15
#define FOUTBUFSIZE 15

///////
// change serial baud rate here
///////
#ifndef _232BAUD
#define _232BAUD 19200
#endif


main()
{
	auto int nIn1, nIn2;
	auto char cOut;

	brdInit();				//initialize board for this demo
	serEopen(_232BAUD);
	serFopen(_232BAUD);
	serEwrFlush();
	serErdFlush();
	serFwrFlush();
	serFrdFlush();
		
	while (1)
	{
		for (cOut='a';cOut<='z';++cOut)
		{			
			serEputc (cOut);								//	Send lowercase byte
			while ((nIn1=serFgetc()) == -1);			// Wait for echo
			serFputc (toupper(nIn1));					//	Send the converted upper case byte
			while ((nIn2=serEgetc()) == -1);			// Wait for echo
			printf ("Serial E sent %c, serial F sent %c, serial E received %c\n",
						cOut, toupper(nIn1), nIn2);
		}
	}
}

