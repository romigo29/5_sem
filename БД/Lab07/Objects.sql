--task2
create sequence s1 start with 1000
    increment by 10
    nominvalue
    nomaxvalue
    nocycle
    nocache
    noorder
    
select s1.nextval from dual;
select s1.currval from dual;
    
drop sequence s1;
--task3
create sequence s2 start with 10
    increment by 10
    nocycle
    maxvalue 100;
    
    
select s2.nextval from dual;
select s2.currval from dual;
    
drop SEQUENCE s2
--task5
create sequence s3 start with 10
    increment by -10
    minvalue -100
    maxvalue 100
    nocycle
    order
    
select s3.nextval from dual;
select s3.currval from dual;
    
drop SEQUENCE s3

--task6
create sequence s4 start with 1
    increment by 1
    minvalue 1
    maxvalue 10
    cycle
    cache 5
    noorder
    
select s4.nextval from dual;
select s4.currval from dual;
    
drop SEQUENCE s3

--task7
select * from all_sequences where sequence_owner = 'RIVCORE';

--task8
create table T1 (
    n1 number(20),
    n2 number(20),
    n3 number(20),
    n4 number(20)
    ) cache storage (buffer_pool keep) tablespace TS_RIV;


begin
    for i in 1..7 loop
        insert into T1 values (s1.nextval, s2.nextval, s3.nextval, s4.nextval);
    end loop;
end;

select * from T1;


--task9
create cluster ABC (
    X number(10),
    V varchar2(12)
) size 200 hashkeys 200
  
drop cluster ABC;
  
--task10
create table A(
    XA number(10),
    VA varchar2(12),
    YA varchar2(12)
) CLUSTER ABC(XA, VA)


--task11
create table B(
    XB number(10),
    VB varchar2(12),
    YB varchar2(12)
) CLUSTER ABC(XB, VB)

--task12
create table C(
    XC number(10),
    VC varchar2(12),
    YC varchar2(12)
) CLUSTER ABC(XC, VC)


--task13
select table_name, cluster_name from user_tables;
select cluster_name from user_clusters;

--task14

SELECT * FROM all_tables where table_name = 'C'

create synonym C1 for RIVCORE.C;
insert into C1 values(1, 'va', 'ya');
select * from C1;

drop synonym C1;

--task15
create public synonym B1 for RIVCORE.B;
insert into B1 values(1, 'va', 'ya');
select * from B1;

alter session set container = orclpdb;

--task16
create table A16(
    XA number(10),
    VA varchar2(12),
    YA number(10),
    CONSTRAINT PK_A16 PRIMARY KEY(XA)
) 

create table B16(
    XB number(10),
    VB varchar2(12),
    YB number(10),
    CONSTRAINT FK_B16 FOREIGN KEY (YB) references A16(XA)
) 

insert into A16 values (1, 'a', 1);
insert into B16 values(1, 'b', 1);

create view V1 
as select * from A16
inner join B16 
on A16.XA = B16.YB;

select * from V1;

--task17
CREATE MATERIALIZED VIEW MV
REFRESH COMPLETE
START WITH SYSDATE
NEXT SYSDATE + 1/1440
AS
SELECT * 
FROM A16
INNER JOIN B16 
ON A16.XA = B16.YB;


select * from MV;

SELECT *
FROM USER_MVIEWS
WHERE MVIEW_NAME = 'MV';

insert into A16 values (3, 'a', 3);
insert into B16 values (3, 'b', 3);
commit;

SELECT LAST_REFRESH_DATE FROM USER_MVIEWS
EXEC DBMS_MVIEW.REFRESH('MV');

drop view V1;
drop materialized view MV;
drop table B16;
drop table A16;


    