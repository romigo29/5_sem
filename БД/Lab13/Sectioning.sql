alter session set container = orclpdb;
ALTER SESSION SET nls_date_format='dd-mm-yyyy hh24:mi:ss';

create tablespace t1
    datafile 't1_riv.dbf'
    size 7 m
    autoextend on
    maxsize unlimited
    extent management local;

create tablespace t2
    datafile 't2_riv.dbf'
    size 7 m
    autoextend on
    maxsize unlimited
    extent management local;

create tablespace t3
    datafile 't3_riv.dbf'
    size 7 m
    autoextend on
    maxsize unlimited
    extent management local;

create tablespace t4
    datafile 't4_riv.dbf'
    size 7 m
    autoextend on
    maxsize unlimited
    extent management local;

alter user rivcore quota unlimited on t1;
alter user rivcore quota unlimited on t2;
alter user rivcore quota unlimited on t3;
alter user rivcore quota unlimited on t4;

select * from user_tablespaces;

--drop tablespace t1 including contents and datafiles;
--drop tablespace t2 including contents and datafiles;
--drop tablespace t3 including contents and datafiles;
--drop tablespace t4 including contents and datafiles;


alter session set nls_date_format='dd-mm-yyyy hh24:mi:ss';
ALTER TABLESPACE TS_KVV ADD DATAFILE 'C:/LABS/LAB2/new_file.dbf' SIZE 100M;
ALTER TABLESPACE TS_KVV ADD DATAFILE 'C:/LABS/LAB2/new_file2.dbf' SIZE 100M;

-- 1. Создайте таблицу T_RANGE c диапазонным секционированием. 
-- Используйте ключ секционирования типа NUMBER.
drop table T_RANGE;
create table T_RANGE
(
    id      number,
    TIME_ID date
)
partition by range (id)
(
    partition p0 values less than (100) tablespace t1,
    partition p1 values less than (200) tablespace t2,
    partition p2 values less than (300) tablespace t3,
    partition PMAX values less than (maxvalue) tablespace t4
);

begin
    for i in 1..400
    loop
        insert into T_RANGE(id, time_id) values (i, sysdate);
    end loop;
end;

select * from T_RANGE partition(p0);
select * from T_RANGE partition(p1);
select * from T_RANGE partition(p2);
select * from T_RANGE partition(pmax);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME
from USER_TAB_PARTITIONS
where table_name = 'T_RANGE';

-- 2. Создайте таблицу T_INTERVAL c интервальным секционированием. 
-- Используйте ключ секционирования типа DATE.
drop table T_INTERVAL;
create table T_INTERVAL
(
    id      number,
    time_id date
)
    partition by range (time_id)
    interval (numtoyminterval(1, 'month'))
(
    partition p0 values less than (to_date('1-10-2025', 'dd-mm-yyyy')) tablespace t1,
    partition p1 values less than (to_date('1-11-2025', 'dd-mm-yyyy')) tablespace t2,
    partition p2 values less than (to_date('1-12-2025', 'dd-mm-yyyy')) tablespace t3
);

insert into T_INTERVAL(id, time_id) values (1, '01-09-2025');
insert into T_INTERVAL(id, time_id) values (2, '16-09-2025');
insert into T_INTERVAL(id, time_id) values (3, '02-10-2025');
insert into T_INTERVAL(id, time_id) values (4, '17-10-2025');
insert into T_INTERVAL(id, time_id) values (5, '03-11-2025');
insert into T_INTERVAL(id, time_id) values (6, '25-11-2025');
insert into T_INTERVAL(id, time_id) values (7, '29-12-2025');
insert into T_INTERVAL(id, time_id) values (8, '29-01-2026');
insert into T_INTERVAL(id, time_id) values (9, '21-03-2026');
insert into T_INTERVAL(id, time_id) values (10, '29-06-2026');
commit;


