/***********************************************************************
* Program:
*    Lab CalendarOptimize, Optimizing a Calendar Program
*    Brother Allred, ECEN 324
* Author:
*    Your Name
* Summary:
*    A summary of the optimizations that you did goes here.
*    A portion of your grade is based on this summary.
*
* Notes:
*    - Your goal is to optimize this program to minimize the run time
*      without using optimization options.  If this code is run with:
*        CAL_COMPILE_OPT="-O3 -DNDEBUG"  
*      There is around a 15x speed up.  
*
*    - Copy starter.cpp to a directory you wish to do your work in and
*      (probably) rename the file:
*        # create some directory to work in
*        mkdir ~/ecen324/lab6    #or ~/ecen324Lab6  or ... 
*        cd ~/theDirectoryYouCreated
*        # copy the starter file and rename it at the same time
*        cp /home/ecen324/calendarOptimize/starter.cpp myLab6.cpp
*
*      You may, at times, also want to have a copy of the original starter.cpp 
*      file in your directory to have the test program run it as well as
*      running the code you are optimizing.
*
*    - You can do pretty much anything with the code you want, as
*      long as it is contained within this file and obeys the following
*      rules:.
*      - Have:  #include "/home/ecen324/calendarOptimize/lab_consts.h"
*      - Use "pos += sprintf(calendar + pos, ..., ...)" for outputting the
*        calendars. You can change how and when your code writes to the
*        output, but you MUST use something of this form in order for the
*        lab to compile with the testing program provided (testCalOpt).
*      - You MUST have "void make_calendars(int[], int[], char*);" defined
*        in this file for your code to compile for the lab.
*      - Month numbers are 1-12
*      - Years numbers are >= 1753
*
*    - You test your optimizations by executing:
*         /home/ecen324/calendarOptimize/testCalOpt
*      This tests ALL of the *.cpp files in current directory!
*      Other options on testCalOpt delete the large temporary files and use
*      a 'grading mode' ouput.  If you are not using the 'diff' results to
*      fix errors in your output, you might want to test using:
*         /home/ecen324/calendarOptimize/testCalOpt -dg
*
*    - Your testing should not be done with the use of any compiler options.
*      However, if you have a copy of the original starter.cpp in your current
*      directory, you can see how well it would run with compiler options 
*      by defining a bash shell environment variable that contains compiler
*      options and telling the 'testCalOpt' program to test with the options.
*      This is done with the following command line:
*
*        CAL_COMPILE_OPT="-Opt1 -Opt2 ..." /home/ecen324/calendarOptimize/testCalOpt -o
*
*      To see specifically how well starter.cpp runs with "-O3 -DNDEBUG" do:
*        CAL_COMPILE_OPT="-O3 -DNDEBUG" /home/ecen324/calendarOptimize/testCalOpt -o
*
*    - To see more options for the test program execute:
*         /home/ecen324/calendarOptimize/testCalOpt -h 
*
*    - Submit your fastest code using the "submit" command.  The code will
*      fail to compile, but that is OK.
*
************************************************************************/

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cassert>

// Get lab specific variables --You must include this #include--
#include "/home/ecen324/calendarOptimize/lab_consts.h"

using namespace std;

// The year the current calendar began to be valid
#define yearSeed 1753
// Just for kicks and giggles
#define buckRogers 2200

/**********************************************************************
 * use modular division and make it one line
 ***********************************************************************/  

bool isLeapYear(int year)
{
   if (year % 400 == 0)
      return true;
   else if (year % 100 == 0)
      return false;
   else if (year % 4 == 0)
      return true;
   else
      return false;
}



/**********************************************************************
 * Use bit manipulation
 ***********************************************************************/
int numDaysInMonth(int month, int year)
{
     
   if (month == 2)
      return 28 + isLeapYear(year);
   else
      return ((month & 1) ^ (month >= 8)) ? 31 : 30;
}

/**********************************************************************
 * DONT CAL IS LEAP TWICE
 ***********************************************************************/
int numDaysInYear(int year)
{
   int days = isLeapYear(year) ? 366 : 365;
   return days;
}


/**********************************************************************
 * YOUR DESCRIPTION HERE
 * Offset is from Monday because Jan 1, 1753 was a Monday.
 ***********************************************************************/
int computeOffset(int month, int year)
{
   int days = 0;

   
   //add all non leap years
   int yearsSinceSeed = year - yearSeed;

   days += yearsSinceSeed * 365;

   //add leap years math
   // Calculate the total number of leap years from year 0 to 'year' and 'yearSeed'
   
   int totalLeapYearsToYear = ((year-1) / 4) - ((year-1) / 100) + ((year-1) / 400);
   // cout("\n\nleaptoyear");
   // cout(totalLeapYearsToYear);
   int totalLeapYearsToYearSeed = 425;


   // Calculate the number of leap years between 'yearSeed' and 'year'
   int leapYearsBetween = totalLeapYearsToYear - totalLeapYearsToYearSeed;

   // Update the 'days' variable
   days += leapYearsBetween;

   // add leap years loop
   // for (int cYear = yearSeed; cYear < year; cYear++)
   // {
   //    //add leap year if it might be one
   //    if(cYear % 4    == 0) // don't bother checking non leaps
   //       if (isLeapYear(cYear))
   //          days++;
   // }

   // Add up the days associated with each month
   for (int cMonth = 1; cMonth < month; cMonth++)
   {
      days += numDaysInMonth(cMonth, year);
   }

   return days % 7;
}

/**********************************************************************
 * Drop to a one liner
 ***********************************************************************/
int displayHeader(int month, int year, char *calendar, int pos) {
    static const char *monthNames[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    pos += sprintf(calendar + pos, "\n%s, %i\n", monthNames[month - 1], year);
    return pos;
}

/********************************************************************
 * YOUR DESCRIPTION HERE
 *******************************************************************/
int displayTable(int numDays, int offset, char *calendar, int pos)
{
   
   // Special case offset==6. We don't want a blank first line
   if (offset == 6)
      offset = -1;

   // // Display the header
   // make this into one print statmeant
   pos += sprintf(calendar + pos, "%s","  Su  Mo  Tu  We  Th  Fr  Sa\n");
   
   // Display the individual locations on the calendar grid
   int dow = 0; // day of week
   int dom;
   for (dom = -offset; dom <= numDays; dom++, dow++)
   {
      // Display the number or not
      if (dom > 0)
         pos += sprintf(calendar + pos, "% 4i", dom);
      if (dom <= 0)
         pos += sprintf(calendar + pos, "    ");

      // Display the end-of-row marker
      if (dow % 7 == 6)
         pos += sprintf(calendar + pos, "\n");
   }

   // Extra ENDL as necessary
   if (dow % 7 != 0)
      pos += sprintf(calendar + pos, "\n");

   return pos;
}  


/**********************************************************************
 * YOUR DESCRIPTION HERE
 ***********************************************************************/
void display(int month, int year, char *calendar)
{
   int pos = 0;
   int offset = computeOffset(month, year);

   
   // Header
   pos += displayHeader(month, year, calendar, pos);

   // Body of the table
   
   pos += displayTable(numDaysInMonth(month, year), offset, calendar, pos);


   return;
}


/**********************************************************************
 * YOU MUST HAVE THIS FUNCTION SIGNATURE DEFINED TO COMPILE YOUR CODE
 * This is used by the driver to test your optimizations
 ***********************************************************************/
void make_calendars(int months[], int years[], char *calendars)
{
   for (int i = 0; i < NUM_CALS; i++)
      display(months[i], years[i], calendars + i * CAL_LENGTH);
}

