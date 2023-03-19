/* 
 * ONOMATEPWNYMO 1: EVANGELOS BALAMOTIS AEM:02927
 * ONOMATEPWNYMO 2: KONSTANTINOS KONSOULAS AEM:02975
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"hw3.h"

struct course{
		int id;
		char prof_name[MAX_NAME_LEN];
		int students;
	};
	
	struct room{
		int size;
		struct course *cariculum[WORKING_HOURS][WORKING_DAYS];
		
	};
	
	
struct course *find_course (struct course courses[], int id){
	int i;
	
	for (i=0; i<MAX_COURSES; i++){
		
		if (courses[i].id == id){
			return &courses[i];
		}
		
	}
	
	for (i=0; i<MAX_COURSES; i++){
		
		if(courses[i].id == 0){
			return &courses[i];
		}
	}
	
	return (NULL);
}

int add_course (struct course courses[], int id, int students, char prof_name[]){
	struct course *res ;
	int i;
	
	res = find_course(courses, id);
	
	if(res == NULL){
		return (-1);
	}
	else if((*res).id == id){
		return (0);
	}
	else{
		for(i = 0 ;(i<MAX_COURSES) && (res != &courses[i]) ;i++);
		
		
		courses[i].id = id;
		strcpy(courses[i].prof_name ,prof_name);
		courses[i].students = students;
		
		return (1);
	}
	
}


void print_courses(struct course courses[]){
	int i;
	
	for(i=0 ;i<MAX_COURSES ;i++){
		
		if(courses[i].id == 0){
			continue;
		}
		
		printf("[%d] %s %d\n",courses[i].id ,courses[i].prof_name ,courses[i].students);
	}
}


int programm_course(struct room rooms[] ,struct course *lesson_ptr ,int duration){
	int room_i ,i ,j,free_hours ,temp_room_i;
	
	enum weekdays {Mon=0, Tue, Wed, Thu, Fri, Sat, Sun}; 
	enum weekdays d;
	
	
	for(room_i = 0 ;room_i<MAX_ROOMS ;room_i++){
		
		if( (*lesson_ptr).students > rooms[room_i].size){
			continue;
		}
		
		for(i =0 ;i<WORKING_HOURS ;i++){
			
			
			for(d=Mon ;d<WORKING_DAYS ;d++){
				
				if(rooms[room_i].cariculum[i][d] == NULL){
					
					for(j=i ,free_hours = 0 ;j<WORKING_HOURS ;j++){
						
						if(rooms[room_i].cariculum[j][d] == NULL){
							free_hours++;
						}
						else{
							break;
							
						}
						
						for(temp_room_i = 0/*room_i+1*/ ;temp_room_i < MAX_ROOMS ;temp_room_i++){
							if(rooms[temp_room_i].cariculum[j][d] != NULL){
								if( (strcmp( lesson_ptr->prof_name , rooms[temp_room_i].cariculum[j][d]->prof_name ) == 0) && (temp_room_i != room_i)){
									free_hours = 0;
									break ;
								}
							}
						}
						
						if(free_hours == 0){
							break;
						}
						
					}
					
					if(free_hours<duration){
						continue;
					}
					else{
						
						for(j=i ;j<i+duration ;j++){
							rooms[room_i].cariculum[j][d] = lesson_ptr ;
							
						}
						
						return (room_i);
					}
					
				}
				
			}
		
		}
	
	}
	
	
	return (-1);
}


void print_programm_of_room(struct room *room_ptr){
	char days[5][5] ; 
	int h,d ;
	
	strcpy(days[0]," MON");
	strcpy(days[1]," TUE");
	strcpy(days[2]," WED");
	strcpy(days[3]," THU");
	strcpy(days[4]," FRI");
	
	printf("\nCapacity: %d\n",room_ptr->size);
	
	printf("      ");
	for(d=0 ;d<WORKING_DAYS ;d++){
		printf("%s",days[d]);
	}
	putchar('\n');
	
	for(h=0 ;h<WORKING_HOURS ;h++){
		printf("\n%d:00  ",(10 +h));
		
		for(d=0 ;d<WORKING_DAYS ;d++){
			if((*room_ptr).cariculum[h][d] == NULL ){
				printf(" -  ");
			}
			else{
				printf("%-4d",room_ptr->cariculum[h][d]->id);
			}
			
		}
	}
	
	putchar('\n');
}


