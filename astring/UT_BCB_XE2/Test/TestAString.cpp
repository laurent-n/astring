#include <System.hpp>
#pragma hdrstop

#include <TestFramework.hpp>

#include "..\..\AString.h"
#if !ASTRING_NOSTL
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#endif
using namespace std;

//#define CPPUNIT_ASSERT_EQUAL(x,y) CheckEquals (x , y) marche pas avec AString==AString
#define CPPUNIT_ASSERT_EQUAL(x,y) Check (x == y)
#define CPPUNIT_ASSERT(x) Check (x)
#define CPPUNIT_FAIL(x) Fail (x)

#if ASTRING_NOSTL
// required for CPPUnit
std::ostream& operator<<(std::ostream& os, const AString& str)
{
    os << str.c_str();
	return os;
}
#endif


class TTestAString : public TTestCase
{
public:
  __fastcall virtual TTestAString(System::String name) : TTestCase(name) {}
  virtual void __fastcall SetUp();
  virtual void __fastcall TearDown();
  
__published:
  void __fastcall Construct();
  void __fastcall Modifiers();
  void __fastcall Handling();
#if !ASTRING_NOSTL
  void __fastcall TestStream();
#endif

  void __fastcall BCBInterop();


};

class TTestAStringException : public TTestCase
{
public:
  __fastcall virtual TTestAStringException(System::String name) : TTestCase(name) {}
  virtual void __fastcall SetUp();
  virtual void __fastcall TearDown();
  
__published:
  void __fastcall Testwhat();
};


void __fastcall TTestAString::Construct()
{
    AString S0;
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S0.c_str(),"") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)0u, S0.size() );

    AString S1("Hello");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S1.c_str(),"Hello") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S1.size() );
	//CPPUNIT_ASSERTSTR_EQUAL ( "Hello",S1.c_str() );


    AString S2(S1);
#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
//   std::string may use COW
	CPPUNIT_ASSERT ( S1.c_str()!=S2.c_str() );
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S2.c_str(),"Hello") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S2.size() );
#endif

	AString S4("Hello",3);
    //cout<<S4<<endl;
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S4.c_str(),"Hel") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)3u, S4.size() );

#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
//   std::string can handle string with \0 inside
    AString S5("Hello",10);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S5.c_str(),"Hello") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S5.size() );
#endif
    AString S6("Hello",-10);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S6.c_str(),"Hello") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S6.size() );

    const AString S7("Hello");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"Hello") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S7.size() );

    const AString S8(123u);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S8.c_str(),"123") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)3u, S8.size() );

    const AString S9(-123);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S9.c_str(),"-123") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)4u, S9.size() );

    const AString S10((float)-123.43);
	//CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S10.c_str(),"-123.45") );
	CPPUNIT_ASSERT_EQUAL ( AString("-123.43"), S10 );
	CPPUNIT_ASSERT_EQUAL ( (size_t)7u, S10.size() );

    const AString S11((double)(-123.45));
	CPPUNIT_ASSERT_EQUAL ( AString("-123.45"), S11 );
	CPPUNIT_ASSERT_EQUAL ( (size_t)7u, S11.size() );
    const AString S12((double)(25));
	CPPUNIT_ASSERT_EQUAL ( AString("25"), S12 );
	CPPUNIT_ASSERT_EQUAL ( (size_t)2u, S12.size() );

    const AString S13((double)(0));
	CPPUNIT_ASSERT_EQUAL ( AString("0"), S13 );
	CPPUNIT_ASSERT_EQUAL ( (size_t)1u, S13.size() );

	const AString S14((double)(0.1203));
	CPPUNIT_ASSERT_EQUAL ( AString("0.1203"), S14 );
	CPPUNIT_ASSERT_EQUAL ( (size_t)6u, S14.size() );
}

