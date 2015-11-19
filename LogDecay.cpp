#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;

void drawLine(double x, double y, double z, double x1, double y1, double z1, int C);

double x,y,z,Lastx,Lasty,Lastz,ZFirstx,ZFirsty,ZLastx,ZLasty,PI,i;

int Colour,Density,Radius,Height,EllipticalDistortionX,EllipticalDistortionY,LogDistortionX,LogDistortionY;

ofstream DXFfile("LogDecay.dxf");

int main(void)
{
cout << "Density, Radius & Height<Eg. 20 50 100/Min 1>\n";
cin >> Density >> Radius >> Height;
cout << "Elliptical & Log Distortion X & Y <Eg. 10 1 1 15/Min 1>\n";
cin >> EllipticalDistortionX >> EllipticalDistortionY >> LogDistortionX >> LogDistortionY;
    
PI=4.0*atan(1.0);

DXFfile<<"0\nSECTION\n2\nENTITIES\n";

for (i=0;i<Density;i++)
{
cout << (i+1) << ".";

Lastx=0;Lasty=0;Lastz=0;

for(z=1;z<=Height;z++)
{

x=Radius * sin((z+i)*(2*PI)/Density) * ((log(z/LogDistortionX))/EllipticalDistortionX)+1/Density;
y=Radius * cos((z+i)*(2*PI)/Density) * ((log(z/LogDistortionY))/EllipticalDistortionY)+1/Density;
               
Colour = (int(z+i) % 255);

drawLine (Lastx,Lasty,Lastz,x,y,z,Colour);

Lastx=x;Lasty=y;Lastz=z;
}

if (i==0){
ZFirstx=x;ZFirsty=y;
}else drawLine(ZLastx,ZLasty,Lastz,Lastx,Lasty,Lastz,0);

if (i==Density) drawLine(ZFirstx,ZFirsty,Lastz,Lastx,Lasty,Lastz,0);

ZLastx=x;ZLasty=y;
}

DXFfile<<"0\nENDSEC\n0\nEOF\n";DXFfile.close();
cout<<"Finished\n";
return 0;
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
