# PGCTALK

Originally by John Elliott, 13 November, 2004.
Posted to github and subsequent modifications by trguhq, 2025 and on.

This example program can be used to experiment with IBM's 
Professional Graphics Controller. PGCTALK lets you send commands to
the PGC and prints its replies (if any).

For best results, you should have two video cards in your PC, and have 
PGCTALK running on the one that isn't a PGC. Otherwise you'll keep having 
to switch between screens with DI 0 and DI 1 commands, and when the PGC 
is in native mode you won't be able to see what you're typing.

If a command line contains the character '!', PGCTALK will be shut down.

If a command line starts with '*', then the remainder is treated as a 
sequence of Hex bytes. The PGC will be put into Hex mode, the bytes will
be sent, and then the PGC will be returned to ASCII mode. For example, to 
display command list 1, the Hex command would be

```*7501```

Note this program was written more as a programming example
than a comprehensive utility. So error handling and ease of use have
been rather neglected.