select * from T_INTERVAL partition (p0);
select * from T_INTERVAL partition (p1);
select * from T_INTERVAL partition (p2);
select * from T_INTERVAL partition (SYS_P301);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME
from USER_TAB_PARTITIONS
where table_name = 'T_INTERVAL';

-- 3. Создайте таблицу T_HASH c хэш-секционированием. 
-- Используйте ключ секционирования типа VARCHAR2.
drop table T_HASH;
create table T_HASH
(
    str varchar2(50),
    id  number
)
partition by hash (str)
(
    partition k0 tablespace t1,
    partition k1 tablespace t2,
    partition k2 tablespace t3,
    partition k3 tablespace t4
);

insert into T_HASH (str, id) values ('test', 1);
insert into T_HASH (str, id) values ('astr', 2);
insert into T_HASH (str, id) values ('commit', 3);
insert into T_HASH (str, id) values ('uuuuuuu', 4);
insert into T_HASH (str, id) values ('uuuuuuu', 5);
insert into T_HASH (str, id) values ('pigeon', 6);
insert into T_HASH (str, id) values ('apple', 7);
commit;

select * from T_HASH partition (k0);
select * from T_HASH partition (k1);
select * from T_HASH partition (k2);
select * from T_HASH partition (k3);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME
from USER_TAB_PARTITIONS
where table_name = 'T_HASH';

-- 4. Создайте таблицу T_LIST со списочным секционированием. 
-- Используйте ключ секционирования типа CHAR.
drop table T_LIST;
create table T_LIST
(
    obj char(3)
)
partition by list(obj)
(
    partition l0 values ('a') tablespace t1,
    partition l1 values ('b') tablespace t2,
    partition l2 values ('c') tablespace t3,
    partition l3 values (default) tablespace t4
);

insert into T_list(obj) values('a');
insert into T_list(obj) values('b');
insert into T_list(obj) values('c');
insert into T_list(obj) values('d');
insert into T_list(obj) values('e');
commit;

select * from T_list partition (l0);
select * from T_list partition (l1);
select * from T_list partition (l2);
select * from T_list partition (l3);

-- 6. Продемонстрируйте для всех таблиц процесс перемещения строк между секциями, 
-- при изменении (оператор UPDATE) ключа секционирования.
alter table T_RANGE enable row movement;
select * from T_RANGE partition(PMAX);
update T_RANGE set id=2 where id=300;
select * from T_RANGE partition(p0) order by id;

alter table T_INTERVAL enable row movement;
select * from T_INTERVAL partition(p0);
update T_INTERVAL set time_id=to_date('16-11-2025') where id=2;
select * from T_INTERVAL partition(p2);

alter table T_HASH enable row movement;
select * from T_HASH partition(k0);
update T_HASH set str='zxcvbnm' where id=2;
select * from T_HASH partition(k3);

alter table T_LIST enable row movement;
select * from T_LIST partition(l0);
update T_LIST set obj='c' where obj='a';
select * from T_LIST partition(l2);

-- 7. Для одной из таблиц продемонстрируйте действие оператора ALTER TABLE MERGE.
alter table T_RANGE merge partitions p1, p2 into partition p3 tablespace t4;
select * from T_RANGE partition(p3);

-- 8. Для одной из таблиц продемонстрируйте действие оператора ALTER TABLE SPLIT.
alter table T_RANGE split partition p3 at (200)
into (partition p1 tablespace t1, partition p2 tablespace t2);
select * from T_RANGE partition(p3);
select * from T_RANGE partition(p1);
select * from T_RANGE partition(p2);

-- 9. Для одной из таблиц продемонстрируйте действие оператора ALTER TABLE EXCHANGE.
drop table T_RANGE2;
create table T_RANGE2
(
    id      number,
    TIME_ID date
);
alter table T_RANGE exchange partition p0 with table T_RANGE2 without validation;
select * from T_RANGE partition (p0);
select * from T_RANGE2;



