/* Includes */
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <semaphore.h>  /* Semaphore */

#define STUDENTS 5 //default number of students

#define   BUF_SIZE   100
char   buf[BUF_SIZE];

sem_t TAReady;  //0
sem_t accessHallwayChairs;  //3
sem_t studentReady; //0
int numberOfHallwayChairs = 3; 
int endOftheDay = 0;
int numberOfStudents;

void teachingAssistant();
void student(void * ptr);

int main(int argc, char *argv[]) {   
    numberOfStudents = STUDENTS;
    if(argv[1]){
        numberOfStudents = atoi(argv[1]);
        //printf("argv1 is %d\n",numberOfStudents);
    }

    //Allocate memory for Students
	pthread_t *Students= (pthread_t*) malloc(sizeof(pthread_t)*numberOfStudents); //threads for Students
    pthread_t TA;   //thread for TA

    // Initialize the semaphores with initial values...
    sem_init(&TAReady, 0, 0);
    sem_init(&accessHallwayChairs, 0, 3);
    sem_init(&studentReady, 0, 0);

    //Simulation Stars
    printf("\n\n...Simulation is starting...\n");

    // Create the barber.
    pthread_create(&TA, NULL, teachingAssistant, NULL);

    // Create the students.
    int i;
    for (i=0; i<numberOfStudents; i++) {
        pthread_create(&Students[i], NULL, student, (void*) (long)i);
    }

    //Waiting for TA thread to finish
    pthread_join(TA, NULL);

    // Join each of the threads to wait for them to finish.
    for (i=0; i<numberOfStudents; i++) {
        pthread_join(&Students[i],NULL);
    }

    free(Students); 
    return 0;
}

void teachingAssistant(){
    while(1){
        if(endOftheDay == numberOfStudents){
            break;
        }
        printf("The TA is sleeping... -_-\n");
        sem_wait(&studentReady);
        printf("The TA is awaken *_*\n");
    
        sem_wait(&accessHallwayChairs);
        numberOfHallwayChairs +=1;
        sem_post(&accessHallwayChairs);

        // Helps Student
        printf("The TA is helping...\n");
        sleep((int) ((drand48() * 3) + 1)); 
        printf("The TA has finished helping!!!\n");
        
        sem_post(&TAReady);
    }
}

void student(void *ptr){
    while(1){
        printf("student %ld is doing programming assignment O_o\n", (long)ptr);
        sleep((int) ((drand48() * 3) + 1)); 
        printf("student %ld needs help from the TA o_o\n", (long)ptr);
        sem_wait(&accessHallwayChairs);
        if (numberOfHallwayChairs > 0){
            printf("student %ld sat on a chair waiting for the TA to finish O.O\n", (long)ptr);
            numberOfHallwayChairs -= 1;
            sem_post(&studentReady);
            sem_wait(TAReady);
            printf("student:%ld gets help :) \n",(long) ptr);
            endOftheDay++;
            break;
        } else{
            printf("student %ld will return at another time :( \n",(long) ptr);  // the part that is gonna be editted
        }
        sem_post(&accessHallwayChairs);
    }
}