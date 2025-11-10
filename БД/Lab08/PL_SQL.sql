select * from all_tables where owner = 'SYSTEM';

--task1
declare 
begin 
    null;
end;

--task2
--for sqlplus SET SERVEROUTPUT ON;
declare 
begin
    dbms_output.put_line('Hello World!');
end;
/

--task3 error division by zero 
declare
    some_num NUMBER;
begin
    some_num := 1/0;
exception 
    when others
    then dbms_output.put_line('Error message: ' || sqlerrm);
        dbms_output.put_line('Error code ' || sqlcode);
end;

--tsak4-5 nested blocks
declare
    some_num NUMBER;
begin
    declare
    begin
        some_num := 1/0;
    exception 
        when others
        then dbms_output.put_line('Error message: ' || sqlerrm);
            dbms_output.put_line('Error code ' || sqlcode);
    end;
    dbms_output.put_line('some_num=' || some_num);
end;

show parameter plsql_warnings; 


--task6 specific symbols
select keyword from V$RESERVED_WORDS
where length = 1 and keyword != 'A' order by keyword

--task7 keywords
select keyword from V$RESERVED_WORDS
where length > 1 and keyword != 'A' order by keyword

--task8
select name, value
from v$parameter where name like 'plsql%'

show parameter plsql;

----------------------------
--task9
declare 
    num1 NUMBER := 1;
    num2 NUMBER := 2;
    
    num3 NUMBER := 1.1;
    num4 NUMBER := 2.2;
    
    num5 NUMBER := 1.1;
    num6 NUMBER(6, -2) := 123.456;
    
    num7 BINARY_FLOAT := 1.1;
    
    num8 BINARY_DOUBLE := 1.1;
    
    num9 NUMBER := 1.1E1;
    num10 NUMBER := 1.1E-1;
    
    flag BOOLEAN := TRUE;
    
begin
    dbms_output.put_line('--- task10--- ');
    dbms_output.put_line(num1);
    dbms_output.put_line(num2);
    dbms_output.put_line('--- task11--- ');
    dbms_output.put_line(num1+num2);
    dbms_output.put_line(num1-num2);
    dbms_output.put_line(num1*num2);
    dbms_output.put_line(num1/num2);
    dbms_output.put_line(num1 mod num2);
    dbms_output.put_line('--- task12--- ');
    dbms_output.put_line(num3);
    dbms_output.put_line(num4);
    dbms_output.put_line('--- task13--- ');
    dbms_output.put_line(num5);
    dbms_output.put_line(num6);
    dbms_output.put_line('--- task14--- ');
    dbms_output.put_line(num7);
    dbms_output.put_line('--- task15--- ');
    dbms_output.put_line(num8);
    dbms_output.put_line('--- task16--- ');
    dbms_output.put_line(num9);
    dbms_output.put_line(num10);
    dbms_output.put_line('--- task17--- ');
    if flag then
        dbms_output.put_line('True');
    else
        dbms_output.put_line('False');
    end if;
end;

--------------------------------------
--task18
declare
  VCHAR_CONST CONSTANT VARCHAR2(20) := 'VCHAR_CONST';
  CHAR_CONST CONSTANT CHAR(20) := 'CHAR_CONST';
  NUMBER_CONST CONSTANT NUMBER := 1;
begin
  DBMS_OUTPUT.PUT_LINE(VCHAR_CONST);
  DBMS_OUTPUT.PUT_LINE(CHAR_CONST);
  DBMS_OUTPUT.PUT_LINE(NUMBER_CONST);
end;

--task19-20
declare
    subject system.subject.subject%type;
    faculty_rec system.faculty%rowtype;
begin
    subject := 'PIS';
    faculty_rec.faculty := 'IDiP';
    DBMS_OUTPUT.PUT_LINE(subject);
    DBMS_OUTPUT.PUT_LINE(rtrim(faculty_rec.faculty));
end;

--task21-22 if-else
DECLARE
  v_num NUMBER := 1;
BEGIN
  IF v_num = 1 THEN
    DBMS_OUTPUT.PUT_LINE('v_num = 1');
  ELSIF v_num = 2 THEN
    DBMS_OUTPUT.PUT_LINE('v_num = 2');
  ELSIF v_num is null THEN
    DBMS_OUTPUT.PUT_LINE('v_num is null');
  ELSE
    DBMS_OUTPUT.PUT_LINE('v_num = 3');
  END IF;
END;

-- task23 case
DECLARE
  v_num NUMBER := 1;
BEGIN
  CASE v_num
    WHEN 1 THEN
      DBMS_OUTPUT.PUT_LINE('v_num = 1');
    WHEN 2 THEN
      DBMS_OUTPUT.PUT_LINE('v_num = 2');
    WHEN 3 THEN
      DBMS_OUTPUT.PUT_LINE('v_num = 3');
    ELSE
      DBMS_OUTPUT.PUT_LINE('v_num is null');
  END CASE;
END;

-- task24 loop
DECLARE
  v_num NUMBER := 1;
BEGIN
  LOOP
    DBMS_OUTPUT.PUT_LINE(v_num);
    v_num := v_num + 1;
    EXIT WHEN v_num > 10;
  END LOOP;
END;

-- task25 while
DECLARE
  v_num NUMBER := 1;
BEGIN
  WHILE v_num <= 10 LOOP
    DBMS_OUTPUT.PUT_LINE(v_num);
    v_num := v_num + 1;
  END LOOP;
END;

-- task26 for

DECLARE
  v_num NUMBER := 1;
BEGIN
  FOR i IN 1..10 LOOP
    DBMS_OUTPUT.PUT_LINE(i);
  END LOOP;
END;





