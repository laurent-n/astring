/**
\mainpage AString Main Page
\author Laurent NAVARRO ln@altidev.com
\version 0.3 2014-05-02
\copyright MIT X11 : See  .h file header
Copyright (C) 2013 Laurent NAVARRO
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL LAURENT NAVARRO BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the Laurent NAVARRO shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the Laurent NAVARRO.
\section desc Description
AString is a library providing a C++ Ansi (8 bits char set) String class

I've create it because, I work with several compiler and it's often difficult to decide which string class I should use,
because some compiler have their own framework string class (Visual Studio CString, QT QString,C++ Builder AnsiString) and all of them support std::string and C char*.
I could always use std:string but framework's strings are generally a bit more richer in features.
I'm also doing multi-threading but on pre-c++ 11 there's no guarantee std::string works fine, you have to investigate, sometimes it's ok, sometimes it's not OK.

AString provide the following features :
- an interface similar (not identical) to std::string
- Can be implemented with std:string or not (#define based). If not, it's a simple buffer based implementation (easy to debug)
- include interoperability with frameworks string class  (C++ Builder,CString )
- include constructor from numbers (int, float, double, ... ) and also String To Number methods (ToInt, ... )
- include sprintf/format like feature
- include a secured strncpy char* copier
- include implode and explode (split/merge of string array)
- Buffer based implementation can be directly used on printf with %s on some compiler
- Can easily be modified

\subsection doc Documentation
Documentation is generated using Doxygen,
available online here <a href="http://www.altidev.com/ln/AString/">http://www.altidev.com/ln/AString/</a>
or at chm format here <a href="http://www.altidev.com/ln/AString/AString.chm">AString.chm</a> and on the repository.

\subsection impl Implementation details
if \c ASTRING_IMPLEMENT_WITH_STD_STRING is set to 0 the string is implemented by a pointer on a buffer. \n
However a specific tips was used to allow string to be used on %s printf like function on some compiler inspired from Chris OldWood blog A Not-So-Clever-Now String Implementation \n
The tips consists to have a pointer pointing on the full buffer itself but on the char* section of the buffer
and to compute with offset to access to stored size and capacity attributes as described here.
\code
+--------------+
+ Size         +   <-- StrBuffer - 2*sizeof(unsigned) or GetSize() or GetRawBuffer()
+--------------+
+ Capacity     +   <-- StrBuffer - 2*sizeof(unsigned) or GetCapacity()
+--------------+
+ char* Buffer +   <-- StrBuffer Attribute, conatains zero terminated characters string.
+--------------+
\endcode
\todo
- Add missing std::string reimplementation (rfind)
- Add some features  ToUpper, ToLower
- Add some useful string simple algorithms
    - ToIntFromHex "0x20" or "20" to 32 , ToIntFromBin "0101" to 5
    - assignAsHex 32 to "0x20" or "20" optional 0x
    - assignAsBin 5 to "0101" parameter to set width and space on result (0101 0011 if param=4)
- Finalize/improve documentation and tests, test on Clang
- Add interaction with some others frameworks (QT, WxWidget, ... )
- Improve implementation of CString interoperability (make constructor param const by avoiding get buffer

\subsection test Test details
I'm using the TDD logic for this library, so it include mainly project to run CPP Unit
Project include test project on
- CodeBlock with Mingw GCC in Windows 32 bits
- CodeBlock with GCC on Linux Fedora 17 32bits
- CodeBlock with GCC on Linux Debian 7 64bits
- Visual Sutdio 2010 32 & 64 bits
- C++ Builder XE2 32 bits

I've done memory check using visual leak detector on Visual Studio http://vld.codeplex.com/ \n
And valgring on Linux, here command to test : \n
- valgrind --tool=memcheck bin/Debug/AStringUT
- valgrind --leak-check=full --tool=memcheck bin/Debug/AStringUT

\subsection release Release History
- version 0.1 2013-08-22 Initial
- version 0.2 2013-12-29
    - Adding Replace of string by a string
    - Adding trim, ltrim, rtrim
- version 0.3 2014-05-02
    - Bugfix on comparison operator
    - Added Char* on left of comparison operator

*/
#ifndef ASTRING_H_INCLUDED
#define ASTRING_H_INCLUDED
//---------- Write your settings Here -------------------
//#define ASTRING_IMPLEMENT_WITH_STD_STRING 1


#if _MSC_VER
#define ASTRING_INTEROP_VSS 1
#endif
//#define ASTRING_NOSTL 1
//---------- End of your settings  -------------------

