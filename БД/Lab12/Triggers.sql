--1.	Создайте таблицу, имеющую несколько атрибутов, один из которых первичный ключ.
drop table MyTable;

create table MyTable(
    a int primary key,
    b varchar(20)
);

select * from mytable;

--2.	Заполните таблицу строками (10 шт.).
begin
    for i in 1..10
    loop
        insert into MyTable values (i, 'test');
    end loop;
end;

select * from MyTable;
--3.	Создайте BEFORE – триггер уровня оператора на события INSERT, DELETE и UPDATE. 

create or replace trigger INS_TRIGGER_BEFORE_EVENT
    before insert
    on MyTable
begin
    DBMS_OUTPUT.PUT_LINE('INS_TRIGGER_BEFORE_EVENT');
end;

create or replace trigger UPD_TRIGGER_BEFORE_EVENT
    before update
    on MyTable
begin
    DBMS_OUTPUT.PUT_LINE('UPD_TRIGGER_BEFORE_EVENT');
end;

create or replace trigger DEL_TRIGGER_BEFORE_EVENT
    before delete
    on MyTable
begin
    DBMS_OUTPUT.PUT_LINE('DEL_TRIGGER_BEFORE_EVENT');
end;

--4.	Этот и все последующие триггеры должны выдавать сообщение на серверную консоль (DMS_OUTPUT) со своим собственным именем. 
select * from MyTable;
insert into MyTable values (11, 'some data');
update MyTable set b = 'updated data' where a = 11;
delete from MyTable where a = 11;


--5.	Создайте BEFORE-триггер уровня строки на события INSERT, DELETE и UPDATE.
create or replace trigger INS_TRIGGER_BEFORE_ROW
    before insert
    on MyTable
    for each row
begin
    DBMS_OUTPUT.PUT_LINE('INS_TRIGGER_BEFORE_ROW');
end;
/

create or replace trigger UPD_TRIGGER_BEFORE_ROW
    before update
    on MyTable
    for each row
begin
    DBMS_OUTPUT.PUT_LINE('UPD_TRIGGER_BEFORE_ROW');
end;
/

create or replace trigger DEL_TRIGGER_BEFORE_ROW
    before delete
    on MyTable
    for each row
begin
    DBMS_OUTPUT.PUT_LINE('DEL_TRIGGER_BEFORE_ROW');
end;
/

update MyTable set b = 'modified' where a > 5;
delete from MyTable where a > 5;
insert all
    into MyTable values(6, 'test')
    into MyTable values(7, 'test')
    into MyTable values(8, 'test')
    into MyTable values(9, 'test')
    into MyTable values(10, 'test')
select * from dual;

select * from MyTable;


--6.	Примените предикаты INSERTING, UPDATING и DELETING.
create or replace trigger TRIGGER_ALL
    before insert or update or delete
    on MyTable
begin
    if INSERTING then
        DBMS_OUTPUT.PUT_LINE('TRIGGER_ALL_BEFORE_INST');
    ELSIF UPDATING then
        DBMS_OUTPUT.PUT_LINE('TRIGGER_ALL_BEFORE_UPD');
    ELSIF DELETING then
        DBMS_OUTPUT.PUT_LINE('TRIGGER_ALL_BEFORE_DEL');
    end if;
end;

select * from MyTable;
insert into MyTable values (11, 'test');
update MyTable set b = 'updated data' where a = 11;
delete from MyTable where a = 11;


--7.	Разработайте AFTER-триггеры уровня оператора на события INSERT, DELETE и UPDATE.
create or replace trigger INS_TRIGGER_AFTER_EVENT
    after insert
    on MyTable
begin
    DBMS_OUTPUT.PUT_LINE('INS_TRIGGER_AFTER_EVENT');
end;
/

create or replace trigger UPD_TRIGGER_AFTER_EVENT
    after update
    on MyTable
