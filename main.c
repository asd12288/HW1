#include <stdio.h>




typedef struct date_t {
    int day;
    int month;
    int year;
} date;



typedef struct citizen_t {
    int id;
    char *name;
    char *last_name;
    date b_day;
    struct citizen_t *next;
} citizen;



citizen *citizen_create(int id, const char *name) {
    citizen *c = (citizen *) malloc(sizeof(citizen));
    if(c == NULL) {
               // ERROR
            return NULL;
    }
    c->id = id;
    c->name = (citize)


    
}


int main() {
    
    

    return 0;
}