#ifndef ASTRING_IMPLEMENT_WITH_STD_STRING
  /**
  Define the implementation type.
  Define if AString is implemented by inheritence of std::string or by a buffer based implementation
  Default = 0 meaning implemented by buffer based implementation
  *******************************************/
  #define ASTRING_IMPLEMENT_WITH_STD_STRING 0
#else
 // Implementation with std::string require to interop with std::string
 #define ASTRING_INTEROP_STD_STRING 1
#endif
#ifndef ASTRING_NOSTL
  /** Allow to remove all references and features relating the STL.
   *  Not compatible with ASTRING_IMPLEMENT_WITH_STD_STRING = 1
   *  Default value=0
   */
  #define ASTRING_NOSTL 0
#endif
#ifndef ASTRING_INTEROP_STD_STRING
  /** Enable interoperability with std::string.
   *  Default value=0
   *  (forced when ASTRING_IMPLEMENT_WITH_STD_STRING = 1)\n
   *  Means constructor and cast operator for std::string
   */
  #define ASTRING_INTEROP_STD_STRING 0
#endif
// C++ Builder VCL interop
#ifndef ASTRING_INTEROP_BCB
	#ifdef __BCPLUSPLUS__
  /** Enable interoperability with C++ Builder AnsiString and UnicodeString
   *  Default value=1 on C++ Builder
   *  Means constructor and cast operator for theses classes
   */
	  #define ASTRING_INTEROP_BCB 1
	#else
	  #define ASTRING_INTEROP_BCB 0
	#endif
#endif
// Visual Studio CString interop
#ifndef ASTRING_INTEROP_VSS
  /** Enable interoperability with Visual Studio CString
   *  Default value=0
   *  Means constructor and cast operator for theses classes
   */
	#define ASTRING_INTEROP_VSS 0
#endif

#if ASTRING_INTEROP_VSS
	#include <atlstr.h>
#endif
#if ASTRING_INTEROP_BCB
	#include<vcl.h>
#endif

#if !ASTRING_NOSTL
#include <iostream>
#include <sstream>
#include <deque>
#include <exception>
#endif
#if ASTRING_IMPLEMENT_WITH_STD_STRING || ASTRING_INTEROP_STD_STRING
#include <string>
#endif


