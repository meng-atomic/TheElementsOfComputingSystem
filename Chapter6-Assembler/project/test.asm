  @8192
  D=A
  @max
  M=D

  @16384
  D=A
  @base
  M=D

(LOOP)
  @KBD
  D=M
  @WHITE // if D=0,that means no key pressed, so clears the screen(all white)
  D;JEQ
  @BLACK
  D;JGT

(WHITE)
  @i
  M=0 // Reset index
(WHITELOOP)
  @i
  D=M
  @max
  D=D-M
  @LOOP
  D;JGE
  // get address
  @i
  D=M
  @base
  D=D+M // D hold pointer to screen memory
  A=D
  M=0
  @i
  M=M+1
  @WHITELOOP
  0;JMP

(BLACK)
  @i
  M=0 // Reset index
(BLACKLOOP)
  @i
  D=M
  @max
  D=D-M
  @LOOP
  D;JGE
  // get address
  @i
  D=M
  @base
  D=D+M // D hold pointer to screen memory
  A=D
  M=-1
  @i
  M=M+1
  @BLACKLOOP
  0;JMP
(END)