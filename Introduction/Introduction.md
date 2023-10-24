# Introduction: Hello, World Below

The true voyage of discovery consists not of going to new places, but of having a new pair of eyes.

## The World Above
```C++
#include <iostream>

int main(int argc, char** argv) {
  std::cout << "Hello, World" << std::endl;
  return 0;
}
```
这样一段简单的程序，需要经过语法词法解析，转换成计算机可以理解的机器语言（编译）,这些机器语言又需要转化到特定的硬件架构上。  
硬件架构是由一堆芯片-寄存器、内存、计算单元(ALU)等构成。门电路可以由基础的与非门和与或门搭建。每个门电路都是又几个切换设备（典型的是晶体管）构成。至于晶体管的构成和工作原理已经超出计算机科学的范围，属于物理学范畴了。  
对底层系统的深入程度是初级程序员和高级开发者区别之一。  
人类的抽象思维能力使得我们可以将一个复杂的计算机系统拆解为相互独立的模块。模块之间通过抽象层连接。抽象通常是逐层构建的，从而获得更高级的系统功能。  
设计高水平的抽象是一项实践艺术，本书基于抽象-实现范式。每一章节都给出硬件或软件的关键抽象，并通过更低一层的基础模块实现它。