class AString
#if ASTRING_IMPLEMENT_WITH_STD_STRING
:public std::string
#endif
{
    public:
    /**
    Default constructor.
    Construct an empty string
    ****************************************************/
    AString();
    /**
    Char *constructor.
    Construct an string by copying a C String.
    \param str source string
    \param count Number of used caracters, if <0 means the full stream.
    ****************************************************/
    AString(const char *str,int count=-1);
    /**
    Copy constructor.
    \param Src source string
    ****************************************************/
    AString(const AString &Src);
    #if ASTRING_INTEROP_STD_STRING
    /**
    std::string constructor.
    \param Src source string
    ****************************************************/
    AString(const std::string &Src);
    #endif
    /**
    Assignement operator from AString.
    \param Src source string
    ****************************************************/
    AString& operator =(const AString& Src);
    #if __cplusplus > 199711L // C++ 11
    #define ASTRING_NOEXCEPT noexcept
        /**
        C++ 11 Move construtor.
        \param Src source string
        ****************************************************/
        AString(AString &&Src) ASTRING_NOEXCEPT;
        /**
        C++ 11 Assignement move operator .
        \param Src source string
        ****************************************************/
        AString& operator =(AString&& Src) ASTRING_NOEXCEPT;
    #else // C++ 98
    #define ASTRING_NOEXCEPT throw()
    #endif
    /**
    int constructor.
    Construct a text representation of Value
    \param value value to convert as text
    ****************************************************/
    AString(int value);
    /**
    unsigned constructor.
    Construct a text representation of Value
    \param value value to convert as text
    ****************************************************/
    AString(unsigned value);
    /**
    double constructor.
    Construct a text representation of Value
    \param value value to convert as text
    ****************************************************/
	AString(double value);
	/**
	destructor.
	****************************************************/
	~AString() ASTRING_NOEXCEPT;
#if ASTRING_INTEROP_BCB
	AString(const AnsiString &src);
	AString(const UnicodeString &src);
	operator AnsiString ();
	operator UnicodeString ();
#endif
#if ASTRING_INTEROP_VSS
	AString(CStringA &src);
	operator CStringA ();
	AString(CStringW &src);
	operator CStringW ();
#endif

#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    /**
    STL like iterator.
    ****************************************************/
    typedef char* iterator;
    /**
    STL like container begin.
    return an iterator on the begining of the string
    ****************************************************/
    iterator begin();
    /**
    STL like container end.
    return an iterator on the character following the end of the string
    ****************************************************/
    iterator end();
    /**
    get pointer on the string.
    return a read/write pointer on the string\n
    if you add or remove a '\0' char throught provided pointer you have to call RecomputeSize().
    @return pointer on string
    ****************************************************/
    char* c_str();
    /**
    get a const pointer on the string.
    return a read only pointer on the string\n
    @return const pointer on string
    ****************************************************/
    const char* c_str() const ;
    /**
    get the number of character contained in the string.
    @return character count
    ****************************************************/
    size_t size() const;
    /**
    get the maximum number of character that can be contained in the string whithout reallocatig memory.
    @return string capacity
    ****************************************************/
    size_t capacity() const;
    /**
    Force the string to allocate supplementary space.
    \param count new string capacity
    ****************************************************/
    void reserve(size_t count);
    /**
    Clear the string.
    After this method call string is empty and Size=Capacity=0
    ****************************************************/
    void clear();

    char& operator[](size_t idx);
    char operator[](size_t idx) const;
    char& at(size_t idx);
    char at(size_t idx) const;
    AString& assign(const char *str,int count=-1);

    AString operator +(const AString &RightStr) const;
    AString operator +(const char *RightStr) const;
    friend AString operator +(const char *LeftStr,const AString &RightStr);
    /**
    Append a string to the current string.
    If the current string has available space, it will be done by extending the current string.
    It's more efficient than \c S1=S1+S2; \n
    To be really efficient, its better to anticipage space requirement by using reserve()
    \code
    S1.reserve(20);
    S1+="My String";
    \endcode
    \param RightStr the appended string
    \return the current string
    ****************************************************/
    AString& operator +=(const AString &RightStr) ;
    bool operator ==(const AString &RightStr) const;
    friend bool operator ==(const char *LeftStr,const AString &RightStr);
    bool operator ==(const char* RightStr) const;
    bool operator !=(const AString &RightStr) const;
    bool operator !=(const char *RightStr) const;
    friend bool operator !=(const char *LeftStr,const AString &RightStr);
    bool operator <(const AString &RightStr) const;
    bool operator <(const char *RightStr) const;
    bool operator >(const AString &RightStr) const;
    bool operator >(const char *RightStr) const;
    friend bool operator <(const char *LeftStr,const AString &RightStr);
    friend bool operator >(const char *LeftStr,const AString &RightStr);
    AString substr( unsigned pos = 0,unsigned count = npos ) const;
    size_t find(const char* Searched,unsigned StartPos=0,unsigned count=0) const;
    size_t find(char Searched,unsigned StartPos=0) const;
#endif //  ! ASTRING_IMPLEMENT_WITH_STD_STRING
    /**
    Replace a set or character by a string on the current string.
    \param pos position where the substitition start
    \param count number of character to substitute
    \param str string to insert on the specified location
    \return the current string
    */
    AString& replace( size_t pos, size_t count,const AString& str );
    /**
    Replace a substring by a nzw string on the current string.
    \param src the substring to replace
    \return the current string
    */
    AString& replace( const AString& src,const AString& str );
    AString& rtrim(const char *CharsToRemove=" \r\n\t");
    AString& ltrim(const char *CharsToRemove=" \r\n\t");
    AString& trim(const char *CharsToRemove=" \r\n\t");
    /**
    Securely copy the current string on a C buffer.
    If the string is longer than what can be hold on the destination buffer, string will
    be truncated.\n
    There's always an ending zero added on the destination buffer
    \code
    char buf[16];
    AString S1("0123456789ABCDEF");
    S1.strncpy(buf,4,5);
    // buf == "567" +ending zero, then 4 characters
    S1.strncpy(buf,5);
    // buf=="0123"
    S1.strncpy(buf,sizeof(buf),10);
    // buf=="ABCDEF"
    \endcode
    \param dest destination C Buffer
    \param destsize size of the buffer
    \param pos position where the copy will occur
    ****************************************************/
    void strncpy( char* dest,size_t destsize,unsigned pos = 0) const;

    /**
    generate a string on a similar manner than sprintf.
    And store the result on the current string
    \code
    S3.Format("Y %d %.02f %s",10,2.52,"DEF");
    // S3 ="Y 10 2.52 DEF"
    \endcode
    \param dest destination C Buffer
    \param destsize size of the buffer
    \param pos position where the copy will occur
    ****************************************************/
    AString& Format(const char *Fmt,...);
    friend AString Asprintf(const char *Fmt,...);

    /** Recompute Size internal attribute based on Content.
     *  Required when using [] operator or direct buffer access you change the size by moving the terminal zero character.
     * \return void
     */
    void RecomputeSize();

    /** Convert the content of the string containg a textual representation of a number to an int.
     * \return int
     * \exception if the text doesn't represent an integer value.
     */
    int ToInt();
    /** Convert the content of the string containg a textual representation of a number to an int an return defval
     * \param defval default value if text can't be converted
     * \return int
     * \exception none
     */
    int ToIntDef(int defval);

#if !ASTRING_NOSTL
    /**
    operate similary with ostringstream.
    Append to the Left string with the textual representation ofthe right parameter and return the current string.
    \code
	AString S2("A");
	S2<<15<<" XXX "<<2.5<<" XXX "<<S1;
	Result : S2 = "A 15 XXX 2.5 XXX 0123456789ABCDEF"), S2 );
	\endcode
    \param Streamable A variable that can be outputed on a stream as text
    ****************************************************/
    template <class T>
    AString& operator<<(const T &Streamable);

    /**
    implode strings to a string whit a separator.
    Merge/join/agregate a collection of string contained on a STL like container to an unique string
    whith separator added between each string.
    \param Separator the separator added between each string.
    \param Ctnr an STL like container of AString : vector<AString>
    ****************************************************/
    template <class T>
    static AString implode(AString Separator,const T &Ctnr);

    /** explode a string in a collection of string (reverse of implode).
     * Split a string in a collection of substring each time Separator is met.
     Result must be a STL like container, deque is suggested.
     * \param Separator the searched separator
     * \param Result the target collection a deque<AString>
     * \return the number if string returned = size of the result container.
        \code
        AString S3("Hello dear friends");
        std::deque<AString> Ds;
        unsigned res=S3.explode(" ",Ds);
        // Result : res=3, Ds contains 2 values : "Hello" ,"dear", "friends"
        \endcode
     */
    template <class ResultContainerType>
    unsigned explode(AString Separator,ResultContainerType &Result);

#endif

#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    static const size_t npos;

    protected:
    char* StrBuffer; /**< offset buffer */
    char* GetRawBuffer();/**< the real address of the buffer */
    unsigned& GetSize(); /**< R/W access to the offset position of size on the buffer */
    unsigned& GetCapacity(); /**< R/W access to the offset position of capacity on the buffer */
    unsigned GetSize() const; /**< Read Only access to the offset position of size on the buffer */
    unsigned GetCapacity() const; /**< Read Only access to the offset position of capacity on the buffer */
    static const char EmptyBuffer[4]; /**< Static const shared buffer for empty string*/
    /** Allocate an internal buffer of a capacity of count character.
     *  + final zero + Control area or point to EmptyBuffer
     * \param count requested capacity
     * \return void
     */
    void AllocBuffer(unsigned count);
#endif
}; // class AString
#if !ASTRING_NOSTL
std::ostream& operator<<(std::ostream& os, const AString& str);
#endif
AString Asprintf(const char *Fmt,...);

