#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* This test checks init calendar */
static int test1() {
   int days = 7;
   Calendar *calendar, *calendar2;
   printf("Test 1: Init_calendar\n");
   init_calendar("Test Calendar", days, comp_minutes, NULL, &calendar);
   print_calendar(calendar, stdout, 1);
   destroy_calendar(calendar);

   if(init_calendar("Lol", 1, comp_minutes, NULL, &calendar2) == SUCCESS) {
      return FAILURE;
   }
   destroy_calendar(calendar2);
   return SUCCESS;
}
/*Testing print_calendar*/
static int test2() {
   Calendar *calendar;
   printf("\nTest 2: Print_calendar\n");
   init_calendar("Testy", 7, comp_minutes, NULL, &calendar);
   printf("Should have nothing under\n");
   if(print_calendar(NULL, stdout, 1) == SUCCESS) {
      printf("Failed\n");
      return FAILURE;
   } 
   destroy_calendar(calendar);
   
   return SUCCESS;
}
/*Testing add_event function*/
static int test3() {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   void *info = NULL;
   printf("\nTest 3: Add_event\n");
   init_calendar("Calendar", days, comp_minutes, NULL, &calendar);

   add_event(calendar, "meep", start_time_mil, duration_minutes, 
             info, activity_day);
   add_event(calendar, "probation", 800, 69, NULL, 1); 
   add_event(calendar, "alcoholic anonym", 420, 42, NULL, 2);
   add_event(calendar, "lunch with jef", 320, 50, NULL, 3);
   if(add_event(calendar, NULL, start_time_mil, duration_minutes, NULL, 3) == SUCCESS) {
      return FAILURE;
   }
   /*Dupe event*/
   if(add_event(calendar, "lunch with jef", 420, 42, NULL, 3) == SUCCESS) {
      return FAILURE;
   }
   print_calendar(calendar, stdout, 1);
   destroy_calendar(calendar);

   return SUCCESS;
}

