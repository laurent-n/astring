#include <cstring>
#include <cppunit/config/SourcePrefix.h>
#include <cppunit/extensions/HelperMacros.h>
#include "AString.h"
#if !ASTRING_NOSTL
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#endif
using namespace std;

#if ASTRING_NOSTL
// required for CPPUnit
std::ostream& operator<<(std::ostream& os, const AString& str)
{
    os << str.c_str();
	return os;
}
#endif


class AStringUnitTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( AStringUnitTest );
  CPPUNIT_TEST( Construct );
  CPPUNIT_TEST( Modifiers );
  CPPUNIT_TEST( Handling );
#if !ASTRING_NOSTL
  CPPUNIT_TEST( TestStream );
#endif
  CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{// Executé entre chaque test

	}

protected:
void Construct()
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

#if ASTRING_INTEROP_VSS
	CStringA CS1("Hello CString");
	AString S15(CS1);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S15.c_str(),"Hello CString") );
	S15+="XX";
	CStringA CS2(S15);
	CPPUNIT_ASSERT ( CS2=="Hello CStringXX") ;
	S15=CS1+" by =";
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S15.c_str(),"Hello CString by =") );

	CStringW CSW1(CS1+" Wide");
	AString S16(CSW1);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S16.c_str(),"Hello CString Wide") );
	S16+="XX";
	CStringA CSW2(S16);
	CPPUNIT_ASSERT ( CSW2==L"Hello CString WideXX") ;

	CString CSx1("Hello CString undef");
	AString S17(CSx1);
	CPPUNIT_ASSERT_EQUAL ( 0, strcmp(S17.c_str(),"Hello CString undef") );
	S17+="XX";
	CStringA CSx2(S17);
	CPPUNIT_ASSERT ( CSx2=="Hello CString undefXX") ;


#endif // ASTRING_INTEROP_VSS


}

void Modifiers()
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
    {
    const AString S1("World");
    CPPUNIT_ASSERT_EQUAL ( 'o', S1[1]);
    CPPUNIT_ASSERT_EQUAL ( 'r', S1.at(2));

    AString S2("Hello");
    CPPUNIT_ASSERT_EQUAL ( 0, strcmp((S2+S1).c_str(),"HelloWorld") );
	CPPUNIT_ASSERT_EQUAL ( (size_t)10u, (S2+S1).size() );

	CPPUNIT_ASSERT ( S1!=S2 );
	CPPUNIT_ASSERT ( S2+" "+S1=="Hello World" );
	// test Char*+AString
    CPPUNIT_ASSERT ( "Hello "+S1=="Hello World" );

    }
	AString S1="abcd";
	AString S2="aacd";
	//S2="Wnrle";
	CPPUNIT_ASSERT ( S1!="Hello" );
	CPPUNIT_ASSERT ( S1=="abcd" );
	CPPUNIT_ASSERT ( !(S1=="abcde") );
	CPPUNIT_ASSERT ( !(S1=="abc") );
	CPPUNIT_ASSERT ( !(S1=="") );
	CPPUNIT_ASSERT ( "abcd"==S1 );
	CPPUNIT_ASSERT ( !("abcde"==S1) );
	CPPUNIT_ASSERT ( !("abc"==S1) );
	CPPUNIT_ASSERT ( S1==AString("abcd") );
	CPPUNIT_ASSERT ( !(S1==AString("abcde")) );
	CPPUNIT_ASSERT ( !(S1==AString("abc")) );
	CPPUNIT_ASSERT ( !(S1==AString("")) );
	CPPUNIT_ASSERT ( !("abcd"!=S1) );
	CPPUNIT_ASSERT (  ("abcde"!=S1) );
	CPPUNIT_ASSERT (  ("abc"!=S1) );
	CPPUNIT_ASSERT ( S1<"bbcd" );
	CPPUNIT_ASSERT ( !(S1<S2) );
	CPPUNIT_ASSERT ( S2<S1 );
	CPPUNIT_ASSERT ( !(S1<"aacd") );
	CPPUNIT_ASSERT ( !(S1<"abcc") );
	CPPUNIT_ASSERT ( !(S1<"abcd") );
	CPPUNIT_ASSERT ( S1<"abcde" );
	CPPUNIT_ASSERT ( S1>"abcc" );
	CPPUNIT_ASSERT ( S1>"abc" );
	CPPUNIT_ASSERT ( S1>"aacd" );
	CPPUNIT_ASSERT ( !(S2>S1) );
	CPPUNIT_ASSERT ( S1>S2 );
	CPPUNIT_ASSERT ( "abc"<S1 );
	CPPUNIT_ASSERT ( !("abcd"<S1) );
	CPPUNIT_ASSERT ( !("abcde"<S1) );
	CPPUNIT_ASSERT ( !("abcd">S1) );
	CPPUNIT_ASSERT ( !("abcc">S1) );
	CPPUNIT_ASSERT ( "abcde">S1 );
	/*S2="accd";
	CPPUNIT_ASSERT ( !(S2<S1) );
	CPPUNIT_ASSERT ( S1<S2 );
	CPPUNIT_ASSERT ( !(S1>S2) );
	CPPUNIT_ASSERT ( S2>S1 ); */
	S2="accd";
	CPPUNIT_ASSERT ( !(S1>S2) );
	CPPUNIT_ASSERT ( !(S1>"accd") );
	CPPUNIT_ASSERT ( !("abcd">S2) );
	CPPUNIT_ASSERT ( (S1<S2) );
	CPPUNIT_ASSERT ( (S1<"accd") );
	CPPUNIT_ASSERT ( ("abcd"<S2) );
	S2="aacd";
	CPPUNIT_ASSERT ( !(S1<S2) );
	CPPUNIT_ASSERT ( !(S1<"aacd") );
	CPPUNIT_ASSERT ( !("abcd"<S2) );
	S2="abcde";
	CPPUNIT_ASSERT ( S1<S2 );
	CPPUNIT_ASSERT ( !(S1==S2) );
	CPPUNIT_ASSERT ( S1!=S2 );

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

void Handling()
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

#if !ASTRING_NOSTL

void TestStream()
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
    AString S3=AString::implode(AString(" "),Vs);
	CPPUNIT_ASSERT_EQUAL ( AString("Hello"), S3 );
    Vs.push_back("dear");
    S3=AString::implode(AString(" "),Vs);
	CPPUNIT_ASSERT_EQUAL ( AString("Hello dear"), S3 );
    Vs.push_back("friends");
    S3=AString::implode(AString(" "),Vs);
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
#endif



};

CPPUNIT_TEST_SUITE_REGISTRATION( AStringUnitTest );
