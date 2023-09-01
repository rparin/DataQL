#ifndef OUTPUT_H
#define OUTPUT_H


//Some Tests found under Final Project Description
/*
//****************************************************************************

//---- employee table ----------
make table employee fields  last,               first,                  dep
insert into employee values Blow,               Joe,                    CS
insert into employee values Johnson,    "Jimmy",                Chemistry
insert into employee values Yang,               Bo,                     CS
insert into employee values "Jackson",  Billy,                  Math
insert into employee values Johnson,    Billy,                  "Phys Ed"
insert into employee values "Van Gogh", "Jim Bob",              "Phys Ed"

select * from employee
Table Name: Employee1, Records: 6

   Records                Last               First                 Dep
         1                Blow                 Joe                  Cs
         2             Johnson               Jimmy           Chemistry
         3                Yang                  Bo                  Cs
         4             Jackson               Billy                Math
         5             Johnson               Billy             Phys Ed
         6            Van Gogh             Jim Bob             Phys Ed

//----- student table ----------
make table student fields       fname,                  lname,          major,
        age
insert into student values      Flo,                    Yao,            CS,
        20
insert into student values      "Flo",                  "Jackson",      Math,
        21
insert into student values      Calvin,                 Woo,            Physics,
        22
insert into student values      "Anna Grace",   "Del Rio",      CS,
        22
select * from student
Table Name: Student1, Records: 4

   Records               Fname               Lname               Major                 Age
         1                 Flo                 Yao                  Cs                  20
         2                 Flo             Jackson                Math                  21
         3              Calvin                 Woo             Physics                  22
         4          Anna Grace             Del Rio                  Cs                  22



//****************************************************************************
//              SIMPLE SELECT
//****************************************************************************

select * from student
Table Name: Student1, Records: 4

   Records               Fname               Lname               Major                 Age
         1                 Flo                 Yao                  Cs                  20
         2                 Flo             Jackson                Math                  21
         3              Calvin                 Woo             Physics                  22
         4          Anna Grace             Del Rio                  Cs                  22

//------- simple strings -------------------
select * from student where lname = Jackson
Table Name: Z_Student1, Records: 1

   Records               Fname               Lname               Major                 Age
         1                 Flo             Jackson                Math                  21

//----- quoted strings ---------------------
select * from student where lname = "Del Rio"
Table Name: Z_Student1, Records: 1

   Records               Fname               Lname               Major                 Age
         1          Anna Grace             Del Rio                  Cs                  22

//-------- non-existing string ------------------
select * from student where lname = "Does Not Exist"
Table Name: Z_Student1, Records: 0

   Records               Fname               Lname               Major                 Age

//****************************************************************************
//              RELATIONAL OPERATORS:
//****************************************************************************

//.................
//:Greater Than   :
//.................
select * from student where lname > Yang
Table Name: Z_Student1, Records: 0

   Records               Fname               Lname               Major                 Age


//. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
select * from student where age > 50
Table Name: Z_Student1, Records: 0

   Records               Fname               Lname               Major                 Age

//. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .
select * from student where age  > 53
Table Name: Z_Student1, Records: 0

   Records               Fname               Lname               Major                 Age

//. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .
select * from student where age > 54
Table Name: Z_Student1, Records: 0

   Records               Fname               Lname               Major                 Age

//.................
//:Greater Equal  :
//.................
select * from student where lname >= Yang
Table Name: Z_Student1, Records: 0

   Records               Fname               Lname               Major                 Age
//. . . . . .  (Greater Equal non-existing: ) . . . . . . . . . . .
select * from employee where last >= Jack
Table Name: Z_Employee1, Records: 4

   Records                Last               First                 Dep
         1             Johnson               Jimmy           Chemistry
         2             Johnson               Billy             Phys Ed
         3            Van Gogh             Jim Bob             Phys Ed
         4                Yang                  Bo                  Cs


//.................
//:Less Than      :
//.................


//. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
select * from student where lname < Jackson
Table Name: Z_Student1, Records: 0

   Records               Fname               Lname               Major                 Age

//. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .
select * from student where age  < 20
Table Name: Z_Student1, Records: 0

   Records               Fname               Lname               Major                 Age


//. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .

select * from student where age  < 19
Table Name: Z_Student1, Records: 0

   Records               Fname               Lname               Major                 Age


//.................
//:Less Equal     :
//.................

select * from student where lname <= Smith
Table Name: Z_Student1, Records: 3

   Records               Fname               Lname               Major                 Age
         1          Anna Grace             Del Rio                  Cs                  22
         2                 Flo             Jackson                Math                  21
         3              Calvin                 Woo             Physics                  22

//. . . . . .  (Less Equal non-existing: ) . . . . . . . . . . .
select * from employee where last <= Peach
Table Name: Z_Employee1, Records: 5

   Records                Last               First                 Dep
         1                Blow                 Joe                  Cs
         2             Jackson               Billy                Math
         3             Johnson               Jimmy           Chemistry
         4             Johnson               Billy             Phys Ed
         5            Van Gogh             Jim Bob             Phys Ed

//****************************************************************************
//              LOGICAL OPERATORS
//****************************************************************************


//.................
//:AND            :
//.................

select * from student where fname = "Flo" and lname = "Yao"
Table Name: Z_Student1, Records: 1

   Records               Fname               Lname               Major                 Age
         1                 Flo                 Yao                  Cs                  20


//.................
//:OR            :
//.................
select * from student where fname = Flo or lname = Jackson
Table Name: Z_Student1, Records: 2

   Records               Fname               Lname               Major                 Age
         1                 Flo                 Yao                  Cs                  20
         2                 Flo             Jackson                Math                  21


//.................
//:OR AND         :
//.................
select * from student where fname = Flo or major = CS and age <= 23
Table Name: Z_Student1, Records: 3

   Records               Fname               Lname               Major                 Age
         1                 Flo                 Yao                  Cs                  20
         2                 Flo             Jackson                Math                  21
         3          Anna Grace             Del Rio                  Cs                  22


//.................
//:AND OR AND     :
//.................

select * from student where age <30 and major=CS or major = Physics and lname = Jackson
Table Name: Z_Student1, Records: 2

   Records               Fname               Lname               Major                 Age
         1                 Flo                 Yao                  Cs                  20
         2          Anna Grace             Del Rio                  Cs                  22


//.................
//:OR AND OR      :
//.................

select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson
Table Name: Z_Student1, Records: 3

   Records               Fname               Lname               Major                 Age
         1                 Flo                 Yao                  Cs                  20
         2                 Flo             Jackson                Math                  21
         3          Anna Grace             Del Rio                  Cs                  22












End of Main
Press any key to continue . . .
*/

#endif // OUTPUT_H