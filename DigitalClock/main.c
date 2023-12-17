#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
int main(void){

    int seconds;
    int minutes;
    int hours;
    seconds=0;
    minutes=0;
    hours=0;
    while(true){
        system("clear");
        printf("%02d:%02d:%02d",hours,minutes,seconds);
        fflush(stdout);
        seconds++;
        if(seconds==60){
            seconds=0;
            minutes++;
        }
        if(minutes==60){
            minutes=0;
            hours++;
        }
        if(hours==24){
            hours=0;
            minutes=0;
            seconds=0;
        }
        sleep(1);
    }
    return 0;
}
