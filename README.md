# Protogen
* Protogen takes in byte code .protogen files and runs them.
* To run, provide the .protogen file as a command line argument to the compiled .pyc file.
* To edit, I'd reccomend getting the HEX-Editor plugin for Notepad++ or some other HEX editor because you can't really do anything otherwise.
* Here's how it works:
*  The bytes are split up into lines. I tend to use lines of length four, but any power of two can be used (unless it's really big).
*  The first line of the program is used for formatting, and the first byte of the first line is what power of two to use for the line width. (I use \x02\x00\x00\x00 for the first line.)
*  
*  Each line after that does actuall stuff.
*  The first byte of the line is used to determine what will happen, and the bytes after are arguments for the opperation.
*  Here's a list of the opperations:
*   (A) The second byte is put into regester A, and the rest are ignored
*   (B) The second byte is put into regester B, and the rest are ignored 
*   (a) The bytes after the first are used to make an address. (a\x04\x00\x00 will go to the second line, or beginning, of the program.) This address is used to get the value of the byte at the address, and this value is put into regester A.
*   (b) Same as (a), but with regester B
*   (c) Gets the address in the same way as (a), but replaces that byte with the contents of regester C
*   (e) Same as (c), but with regester E
*   (+) Adds the regesters A and B together and stores the result in regester C. Regester E is set to 0 if no overflow occurs and 1 if overflow occurs.
*   (-) Subtracts regester B from regester A (A-B) and stores the result in regester C. Regester E is set to 0 if no underflow occurs and 255 if underflow occurs.
*   (&) A bitwise and is preformed on regesters A and B to store in regester C. Regester E is set to 0.
*   (|) A bitwise or is preformed on regesters A and B to store in regester C. Regester E is set to 0.
*   (^) A bitwise xor is preformed on regesters A and B to store in regester C. Regester E is set to 0.
*   (j) The program jumps to the address, which is found the same way as (a). (j\x00\x01\x00 will jump to line 64)
*   (>) Jumps like (j) only if regester A > regester B
*   (=) Jumps like (j) only if regester A == regester B
*   (<) Jumps like (j) only if regester A < regester B
*   (r) Jumps like (j), but once the program hits a (\x00) the line after this one is jumped to.
*   (\x00) This indecates the end of a piece of code and either ends the program or pops a value off the stack of addresses to return to provided by (r).
*   (,) Takes a character as input and stored the byte in regester C. This can be repeated for each character of input, and regester E will be one for the last character inputted and zero for all before.
*   (.) Outputs regester A as a character (unicode in this case, but ASCII would be better.)
*   (;) Takes a number as input and stores the byte in regester C.
*   (:) Outputs regester A as an integer.
*   (?) Sets the C regester to a random number between regester A and B inclusive
*  
*  Regesters A and B are input, and regester C and E are output. Regester C is the primary output, and regester E is used to indecate possible errors like over/underflow.
