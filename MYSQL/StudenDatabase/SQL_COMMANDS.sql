/*create database*/
create database StudentManagement;

create table student(
  studentId int primary key not null auto_increment,
  lastName varchar(20),
  firstName varchar(20),
  address varchar(255),
  phone varchar(10),
  email varchar(30));
create table instructor(
  instructorId int primary key not null auto_increment,
  firstName varchar(20),
  lastName varchar(20));

create table course(
  courseId int primary key not null auto_increment,
  title varchar(50),
  instructorId int not null);
create table student_course(
  studentId int not null,
  courseId int not null,
  grade int);
/*add constraints*/
alter table student_course add constraint st_cr_primary_key primary key(studentId,courseId);
alter table student auto_increment=1000;
alter table instructor auto_increment=100;
alter table course auto_increment=300;
/*Anyone can take th code and insert sample records*/


