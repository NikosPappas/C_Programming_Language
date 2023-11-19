#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct student{
    int studentId;
    char lastName[255];
    char firstName[255];
    char address[255];
    char phone[10];
    char email[30];
}Student;
typedef struct instructor{
    int instructorId;
    char firstName[20];
    char lastName[20];
}Instructor;

typedef struct course{
    int courseId;
    char title[50];
    int instructorId;
}course;

typedef struct studentCourse{
    int studentId;
    int courseId;
    unsigned int grade;
}StudentCourse;
void show_stmt_error(MYSQL_STMT *);//done

/*Course interface*/
void listAllCourses(MYSQL *);//done
void updateCourse(MYSQL *,course *);
void deleteAllCourses(MYSQL *);//done
void insertCourse(MYSQL *,course *);//done

/*Instructor  interface*/
void listAllInstructors(MYSQL *);//done
void insertInstructor(MYSQL *,Instructor *);//done
void deleteAllInstructors(MYSQL *);//done
void deleteInstructor(MYSQL *,Instructor *);//done


/*Student interface*/
void insertSudentInf(MYSQL *,Student *);//done
void deleteStudent(MYSQL *,Student *);//done
void deleteAllStudent(MYSQL *);//done
void listAllStudents(MYSQL *);//done

void showStudentWithGrades(MYSQL *);//done
int main(void){
      const char *user="root";
      const char *password="n1k0l2s!@";
      const char *db="StudentManagement";
      const char *host="localhost";
      MYSQL *conn=NULL;
      MYSQL_RES *result=NULL;
      MYSQL_STMT *stmt=NULL;
      MYSQL_BIND bind[5];
      int num_fields;
      Student temp;
      MYSQL_ROW row;
      if((conn=mysql_init(0))==NULL){
              fprintf(stderr,"Unable to initializa connection struct.\n");
              exit(1);
      }
      else{
          printf("Con pointer ok.\n");
      }
     if(!(mysql_real_connect(conn,host,user,password,db,0,NULL,0))){
         fprintf(stderr,"Failed to connect Mariadb Server %s.Error %s\n",host,mysql_error(conn));
         mysql_close(conn);
         exit(1);
     }
     else{
         printf("Real connection ok.\n");
     }

     listAllStudents(conn);
     printf("===========\n");
     listAllInstructors(conn);
     printf("===================\n");
     listAllCourses(conn);
     temp.studentId=314;
     deleteStudent(conn,&temp);
     listAllStudents(conn);
     showStudentWithGrades(conn);
     mysql_close(conn);
     return 0;
}
void showStudentWithGrades(MYSQL *conn){
    char *query="SELECT firstName,lastName,title,grade from student,course,student_course where student.studentId=student_course.studentId and course.courseId=student_course.courseId;";
    MYSQL_RES *result;
    MYSQL_ROW row;
    int num_fields=0;
    if(mysql_query(conn,query)){
            printf("Error: %s\n",mysql_error(conn));
    }
    result=mysql_store_result(conn);
    if(result==NULL){
      printf("Error %s.\n",mysql_error(conn));
    }
    num_fields=mysql_num_fields(result);
    while((row=mysql_fetch_row(result))){
       for(int i=0;i<num_fields;i++){
         printf("%s ",row[i] ? row[i]:"NULL");
         }
       printf("\n");
    }
    mysql_free_result(result);
}
void deleteInstructor(MYSQL *conn,Instructor *inst){
    MYSQL_BIND bind[6];
    MYSQL_STMT *stmt;
    
    stmt=mysql_stmt_init(conn);
    if(stmt==NULL){
        show_stmt_error(stmt);
    }
    if(mysql_stmt_prepare(stmt,"DELETE FROM instructor WHERE instructorId=?",-1)){
        show_stmt_error(stmt);
    }
    memset(bind,0,sizeof(MYSQL_BIND)*6);
    bind[0].buffer=&inst->instructorId;
    bind[0].buffer_type=MYSQL_TYPE_LONG;
    bind[0].buffer_length=sizeof(inst->instructorId);

    mysql_stmt_bind_param(stmt,bind);
    if(mysql_stmt_execute(stmt)){
        show_stmt_error(stmt);
    }
    mysql_stmt_close(stmt);

}
void show_stmt_error(MYSQL_STMT *stmt){
    printf("Error (%d) [%s] %s.\n",mysql_stmt_errno(stmt),mysql_stmt_sqlstate(stmt),mysql_stmt_error(stmt));
}
void deleteStudent(MYSQL *conn,Student *s){
    MYSQL_BIND bind[6];
    MYSQL_STMT *stmt;

    stmt=mysql_stmt_init(conn);
    if(stmt==NULL){
        show_stmt_error(stmt);
    }
    if(mysql_stmt_prepare(stmt,"DELETE FROM student WHERE studentId=?",-1)){
        show_stmt_error(stmt);
    }
    memset(bind,0,sizeof(MYSQL_BIND)*6);
    bind[0].buffer=&s->studentId;
    bind[0].buffer_type=MYSQL_TYPE_LONG;
    bind[0].buffer_length=sizeof(s->studentId);
    mysql_stmt_bind_param(stmt,bind);
    if(mysql_stmt_execute(stmt)){
        show_stmt_error(stmt);
    }
    mysql_stmt_close(stmt);

}
void insertCourse(MYSQL *conn,course *c){
    MYSQL_BIND bind[2];
    MYSQL_STMT *stmt;
    char *query="INSERT INTO course(title,instructorId) VALUES(?,?)";
    unsigned int array_size=3;
    unsigned row=sizeof(course);

    memset(bind,0,sizeof(MYSQL_BIND)*2);

    bind[0].buffer=&c->title;
    bind[0].buffer_type=MYSQL_TYPE_STRING;    
    bind[0].buffer_length=sizeof(c->title);

    bind[1].buffer=&c->instructorId;
    bind[1].buffer_type=MYSQL_TYPE_LONG;
    bind[1].buffer_length=sizeof(c->instructorId);

    stmt=mysql_stmt_init(conn);
    if(mysql_stmt_prepare(stmt,query,strlen(query)-1)){
        printf("Insert course,error creating stmt.\n");
    }
    mysql_stmt_attr_set(stmt,STMT_ATTR_ARRAY_SIZE,&array_size);
    mysql_stmt_attr_set(stmt,STMT_ATTR_ROW_SIZE,&row);

    mysql_stmt_bind_param(stmt,bind);
    if(mysql_stmt_execute(stmt)){
        printf("Error executing isnert statement in course.\n");
    }
    else{
        printf("Rows affected %ld\n",(long)mysql_affected_rows(conn));
    }
    mysql_stmt_close(stmt);
}
void deleteAllCourses(MYSQL *conn){
  char *query="DELETE * from course;";
  if(mysql_query(conn,query)){
      fprintf(stderr,"Delete all courses error: %s\n",mysql_error(conn));
  }
  else{
      printf("Total records deleted : %ld\n",(long)mysql_affected_rows(conn));
  }
}
void listAllCourses(MYSQL *conn){
    char *query="SELECT * FROM course;";
    MYSQL_ROW row;
    MYSQL_RES *result;
    int num_fields;
    if(mysql_query(conn,query)){
        printf("Course.Select error\n");
    }
    result=mysql_store_result(conn);
    if(result==NULL){
        printf("Courses.Error NULL SET.\n");
    }
    num_fields=mysql_num_fields(result);
    while((row=mysql_fetch_row(result))){
            for(int i=0;i<num_fields;i++){
              printf("%s ",row[i]?row[i]:"NULL");
            }
            printf("\n");
    }
    mysql_free_result(result);

}
void listAllInstructors(MYSQL *conn){
    char *query="SELECT * FROM instructor;";
    MYSQL_RES *result;
    MYSQL_ROW row;
    int num_fields;
    if(mysql_query(conn,query)){
        printf("Error creating query.%s",mysql_error(conn));
    }
    result=mysql_store_result(conn);
    if(result==NULL){
        printf("Instructor result error.\n");
    }
    num_fields=mysql_num_fields(result);
    while((row=mysql_fetch_row(result))){
            for(int i=0;i<num_fields;i++){
            printf("%s ",row[i] ? row[i]:"NULL");
          }
          printf("\n");
    }
    mysql_free_result(result);
}
void insertInstructor(MYSQL *conn,Instructor *inst){
    MYSQL_BIND bind[2];
    MYSQL_STMT *stmt;
    size_t array_size=sizeof(bind);
    unsigned row=sizeof(Instructor);
    char *query="INSERT INTO instructor(firstName,lastName) VALUES(?,?)";
    stmt=mysql_stmt_init(conn);
    memset(bind,0,sizeof(MYSQL_BIND));
    bind[0].buffer=&inst->firstName;
    bind[0].buffer_type=MYSQL_TYPE_STRING;
    bind[0].buffer_length=sizeof(inst->firstName);

    bind[1].buffer=&inst->lastName;
    bind[1].buffer_type=MYSQL_TYPE_STRING;
    bind[1].buffer_length=sizeof(inst->lastName);
    
    mysql_stmt_attr_set(stmt,STMT_ATTR_ARRAY_SIZE,&array_size);
    mysql_stmt_attr_set(stmt,STMT_ATTR_ROW_SIZE,&row);

    mysql_stmt_bind_param(stmt,bind);
   
    if(mysql_stmt_execute(stmt)){
        show_stmt_error(stmt);
    }
    else{
        printf("%d records inserted.\n",mysql_affected_rows(conn));

    }
    mysql_stmt_close(stmt);
}
void deleteAllInstructors(MYSQL *conn){
    char *query="DELETE * FROM instructor;";
    if(mysql_query(conn,query)){
    fprintf(stderr,"Eror deleting instructor records.\n");
    }
    else{
        printf("Total records deleted: %ld.\n",mysql_affected_rows(conn));
    }

}
void listAllStudents(MYSQL *conn){
    MYSQL_ROW row;
    MYSQL_RES *result=NULL;
    int num_fields;
    if(mysql_query(conn,"SELECT * FROM student;")){
            fprintf(stderr,"Error listing all student records: %s",mysql_error(conn));
      }
    else{
        result=mysql_store_result(conn);
        if(result==NULL){
            fprintf(stderr,"Error %s\n",mysql_error(conn));
        }
        num_fields=mysql_num_fields(result);
        while((row=mysql_fetch_row(result))){
            for(int i=0;i<num_fields;i++){
                printf("%s\t\t\t",row[i]?row[i]:NULL);
            }
            printf("\n");
        }
    }
    mysql_free_result(result);

}
void deleteAllStudent(MYSQL *conn){
    if(mysql_query(conn,"DELETE * FROM student;")){
        fprintf(stderr,"Error deleting records: %s",mysql_error(conn));
    }
    else{
        printf("Data deleted succesfully.\n");
    }
}
void insertSudentInf(MYSQL *conn,Student *s){
    MYSQL_BIND bind[5];
    MYSQL_STMT *stmt=NULL;
    unsigned int array_size=5;
    size_t row_size=sizeof(Student);
    memset(bind,0,sizeof(MYSQL_BIND)*5);
    bind[0].buffer=&s->lastName;
    bind[0].buffer_type=MYSQL_TYPE_STRING;  
    bind[0].buffer_length=sizeof(s->lastName);
   
    bind[1].buffer=&s->firstName;
    bind[1].buffer_type=MYSQL_TYPE_STRING;
    bind[1].buffer_length=sizeof(s->firstName);
   
    bind[2].buffer=&s->address;
    bind[2].buffer_type=MYSQL_TYPE_STRING;  
    bind[2].buffer_length=sizeof(s->address);

    bind[3].buffer=&s->phone;
    bind[3].buffer_type=MYSQL_TYPE_STRING;
    bind[3].buffer_length=sizeof(s->phone);

    bind[4].buffer=&s->email;
    bind[4].buffer_type=MYSQL_TYPE_STRING;
    bind[4].buffer_length=sizeof(s->email);
    
    mysql_stmt_attr_set(stmt,STMT_ATTR_ARRAY_SIZE,&array_size);
    mysql_stmt_attr_set(stmt,STMT_ATTR_ROW_SIZE,&row_size);

    if(mysql_stmt_bind_param(stmt,bind)==false){
       fprintf(stderr,"Error[%s]  (%s)  %s \n",mysql_stmt_errno(stmt),mysql_stmt_sqlstate(stmt),mysql_stmt_error(stmt)); 
    }
    if(mysql_stmt_execute(stmt)){
        fprintf(stderr,"Error[%s} (%s) %s\n",mysql_stmt_errno(stmt),mysql_stmt_sqlstate(stmt),mysql_stmt_error(stmt));
    }
    printf("Student inserted successfully.\n");
    mysql_stmt_close(stmt);
}

