#include "AString.h"
#include <cstring>
#include <stdarg.h>
#include <stdio.h>
#include <sstream>

//#define ASTRING_DEBUG_TRACE_ENABLE
#ifdef ASTRING_DEBUG_TRACE_ENABLE
#define ASTRING_DEBUG_TRACE(a,b) std::cout<<"ASTRING DEBUG :"<<a<<b<<std::endl
#else
#define ASTRING_DEBUG_TRACE(a,b)
#endif
#if ASTRING_IMPLEMENT_WITH_STD_STRING
  #define ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(x) x
#else
  #define ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(x)
#endif


#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
const char AString::EmptyBuffer[4]={0,0,0,0};
const size_t AString::npos=(size_t)-1;
#endif

//-------------------------------------------------------------
AString::AString()
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string())
{
	ASTRING_DEBUG_TRACE("construct empty","");
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    StrBuffer=(char*)&EmptyBuffer;
#endif
}
//-------------------------------------------------------------
AString::~AString() ASTRING_NOEXCEPT
{
	ASTRING_DEBUG_TRACE("destroy ",c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    if(StrBuffer!=EmptyBuffer)
        delete [] GetRawBuffer();
#endif
}
//-------------------------------------------------------------
AString::AString(const char *str,int count/*=-1*/)
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string(str,count<0?strlen(str):count))
{
	ASTRING_DEBUG_TRACE("construct char* ",str);
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    unsigned len;
    if(count>=0)
        len=count;
    else
        len=(unsigned)strlen(str);
    if(len>0)
    {
        AllocBuffer(len);
        memcpy(StrBuffer,str,len);
        StrBuffer[len]=0;
        GetSize()=(unsigned)strlen(StrBuffer); // in case str contains a \0
    }
    else
        StrBuffer=(char*)&EmptyBuffer;
#endif
}
//-------------------------------------------------------------
AString::AString(int value)
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string())
{
    // Todo reimplement it without std::stringstream
    // Todo support multiple base
	ASTRING_DEBUG_TRACE("construct int ",value);
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
	StrBuffer=(char*)&EmptyBuffer;
#endif
	/*std::stringstream ss;
	ss << value;
	assign(ss.str().c_str());*/
	Format("%d",value);
}
//-------------------------------------------------------------
AString::AString(unsigned value)
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string())
{
    // Todo reimplement it without std::stringstream
    // Todo support multiple base
	ASTRING_DEBUG_TRACE("construct unsigned ",value);
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
	StrBuffer=(char*)&EmptyBuffer;
#endif
	/*std::stringstream ss;
	ss << value;
	assign(ss.str().c_str());*/
	Format("%u",value);
}
/*AString::AString(float value)
{
    // Todo reimplement it without std::stringstream
	ASTRING_DEBUG_TRACE("construct float ",value);
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
	StrBuffer=(char*)&EmptyBuffer;
#endif

	Format("%f",value);
}*/
AString::AString(double value)
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string())
{
    ASTRING_DEBUG_TRACE("construct double ",value);
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
	StrBuffer=(char*)&EmptyBuffer;
#endif
	/*std::stringstream ss;
	ss << value;
	assign(ss.str().c_str());	*/
    #if __MINGW32__ || _MSC_VER
	Format("%f",value);
	#else
	Format("%lf",value);
	#endif
	// Trim right 0 and . but keep at least one char
	for(unsigned i=(unsigned)size()-1;i>0;i--)
        if((at(i)=='0')||(at(i)=='.'))
            at(i)=0;
        else
            break;
    RecomputeSize();

}
//-------------------------------------------------------------
AString::AString(const AString &Src)
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string(Src))
{
	ASTRING_DEBUG_TRACE("construct copy ",Src.c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    unsigned len=(unsigned)Src.size();
    if(len>0)
    {
        AllocBuffer(len);
        GetSize()=len;
        memcpy(StrBuffer,Src.StrBuffer,len);
        StrBuffer[len]=0;
    }
    else
        StrBuffer=(char*)&EmptyBuffer;
#endif
}

#if ASTRING_INTEROP_BCB
AString::AString(const AnsiString &Src)
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string(Src.c_str() ))
{
	ASTRING_DEBUG_TRACE("construct AnsiString ",Src.c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
	unsigned len=(unsigned)Src.Length();
	if(len>0)
	{
		AllocBuffer(len);
		GetSize()=len;
		memcpy(StrBuffer,Src.c_str(),len);
		StrBuffer[len]=0;
	}
	else
		StrBuffer=(char*)&EmptyBuffer;
#endif
}
AString::AString(const UnicodeString &Src)
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string(AnsiString(Src).c_str() )) 
{
	ASTRING_DEBUG_TRACE("construct AnsiString ",Src.c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
	AnsiString AS(Src); // Convert to Ansi
	unsigned len=(unsigned)AS.Length();
	if(len>0)
	{
		AllocBuffer(len);
		GetSize()=len;
		memcpy(StrBuffer,AS.c_str(),len);
		StrBuffer[len]=0;
	}
	else
		StrBuffer=(char*)&EmptyBuffer;
#endif
}
AString::operator AnsiString ()
{
	return AnsiString(c_str());
}
AString::operator UnicodeString ()
{
	return UnicodeString(c_str());
}

#endif   //ASTRING_INTEROP_BCB

#if ASTRING_INTEROP_VSS
AString::AString(CStringA &Src)
	ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string(Src.GetBuffer(0) ))
{
//	ASTRING_DEBUG_TRACE("construct CStringA ",Src.c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
	unsigned len=(unsigned)Src.GetLength();
	if(len>0)
	{
		AllocBuffer(len);
		GetSize()=len;		
		memcpy(StrBuffer,Src.GetBuffer(0),len);
		StrBuffer[len]=0;
	}
	else
		StrBuffer=(char*)&EmptyBuffer;
#endif
}
AString::operator CStringA ()
{
	return CStringA(c_str());
}