void __fastcall TTestAString::Modifiers()
{
    AString S6("Hello",-10);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S6.c_str(),"Hello") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S6.size() );

    S6.reserve(20);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S6.c_str(),"Hello") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S6.size() );
	CPPUNIT_ASSERT ( 20u<= S6.capacity() );
    const char *ptr=S6.c_str();
    (S6+=" ")+="World";
    CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S6.c_str(),"Hello World") );
	CPPUNIT_ASSERT ( S6.c_str()==ptr );
	CPPUNIT_ASSERT_EQUAL ( (size_t)11u, S6.size() );
	CPPUNIT_ASSERT ( 20u<= S6.capacity() );

    S6[6]='w';
    S6.at(7)='O';
    CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S6.c_str(),"Hello wOrld") );
    CPPUNIT_ASSERT_EQUAL ( 'e', S6[1]);
    CPPUNIT_ASSERT_EQUAL ( 'l', S6.at(2));
    // Test const accesors
    const AString S1("World");
    CPPUNIT_ASSERT_EQUAL ( 'o', S1[1]);
    CPPUNIT_ASSERT_EQUAL ( 'r', S1.at(2));

    AString S2("Hello");
    CPPUNIT_ASSERT_EQUAL ( 0, strcmp((S2+S1).c_str(),"HelloWorld") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10u, (S2+S1).size() );

	CPPUNIT_ASSERT ( S1!=S2 );
	CPPUNIT_ASSERT ( S1!="Hello" );
	CPPUNIT_ASSERT ( S1=="World" );
	CPPUNIT_ASSERT ( S2+" "+S1=="Hello World" );
	// test Char*+AString
    CPPUNIT_ASSERT ( "Hello "+S1=="Hello World" );

    AString S7;
    S6.assign("Hello");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S6.c_str(),"Hello") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S6.size() );

    S6.assign("Test",2);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S6.c_str(),"Te") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)2u, S6.size() );

    S7.assign("Hello");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"Hello") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S7.size() );

	S7=AString("0123456789");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"0123456789") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10u, S7.size() );

	S7="ABCD";
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"ABCD") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)4u, S7.size() );

    S7.reserve(5);
	S7.replace(1,2,"bc");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"AbcD") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)4u, S7.size() );

	S7.replace(1,2,"bcxy");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"AbcxyD") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)6u, S7.size() );
	CPPUNIT_ASSERT ( 6u<= S7.capacity() );

	S7=AString("0123456789");
	S7.replace(1,4,"XY");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"0XY56789") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)8u, S7.size() );


	S7=AString("LE abcd LELE");
	S7.replace("LE","LES");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"LES abcd LESLES") );

	S7.replace("LES","La");
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"La abcd LaLa") );

    S7=AString(" \t\n\r AB CD \t\n\r ");
    S7.rtrim();
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str()," \t\n\r AB CD") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10u, S7.size() );

    S7=AString(" \t\n\r AB CD \t\n\r ");
    S7.ltrim();
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"AB CD \t\n\r ") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10u, S7.size() );

    S7=AString(" \t\n\r AB CD \t\n\r ");
    S7.trim();
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"AB CD") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)5u, S7.size() );

    S7=AString(" \t\n\r  \t\n\r ");
    S7.ltrim();
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)0u, S7.size() );

    S7=AString(" \t\n\r  \t\n\r ");
    S7.rtrim();
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)0u, S7.size() );

    S7=AString(" \t\n\r  \t\n\r ");
    S7.trim();
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S7.c_str(),"") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)0u, S7.size() );

}

void __fastcall TTestAString::Handling()
{
    AString S1("0123456789ABCDEF");
    AString S2("Test");
    S2=S1.substr(5,3);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S2.c_str(),"567") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)3u, S2.size() );

    S2=S1.substr(0,4);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S2.c_str(),"0123") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)4u, S2.size() );

    S2=S1.substr(10);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S2.c_str(),"ABCDEF") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)6u, S2.size() );

    S2=S1.substr();
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S2.c_str(),"0123456789ABCDEF") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)16u, S2.size() );

    char buf[16]="abcdefghijklmno";
    S1.strncpy(buf,4,5); //4 = 3 car + \0

	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(buf,"567") );

    S1.strncpy(buf,5);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(buf,"0123") );

    S1.strncpy(buf,sizeof(buf),10);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(buf,"ABCDEF") );


	CPPUNIT_ASSERT_EQUAL ( (size_t)1u, S1.find("1") );
	CPPUNIT_ASSERT_EQUAL ( AString::npos, S1.find("1",3) );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10u, S1.find("A") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10u, S1.find("A",3) );
	CPPUNIT_ASSERT_EQUAL ( (size_t)1u, S1.find("123") );
	CPPUNIT_ASSERT_EQUAL ( AString::npos, S1.find("124") );
	CPPUNIT_ASSERT_EQUAL ( AString::npos, S1.find("1",50) );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10u, S1.find('A') );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10u, S1.find('A',3) );
	CPPUNIT_ASSERT_EQUAL ( AString::npos, S1.find('X') );

    AString S3=Asprintf("X %d %.02f %s",1,2.5,"ABC");
	CPPUNIT_ASSERT_EQUAL ( AString("X 1 2.50 ABC"), S3 );
	CPPUNIT_ASSERT_EQUAL ( (size_t)12u, S3.size() );
	S3="ABC";
    S3.Format("Y %d %.02f %s",10,2.52,"DEF");
	CPPUNIT_ASSERT_EQUAL ( AString("Y 10 2.52 DEF"), S3 );
	CPPUNIT_ASSERT_EQUAL ( (size_t)13u, S3.size() );
    #if !defined(__GNUC__)
    S1="An AString here";
	#if ! ASTRING_IMPLEMENT_WITH_STD_STRING
    S3.Format("X %d %d %s %s %d Y",10,20,"A C string",S1,15);
	CPPUNIT_ASSERT_EQUAL ( AString("X 10 20 A C string An AString here 15 Y"), S3 );
	CPPUNIT_ASSERT_EQUAL ( (size_t)39u, S3.size() );
	#endif
    #endif

    try{
        throw AStringException("ABCDEF");
    }
    #if ASTRING_NOSTL
    catch(AStringException &e)
    #else
    catch(std::exception &e)
    #endif
    {
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(e.what(),"ABCDEF") );
    }
    catch(...)
    {
	CPPUNIT_FAIL ( "wrong exception type" );
	}
}


