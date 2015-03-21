#include <iostream>
#include "AString.h"

using namespace std;

int main()
{
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
    return 0;
}
