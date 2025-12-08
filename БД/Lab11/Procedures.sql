set serveroutput on;
select * from TEACHER;
select * from PULPIT;
--1. Разработайте локальную процедуру 
--GET_TEACHERS (PCODE TEACHER.PULPIT%TYPE) 
--Процедура должна выводить список преподавателей из таблицы TEACHER (в стандартный серверный вывод), 
--работающих на кафедре заданной кодом в параметре. Разработайте анонимный блок и продемонстрируйте выполнение процедуры.
declare
    procedure GET_TEACHERS (PCODE TEACHER.PULPIT%TYPE) 
is
begin
    for t in (select * from TEACHER where PULPIT = PCODE)
    loop
        dbms_output.put_line(t.TEACHER_NAME);
    end loop;
end;


begin
    GET_TEACHERS ('ИСиТ');
end;

-- 2. Разработайте локальную функцию GET_NUM_TEACHERS (PCODE TEACHER.PULPIT%TYPE) RETURN NUMBER
-- Функция должна выводить количество преподавателей из таблицы TEACHER, 
-- работающих на кафедре заданной кодом в параметре. 
-- Разработайте анонимный блок и продемонстрируйте выполнение процедуры.
declare
    function GET_NUM_TEACHERS (PCODE TEACHER.PULPIT%TYPE) 
RETURN NUMBER
is
    teachers_amount number;
begin
    select count(*) into teachers_amount from TEACHER where PULPIT = PCODE;
    return teachers_amount;
end;

begin
    dbms_output.put_line(GET_NUM_TEACHERS ('ИСиТ'));
end;

-- 4. Разработайте процедуры:
-- GET_TEACHERS (FCODE FACULTY.FACULTY%TYPE)
-- Процедура должна выводить список преподавателей из таблицы TEACHER (в стандартный серверный вывод), 
-- работающих на факультете, заданным кодом в параметре.


declare
     procedure GET_TEACHERS (FCODE FACULTY.FACULTY%TYPE)
is
begin
    for t in (select * from TEACHER where PULPIT in 
            (select PULPIT from PULPIT where FACULTY = FCODE))
    loop
        dbms_output.put_line(t.TEACHER_NAME);
    end loop;
end;

begin
    GET_TEACHERS ('ИДиП');
end;

-- GET_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE)
-- Процедура должна выводить список дисциплин из таблицы SUBJECT, закрепленных за кафедрой,
-- заданной кодом кафедры в параметре. 
-- Разработайте анонимный блок и продемонстрируйте выполнение процедуры.
select * from SUBJECT;
declare
    procedure GET_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE)
is
begin
    for s in (select * from SUBJECT where PULPIT = PCODE)
    loop
        dbms_output.put_line(s.SUBJECT_NAME);
    end loop;
end;

begin
    GET_SUBJECTS ('ИСиТ');
end;

-- 5. Разработайте локальную функцию
-- GET_NUM_TEACHERS (FCODE FACULTY.FACULTY%TYPE)RETURN NUMBER
-- Функция должна выводить количество преподавателей из таблицы TEACHER, работающих
-- на факультете, заданным кодом в параметре. 
-- Разработайте анонимный блок и продемонстрируйте выполнение процедуры.

declare
    function GET_NUM_TEACHERS (FCODE FACULTY.FACULTY%TYPE)
RETURN NUMBER

is
    teachers_amount number;
begin
     select count(*) into teachers_amount from TEACHER where PULPIT in 
            (select PULPIT from PULPIT where FACULTY = FCODE);
    return teachers_amount;
end;

begin
    dbms_output.put_line(GET_NUM_TEACHERS ('ИДиП'));
end;


-- GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) RETURN NUMBER 
-- Функция должна выводить количество дисциплин из таблицы SUBJECT, закрепленных за кафедрой, 
-- заданной кодом кафедры параметре. 
-- Разработайте анонимный блок и продемонстрируйте выполнение процедуры.
select * from SUBJECT;
declare
    function GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) 
RETURN NUMBER 

is
    teachers_amount number;
begin
     select count(*) into teachers_amount from SUBJECT where PULPIT = PCODE;
    return teachers_amount;
end;

begin
    dbms_output.put_line(GET_NUM_SUBJECTS('ИСиТ'));
end;

--6. Разработайте пакет TEACHERS, содержащий процедуры и функции:
-- GET_TEACHERS (FCODE FACULTY.FACULTY%TYPE)
-- GET_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE)
-- GET_NUM_TEACHERS (FCODE FACULTY.FACULTY%TYPE) RETURN NUMBER 
-- GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) RETURN NUMBER
create or replace package TEACHERS
as
    procedure GET_TEACHERS (FCODE FACULTY.FACULTY%TYPE);
    procedure GET_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE);
    function GET_NUM_TEACHERS (FCODE FACULTY.FACULTY%TYPE) RETURN NUMBER ;
    function GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) RETURN NUMBER ;
end TEACHERS;

create or replace package body TEACHERS
is
procedure GET_TEACHERS (FCODE FACULTY.FACULTY%TYPE) is 
begin
    for t in (select * from TEACHER where PULPIT in 
            (select PULPIT from PULPIT where FACULTY = FCODE))
    loop
        dbms_output.put_line(t.TEACHER_NAME);
    end loop;
end GET_TEACHERS;

procedure GET_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE)
is
begin
    for s in (select * from SUBJECT where PULPIT = PCODE)
    loop
        dbms_output.put_line(s.SUBJECT_NAME);
    end loop;
end GET_SUBJECTS;

function GET_NUM_TEACHERS (FCODE FACULTY.FACULTY%TYPE) RETURN NUMBER is
    teachers_amount number;
begin
     select count(*) into teachers_amount from TEACHER where PULPIT in 
            (select PULPIT from PULPIT where FACULTY = FCODE);
    return teachers_amount;
end GET_NUM_TEACHERS;

function GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) RETURN NUMBER is
    teachers_amount number;
begin
     select count(*) into teachers_amount from SUBJECT where PULPIT = PCODE;
    return teachers_amount;
end GET_NUM_SUBJECTS;

end TEACHERS;

--7. Разработайте анонимный блок и продемонстрируйте выполнение процедур и функций пакета TEACHERS.
begin
dbms_output.put_line('Пакет TEACHERS');
  TEACHERS.GET_TEACHERS('ИДиП');
  dbms_output.put_line('========');
  TEACHERS.GET_SUBJECTS('ИСиТ');
  dbms_output.put_line('========');
  dbms_output.put_line(TEACHERS.GET_NUM_TEACHERS('ИДиП'));
  dbms_output.put_line('========');
  dbms_output.put_line(TEACHERS.GET_NUM_SUBJECTS('ИСиТ'));
end;
