#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendar.h"
#include "event.h"

/*Sahil Patel, spatel88, 119012512*/
int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event);
int find_event(Calendar *calendar, const char *name, Event **event);
int clear_day(Calendar *calendar, int day);

int init_calendar(const char *name, int days, 
                int (*comp_func) (const void *ptr1, const void *ptr2), 
                void (*free_info_func) (void *ptr), Calendar ** calendar) {
    
    /*Checking conditions*/
    if((*calendar) && name && days >= 1) {
        *calendar = (Calendar *)malloc(sizeof(Calendar));
        if((*calendar) == NULL) {
            return FAILURE;
        } 

        (*calendar)->name = (char *)malloc(strlen(name) + 1);
        if((*calendar)->name == NULL) {
            return FAILURE;
        }
        strcpy((*calendar)->name, name);/*Copy the name*/

        (*calendar)->events = calloc(days, sizeof(Event));
        if((*calendar)->events == NULL) {
            return FAILURE;
        }
        /*Initilize all param.*/
        (*calendar)->total_events = 0;
        (*calendar)->days = days;
        (*calendar)->comp_func = comp_func;
        (*calendar)->free_info_func = free_info_func;
        return SUCCESS;
    }   
    return FAILURE; 
   
}

int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    int i = 0;
    Event *curr;
    if(calendar && output_stream) {
        /*If this info is requested, then print*/
        if(print_all) {
            fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name); 
            fprintf(output_stream, "Days: %d\n", calendar->days);
            fprintf(output_stream, "Total Events: %d\n", calendar->total_events);
            fprintf(output_stream, "\n");/*Empty line*/
        }
    
        fprintf(output_stream, "**** Events ****\n");
        if(calendar->total_events > 0) {
            for(i = 0; i < calendar->days; i++) {
                fprintf(output_stream, "Day %d\n", i + 1);
                /*Find curr event node than iterate and print*/
                curr = calendar->events[i];
                while(curr) {
                    fprintf(output_stream, "Event's Name: \"%s\", ", curr->name);
                    fprintf(output_stream, "Start_time: %d, ", curr->start_time);
                    fprintf(output_stream, "Duration: %d\n", curr->duration_minutes);
                    /*Assign to next event node*/
                    curr = curr->next; 
                }
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

int add_event(Calendar *calendar, const char *name, int start_time, 
            int duration_minutes, void *info, int day) {
    Event *curr = NULL, *prev = NULL, *new_event = NULL;

    /*Checking conditions*/
    if(calendar && name && start_time >= 0 && start_time <= 2400 
    && duration_minutes > 0 && day > 0 && day <= calendar->days) {
        /*Makes sure event does not alrdy exist*/
        if(find_event(calendar, name, NULL) == FAILURE) {
            /*Verify all memory alloc. were successful*/
            new_event = (Event *)malloc(sizeof(Event));
            if(new_event == NULL) {
                return FAILURE;
            }
            new_event->name = (char *)malloc(strlen(name) + 1);
            if(new_event->name == NULL) {
                return FAILURE;
            }
            /*Initialize params.*/
            strcpy(new_event->name, name);
            new_event->info = info;
            new_event->duration_minutes = duration_minutes;
            new_event->start_time = start_time;

            /*Find first node and iterate*/
            curr = calendar->events[day - 1];
            if(!curr) {
                /*If list is empty than make the new event the first one*/
                calendar->events[day - 1] = new_event;
                new_event->next = NULL;
            } else {
                /*Finds the compared result*/
                int result = calendar->comp_func(curr, new_event);
                if(result > 0) { /*If it is the largest node*/
                    calendar->events[day - 1] = new_event;
                    new_event->next = curr;
                } else {
                    prev = curr;
                    curr = curr->next;
                    /*Iterate till you find the correct pos.*/
                    while(curr && calendar->comp_func(curr, new_event) <= 0) {
                        prev = curr;
                        curr = curr->next;
                    }
                    /*Change references to maintain continuity of linked list*/
                    new_event->next = curr;
                    prev->next = new_event;
                }
            }
            /*Increment total events*/
            calendar->total_events++;
            return SUCCESS; 
        }   
    }
    return FAILURE;
}

int find_event(Calendar *calendar, const char *name, Event **event) {
    int i = 0;
    if(calendar && name) {
        for(i = 0; i < calendar->days; i++) {
            /*Uses find event in day function*/
            if(find_event_in_day(calendar, name, i + 1, event) == SUCCESS) {
                return SUCCESS;
            }
        }
    }
    return FAILURE;
    
}

int find_event_in_day(Calendar *calendar, const char *name, int day, 
                    Event **event) {
    Event *curr;
    /*Preconditions*/
    if(!calendar || !name || day < 1 || day > calendar->days) {
        return FAILURE;
    }
    /*Find first element*/
    curr = calendar->events[day - 1];
    while(curr) {
        /*If the curr_event name is equal to the param. name
        and if the event param is not null than set the address*/
        if(!strcmp(curr->name, name)) {
            if(event) {
                *event = curr;
            }
            return SUCCESS;
        }
        curr = curr->next;
    }
    return FAILURE;
}
int remove_event(Calendar *calendar, const char *name) {
    Event *event, *prev;
    int i = 0;
    /*Checking parameters*/
    if(calendar && name) {
        /*Checking if the event exists*/
        if(find_event(calendar, name, &event) != FAILURE) {
            for(i = 0; i < calendar->days; i++) {
                event = calendar->events[i];
                if(event) {
                    /*Checking if the event is at the front*/
                    if(!strcmp(event->name, name)) {
                        calendar->events[i] = event->next;
                        event->next = NULL;
                    } else {
                        /*Iterate till we refind event 
                        and also have prev point*/
                        while(event && strcmp(event->name, name)) {
                            prev = event;
                            event = event->next;
                        }
                        /*Changing references to 
                        maintain linked list continunity*/
                        prev->next = event->next;
                        event->next = NULL;

                    }
                    /*Freeing info if it exists + free_info exists*/
                    if(event->info && calendar->free_info_func) {
                        calendar->free_info_func(event->info);
                    }
                    free(event->name);
                    free(event);
                    calendar->total_events--; /*Decrement #*/
                    return SUCCESS;
                }
            }
        }
    }
    return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name) {
    Event *event;
    if(find_event(calendar, name, &event) == FAILURE) {
        return NULL;
    }/*^^^If event does not exist than just return null*/
    return event->info;
}

int clear_calendar(Calendar *calendar) {
    int i = 0;
    if(calendar) { /*Uses clear_day function*/
        for(i = 0; i < calendar->days; i++) {
            clear_day(calendar, i+1);
        }
        return SUCCESS;
    }
    return FAILURE;
}

int clear_day(Calendar *calendar, int day) {
    Event *curr_event, *placeholder; 
    /*Checks preconditions*/
    if(calendar && day > 0 && day <= calendar->days) {
        if(calendar->total_events > 0) {
            /*Finds first element*/
            curr_event = calendar->events[day - 1];
            while(curr_event != NULL)  {
                /*Gets reference curr_event 
                so we can keep the next event reference*/
                placeholder = curr_event;
                curr_event = curr_event->next;
                if(calendar->free_info_func && placeholder->info) {
                    calendar->free_info_func(placeholder->info);
                }
                /*Frees info in necessary than name, than the entire event struct*/
                free(placeholder->name);
                free(placeholder);
                calendar->total_events--;
            }
            calendar->events[day - 1] = NULL; /*Setting to empty list*/
            return SUCCESS;
        }
    }
    return FAILURE;
}

int destroy_calendar(Calendar *calendar) {
    if(calendar) {
        /*Deallocates entire calendar using clear_calendar func*/
        clear_calendar(calendar);
        /*Free event array*/
        free(calendar->events);
        /*Frees the name*/
        free(calendar->name);
        /*Finally frees the entire structure*/
        free(calendar);
        return SUCCESS;
    }
    return FAILURE;
}
