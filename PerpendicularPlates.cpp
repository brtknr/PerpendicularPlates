/*
Program Name        Perpendicular Plates

Author              Bharat Kunwar

Program Objective   To produce a seemingly "random" line growing in the Z direction
                    defined by a mathematical function and to draw circular rings
                    around the end of each of the lines on the planes perpendicular
                    to the lines following which connecting up the rings to produce
                    a renderable AutoCAD drawing that exhibits a tunnel-like effect.

Input Variables     Segment      Greater the value, the smoother the circle however
                                 takes longer to process and load.
                                 Recommended values: 30~50
                    Height       The approximate height of the drawing. Corrections
                                 are automatically applied to bring the drawing as
                                 close to the specified height as possible and the
                                 starting point in the drawing is always 0,0,0.
                                 Recommended values: 10~1000                               
                    Growth Rate  How quickly should the central pillar grow? This
                                 directly affects the height as its difficult to
                                 control the height at a faster growth rate (> 1).
                                 Recommended values: 0.01~4.00                                
                    Curve Type   This lets the user choose the curve type. More can
                                 be added by going into the "Equate" module to create
                                 new variations in the way the curve looks.
                                 Recommended values: 0~3 (default is 0)
                                 
Date Completed      3 December 2009

Total Time Spent    ~20 hours 
*/
// Load Libraries
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;

// Predefine Variables
void drawLine(double a[],double b[],int Colour);
void drawShape(double a[],double b[],double c[],double d[],int Colour);
double Equate(double z);
double Pi, Rate, Theta, Phi, Alpha, MaxHeight, ThisX, ThisY, ThisZ, ThisR, LastX, LastY, LastZ, LastR, Correction, h;
int Colour, Segment, Count, Height, Curve;
ofstream DXFfile("PerpendicularPlates.dxf");      // Open the DXF File
ofstream LOGfile("PerpendicularPlates.log");      // Open the log file