/** AString Exception class, inherit from std::exception.
    Can be inherited for your own exception with a AString contructor.
 */
class AStringException
#if !ASTRING_NOSTL
    : public std::exception
#endif
{
    public:

    /** Contruct the exception with a AString message
     * \param _Msg Message of the exception
     */
    AStringException(AString _Msg):Msg(_Msg) {}
    /** Return a char* on the message like std::exception does.
     * \return the message
     */
    virtual const char* what() const ASTRING_NOEXCEPT
    {return Msg.c_str();}
    AString Msg;
};

//-------------------------------------------------------------------------------------
// Implementation on inline & template function
//-------------------------------------------------------------------------------------
#if !ASTRING_NOSTL
template <class T>
AString& AString::operator<<(const T &Streamable)
{
    std::ostringstream ss;
	ss<<Streamable;
	*this+=ss.str().c_str();
    return *this;
}
template <class T>
AString AString::implode(AString Separator,const T &Ctnr)
{
    typename T::const_iterator it;
    size_t StrLen=0;
    AString res;
    if(Ctnr.empty())
        return res;
    for(it=Ctnr.begin();it!=Ctnr.end();++it)
        StrLen+=it->size()+Separator.size();
    res.reserve(StrLen);
    res+=*Ctnr.begin();
    for(it=++Ctnr.begin();it!=Ctnr.end();++it)
        {
        res+=Separator;
        res+=*it;
        }
    return res;
}

template <class ResultContainerType>
unsigned AString::explode(AString Separator,ResultContainerType &Result)
{
	size_t p;
	unsigned start=0;
	Result.clear();
	while(1)
	{
		p=find(Separator.c_str(),start);
		if(p==npos)
		{
			Result.push_back(substr(start));
			return (unsigned int)Result.size();
		}
		else
		{
			Result.push_back(substr(start,(unsigned)p - start));
			start=(unsigned)p+(unsigned)Separator.size();
		}
	}
}
#endif // !ASTRING_NOSTL

#endif // ASTRING_H_INCLUDED