begin
    DBMS_OUTPUT.PUT_LINE('UPD_TRIGGER_AFTER_EVENT');
end;
/

create or replace trigger DEL_TRIGGER_AFTER_EVENT
    after delete
    on MyTable
begin
    DBMS_OUTPUT.PUT_LINE('DEL_TRIGGER_AFTER_EVENT');
end;
/

select * from MyTable;
insert into MyTable values (11, 'some data');
update MyTable set b = 'updated data' where a = 11;
delete from MyTable where a = 11;


--8.	Разработайте AFTER-триггеры уровня строки на события INSERT, DELETE и UPDATE.
create or replace trigger INS_TRIGGER_AFTER_ROW
    after insert
    on MyTable
    for each row
begin
    DBMS_OUTPUT.PUT_LINE('INS_TRIGGER_AFTER_ROW');
end;
/
create or replace trigger UPD_TRIGGER_AFTER_ROW
    after update
    on MyTable
    for each row
begin
    DBMS_OUTPUT.PUT_LINE('UPD_TRIGGER_AFTER_ROW');
end;
/
create or replace trigger DEL_TRIGGER_AFTER_ROW
    after delete
    on MyTable
    for each row
begin
    DBMS_OUTPUT.PUT_LINE('DEL_TRIGGER_AFTER_ROW');
end;
/

update MyTable set b = 'modified' where a > 5;
delete from MyTable where a > 5;
insert all
    into MyTable values(6, 'test')
    into MyTable values(7, 'test')
    into MyTable values(8, 'test')
    into MyTable values(9, 'test')
    into MyTable values(10, 'test')
select * from dual;

select * from MyTable;

--9.	Создайте таблицу с именем AUDIT. Таблица должна содержать поля: OperationDate, 
--OperationType (операция вставки, обновления и удаления),
--TriggerName(имя триггера),
--Data (строка с значениями полей до и после операции).
create table AUDITS
(
    OperationDate date,
    OperationType varchar2(50),
    TriggerName   varchar2(50),
    Data          varchar2(40)
);

drop trigger INS_TRIGGER_BEFORE_EVENT;
/
drop trigger UPD_TRIGGER_BEFORE_EVENT;
/
drop trigger DEL_TRIGGER_BEFORE_EVENT;
/
drop trigger INS_TRIGGER_BEFORE_ROW;
/
drop trigger UPD_TRIGGER_BEFORE_ROW;
/
drop trigger DEL_TRIGGER_BEFORE_ROW;
/
drop trigger TRIGGER_ALL;
/
drop trigger INS_TRIGGER_AFTER_EVENT;
/
drop trigger UPD_TRIGGER_AFTER_EVENT;
/
drop trigger DEL_TRIGGER_AFTER_EVENT;
/
drop trigger INS_TRIGGER_AFTER_ROW;
/
drop trigger UPD_TRIGGER_AFTER_ROW;
/
drop trigger DEL_TRIGGER_AFTER_ROW;
/


select * from user_triggers;

--10.	Измените триггеры таким образом, чтобы они регистрировали все операции с исходной таблицей в таблице AUDIT.
create or replace trigger TRIGGER_BEFORE_AUDIT
    before insert or update or delete
    on MyTable
    for each row
declare
begin
    if inserting then
         DBMS_OUTPUT.PUT_LINE('TRIGGER_BEFORE_AUDIT - INSERT' );
         insert into AUDITS values (
                                    sysdate,
                                    'insert',
                                    'TRIGGER_BEFORE_AUDIT',
                                    :new.a || ' ' || :new.b
                                   );
    elsif updating then
        DBMS_OUTPUT.PUT_LINE('TRIGGER_BEFORE_AUDIT - UPDATE' );
        insert into AUDITS values (
                                    sysdate,
                                    'update',
                                    'TRIGGER_BEFORE_AUDIT',
                                     :old.a || ' ' || :old.b || ' -> ' || :new.a || ' ' || :new.b
                                   );
    elsif deleting then
         DBMS_OUTPUT.PUT_LINE('TRIGGER_BEFORE_AUDIT - DELETE' );
         insert into AUDITS values (
                                    sysdate,
                                    'delete',
                                    'TRIGGER_BEFORE_AUDIT',
                                    :old.a || ' ' || :old.b
                                   );
    end if;
    
