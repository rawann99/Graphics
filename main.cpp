#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include<cmath>
#include<algorithm>
#include<vector>
#include <iostream>
#include <ostream>
#include<fstream>
#include<sstream>
using namespace std;


#include <tchar.h>
#include <windows.h>
#include<iostream>
#define FILE_MENU_SAVE 1
#define FILE_MENU_LOAD 2
#define FILE_MENU_CLEAR 3
#define FILE_MENU_DDA 4
#define FILE_MENU_MIDPOINT 5
#define FILE_MENU_PARAMITRIC 6
#define FILE_MENU_RED 7
#define FILE_MENU_GREEN 8
#define FILE_MENU_BLUE 9
#define FILE_MENU_YELLOW 10
#define FILE_MENU_CIRCLE_MIDPOINT_MODIFIED 12
#define FILE_MENU_CIRCLE_ITERATIVE_POLAR 13
#define FILE_MENU_CIRCLE_MIDPOINT 14
#define FILE_MENU_CIRCLE_DIRECT 15
#define FILE_MENU_ELLIPSE_DIRECT 16
#define FILE_MENU_ELLIPSE_Polar 17
#define FILE_MENU_FIRST_QUARTER 18
#define FILE_MENU_SECOND_QUARTER 19
#define FILE_MENU_THIRD_QUARTER  20
#define FILE_MENU_FOURTH_QUARTER 21
#define FILE_MENU_LINE_CLIPPING 22
#define FILE_MENU_CIRCLE_CLIPPING 23
#define FILE_MENU_CLIPPING_WINDOW 24
#define FILE_MENU_ELLIPSE_CLLIPING 25







/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    //wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;  The Default Color
    wincl.hCursor = LoadCursor (NULL, IDC_HAND);
    wincl.hbrBackground = (HBRUSH) RGB(0,0,0);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               1000,                 /* The programs width */
               1000,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
static COLORREF RED=RGB(255,0,0);
static COLORREF GREEN=RGB(0,255,0);
static COLORREF BLUE=RGB(0,0,255);
static COLORREF YELLOW=RGB(255,255,0);
static COLORREF BLACK =RGB(0,0,0);
static COLORREF color=BLACK;

struct Shape
{
    string algo;
    string color;
    int x1;
    int y1;
    int x2;
    int y2;
};
int ROUND (double x)
{
    return (int)(x+.5);
}
typedef struct Point
{
    int x;
    int y;
} pt;
pt winMin = {200, 350};
pt winMax = {900, 800};
pt p1 = {400, 400};
pt p2 = {1200, 500};
union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1;
        unsigned right:1;
        unsigned bottom:1;
        unsigned top:1;

    };
};

OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)out.left=1;
    else if(x>xright)out.right=1;
    if(y<ytop)out.top=1;
    else if(y>ybottom)out.bottom=1;
    return out;
}
void V_Intersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void H_Intersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)V_Intersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)H_Intersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)V_Intersect(x1,y1,x2,y2,xright,&xi,&yi);
            else H_Intersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)V_Intersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)H_Intersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)V_Intersect(x1,y1,x2,y2,xright,&xi,&yi);
            else H_Intersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {
        MoveToEx(hdc,ROUND(x1),ROUND(y1),NULL);
        LineTo(hdc,ROUND(x2),ROUND(y2));
    }
}
void Point_Clipping(HDC hdc,int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)
        SetPixel(hdc,x,y,color);
}
void Drawcircle_Direct_Clipping(HDC hdc,int xc,int yc,int R,COLORREF color)
{
    double d=1.0/R;
    for(double theta =0; theta<=6.28; theta+=d)
    {
        int x= ROUND ( xc + R * cos(theta) );
        int y = ROUND (yc + R * sin (theta));
        Point_Clipping(hdc,x,y,winMin.x,winMin.y,winMax.x,winMax.y,color);

    }

}
void DrawEllipse_Polar_Clipping(HDC hdc,int xc,int yc,int R1,int R2,COLORREF color)
{
    for(double theta =0; theta<=6.28; theta+=.0001)
    {
        int x= ROUND ( xc + R1 * cos(theta) );
        int y = ROUND (yc + R2 * sin (theta));
        Point_Clipping(hdc,x,y,winMin.x,winMin.y,winMax.x,winMax.y,color);
    }

}


