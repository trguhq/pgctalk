/*
Copyright (c) 2004 John Elliott <jce@seasip.demon.co.uk>

This software is provided 'as-is', without any express or implied warranty. 
In no event will the authors be held liable for any damages arising from the 
use of this software.

Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it 
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not 
claim that you wrote the original software. If you use this software in a 
product, an acknowledgment in the product documentation would be appreciated
but is not required.

    2. Altered source versions must be plainly marked as such, and must not 
be misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.

*/

/* This program is a simple interface to the PGC command buffer. It allows
 * the user to type arbitrary commands and see the results.
 *
 * For best results, have a 2-head system with this program running on the
 * non-PGC head. Otherwise you'll keep having to switch to and fro with 
 * DI 0 and DI 1 commands, and won't be able to see what you're typing.
 */

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>
#include <conio.h>

typedef unsigned char byte;

/* Base address of the PGC transfer buffer*/
#define gl_pgc ((byte far *)MK_FP(0xC600, 0))

/* PGC ring buffer pointers */
#define IN_WRPTR (gl_pgc[0x300])
#define IN_RDPTR (gl_pgc[0x301])
#define OUT_WRPTR (gl_pgc[0x302])
#define OUT_RDPTR (gl_pgc[0x303])
#define ERR_WRPTR (gl_pgc[0x304])
#define ERR_RDPTR (gl_pgc[0x305])

/* Write a byte to the PGC command buffer. */
void pgc_write(byte b)
{
	gl_pgc[IN_WRPTR] = b;
	++IN_WRPTR;
}

/* Read a byte from the PGC output buffer. Returns EOF if no data available. */
int pgc_rdout()
{
	int rv;

	if (OUT_WRPTR == OUT_RDPTR) return EOF;

	rv = gl_pgc[0x100 + OUT_RDPTR];
	++OUT_RDPTR;
	return rv;	
}

/* Read a byte from the PGC error buffer. Returns EOF if no data available. */
int pgc_rderr()
{
	int rv;

	if (ERR_RDPTR == ERR_WRPTR) return EOF;

	rv = gl_pgc[0x200 + ERR_RDPTR];
	++ERR_RDPTR;
	return rv;	
}

/* Write an ASCII command to the PGC. */
void pgc_cmd(const char *s)
{
	while (*s)
	{
		pgc_write(*s);
		++s;
	}
}

/* Write an ASCII command to the PGC. Display any results returned. */
void pgc_say(const char *s)
{
	int ch, n;

	pgc_cmd(s);

	n = 0;
	while ((ch = pgc_rdout()) != EOF)
	{
		if (!n) { printf("Out: "); ++n; }
		putchar(ch);
	}
	if (n) printf("\n");
	n = 0;
	while ((ch = pgc_rderr()) != EOF)
	{
		if (!n) { printf("Err: "); ++n; }
		putchar(ch);
	}
	if (n) printf("\n");

}

/* Write a Hex command to the PGC. Display any results returned as Hex.
 *
 * The PGC is switched to Hex mode before the command is executed, and 
 * back to ASCII mode afterwards.
 *
 */
void pgc_xsay(const char *s)
{
	char buf[128], *p;
	char x[3];
	int hexv, n, ch;

/* Switch to Hex mode */
	pgc_say("CX\n");

/* Extract only hex digits from the passed string */
	p = buf;
	while (*s)
	{
		if (isxdigit(*s)) *p++ = *s;
		++s;	
	}
	*p = 0;
	p = buf;
/* Parse each pair of digits as a hex byte, and feed that to the PGC */
	while (*p)
	{
		sprintf(x, "%-2.2s", p);
		if (strlen(p) < 2) p = "";
		else p += 2;
		sscanf(x, "%x", &hexv);
		printf("%02x ", hexv);
		pgc_write(hexv);
	}
	printf("\n");
/* Read output and error, and render them as Hex. */
	n = 0;
	while ((ch = pgc_rdout()) != EOF)
	{
		if (!n) { printf("Out: "); ++n; }
		printf("%02x ", ch);
	}
	if (n) printf("\n");
	n = 0;
	while ((ch = pgc_rderr()) != EOF)
	{
		if (!n) { printf("Err: "); ++n; }
		printf("%02x ", ch);
	}
	if (n) printf("\n");
/* Switch back to ASCII mode */
	pgc_say("CA\n");
}



/* Main loop. Commands starting with '*' are parsed as hex bytes; others
 * are treated as ASCII. A '!' character anywhere in its input causes 
 * PGCTALK to terminate. */
int main(int argc, char **argv)
{
	char buf[128];

	do
	{
		printf("Enter command (! to quit; start with * for hex cmd)\n");
		fgets(buf, sizeof(buf), stdin);
		if (strchr(buf, '!')) return 0;
		if (buf[0] == '*')
		{
			pgc_xsay(buf);
		}
		else
		{
			pgc_say(buf);
		}
	}
	while (1);
	return 0;
}