/*Testin find_event*/
static int test4() {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   void *info = NULL;
   Event *event;
   printf("\nTest 4: Find_Event\n");
   init_calendar("Calendar", days, comp_minutes, NULL, &calendar);

   add_event(calendar, "meep", start_time_mil, duration_minutes, 
             info, activity_day);
   add_event(calendar, "probation", 800, 69, NULL, 1); 
   add_event(calendar, "alcoholic anonym", 420, 42, NULL, 2);
   add_event(calendar, "lunch with jef", 320, 50, NULL, 3);
   if(find_event(calendar, "meep", &event) == FAILURE) {
      return FAILURE;
   } else {
      printf("Found event: %s\n", event->name);
   }
   
   destroy_calendar(calendar);
   return SUCCESS;
}
/*Testing find_event_in_day*/
static int test5() {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   void *info = NULL;
   printf("\nTest 5: Find_Event_in_day\n");
   init_calendar("Calendar", days, comp_minutes, NULL, &calendar);

   add_event(calendar, "meep", start_time_mil, duration_minutes, 
             info, activity_day);
   add_event(calendar, "probation", 800, 69, NULL, 1); 
   add_event(calendar, "alcoholic anonym", 420, 42, NULL, 2);
   add_event(calendar, "lunch with jef", 320, 50, NULL, 3);
   if(find_event_in_day(calendar, "probation", 2, NULL) == SUCCESS) {
      printf("Found something in wrong day\n");
      return FAILURE;
   } else if(find_event_in_day(calendar, "probation", 1, NULL) == SUCCESS) {
      printf("Correctly found event\n");
   } else {
      return FAILURE;
   }
   if(find_event_in_day(calendar, "too high of day", 5, NULL) == FAILURE) {
      printf("Failed because inputted day was higher than possible days\n");
   } else {
      return FAILURE;
   }

   destroy_calendar(calendar);
   return SUCCESS;
}
/*Testing remove_event*/
static int test6() {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   void *info = NULL;
   printf("\nTest 6: remove_event\n");
   init_calendar("Calendar", days, comp_minutes, NULL, &calendar);

   add_event(calendar, "meep", start_time_mil, duration_minutes, 
             info, activity_day);
   add_event(calendar, "probation", 800, 69, NULL, 1); 
   add_event(calendar, "alcoholic anonym", 420, 42, NULL, 2);
   add_event(calendar, "lunch with jef", 320, 50, NULL, 3);
   if(remove_event(calendar, "meep") == SUCCESS) {
      print_calendar(calendar, stdout, 0);
      printf("Correctly removed meep event\n");
   } else {
      return FAILURE;
   }

   if(remove_event(calendar, "fake event") == SUCCESS) {
      return FAILURE;
   }
   destroy_calendar(calendar);
   return SUCCESS;
}
/*Testing get_event_info*/
static int test7() {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   char info[4] = "lol";
   void *info_ptr = &info; 
   char *return_info;
   printf("\nTest 7: get_event_info\n");
   init_calendar("Calendar", days, comp_minutes, NULL, &calendar);

   add_event(calendar, "meep", start_time_mil, duration_minutes, 
   info_ptr, activity_day);
   add_event(calendar, "probation", 800, 69, NULL, 1); 
   add_event(calendar, "alcoholic anonym", 420, 42, NULL, 2);
   add_event(calendar, "lunch with jef", 320, 50, NULL, 3);   
   return_info = (char *)get_event_info(calendar, "meep");
   if(strcmp(return_info, "lol")) {
      printf("Didn't return correct info\n");
      return FAILURE;
   } else {
      printf("Returned correct info\n");
   }

   destroy_calendar(calendar);
   return SUCCESS;
}
/*Testing clear_calendar*/
static int test8() {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   void *info = NULL;
   printf("\nTest 8: clear_calendar\n");
   init_calendar("Calendar", days, comp_minutes, NULL, &calendar);

   add_event(calendar, "meep", start_time_mil, duration_minutes, 
             info, activity_day);
   add_event(calendar, "probation", 800, 69, NULL, 1); 
   add_event(calendar, "alcoholic anonym", 420, 42, NULL, 1);
   add_event(calendar, "lunch with jef", 320, 50, NULL, 3);
   if(clear_calendar(calendar) == SUCCESS) {
      print_calendar(calendar, stdout, 1);
   } else {
      return FAILURE;
   }

   if(clear_calendar(NULL) == SUCCESS) {
      return FAILURE;
   }
   destroy_calendar(calendar);
   return SUCCESS;
}
/*Testing clear_day*/
static int test9() {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   void *info = NULL;
   printf("\nTest 9: clear_day\n");
   init_calendar("Calendar", days, comp_minutes, NULL, &calendar);

   add_event(calendar, "meep", start_time_mil, duration_minutes, 
             info, activity_day);
   add_event(calendar, "probation", 800, 69, NULL, 1); 
   add_event(calendar, "alcoholic anonym", 420, 42, NULL, 2);
   add_event(calendar, "lunch with jef", 320, 50, NULL, 3);
   if(clear_day(calendar, 1) == SUCCESS) {
      print_calendar(calendar, stdout, 1); 
      printf("Should have no events in day 1\n");
   } else {
      return FAILURE;
   }

   destroy_calendar(calendar);
   return SUCCESS;
}
/*Testing destory_calendar*/
static int test10() {
   int days = 3, start_time_mil = 900, duration_minutes = 50;
   int activity_day = 1;
   Calendar *calendar;
   void *info = NULL;
   printf("\nTest 10: destroy_calendar\n");
   init_calendar("Calendar", days, comp_minutes, NULL, &calendar);

   add_event(calendar, "meep", start_time_mil, duration_minutes, 
             info, activity_day);
   add_event(calendar, "probation", 800, 69, NULL, 1); 
   add_event(calendar, "alcoholic anonym", 420, 42, NULL, 2);
   add_event(calendar, "lunch with jef", 320, 50, NULL, 3);
   if(destroy_calendar(NULL) == FAILURE) {
      printf("Destroy cal working correctly\n");
   }

   if(destroy_calendar(calendar) == SUCCESS) {
      printf("Calendar successfully destroyed\n");
   } else {
      return FAILURE;
   }
   
   return SUCCESS;
}
int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) result = FAILURE;
   if (test2() == FAILURE) result = FAILURE;
   if (test3() == FAILURE) result = FAILURE;
   if (test4() == FAILURE) result = FAILURE;
   if (test5() == FAILURE) result = FAILURE;
   if (test6() == FAILURE) result = FAILURE;
   if (test7() == FAILURE) result = FAILURE;
   if (test8() == FAILURE) result = FAILURE;
   if (test9() == FAILURE) result = FAILURE;
   if (test10() == FAILURE) result = FAILURE;

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