void Draw_Rectangle(HDC hdc,pt winMin, pt winMax)
{
    int x, y;
    x = winMin.x;
    for(y = winMin.y; y < winMax.y; y++)
    {
        SetPixel(hdc,x,y, color);
    }
    y = winMin.y;
    for(x = winMin.x; x < winMax.x; x++)
    {
        SetPixel(hdc,x,y,  color);
    }
    x = winMax.x;
    for(y = winMin.y; y < winMax.y; y++)
    {
        SetPixel(hdc,x,y,  color);
    }
    y = winMax.y;
    for(x = winMin.x; x < winMax.x; x++)
    {
        SetPixel(hdc,x,y,  color);
    }
}



void DrawlineDDA_Clipping(HDC  hdc,int x1,int y1,int x2,int y2, COLORREF color)  // for floating point multiplication
{
    int dx=x2-x1;
    int dy=y2-y1;
    if(abs(dy)<=abs(dx))
    {

        double m= (double)dy/dx;
        double c= y1-m*x1;
        if(x1>x2)
        {
            std:: swap (x1,x2);
            std :: swap (y1,y2);
        }

        int x=x1;
        double y=y1;

        while (x<x2)
        {
            x++;
            y+=m;
            CohenSuth(hdc,x1,y1,x2,y2,winMin.x,winMin.y,winMax.x,winMax.y,color);
        }
    }
    else
    {
        double m=( double) dx/dy;
        if(y1>y2)
        {
            std:: swap (x1,x2);
            std :: swap (y1,y2);
        }

        double x=x1;
        int y=y1;

        while(y<=y2)
        {
            y++;
            x+=m;
            CohenSuth(hdc,x1,y1,x2,y2,winMin.x,winMin.y,winMax.x,winMax.y,color);

        }
    }
}

void DrawlineDDA(HDC  hdc,int x1,int y1,int x2,int y2, COLORREF color)  // for floating point multiplication
{
    int dx=x2-x1;
    int dy=y2-y1;
    if(abs(dy)<=abs(dx))
    {

        double m= (double)dy/dx;
        double c= y1-m*x1;
        if(x1>x2)
        {
            std:: swap (x1,x2);
            std :: swap (y1,y2);
        }

        int x=x1;
        double y=y1;
        SetPixel(hdc,x,y,color);
        while (x<x2)
        {
            x++;
            y+=m;
            SetPixel(hdc, x,y,color);
        }
    }
    else
    {
        double m=( double) dx/dy;
        if(y1>y2)
        {
            std:: swap (x1,x2);
            std :: swap (y1,y2);
        }

        double x=x1;
        int y=y1;
        SetPixel(hdc,x,y,color);
        while(y<=y2)
        {
            y++;
            x+=m;
            SetPixel(hdc,x,y,color);

        }
    }
}