void print_programm(struct room rooms[]){
	int i ;
	struct room *room_ptr;
	
	for(i=0 ;i<MAX_ROOMS ;i++){
		room_ptr = &rooms[i] ;
		
		print_programm_of_room(room_ptr);
	}
	
	
}


int remove_course (struct course courses[], int id, struct room rooms[]){
	int r, h, d ,i ,n ,id_found = 0;
	
	for (r=0 ;r<MAX_ROOMS ;r++){
		
		for (h=0 ;h<WORKING_HOURS ;h++){
			
			for (d=0 ;d<WORKING_DAYS ;d++){
				if(rooms[r].cariculum[h][d] == NULL){
					
				}
				else if(rooms[r].cariculum[h][d]->id == id){
					rooms[r].cariculum[h][d] = NULL;
					id_found = 1;
				}
				
			}
					
		}
		
	}
	
	
	for (i=0 ;i<MAX_COURSES ;i++){
		
		if(courses[i].id == id){
			
			courses[i].id = 0;
			
			for(n=0 ;n<MAX_NAME_LEN ;n++){
				courses[i].prof_name[n] = '\0';
			}
			
			courses[i].students = 0;
			
			break;
		}
	}
	
	if(id_found){
		return (1);
	}
	else{
		return (0);
	}
	
}


int main (int argc, char *argv[]) {
	int i ,id ,students ,res ,lesson_duration ,lesson_id;
	char menu_option,prof_name[MAX_NAME_LEN] ;
	struct course *lesson_ptr;
	
	
	if(argc-1 > MAX_ROOMS){
		printf("Incorrect command-line arguments!\n");
		return (0);
	}
	
	
	for(i=1 ;i < argc ;i++){
		
		if(atoi(argv[i]) < 0){
			printf("Incorrect command-line arguments!\n");
			return (0);
		}
		
	}
	
	for(i=1 ;i<argc-1 ;i++){
		
		if( atoi(argv[i]) > atoi(argv[i+1]) ){
			printf("Incorrect command-line arguments!\n");
			return (0);
		}
		
	}
	
	
	struct course courses[MAX_COURSES] = {{'\0'}};
	struct room rooms[MAX_ROOMS] = {{'\0'}};
	
	
	for(i=0 ;i<argc-1 ;i++){
		
		rooms[i].size = atoi(argv[i+1]);
	}
	
	
	do{
		menu_option = 'q';
		
		print_menu(); 
		do{
			menu_option = getchar();
		}while( (menu_option ==' ') || (menu_option == '\n') );
		
		
		switch(menu_option){
			case 'a':{
				printf("professor course students: ");
				scanf(" %s",prof_name);
				scanf("%d",&id);
				scanf("%d",&students);
				
				res = add_course(courses ,id ,students ,prof_name);
				
				if(res == 0){
					printf("%d exists\n",id);
				}
				else if(res == 1){
					printf("%d added\n",id);
				}
				else{
					printf("No space\n");
				}
				
				break;
			}
			case 'c':{
				print_courses(courses);
				break;
			}
			case 's':{
				printf("course duration: ");
				scanf("%d %d",&lesson_id ,&lesson_duration);
				
				
				if(lesson_duration < 1){
					printf("Invalid duration\n");
				}
				
				lesson_ptr = find_course(courses ,lesson_id);
				
				
				if((*lesson_ptr).id == 0){
					printf("%d not found\n",lesson_id);
				}
				else{
					res = programm_course(rooms ,lesson_ptr ,lesson_duration);
					
					if(res == -1){
						printf("%d not scheduled\n",lesson_id);
					}
					else{
						printf("%d scheduled in %d\n",lesson_id ,res);
					}
				}
				
				
				break;
			}
			case 'p':{
				print_programm(rooms);
				break;
				
			}
			case 'r':{
				printf("course: ");
				scanf("%d", &id);
				
				res = remove_course(courses, id, rooms);
				
				if (res==0){
					printf("%d not deleted\n", id);
				}
				else{
					printf("%d deleted\n", id);
				}
				break;
			}
			case 'q':{
				return (0);
				break;
			}
			
			
			
		}
		
		
	}while(1);
	
	
	return (0);
}