end;


create or replace trigger TRIGGER_AFTER_AUDIT
    after insert or update or delete
    on MyTable
    for each row
begin
    if inserting then
         DBMS_OUTPUT.PUT_LINE('TRIGGER_AFTER_AUDIT - INSERT' );
         insert into AUDITS values (
                                    sysdate,
                                    'insert',
                                    'TRIGGER_AFTER_AUDIT',
                                    :new.a || ' ' || :new.b
                                   );
    elsif updating then
        DBMS_OUTPUT.PUT_LINE('TRIGGER_AFTER_AUDIT - UPDATE' );
        insert into AUDITS values (
                                    sysdate,
                                    'update',
                                    'TRIGGER_AFTER_AUDIT',
                                     :old.a || ' ' || :old.b || ' -> ' || :new.a || ' ' || :new.b
                                   );
    elsif deleting then
         DBMS_OUTPUT.PUT_LINE('TRIGGER_AFTER_AUDIT - DELETE' );
         insert into AUDITS values (
                                    sysdate,
                                    'delete',
                                    'TRIGGER_AFTER_AUDIT',
                                    :old.a || ' ' || :old.b
                                   );
    end if;
end;

select * from AUDITS;
truncate table AUDITS;

select * from MyTable;
insert into MyTable values (11, 'some data');
update MyTable set b = 'updated data' where a = 11;
delete from MyTable where a = 11;

--11.	Выполните операцию, нарушающую целостность таблицы по первичному ключу. Выясните, зарегистрировал ли триггер это событие. Объясните результат.
insert into MyTable values (1, 'some data');

--12.	Удалите (drop) исходную таблицу. Объясните результат. Добавьте триггер, запрещающий удаление исходной таблицы.
drop table MyTable;

create table MyTable(
    a int primary key,
    b varchar(20)
);

drop trigger TRIGGER_PREVENT_TABLE_DROP;

create or replace trigger TRIGGER_PREVENT_TABLE_DROP
    before drop
    on schema
begin
    if DICTIONARY_OBJ_NAME = 'MYTABLE'
    then
        RAISE_APPLICATION_ERROR(-20000, 'You can not drop table MyTable.');
    end if;
end;

--13.	Удалите (drop) таблицу AUDIT. Просмотрите состояние триггеров с помощью SQL-DEVELOPER. Объясните результат. Изм
drop table AUDITS;
flashback table AUDIT_LOG to before drop;
select * from user_triggers;

drop trigger TRIGGER_BEFORE_AUDIT;
drop trigger TRIGGER_AFTER_AUDIT;

--14.	Создайте представление над исходной таблицей. Разработайте INSTEADOF INSERT-триггер. Триггер должен добавлять строку в таблицу.
--drop view MyTable_View;
create view MyTable_View
as select * from MyTable;


--drop trigger TRIGGER_INSTEAD_OF_INSERT;
create or replace trigger TRIGGER_INSTEAD_OF_INSERT
    instead of insert
    on MyTable_View
begin
    if INSERTING then
        DBMS_OUTPUT.PUT_LINE('TRIGGER_INSTEAD_OF_INSERT');
        insert into MyTable values (52, 'instead data');
    end if;
end TRIGGER_INSTEAD_OF_INSERT;

select * from MyTable_View;
insert into MyTable_View values (25, 'some data');
select * from MyTable;
delete from MyTable where a = 52;

drop trigger TRIGGER_INSTEAD_OF_INSERT;
drop trigger TRIGGER_PREVENT_TABLE_DROP;

select * from user_triggers;

