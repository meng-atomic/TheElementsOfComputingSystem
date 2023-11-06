  @R2
  M=0
  @i
  M=0
(LOOP)
  @i  // load i's address in A-register
  D=M // store i value in D-register
  @R1 // load R1's address in A-register, namely RAM[1]
  D=D-M // calculate i - MEM[R1], if (i - MEM[R1]) >= 0; break
  @END  // load END in A-register, Jump to END if condition is met
  D;JGE
  @i    // load i's address in A-register
  M=M+1 // performs i = i + 1
  @R0   // load R0's address in A-Register
  D=M   // load MEM[R0] in D-register
  @R2   // load R2's address in A-Register
  M=D+M // performs R2 = R2 + R0
  @LOOP
  0;JMP
(END)
  @END
  0;JMP