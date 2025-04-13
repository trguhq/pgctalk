PGCTALK / PGCBMP                               John Elliott, 13 November 2004
=============================================================================

  These are example programs which can be used to experiment with IBM's 
Professional Graphics Controller. The two programs are:

* PGCBMP - loads and displays 256-colour .BMP files in the native 640x480
          mode. Syntax is:

		PGCBMP <bmpfile>

 The PGC will be put in native mode and the bitmap will be displayed. The 
image will remain onscreen until you press ESCAPE; then the screen will 
switch back to CGA mode and the program will quit. 

 To abort display before the picture has finished loading, press ESCAPE once 
(to stop loading) and again (to return to DOS). 

* PCTALK - lets you send commands to the PGC and prints its replies (if any).

  For best results, you should have two video cards in your PC, and have 
PGCTALK running on the one that isn't a PGC. Otherwise you'll keep having 
to switch between screens with DI 0 and DI 1 commands, and when the PGC 
is in native mode you won't be able to see what you're typing.

  If a command line contains the character '!', PGCTALK will be shut down.

  If a command line starts with '*', then the remainder is treated as a 
sequence of Hex bytes. The PGC will be put into Hex mode, the bytes will
be sent, and then the PGC will be returned to ASCII mode. For example, to 
display command list 1, the Hex command would be

*7501

                               - * -

  Note that both of these programs are written more as programming examples 
than as comprehensive utilities. So error handling and ease of use have
been rather neglected.