void Draw_8_points(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);


}
void Draw_8_points_quarter_1(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    DrawlineDDA(hdc,xc,yc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);
    DrawlineDDA(hdc,xc,yc,xc+y,yc-x,color);


}
void Draw_8_points_quarter_2(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    DrawlineDDA(hdc,xc,yc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    DrawlineDDA(hdc,xc,yc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);


}
void Draw_8_points_quarter_3(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    DrawlineDDA(hdc,xc,yc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    DrawlineDDA(hdc,xc,yc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);


}
void Draw_8_points_quarter_4(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    DrawlineDDA(hdc,xc,yc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    DrawlineDDA(hdc,xc,yc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);


}
void DrawCircle_MidPoint_QUARTER_1_FILLING(HDC hdc, int xc, int yc, int R, COLORREF color )// midpoint
{
    int x = 0, y = R;
    int d = 1 - R;
    Draw_8_points_quarter_1(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d <= 0)
        {
            d += 2 * x + 3;
            x++;
        }
        else
        {
            d += 2 * (x - y) + 5;
            x++;
            y--;
        }
        Draw_8_points_quarter_1(hdc, xc, yc, x, y, color);
    }
}
void DrawCircle_MidPoint_QUARTER_2_FILLING(HDC hdc, int xc, int yc, int R, COLORREF color )// midpoint
{
    int x = 0, y = R;
    int d = 1 - R;
    Draw_8_points_quarter_2(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d <= 0)
        {
            d += 2 * x + 3;
            x++;
        }
        else
        {
            d += 2 * (x - y) + 5;
            x++;
            y--;
        }
        Draw_8_points_quarter_2(hdc, xc, yc, x, y, color);
    }
}
void DrawCircle_MidPoint_QUARTER_3_FILLING(HDC hdc, int xc, int yc, int R, COLORREF color )// midpoint
{
    int x = 0, y = R;
    int d = 1 - R;
    Draw_8_points_quarter_3(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d <= 0)
        {
            d += 2 * x + 3;
            x++;
        }
        else
        {
            d += 2 * (x - y) + 5;
            x++;
            y--;
        }
        Draw_8_points_quarter_3(hdc, xc, yc, x, y, color);
    }
}
void DrawCircle_MidPoint_QUARTER_4_FILLING(HDC hdc, int xc, int yc, int R, COLORREF color )// midpoint
{
    int x = 0, y = R;
    int d = 1 - R;
    Draw_8_points_quarter_4(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d <= 0)
        {
            d += 2 * x + 3;
            x++;
        }
        else
        {
            d += 2 * (x - y) + 5;
            x++;
            y--;
        }
        Draw_8_points_quarter_4(hdc, xc, yc, x, y, color);
    }
}
void DrawCircle_MidPoint_Modified(HDC hdc, int xc, int yc, int R, COLORREF color )// modified Midpoin
{
    int x = 0, y = R;
    int d = 1 - R;
    int d1 = 3, d2 = 5 - 2 * R;
    Draw_8_points(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d <= 0)
        {
            d += d1;
            d2 += 2;

        }
        else
        {
            d += d2;
            d2 += 4;
            y--;
        }
        x++;
        d1 += 2;
        Draw_8_points(hdc, xc, yc, x, y, color);
    }
}
void Drawelipse(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
}
void Draw_Ellipse_Direct(HDC hdc, int xc, int yc, int A,int B, COLORREF color )
{

    int x = 0, y = B;
    Drawelipse(hdc,xc, yc, x, y, color);

    while (x*(B*B) < ((A*A)*y))
    {
        x++;
        y =round( sqrt(((B*B)*((A*A)-(x*x)))/(A*A)));
        Drawelipse(hdc,xc, yc, x, y, color);


    }
    y = 0, x = A;
    Drawelipse(hdc,xc, yc, x, y, color);


    while (x*(B*B) >((A*A)*y))
    {
        y++;
        x = round (sqrt(((A*A)*((B*B) - (y * y)))/(B*B)));
        Drawelipse(hdc,xc, yc, x, y, color);


    }
}

void DrawCircle_MidPoint(HDC hdc, int xc, int yc, int R, COLORREF color )// midpoint
{
    int x = 0, y = R;
    int d = 1 - R;
    Draw_8_points(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d <= 0)
        {
            d += 2 * x + 3;
            x++;
        }
        else
        {
            d += 2 * (x - y) + 5;
            x++;
            y--;
        }
        Draw_8_points(hdc, xc, yc, x, y, color);
    }
}

void DrawCircle_Iterative_Polar(HDC hdc,int xc,int yc,int R,COLORREF color) // iterative Polar
{
    double dtheta = 1.0 / R;
    double x=R,y=0;
    double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
    Draw_8_points(hdc,xc,yc,x,y,color);
    while(x>y)
    {
        double x1=x*cdtheta-y*sdtheta;
        y=x*sdtheta+y*cdtheta;
        x=x1;


        Draw_8_points(hdc,xc,yc,x,y,color);
    }
}
void Drawline_Parametric(HDC hdc,int x1,int y1,int x2,int y2, COLORREF c) // parameteric eq modified
{

    int dx=x2-x1;
    int dy =y2-y1;
    double dt=1.0/  std:: max(dx,dy);
    for(double t=0.0; t<=1; t+=dt)
    {
        int x=ROUND (x1+t*dx);
        int y =ROUND (y1+t*dy);
        SetPixel(hdc,x,y,c);
    }
}



