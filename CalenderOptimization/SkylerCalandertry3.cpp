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
 * use modular division
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

   // Add up the days associated with each year
   
   for (int cYear = yearSeed; cYear < year; cYear++)
   {
      //iff odd no need to call and if >=1756 no need 
      days += 365+isLeapYear(cYear);
   }

   // Add up the days associated with each month
   for (int cMonth = 1; cMonth < month; cMonth++)
   {
      days += numDaysInMonth(cMonth, year);
   }

   return days % 7;
}

/**********************************************************************
 * use SWITCH CASE
 ***********************************************************************/
int displayHeader(int month, int year, char *calendar, int pos)
{
   pos += sprintf(calendar + pos, "\n");

   switch(month) {
   case 1: pos += sprintf(calendar + pos, "%s", "January"); break;
   case 2: pos += sprintf(calendar + pos, "%s", "February"); break;
   case 3: pos += sprintf(calendar + pos, "%s", "March"); break;
   case 4: pos += sprintf(calendar + pos, "%s", "April"); break;
   case 5: pos += sprintf(calendar + pos, "%s", "May"); break;
   case 6: pos += sprintf(calendar + pos, "%s", "June"); break;
   case 7: pos += sprintf(calendar + pos, "%s", "July"); break;
   case 8: pos += sprintf(calendar + pos, "%s", "August"); break;
   case 9: pos += sprintf(calendar + pos, "%s", "September"); break;
   case 10: pos += sprintf(calendar + pos, "%s", "October"); break;
   case 11: pos += sprintf(calendar + pos, "%s", "November"); break;
   case 12: pos += sprintf(calendar + pos, "%s", "December"); break;
   default: break;
}

  

   // Write the year
   pos += sprintf(calendar + pos, ", ");
   pos += sprintf(calendar + pos, "%i", year);
   pos += sprintf(calendar + pos, "\n");

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
   // for (int i = 0; i < 7; i++)
   // {
   //    pos += sprintf(calendar + pos, "  ");
   //    if (i == 0)
   //       pos += sprintf(calendar + pos, "%s", "Su");
   //    if (i == 1)
   //       pos += sprintf(calendar + pos, "%s", "Mo");
   //    if (i == 2)
   //       pos += sprintf(calendar + pos, "%s", "Tu");
   //    if (i == 3)
   //       pos += sprintf(calendar + pos, "%s", "We");
   //    if (i == 4)
   //       pos += sprintf(calendar + pos, "%s", "Th");
   //    if (i == 5)
   //       pos += sprintf(calendar + pos, "%s", "Fr");
   //    if (i == 6)
   //       pos += sprintf(calendar + pos, "%s", "Sa");
   // }
   // pos += sprintf(calendar + pos, "\n");

   // make this into one print statmeant
   pos += sprintf(calendar + pos, "%s","  Su  Mo  Tu  We  Th  Fr  Sa\n");
   
   // make this into

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