void __fastcall TTestAString::BCBInterop()
{
	AnsiString AS1="Hello AnsiString";
	UnicodeString US1="Hello UnicodeString";

	AnsiString AS2="XXX";
	UnicodeString US2="XXX";

	AString S1(AS1);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S1.c_str(),"Hello AnsiString") );
	AS2=S1;
	Check(AS2==AS1);

	AString S2(US1);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S2.c_str(),"Hello UnicodeString") );
	US2=S2;
	Check(US2==US1);
	AS2+=" 2";
	US2+=" 2";
	S1=AS2;
	S2=US2;
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S1.c_str(),"Hello AnsiString 2") );
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S2.c_str(),"Hello UnicodeString 2") );


}

void __fastcall TTestAString::TestStream()
{
	AString S1("0123456789ABCDEF");
	std::stringstream ss;
	ss<<S1;
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S1.c_str(),ss.str().c_str()) );

	AString S2("A");
	S2<<15<<" XXX "<<2.5<<" XXX "<<S1;
	CPPUNIT_ASSERT_EQUAL ( AString("A15 XXX 2.5 XXX 0123456789ABCDEF"), S2 );

	std::vector<AString> Vs;
	Vs.push_back("Hello");
	Vs.push_back("dear");
	Vs.push_back("friends");
    AString S3=AString::implode(AString(" "),Vs);
	CPPUNIT_ASSERT_EQUAL ( AString("Hello dear friends"), S3 );

    std::deque<AString> Ds;
    unsigned res=S3.explode(" ",Ds);
	CPPUNIT_ASSERT_EQUAL ( AString("Hello"), Ds[0] );
	CPPUNIT_ASSERT_EQUAL ( AString("dear"), Ds[1] );
	CPPUNIT_ASSERT_EQUAL ( AString("friends"), Ds[2] );
    CPPUNIT_ASSERT_EQUAL ( 3u, res );
    res=S3.explode("?",Ds);
	CPPUNIT_ASSERT_EQUAL ( AString("Hello dear friends"), Ds[0] );
	CPPUNIT_ASSERT_EQUAL ( (unsigned int)1, (unsigned int)Ds.size() );
    CPPUNIT_ASSERT_EQUAL ( 1u, res );

    res=S3.explode(" ",Vs);
	CPPUNIT_ASSERT_EQUAL ( AString("Hello"), Vs[0] );
	CPPUNIT_ASSERT_EQUAL ( AString("dear"), Vs[1] );
	CPPUNIT_ASSERT_EQUAL ( AString("friends"), Vs[2] );
	CPPUNIT_ASSERT_EQUAL ( (unsigned int)3, (unsigned int)Vs.size() );
    CPPUNIT_ASSERT_EQUAL ( 3u, res );

    S3="";
    res=S3.explode("?",Ds);
	CPPUNIT_ASSERT_EQUAL ( AString(""), Ds[0] );
	CPPUNIT_ASSERT_EQUAL ( 1u, (unsigned int)Ds.size() );
    CPPUNIT_ASSERT_EQUAL ( 1u, res );
    // Iterator test
    S3="FCEGBDA";
    std::sort(S3.begin(),S3.end());
	CPPUNIT_ASSERT_EQUAL ( AString("ABCDEFG"), S3 );
	S3="FCEGBDA";
	std::sort(S3.begin(),S3.end(),std::greater<char>());
	CPPUNIT_ASSERT_EQUAL ( AString("GFEDCBA"), S3 );
	S3="25";
	CPPUNIT_ASSERT_EQUAL ( 25, S3.ToInt() );
	S3="-25";
	CPPUNIT_ASSERT_EQUAL ( -25, S3.ToIntDef(999) );
	S3="ABC";
	CPPUNIT_ASSERT_EQUAL ( 999, S3.ToIntDef(999) );

}


void __fastcall TTestAStringException::SetUp()
{
}

void __fastcall TTestAStringException::TearDown()
{
}

void __fastcall TTestAStringException::Testwhat()
{
  //  const char* what() const
}


void __fastcall TTestAString::SetUp()
{
}

void __fastcall TTestAString::TearDown()
{
}



static void registerTests()
{
  Testframework::RegisterTest(TTestAString::Suite());
  Testframework::RegisterTest(TTestAStringException::Suite());
}
#pragma startup registerTests 33
