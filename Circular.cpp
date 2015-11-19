#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <windows.h>

using namespace std;

void drawCircle(double x, double y, double z, double r, int C);
void drawLine(double x, double y, double z, double x1, double y1, double z1, int C);

double x,y,z,h,Lastx,Lasty,Lastz,Lastr,PI,r,a,Rate;

char temp;

int Colour,Segment,Height;

ofstream DXFfile("Circular.dxf");

int main(void)
{
cout << "Segments, Height, Growth Rate <Eg. 15 100 0.5/Min 1>\n";
cin >> Segment >> Height >> Rate;

PI=4.0*atan(1.0);

DXFfile<<"0\nSECTION\n2\nENTITIES\n";
cout<<"Start\n";
for(h=1;(-1)*z<Height;h+=pow(log((-1)*z+2),2))
{
cout<<z<<"\n";

x=log(h*h)*sin(PI*h*log(h));
y=log(h*h)*cos(PI*h*log(h));
z+=(-1)*pow(h,Rate);

r=pow((pow(x-Lastx,2)+pow(y-Lasty,2)+pow(z-Lastz,2)),0.5);

Colour=16+int((-1)*z)%239;

if ((-1)*z>=Height){
z=(-1)*Height;                    
}

drawCircle (x,y,z+Height,r,Colour);

for (a=0;a<2*PI;a+=2*PI/Segment){
drawLine (Lastr*sin(a)+Lastx,Lastr*cos(a)+Lasty,Lastz+Height,r*sin(a)+x,r*cos(a)+y,z+Height,Colour);
}

Lastx=x;Lasty=y;Lastz=z;Lastr=r;
}

DXFfile<<"0\nENDSEC\n0\nEOF\n";DXFfile.close();
cout<<"Finished\n";

Sleep(2000);

return 0;
}

void drawCircle(double x, double y, double z, double r, int C){
DXFfile<<"0\nCIRCLE\n8\n0\n";
DXFfile<<"10\n"<<x<<"\n";
DXFfile<<"20\n"<<y<<"\n";
DXFfile<<"30\n"<<z<<"\n";
DXFfile<<"40\n"<<r<<"\n";
DXFfile<<"62\n"<<C<<"\n";
}

void drawLine(double x, double y, double z, double x1, double y1, double z1, int C){
DXFfile<<"0\nLINE\n8\n0\n";
DXFfile<<"10\n"<<x<<"\n";
DXFfile<<"20\n"<<y<<"\n";
DXFfile<<"30\n"<<z<<"\n";
DXFfile<<"11\n"<<x1<<"\n";
DXFfile<<"21\n"<<y1<<"\n";
DXFfile<<"31\n"<<z1<<"\n";
DXFfile<<"62\n"<<C<<"\n";
}
