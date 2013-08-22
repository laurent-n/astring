/** \file
Test of std::string based implementation.
This file include all files and rename class names. \n
It's a pretty crap tips allowing to test de std::string based implementation as the same way in the same run.
If you met issue just drop this file and set ASTRING_IMPLEMENT_WITH_STD_STRING to 0 or 1 depending your test.
********************************************************************/

#define AString AString2
#define AStringException AStringException2
#define Asprintf Asprintf2
#define AStringUnitTest AStringUnitTest2

#define ASTRING_IMPLEMENT_WITH_STD_STRING 1

#include "AString.h"
#include "AString.cpp"
#include "AStringUnitTest.cpp"

