**AString** is a compiler portable C++ String Class with some enhanced features.

Works on : GCC, Visual Sudio, C++ Builder
##Description
AString is a library providing a C++ Ansi (8 bits char set) String class

I've create it because, I work with several compiler and it's often difficult to decide which string class I should use,
because some compiler have their own framework string class (Visual Studio CString, QT QString,C++ Builder AnsiString) and all of them support std::string and C char`*`.
I could always use std:string but framework's strings are generally a bit more richer in features.
I'm also doing multi-threading but on pre-c++ 11 there's no guarantee std::string works fine, you have to investigate, sometimes it's ok, sometimes it's not OK.

AString provide the following features :
  * an interface similar (not identical) to std::string
  * Can be implemented with std:string or not (#define based). If not, it's a simple buffer based implementation (easy to debug)
  * include interoperability with frameworks string class  (C++ Builder,CString )
  * include constructor from numbers (int, float, double, ... ) and also String To Number methods (ToInt, ... )
  * include sprintf/format like feature
  * include a secured strncpy char`*` copier
  * include implode and explode (split/merge of string array)
  * Buffer based implementation can be directly used on printf with %s on some compiler
  * Can easily be modified

##Documentation
Documentation is generated using Doxygen, available online here http://www.altidev.com/ln/AString/ or at chm format here http://www.altidev.com/ln/AString/AString.chm and on the repository.

##DEMO
```C++
// Basic assignement and display
AString SHello("Hello");
AString SWorld,Res;
SWorld="World";
Res=SHello+" "+SWorld;
cout << Res << endl;
if((SHello=="Hello")&&(SWorld<"Z"))
   cout << "Comparison OK" << endl;
// OStringStream behavior integrated. Simplify transformation
SHello<<" append "<<10;
cout << SHello << endl;
// sprintf like format returning a astring but without handling
cout << Asprintf("Like sprintf %d %.02f %s",1,2.5,"ABC")<< endl;
// or with the result on the variable
SHello.Format("\tHello %d %s X ",10,"World");
cout<<SHello.trim("X \r\n\t")<<endl;
// Replace string by string
cout<<SHello.replace("World","Friends")<<endl;
// Safe strncpy
char Dest[10]="123456789";
SHello.strncpy(Dest,5); // 5 size of the buffer copy max 4 char+ 0
cout<<"strncpy always zero ended "<<Dest<<endl;
// Easy conversion from and To int with exception or default value
cout<< (AString(10+5).ToInt()+3)<<"  "<<SHello.ToIntDef(999)<<endl;
// Explode and implode
AString S3("Hello dear friends");
std::deque<AString> Ds;

cout<<"explode count="<<S3.explode(" ",Ds);;
cout<<" implode="<<AString::implode("-",Ds)<<endl;
```
Providing this result :
```
 Hello World
 Comparison OK
 Hello append 10
 Like sprintf 1 2.50 ABC
 Hello 10 World
 Hello 10 Friends
 strncpy always zero ended Hell
 18  999
 explode count=3 implode=Hello-dear-friends
```
##Todo
  * Add missing std::string reimplementation (rfind,...)
  * Add some features  ToUpper, ToLower
  * Add some useful string simple algorithms
    * ToIntFromHex "0x20" or "20" to 32 , ToIntFromBin "0101" to 5
    * assignAsHex 32 to "0x20" or "20" optional 0x
    * assignAsBin 5 to "0101" parameter to set width and space on result (0101 0011 if param=4)
  * Finalize/improve documentation and tests, test on Clang
  * Add interaction with some others frameworks (QT, WxWidget, ... )
  * Improve implementation of CString interoperability (make constructor param const by avoiding get buffer

##Test details
I'm using the TDD logic for this library, so it include mainly project to run CPP Unit Project include test project on
  * CodeBlock with Mingw GCC in Windows 32 bits
  * CodeBlock with GCC on Linux Fedora 17 32bits
  * CodeBlock with GCC on Linux Debian 7 64bits
  * Visual Sutdio 2010 32 & 64 bits
  * C++ Builder XE2 32 bits

I've done memory check using visual leak detector on Visual Studio http://vld.codeplex.com/ 
 And DrMemory on Windows 
 And valgring on Linux, here command to test : 
``` 
  * valgrind –tool=memcheck bin/Debug/AStringUT
  * valgrind –leak-check=full –tool=memcheck bin/Debug/AStringUT 
```  
I've done coverage test on GCC using GCov. 
I've used CppCheck also.



##Release History
  * version 0.1 2013-08-22 Initial
  * version 0.2 2013-12-29
    * Adding Replace of string by a string
    * Adding trim, ltrim, rtrim 
  * version 0.3 2014-05-02
    * Bugfix on comparison operator
    * Added Char* on left of comparison operator
    * Added demo project
  * version 0.4 2014-09-03
    * Added test for 100% coverage on Gcc
    * Minor Bugfix in case of reserve(0)