// Module that determines the shape of the curve
double Equate(double z){     
       switch (Curve) {
       case 1:return z;break;
       case 2:return z*log(z)+1;break;
       case 3:return sin((2*Pi)/z);break;
       default:return pow(log(Height-z+1),2)+1;break;
       }
}
// Module to draw line
void drawLine(double a[],double b[],int Colour){
    DXFfile<<"0\nLINE\n8\n0\n";
    DXFfile<<"10\n"<<a[0]<<"\n";
    DXFfile<<"20\n"<<a[1]<<"\n";
    DXFfile<<"30\n"<<Correction+a[2]<<"\n";
    DXFfile<<"11\n"<<b[0]<<"\n";
    DXFfile<<"21\n"<<b[1]<<"\n";
    DXFfile<<"31\n"<<Correction+b[2]<<"\n";
    DXFfile<<"62\n"<<Colour<<"\n";
}
// Module to draw shape
void drawShape(double a[],double b[],double c[],double d[],int Colour){
	DXFfile<<"0\n3DFACE\n8\n0\n";
	DXFfile<<"10\n"<<a[0]<<"\n20\n"<<a[1]<<"\n30\n"<<Correction+a[2]<<"\n";
	DXFfile<<"11\n"<<b[0]<<"\n21\n"<<b[1]<<"\n31\n"<<Correction+b[2]<<"\n";
	DXFfile<<"12\n"<<c[0]<<"\n22\n"<<c[1]<<"\n32\n"<<Correction+c[2]<<"\n";
	DXFfile<<"13\n"<<d[0]<<"\n23\n"<<d[1]<<"\n33\n"<<Correction+d[2]<<"\n";
    DXFfile<<"62\n"<<Colour<<"\n";
}
// MAIN MODULE
int main(void){
    // Variable input
    cout<<"Please input the following in the given order separated by a space:\n";
    cout<<"Segments, ~Height, Growth Rate, Curve Type(0~3) <Eg. 50 750 0.5 0>\n";
    cin>>Segment>>Height>>Rate>>Curve;
    cout<<"Starting...\n";   
    // Define normal variables
    Pi=4.0*atan(1.0); Correction=Height; Count=0; 
    // Count layers         
    for(h=1;Correction>0;Correction-=pow(h+=Equate(Correction),Rate)) Count++;
    Correction*=-1; 
    // Apply height correction
    LOGfile<<"Layer Count: "<<Count<<" :Correct Height: "<<Correction+Height<<"\n";
    // Define layer count dependent variables
    double p[Count+1][Segment][3], c[Count+1][5];
    for (int a=0;a<Segment;a++){p[0][a][0]=0; p[0][a][1]=0; p[0][a][2]=Height;}
    c[0][0]=ThisX=0; c[0][1]=ThisY=0; c[0][2]=ThisZ=Height; c[0][3]=ThisR=0; c[0][4]=0; h=1;
    // Determine co-ordinates
    cout<<"Determining coordinates...\n";
    for(int i=1;i<=Count;i++){      
        // Call graph function
        h+=Equate(ThisZ);
        // Exchange values
        LastX=ThisX; LastY=ThisY; LastZ=ThisZ; LastR=ThisR;
        // Determine co-ordinates of the CENTRE of the circle
        // X, Y and Z co-ordinates of the Circle
        c[i][0] = ThisX = log(h*h)*cos(Pi*h*log(h));
        c[i][1] = ThisY = log(h*h)*sin(Pi*h*log(h));
        c[i][2] = ThisZ = LastZ-pow(h,Rate);
        // Radius and Colour of the Circle
        c[i][3] = ThisR = pow((pow(ThisX-LastX,2)+pow(ThisY-LastY,2)+pow(LastZ-ThisZ,2)),0.5);
        c[i][4] = 16 + int(pow(pow(ThisZ,2),0.5)) % 239;              
        // Horizontal angle
        Phi = atan((ThisY-LastY)/(ThisX-LastX));        
        if (ThisX-LastX <0) Phi+=Pi;                    // Phi negative quadrant correction
        // Maximum height for the given radius
        MaxHeight=ThisR*pow((pow(ThisX-LastX,2)+pow(ThisY-LastY,2)),0.5)/(LastZ-ThisZ);   
        // Determine co-ordinates of the PERIMETER of the circle
        for (int a=0;a<Segment;a++){
            Theta=a*2*Pi/Segment;
            // Vertical angle
            //    aTAN (Height of the point....../Radius.)
            Alpha=atan((MaxHeight*cos(Theta-Phi))/(ThisR));
            // X, Y and Z co-ordinates of the Perimeter         
            p[i][a][0]=ThisX+ThisR*cos(Alpha)*cos(Theta);
            p[i][a][1]=ThisY+ThisR*cos(Alpha)*sin(Theta);
            p[i][a][2]=ThisZ+ThisR*sin(Alpha);
        }
        // Store values to the Log file       
        LOGfile<<"DeltaXYZ "<<ThisX<<" - "<<LastX<<" = "<<ThisX-LastX<<" : "<<ThisY<<" - "<<LastY<<" = "<<ThisY-LastY<<" : "<<LastZ<<" - "<<ThisZ<<" = "<<LastZ-ThisZ<<"\nPhi:"<<Phi<<" :MaxHeight:"<<MaxHeight<<" :Radius:"<<c[i][3]<<" :Rxy:"<<pow((pow(c[i][0]-c[i-1][0],2)+pow(c[i][1]-c[i-1][1],2)),0.5)<<"\n";
        cout<<".";         //Progress dot
    }
    LOGfile.close();       // Close the log file
    // Output to file
    cout<<"\nOutputting to file...\n";
    DXFfile<<"0\nSECTION\n2\nENTITIES\n";    
    for  (int j=0; j<Segment; j++){
         for (int i=1; i<=Count; i++){
             // Draw the perimeter 3D shape
             if (j==0) drawShape(p[i][j],p[i-1][j],p[i-1][Segment-1],p[i][Segment-1],int(c[i][4])); 
             else drawShape(p[i][j],p[i-1][j],p[i-1][j-1],p[i][j-1],int(c[i][4]));
             // Draw the centre line
             if (j==0) drawLine(c[i-1],c[i],0);
         }
         cout<<".";       //Progress dot
    }
    //Close the DXF file
    DXFfile<<"0\nENDSEC\n0\nEOF\n";DXFfile.close(); 
    cout<<"\nFinished."; return 0;
}