void Drawcircle_Direct(HDC hdc,int xc,int yc,int R,COLORREF color)
{
    double d=1.0/R;
    for(double theta =0; theta<=6.28; theta+=d)
    {
        int x= ROUND ( xc + R * cos(theta) );
        int y = ROUND (yc + R * sin (theta));
        SetPixel( hdc, x,y,color );
    }

}
void DrawEllipse_Polar(HDC hdc,int xc,int yc,int R1,int R2,COLORREF color)
{
    for(double theta =0; theta<=6.28; theta+=.0001)
    {
        int x= ROUND ( xc + R1 * cos(theta) );
        int y = ROUND (yc + R2 * sin (theta));
        SetPixel( hdc, x,y,color );
    }

}
void Drawline_MidPoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color )// Bresenham Algorithm(midpoint)
{
    int dy = y2 - y1;
    int dx = x2 - x1;
    if (dy < dx)
    {
        if (x2 < x1)
        {
            std:: swap(x1, x2);
            std:: swap(y1, y2);
        }
        int x = x1;
        int y = y1;
        SetPixel(hdc,x1,y1,color);
        int d = dx - 2*dy;
        int d1 = -2 * dy;
        int d2 = 2 * dx - 2 * dy;
        while (x < x2)
        {
            if (d > 0)
            {
                x++;
                d += d1;
            }
            else
            {
                x++;
                y++;
                d += d2;
            }
            SetPixel(hdc, x, y, color);
        }
    }
    else
    {

        if (y2 < y1)
        {
            std:: swap(x1, x2);
            std::swap(y1, y2);
        }
        int x = x1;
        int y = y1;
        SetPixel(hdc, x1, y1, color);
        int d = 2 * dx - dy;
        int d1 = 2 * dx;
        int d2 = 2 * dx - 2 * dy;
        while (y < y2)
        {
            if (d < 0)
            {
                y++;
                d += d1;
            }
            else
            {
                x++;
                y++;
                d += d2;
            }
            SetPixel(hdc, x, y, color);
        }
    }
}


HMENU hMenu;
HMENU hShape;
void add_Menu(HWND hwnd)
{
    hMenu=CreateMenu();
    HMENU hfileMenu=CreateMenu();
    hShape=CreateMenu();
    HMENU hColor=CreateMenu();
    HMENU hCircle=CreateMenu();
    HMENU hLine=CreateMenu();
    HMENU hEllipse=CreateMenu();
    HMENU hfilling=CreateMenu();
    HMENU hclipping=CreateMenu();

    AppendMenuA(hMenu,MF_POPUP,(UINT_PTR)hfileMenu,"Menu");
    AppendMenu(hEllipse,MF_STRING,FILE_MENU_ELLIPSE_DIRECT,"DIRECT");
    AppendMenu(hEllipse,MF_STRING,FILE_MENU_ELLIPSE_Polar,"POLAR");
    AppendMenu(hCircle,MF_STRING,FILE_MENU_CIRCLE_DIRECT,"DIRECT");
    AppendMenu(hCircle,MF_STRING,FILE_MENU_CIRCLE_ITERATIVE_POLAR,"POLARI_ITERATIVE_POLAR");
    AppendMenu(hCircle,MF_STRING,FILE_MENU_CIRCLE_MIDPOINT,"MIDPONT");
    AppendMenu(hCircle,MF_STRING, FILE_MENU_CIRCLE_MIDPOINT_MODIFIED,"MODIFEID_MIDPOINT");
    AppendMenu(hLine,MF_STRING,FILE_MENU_DDA,"DDA");
    AppendMenu(hLine,MF_STRING,FILE_MENU_MIDPOINT,"MIDPOINT");
    AppendMenu(hLine,MF_STRING,FILE_MENU_PARAMITRIC,"PARAMITRIC");
    AppendMenu(hColor,MF_POPUP,FILE_MENU_RED,"RED");
    AppendMenu(hColor,MF_POPUP,FILE_MENU_GREEN,"GREEN");
    AppendMenu(hColor,MF_POPUP,FILE_MENU_BLUE,"BLUE");
    AppendMenu(hColor,MF_POPUP,FILE_MENU_YELLOW,"YELLOW");

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hCircle,"CIRCLE");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hLine,"LINE");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hEllipse,"ELLIPSE");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hfilling,"CIRCLE WITH FILLING");
    AppendMenu(hfilling,MF_STRING,FILE_MENU_FIRST_QUARTER,"FIRST_QUARTER");
    AppendMenu(hfilling,MF_STRING,FILE_MENU_SECOND_QUARTER,"SECOND_QUARTER");
    AppendMenu(hfilling,MF_STRING,FILE_MENU_THIRD_QUARTER,"THIRD_QUARTER");
    AppendMenu(hfilling,MF_STRING,FILE_MENU_FOURTH_QUARTER,"FOURTH_QUARTER");
    AppendMenu(hMenu,MF_STRING,FILE_MENU_CLIPPING_WINDOW,"CLIPPING WINDOW");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hclipping,"CLIPPING SHAPES");
    AppendMenu(hclipping,MF_STRING,FILE_MENU_LINE_CLIPPING,"LINE_CLIPPING");
    AppendMenu(hclipping,MF_STRING,FILE_MENU_CIRCLE_CLIPPING,"CIRCLE_CLIPPING");
    AppendMenu(hclipping,MF_STRING,FILE_MENU_ELLIPSE_CLLIPING,"ELLIPSE_CLIPPING");
    AppendMenu(hfileMenu,MF_STRING,FILE_MENU_SAVE,"SAVE");
    AppendMenu(hfileMenu,MF_STRING,FILE_MENU_LOAD,"LODA");

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hColor,"COLOR");
    AppendMenu(hfileMenu,MF_STRING,FILE_MENU_CLEAR,"CLEAR");

    SetMenu(hwnd,hMenu);
}