AString::AString(CStringW &Src)
	ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string(CStringA(Src).GetBuffer(0) ))
{
//	ASTRING_DEBUG_TRACE("construct CStringW ",Src.c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
	CStringA Cs(Src);
	unsigned len=(unsigned)Cs.GetLength();
	if(len>0)
	{
		AllocBuffer(len);
		GetSize()=len;
		memcpy(StrBuffer,Cs.GetBuffer(0),len);
		StrBuffer[len]=0;
	}
	else
		StrBuffer=(char*)&EmptyBuffer;
#endif
}
AString::operator CStringW ()
{
	return CStringW(CStringA(c_str()));
}
#endif // ASTRING_INTEROP_VSS


//-------------------------------------------------------------
#if ASTRING_INTEROP_STD_STRING
AString::AString(const std::string &Src)
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string(Src))
{
	ASTRING_DEBUG_TRACE("construct string copy ",Src.c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    unsigned len=(unsigned)Src.size();
    if(len>0)
    {
        AllocBuffer(len);
        GetSize()=len;
        memcpy(StrBuffer,Src.c_str(),len);
        StrBuffer[len]=0;
    }
    else
        StrBuffer=(char*)&EmptyBuffer;
#endif
}
#endif
//-------------------------------------------------------------
AString& AString::operator =(const AString& Src)
{
	ASTRING_DEBUG_TRACE("operator = AString ",Src.c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    if(StrBuffer!=((char*)&EmptyBuffer))
        delete [] GetRawBuffer();
    unsigned len=(unsigned)Src.size();
    if(len>0)
    {
        AllocBuffer(len);
        GetSize()=len;
        memcpy(StrBuffer,Src.StrBuffer,len);
        StrBuffer[len]=0;
    }
    else
        StrBuffer=(char*)&EmptyBuffer;
#else
    assign(Src);
#endif
    return *this;
}
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
//-------------------------------------------------------------
AString& AString::assign(const char *str,int count/*=-1*/)
{
	ASTRING_DEBUG_TRACE("assign char* ",str);
    unsigned len;
    if(count>=0)
        len=count;
    else
        len=(unsigned)strlen(str);
    clear();
    if(len>0)
    {
        AllocBuffer(len);
        GetSize()=len;
        memcpy(StrBuffer,str,len);
        StrBuffer[len]=0;
    }
    else
        StrBuffer=(char*)&EmptyBuffer;
    return *this;
}
//-------------------------------------------------------------
char* AString::c_str()
{
    return StrBuffer;
}
//-------------------------------------------------------------
const char* AString::c_str() const
{
    return StrBuffer;
}
//-------------------------------------------------------------
size_t AString::size() const
{
    if(StrBuffer==((char*)&EmptyBuffer))
        return 0;
    else
        return GetSize();
}
//-------------------------------------------------------------
size_t AString::capacity() const
{
    if(StrBuffer==((char*)&EmptyBuffer))
        return 0;
    else
        return GetCapacity();
}
//-------------------------------------------------------------
char* AString::GetRawBuffer()
{
    if(StrBuffer==((char*)&EmptyBuffer))
        return NULL;
    else
        return StrBuffer-2*sizeof(unsigned);
}
//-------------------------------------------------------------
void AString::clear()
{
	ASTRING_DEBUG_TRACE("clear ","");
    if(StrBuffer!=EmptyBuffer)
    {
        delete [] GetRawBuffer();
        StrBuffer=(char*)EmptyBuffer;
    }
}
//-------------------------------------------------------------
void AString::reserve(size_t count)
{
	ASTRING_DEBUG_TRACE("reserve ",count);
    char *PrevStr=c_str();
    unsigned PrevLen=(unsigned)size();
    char *PrevBuffer=GetRawBuffer();
    AllocBuffer((unsigned)count);
    GetSize()=(unsigned)count;
    if(count)
    {
        if(PrevLen>(unsigned)count)
            PrevLen=(unsigned)count;
        memcpy(StrBuffer,PrevStr,PrevLen);
        StrBuffer[PrevLen]=0;
		GetSize()=PrevLen;
    }
    if((PrevBuffer)&&(PrevBuffer!=EmptyBuffer))
        delete []PrevBuffer;


}
//-------------------------------------------------------------
void AString::AllocBuffer(unsigned count)
{
	ASTRING_DEBUG_TRACE("Alloc buffer ",count);

    if(count>0)
    {
        count|=7; // round up to 8 bytes (with the +1 of the next line)
        StrBuffer=new char[count+1+2*sizeof(unsigned)];
        StrBuffer+=2*sizeof(unsigned);
        GetSize()=0;
        GetCapacity()=count;
        StrBuffer[0]=0;
    }
    else
        StrBuffer=(char*)&EmptyBuffer;
}
//-------------------------------------------------------------
char& AString::operator[](size_t idx)
{
    if(idx>size())
        throw AStringException("Invalid index");
    return StrBuffer[idx];
}
//-------------------------------------------------------------
char AString::operator[](size_t idx) const
{
    if(idx>size())
        throw AStringException("Invalid index");
    return StrBuffer[idx];
}
//-------------------------------------------------------------
char& AString::at(size_t idx)
{
    if(idx>size())
        throw AStringException("Invalid index");
    return StrBuffer[idx];
}
//-------------------------------------------------------------
char AString::at(size_t idx) const
{
    if(idx>size())
        throw AStringException("Invalid index");
    return StrBuffer[idx];
}

//-------------------------------------------------------------
unsigned& AString::GetSize()
{
   return *((unsigned*)(StrBuffer-2*sizeof(unsigned)));
}
//-------------------------------------------------------------
unsigned& AString::GetCapacity()
{
   return *((unsigned*)(StrBuffer-sizeof(unsigned)));
}
//-------------------------------------------------------------
unsigned AString::GetSize() const
{
   return *((unsigned*)(StrBuffer-2*sizeof(unsigned)));
}
//-------------------------------------------------------------
unsigned AString::GetCapacity() const
{
   return *((unsigned*)(StrBuffer-sizeof(unsigned)));
}
//-------------------------------------------------------------
AString AString::operator +(const AString &RightStr) const
{
   AString res;
   unsigned LLen=(unsigned)size();
   unsigned RLen=(unsigned)RightStr.size();
   res.reserve(LLen+RLen );
   res.GetSize()=LLen+ RLen;
   if(LLen)
     memcpy(res.StrBuffer,StrBuffer,LLen);
   if(RLen)
     memcpy(&res.StrBuffer[LLen],RightStr.StrBuffer,RLen);
   res.StrBuffer[res.GetSize()]=0;
   return res;
}
//-------------------------------------------------------------
AString AString::operator +(const char* RightStr) const
{
   AString res;
   unsigned LLen=(unsigned)size();
   unsigned RLen=(unsigned)strlen(RightStr);
   res.reserve(LLen+RLen );
   res.GetSize()=LLen+ RLen;
   if(LLen)
     memcpy(res.StrBuffer,StrBuffer,LLen);
   if(RLen)
     memcpy(&res.StrBuffer[LLen],RightStr,RLen);
   res.StrBuffer[res.GetSize()]=0;
   return res;
}
//-------------------------------------------------------------
AString operator +(const char *LeftStr,const AString &RightStr)
{
   AString res;
   unsigned LLen=(unsigned)strlen(LeftStr);
   unsigned RLen=(unsigned)RightStr.size();
   res.reserve(LLen+RLen );
   res.GetSize()=LLen+ RLen;
   if(LLen)
     memcpy(res.StrBuffer,LeftStr,LLen);
   if(RLen)
     memcpy(&res.StrBuffer[LLen],RightStr.StrBuffer,RLen);
   res.StrBuffer[res.GetSize()]=0;
   return res;
}
//-------------------------------------------------------------
AString& AString::operator +=(const AString &RightStr)
{
   unsigned RLen=(unsigned)RightStr.size();
   unsigned FinalLength=(unsigned)size()+RLen;
   if(FinalLength>capacity())
    reserve(FinalLength);
   if(RLen)
     memcpy(&StrBuffer[size()],RightStr.StrBuffer,RLen);
   GetSize()=FinalLength;
   StrBuffer[FinalLength]=0;
   return *this;
}
//-------------------------------------------------------------
bool AString::operator ==(const AString &RightStr) const
{
   unsigned RLen=(unsigned)RightStr.size();
   if(RLen!=size())
    return false;
   char *sL=StrBuffer;
   char *sR=RightStr.StrBuffer;
   while(*sL)
   {
       if(*sL!=*sR)
        return false;
      ++sL; // Go to next char
      ++sR;
   }
   return true;
}
//-------------------------------------------------------------
bool AString::operator ==(const char* RightStr) const
{
   char *sL=StrBuffer;
   const char *sR=RightStr;
   while(*sL)
   {
       if(*sL!=*sR)
        return false;
      ++sL; // Go to next char
      ++sR;
   }
   return true;
}
//-------------------------------------------------------------
bool AString::operator <(const AString &RightStr) const
{
   char *sL=StrBuffer;
   char *sR=RightStr.StrBuffer;
   while(*sL)
   {
       if(*sL<*sR)
        return true;
      ++sL; // Go to next char
      ++sR;
   }
   return false;
}
//-------------------------------------------------------------
bool AString::operator <(const char *&RightStr) const
{
   char *sL=StrBuffer;
   const char *sR=RightStr;
   while(*sL)
   {
       if(*sL<*sR)
        return true;
      ++sL; // Go to next char
      ++sR;
   }
   return false;
}
//-------------------------------------------------------------
bool operator <(const char *LeftStr,const AString &RightStr)
{
   const char *sL=LeftStr;
   char *sR=RightStr.StrBuffer;
   while(*sL)
   {
       if(*sL<*sR)
        return true;
      ++sL; // Go to next char
      ++sR;
   }
   return false;
}

//-------------------------------------------------------------
bool AString::operator !=(const AString &RightStr) const
{
    return !(*this==RightStr);
}
//-------------------------------------------------------------
bool AString::operator !=(const char* RightStr) const
{
    return !(*this==RightStr);
}
//-------------------------------------------------------------
AString AString::substr( unsigned pos /*= 0*/,unsigned count /*= npos*/ ) const
{
    if(pos>size())
        throw AStringException("substr:Invalid position");
    if((count>(unsigned)size())||((pos+count)>size()))
        count=(unsigned)size()-pos;
    return  AString(&StrBuffer[pos],count);
}
//-------------------------------------------------------------
size_t AString::find(const char* Searched,unsigned StartPos/*=0*/,unsigned count/*=0*/) const
{
    if(count==0)
        count=(unsigned)strlen(Searched);
    if(count==0)
        throw AStringException("Find:Invalid separator length");
    if(StartPos>size())
        return npos;
    if(count>size())
        return npos;
    char *Source=StrBuffer+StartPos;
    unsigned c;
    while(*Source)
    {
        c=0;
        while(c<count)
        {
            if(Source[c]!=Searched[c])
                break;
            c++;
        }
        if(c==count)
            return (unsigned)(Source-StrBuffer);
        Source++;
    }
    return npos;
}
//-------------------------------------------------------------
size_t AString::find(char Searched,unsigned StartPos/*=0*/) const
{
    return find(&Searched,StartPos,1);
}
//-------------------------------------------------------------
AString::iterator AString::begin()
{
    return c_str();
}
//-------------------------------------------------------------
AString::iterator AString::end()
{
    return c_str()+size();
}
#endif
//-------------------------------------------------------------
#if !ASTRING_NOSTL
std::ostream& operator<<(std::ostream& os, const AString& str)
{
    os << str.c_str();
	return os;
}
#endif

//-------------------------------------------------------------
void AString::strncpy( char* dest,size_t destsize,unsigned pos /*= 0*/) const
{
    if(pos>size())
        throw AStringException("Invalid position");
    if(destsize<1)
        throw AStringException("Invalid destination size should be >0");
    destsize--; // zero based and/or final 0
    if((destsize>size())||((pos+destsize)>size()))
        destsize=size()-pos;
    memcpy(dest,&c_str()[pos],destsize);
    dest[destsize]=0;

}
//-------------------------------------------------------------
void AString::RecomputeSize()
{
    unsigned NewSize=(unsigned)strlen(c_str());
    if(NewSize==size())
        return;
#if ASTRING_IMPLEMENT_WITH_STD_STRING
    resize(NewSize);
#else
    GetSize()=NewSize;
#endif
}
//-------------------------------------------------------------
int AString::ToInt()
{
    int x;
	#ifdef _MSC_VER
	if(sscanf_s(c_str(),"%d",&x)<=0)
	#else
	if(sscanf(c_str(),"%d",&x)<=0)
	#endif
        throw AStringException(Asprintf("ToInt %s is not an integer ",c_str()));
    return x;
}
//-------------------------------------------------------------
int AString::ToIntDef(int defval)
{
    int x;
#ifdef _MSC_VER
	if(sscanf_s(c_str(),"%d",&x)<=0)
#else
    if(sscanf(c_str(),"%d",&x)<=0)
#endif
        return defval;
    return x;
}
//-------------------------------------------------------------
#if __cplusplus > 199711L // C++ 11
//-------------------------------------------------------------
AString::AString(AString &&Src) ASTRING_NOEXCEPT
ASTRING_IMPLEMENT_WITH_STD_STRING_CODE(:std::string(Src))
{
	ASTRING_DEBUG_TRACE("Move constructor ",Src.c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    StrBuffer=Src.StrBuffer;
    Src.StrBuffer=(char*)&EmptyBuffer;
#endif
}
//-------------------------------------------------------------
AString& AString::operator =(AString&& Src) ASTRING_NOEXCEPT
{
	ASTRING_DEBUG_TRACE("Move operator = ",Src.c_str());
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    if(StrBuffer!=((char*)&EmptyBuffer))
        delete [] GetRawBuffer();
    StrBuffer=Src.StrBuffer;
    Src.StrBuffer=(char*)&EmptyBuffer;
    return *this;
#else
    return this->operator =(Src);
#endif
}
#endif // C++ 11

//-------------------------------------------------------------
AString Asprintf(const char *Fmt,...)
{
	int needed;
    char* TempBuffer;
    AString ret;
    #ifdef _MSC_VER
		va_list argptr;
		va_start(argptr, Fmt);
        needed=_vscprintf(Fmt, argptr);
    #else
        char Temp[4];
		va_list argptr;
		va_start(argptr, Fmt);
        needed=vsnprintf(Temp,0, Fmt, argptr);
    #endif
    va_end(argptr);

	#if ASTRING_IMPLEMENT_WITH_STD_STRING
        TempBuffer =new char[needed+1];
    #else
        ret.reserve(needed);
        TempBuffer=ret.c_str();
        ret.GetSize()=needed;
    #endif
    va_start(argptr, Fmt);
    #ifdef _MSC_VER
        vsnprintf_s(TempBuffer,needed+1,_TRUNCATE, Fmt, argptr);
    #else
        vsnprintf(TempBuffer,needed+1, Fmt, argptr);
        TempBuffer[needed]=0;
    #endif
    va_end(argptr);
    #if ASTRING_IMPLEMENT_WITH_STD_STRING
        ret=TempBuffer;
        delete [] TempBuffer;
    #endif
    return ret;
}
//-------------------------------------------------------------
//AString& AString::Format(const AString &Fmt,...)
AString& AString::Format(const char *Fmt,...)
{
    char* TempBuffer;
	int needed;
    va_list argptr;
    va_start(argptr, Fmt);
    #ifdef _MSC_VER
        needed=_vscprintf(Fmt, argptr);
    #else
        char Temp[4];
        needed=vsnprintf(Temp,0, Fmt, argptr);
    #endif
    va_end(argptr);
    #if ASTRING_IMPLEMENT_WITH_STD_STRING
        TempBuffer =new char[needed+1];
    #else
        reserve(needed);
        TempBuffer=c_str();
        GetSize()=needed;
    #endif
	va_start(argptr, Fmt);
    #ifdef _MSC_VER
        vsnprintf_s(TempBuffer,needed+1,_TRUNCATE, Fmt, argptr);
    #else
        vsnprintf(TempBuffer,needed+1, Fmt, argptr);
        TempBuffer[needed]=0;
    #endif
    va_end(argptr);
    #if ASTRING_IMPLEMENT_WITH_STD_STRING
        assign(TempBuffer);
        delete [] TempBuffer;
    #endif
    return *this;

}


