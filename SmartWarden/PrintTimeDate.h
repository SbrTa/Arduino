#include "TimeHead.h"

char PrintTime[100];
char PrintHH[100];
char PrintMM[100];
char PrintSS[100];
char AMPM[100];
char PrintDate[100];
char HomeTime[100];
char HHMM[100];


void PTD()
{
    ts Time = GET_TIME();
    char tt[10];

// Add Hour:Min:Sec AM/PM to PrintTime as a string
    *PrintTime = '\0';
    *HomeTime = '\0';
    *HHMM = '\0';
    int TimeHH = Time.hour;
    if(Time.hour<10) strcat(HHMM,"0");
    sprintf(tt, "%d", TimeHH);
    strcat(HHMM,tt);
    strcat(HHMM,":");
    sprintf(tt, "%d", Time.min);
    strcat(HHMM,tt);

    if(TimeHH>=12)
    {
        strcpy(AMPM,"PM");
        TimeHH = TimeHH-12;
    }
    else
    {
        strcpy(AMPM,"AM");
    }
    sprintf(tt, "%d", TimeHH);
    strcpy(PrintHH,tt);
    if(TimeHH<=9) strcat(PrintTime,"0");
    strcat(PrintTime,tt);
    strcat(PrintTime,":");
    //Serial.println(PrintTime);

    sprintf(tt, "%d", Time.min);
    strcpy(PrintMM,tt);
    if(Time.min<=9) strcat(PrintTime,"0");
    strcat(PrintTime,tt);
    strcpy(HomeTime,PrintTime);
    strcat(PrintTime,":");
    //Serial.println(PrintTime);

    sprintf(tt, "%d", Time.sec);
    strcpy(PrintSS,tt);
    if(Time.sec<=9) strcat(PrintTime,"0");
    strcat(PrintTime,tt);
    strcat(PrintTime," ");
    strcat(PrintTime,AMPM);
    strcat(HomeTime," ");
    strcat(HomeTime,AMPM);

    //Serial.print("Time : "); Serial.println(PrintTime);


// Add Date-Month-Year to PrintDate as a string
    *PrintDate = '\0';
    sprintf(tt, "%d", Time.mday);
    if(Time.mday<10) strcat(PrintDate,"0");
    strcat(PrintDate,tt);
    strcat(PrintDate,"-");

    sprintf(tt, "%d", Time.mon);
    if(Time.mon<10) strcat(PrintDate,"0");
    strcat(PrintDate,tt);
    strcat(PrintDate,"-");

    sprintf(tt, "%d", Time.year);
    strcat(PrintDate,tt);

    //Serial.print("Date : "); Serial.println(PrintDate);

}