static int xm1,ym1,xm2,ym2,r,menu=0,r1=100,r2=50,A=500,B=400, R,L=0,xl1,yl1,xl2,yl2,R1;
static Shape node[100];
HDC hdc;
vector <string> SPLIT(string str)
{

    vector <string> split;
    string word="";
    for (auto x : str)
    {
        if (x == ' ')
        {

            split.push_back(word);
            word="";
        }
        else
        {
            word = word + x;
        }
    }
    return split;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    hdc=GetDC(hwnd);

    switch (message)                  /* handle the messages */
    {

    case WM_COMMAND:
        add_Menu(hwnd);
        add_Menu(hwnd);
        switch(wParam)
        {
        case FILE_MENU_DDA:
            menu=1;
            break;
        case FILE_MENU_RED:
            color=RED;
            break;
        case FILE_MENU_GREEN:
            color=GREEN;
            break;
        case FILE_MENU_BLUE:
            color=BLUE;
            break;
        case FILE_MENU_YELLOW:
            color=YELLOW;
            break;
        case FILE_MENU_CIRCLE_MIDPOINT_MODIFIED:
            menu=4;
            break;
        case FILE_MENU_PARAMITRIC:
            menu=2;
            break;
        case FILE_MENU_MIDPOINT:
            menu=3;
            break;
        case FILE_MENU_CIRCLE_ITERATIVE_POLAR:
            menu=5;
            break;
        case FILE_MENU_CIRCLE_MIDPOINT:
            menu=6;
            break;
        case FILE_MENU_CIRCLE_DIRECT:
            menu=7;
            break;
        case FILE_MENU_ELLIPSE_Polar:
            menu=8;
            break;
        case FILE_MENU_ELLIPSE_DIRECT:
            menu=9;
            break;
        case FILE_MENU_FIRST_QUARTER:
            menu=10;
            break;
        case FILE_MENU_SECOND_QUARTER:
            menu=11;
            break;
        case FILE_MENU_THIRD_QUARTER:
            menu=12;
            break;
        case FILE_MENU_FOURTH_QUARTER:
            menu=13;
            break;
        case FILE_MENU_CLIPPING_WINDOW:
            menu =14;
            break;

        case FILE_MENU_LINE_CLIPPING :
            menu=15;
            break;
        case FILE_MENU_CIRCLE_CLIPPING:
            menu =16;
            break;
        case FILE_MENU_ELLIPSE_CLLIPING :
            menu = 17;
            break;

        case FILE_MENU_SAVE:
            menu=18;
            break;
        case FILE_MENU_LOAD:
            menu=19;
            break;
        case FILE_MENU_CLEAR:
            menu=20;

            break;



        }

        break;

    case WM_LBUTTONDOWN:
    {

        xm1=LOWORD(lParam);
        ym1=HIWORD(lParam);
        break;
    }
    case WM_LBUTTONUP:
    {
        xm2=LOWORD(lParam);
        ym2=HIWORD(lParam);

        if(menu==1)
        {

            DrawlineDDA(hdc,xm1,ym1,xm2,ym2,color);

            if(color==YELLOW)
            {
                node[L].algo="lineDDA";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="lineDDA";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="lineDDA";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="lineDDA";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="lineDDA";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
        }

        if(menu==2)
        {
            Drawline_Parametric(hdc,xm1,ym1,xm2,ym2,color);
            if(color==YELLOW)
            {
                node[L].algo="line_Parametric";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="line_Parametric";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="line_Parametric";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="line_Parametric";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="line_Parametric";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==3)
        {
            Drawline_MidPoint(hdc,xm1,ym1,xm2,ym2,color);

            if(color==YELLOW)
            {
                node[L].algo="line_Midpoint";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="line_Midpoint";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="line_Midpoint";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="line_Midpoint";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="line_Midpoint";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }


        }
        if(menu==4)
        {
            r=ROUND(std::sqrt(std::pow(xm2-xm1,2.0)+pow(ym2-ym1,2.0)));//R:eq
            DrawCircle_MidPoint_Modified(hdc,xm1,ym1,r,color);

            if(color==YELLOW)
            {
                node[L].algo="Circle_MidPoint_Modified";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Circle_MidPoint_Modified";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Circle_MidPoint_Modified";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Circle_MidPoint_Modified";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Circle_MidPoint_Modified";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==5)
        {
            r=ROUND(std::sqrt(std::pow(xm2-xm1,2.0)+pow(ym2-ym1,2.0)));//R:eq
            DrawCircle_Iterative_Polar(hdc,xm1,ym1,r,color);
            if(color==YELLOW)
            {
                node[L].algo="Circle_Iterative_Polar";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Circle_Iterative_Polar";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Circle_Iterative_Polar";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Circle_Iterative_Polar";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Circle_Iterative_Polar";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==6)
        {
            r=ROUND(std::sqrt(std::pow(xm2-xm1,2.0)+pow(ym2-ym1,2.0)));//R:eq
            DrawCircle_MidPoint(hdc,xm1,ym1,r,color);
            if(color==YELLOW)
            {
                node[L].algo="Circle_MidPoint";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Circle_MidPoint";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Circle_MidPoint";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Circle_MidPoint";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Circle_MidPoint";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==7)
        {
            r=ROUND(std::sqrt(std::pow(xm2-xm1,2.0)+pow(ym2-ym1,2.0)));//R:eq
            Drawcircle_Direct(hdc,xm1,ym1,r,color);
            if(color==YELLOW)
            {
                node[L].algo="circle_Direct";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="circle_Direct";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="circle_Direct";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="circle_Direct";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="circle_Direct";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==8)
        {

            DrawEllipse_Polar(hdc,xm1,ym1,r1,r2,color);
            if(color==YELLOW)
            {
                node[L].algo="Ellipse_Polar";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Ellipse_Polar";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Ellipse_Polar";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Ellipse_Polar";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Ellipse_Polar";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==9)
        {

            Draw_Ellipse_Direct(hdc,xm1,ym1,A,B,color);
            if(color==YELLOW)
            {
                node[L].algo="Ellipse_Direct";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Ellipse_Direct";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Ellipse_Direct";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Ellipse_Direct";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Ellipse_Direct";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }


        }
        if(menu==10)
        {
            r=ROUND(std::sqrt(std::pow(xm2-xm1,2.0)+pow(ym2-ym1,2.0)));//R:eq
            DrawCircle_MidPoint_QUARTER_1_FILLING(hdc,xm1,ym1,r,color);
            if(color==YELLOW)
            {
                node[L].algo="Circle_MidPoint_QUARTER_1_FILLING";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Circle_MidPoint_QUARTER_1_FILLING";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Circle_MidPoint_QUARTER_1_FILLING";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Circle_MidPoint_QUARTER_1_FILLING";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Circle_MidPoint_QUARTER_1_FILLING";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==11)
        {
            r=ROUND(std::sqrt(std::pow(xm2-xm1,2.0)+pow(ym2-ym1,2.0)));//R:eq
            DrawCircle_MidPoint_QUARTER_2_FILLING(hdc,xm1,ym1,r,color);
            if(color==YELLOW)
            {
                node[L].algo="Circle_MidPoint_QUARTER_2_FILLING";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Circle_MidPoint_QUARTER_2_FILLING";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Circle_MidPoint_QUARTER_2_FILLING";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Circle_MidPoint_QUARTER_2_FILLING";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Circle_MidPoint_QUARTER_2_FILLING";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==12)
        {
            r=ROUND(std::sqrt(std::pow(xm2-xm1,2.0)+pow(ym2-ym1,2.0)));//R:eq
            DrawCircle_MidPoint_QUARTER_3_FILLING(hdc,xm1,ym1,r,color);
            if(color==YELLOW)
            {
                node[L].algo="Circle_MidPoint_QUARTER_3_FILLING";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Circle_MidPoint_QUARTER_3_FILLING";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Circle_MidPoint_QUARTER_3_FILLING";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Circle_MidPoint_QUARTER_3_FILLING";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Circle_MidPoint_QUARTER_3_FILLING";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==13)
        {
            r=ROUND(std::sqrt(std::pow(xm2-xm1,2.0)+pow(ym2-ym1,2.0)));//R:eq
            DrawCircle_MidPoint_QUARTER_4_FILLING(hdc,xm1,ym1,r,color);
            if(color==YELLOW)
            {
                node[L].algo="Circle_MidPoint_QUARTER_4_FILLING";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Circle_MidPoint_QUARTER_4_FILLING";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Circle_MidPoint_QUARTER_4_FILLING";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Circle_MidPoint_QUARTER_4_FILLING";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Circle_MidPoint_QUARTER_4_FILLING";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu ==14)
        {
            Draw_Rectangle(hdc,winMin,winMax);

            node[L].algo="Draw_Rectangle";
            node[L].color="BLACK";
            node[L].x1=xm1;
            node[L].x2=xm2;
            node[L].y1=ym1;
            node[L].y2=ym2;
            L++;




        }
        if(menu ==15)
        {
            DrawlineDDA_Clipping(hdc,xm1,ym1,xm2,ym2,color);
            if(color==YELLOW)
            {
                node[L].algo="DrawlineDDA_Clipping";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="DrawlineDDA_Clipping";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="DrawlineDDA_Clipping";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="DrawlineDDA_Clipping";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="DrawlineDDA_Clipping";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu ==16)
        {
            R=ROUND(std::sqrt(std::pow(xm2-xm1,2.0)+pow(ym2-ym1,2.0)));
            Drawcircle_Direct_Clipping(hdc,xm1,ym2,R,color);

            if(color==YELLOW)
            {
                node[L].algo="circle_Direct_Clipping";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="circle_Direct_Clipping";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="circle_Direct_Clipping";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="circle_Direct_Clipping";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="circle_Direct_Clipping";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
        }
        if(menu ==17)
        {
            DrawEllipse_Polar_Clipping(hdc,xm1,ym1,r1,r2,color);
            if(color==YELLOW)
            {
                node[L].algo="Ellipse_Polar_Clipping";
                node[L].color="YELLOW";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==RED)
            {
                node[L].algo="Ellipse_Polar_Clipping";
                node[L].color="RED";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLUE)
            {
                node[L].algo="Ellipse_Polar_Clipping";
                node[L].color="BLUE";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==GREEN)
            {
                node[L].algo="Ellipse_Polar_Clipping";
                node[L].color="GREEN";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }
            if(color==BLACK)
            {
                node[L].algo="Ellipse_Polar_Clipping";
                node[L].color="BLACK";
                node[L].x1=xm1;
                node[L].x2=xm2;
                node[L].y1=ym1;
                node[L].y2=ym2;
                L++;

            }

        }
        if(menu==18)
        {
            ofstream shape;
            shape.open("shapefile.txt");
            for(int i=0; i<L; i++)
            {

                shape<<node[i].algo<<" "<<node[i].color<<" "<<node[i].x1<<" "<<node[i].y1<<" "<<node[i].x2<<" "<<node[i].y2<<" "<<endl;

            }
            shape.close();

        }
        if(menu==19)
        {
            ifstream load;
            load.open("shapefile.txt");
            string line;
            vector<string>split;
            while (getline(load,line))
            {
                split=SPLIT(line);
                stringstream temp(split[2]);
                temp>>xl1;
                stringstream temp1(split[3]);
                temp1>>yl1;
                stringstream temp2(split[4]);
                temp2>>xl2;
                stringstream temp3(split[5]);
                temp3>>yl2;
                if(split[1]=="BLACK")
                {
                    color=BLACK;
                }
                if(split[1]=="YELLOW")
                {
                    color=YELLOW;
                }
                if(split[1]=="RED")
                {
                    color=RED;
                }
                if(split[1]=="BLUE")
                {
                    color=BLUE;
                }
                if(split[1]=="GREEN")
                {
                    color=GREEN;
                }
                if(split[0]=="lineDDA" )
                {
                    DrawlineDDA(hdc,xl1,yl1,xl2,yl2,color);
                }
                if(split[0]=="line_Parametric")
                {
                    Drawline_Parametric(hdc,xl1,yl1,xl2,yl2,color);
                }
                if(split[0]=="line_Midpoint")
                {
                    Drawline_MidPoint(hdc,xl1,yl1,xl2,yl2,color);
                }
                if(split[0]=="Circle_MidPoint_Modified")
                {
                    R1=ROUND(std::sqrt(std::pow(xl2-xl1,2.0)+pow(yl2-yl1,2.0)));//R:eq
                    DrawCircle_MidPoint_Modified(hdc,xl1,yl1,R1,color);
                }
                if(split[0]=="Circle_Iterative_Polar")
                {
                    R1=ROUND(std::sqrt(std::pow(xl2-xl1,2.0)+pow(yl2-yl1,2.0)));//R:eq
                    DrawCircle_Iterative_Polar(hdc,xl1,yl1,R1,color);
                }
                if(split[0]=="Circle_MidPoint")
                {
                    R1=ROUND(std::sqrt(std::pow(xl2-xl1,2.0)+pow(yl2-yl1,2.0)));//R:eq
                    DrawCircle_MidPoint(hdc,xl1,yl1,R1,color);
                }
                if(split[0]=="circle_Direct")
                {
                    R1=ROUND(std::sqrt(std::pow(xl2-xl1,2.0)+pow(yl2-yl1,2.0)));//R:eq
                    Drawcircle_Direct(hdc,xl1,yl1,R1,color);
                }
                if(split[0]=="Ellipse_Polar")
                {

                    DrawEllipse_Polar(hdc,xl1,yl1,r1,r2,color);
                }
                if(split[0]=="Ellipse_Direct")
                {
                    Draw_Ellipse_Direct(hdc,xl1,yl1,A,B,color);
                }
                if(split[0]=="Circle_MidPoint_QUARTER_1_FILLING")
                {
                    R1=ROUND(std::sqrt(std::pow(xl2-xl1,2.0)+pow(yl2-yl1,2.0)));//R:eq
                    DrawCircle_MidPoint_QUARTER_1_FILLING(hdc,xl1,yl1,R1,color);
                }
                if(split[0]=="Circle_MidPoint_QUARTER_2_FILLING")
                {
                    R1=ROUND(std::sqrt(std::pow(xl2-xl1,2.0)+pow(yl2-yl1,2.0)));//R:eq
                    DrawCircle_MidPoint_QUARTER_2_FILLING(hdc,xl1,yl1,R1,color);
                }
                if(split[0]=="Circle_MidPoint_QUARTER_3_FILLING")
                {
                    R1=ROUND(std::sqrt(std::pow(xl2-xl1,2.0)+pow(yl2-yl1,2.0)));//R:eq
                    DrawCircle_MidPoint_QUARTER_3_FILLING(hdc,xl1,yl1,R1,color);
                }
                if(split[0]=="Circle_MidPoint_QUARTER_4_FILLING")
                {
                    R1=ROUND(std::sqrt(std::pow(xl2-xl1,2.0)+pow(yl2-yl1,2.0)));//R:eq
                    DrawCircle_MidPoint_QUARTER_4_FILLING(hdc,xl1,yl1,R1,color);
                }
                if(split[0]=="Draw_Rectangle")
                {
                    Draw_Rectangle(hdc,winMin,winMax);

                }
                if(split[0]=="DrawlineDDA_Clipping")
                {
                    DrawlineDDA_Clipping(hdc,xl1,yl1,xl2,yl2,color);
                }
                if(split[0]=="Ellipse_Polar_Clipping")
                {
                    DrawEllipse_Polar_Clipping(hdc,xl1,yl1,r1,r2,color);
                }
                if(split[0]=="circle_Direct_Clipping")
                {
                    R1=ROUND(std::sqrt(std::pow(xl2-xl1,2.0)+pow(yl2-yl1,2.0)));//R:eq
                    Drawcircle_Direct_Clipping(hdc,xl1,yl1,R1,color);
                }

                split.clear();


            }
        }
        if(menu==20)
        {
            ShowWindow(hwnd, SW_HIDE);

            ShowWindow(hwnd, SW_SHOW);

        }


        break;
    }



    case WM_CREATE:
        add_Menu(hwnd);
        add_Menu(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
