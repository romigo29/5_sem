--task1 perm tablespace
create tablespace TS_RIV
datafile 'C:\app\Tablespaces\TS_RIV.dbf'
size 7M
autoextend on next 5M
maxsize 100M;

drop tablespace TS_RIV;

--task2 temp tablespace
create temporary tablespace TS_RIV_TEMP
tempfile 'C:\app\Tablespaces\TS_RIV_TEMP.dbf'
size 5M
autoextend on next 3M
maxsize 30M;

drop tablespace TS_RIV_TEMP;

--task3  
select TABLESPACE_NAME, STATUS, contents logging from SYS.DBA_TABLESPACES;

--task4  role
alter session set "_ORACLE_SCRIPT" = true

create ROLE RL_RIVCORE;
grant create session,
    create table,
    create view,
    create procedure,
    drop any table,
    drop any view,
    drop any procedure
to RL_RIVCORE;
     
--task5
select * from dba_roles where role like 'RL%';
select * from dba_sys_privs where grantee like 'RL%';

drop role RL_RIVCORE;
--task6  security profile
create profile PF_RIVCORE limit
password_life_time 180
sessions_per_user 3 
failed_login_attempts 7
password_lock_time 1
password_reuse_time 10
password_grace_time default
connect_time 180 
idle_time 30

drop profile PF_RIVCORE;
    
--task7
select * from DBA_PROFILES where profile like 'PF%';
select * from DBA_PROFILES where profile = 'DEFAULT';


--task8 create user
create user RIVCORE identified by 1111
default tablespace TS_RIV
temporary tablespace TS_RIV_TEMP
profile PF_RIVCORE
account unlock
password expire;
grant RL_RIVCORE to RIVCORE;

SELECT username FROM all_users WHERE username = 'RIVCORE';

drop user RIVCORE

--task 10
create table exampTable (
    id number
);

create view exampView as select * from exampTable;
drop view exampView;
drop table exampTable;


--task11
create tablespace RIV_QDATA
    datafile 'C:\app\Tablespaces\RIV_QDATA.dbf'
    size 10M
    offline;

select TABLESPACE_NAME, STATUS, contents logging from SYS.DBA_TABLESPACES;

alter tablespace RIV_QDATA online;

alter user RIVCORE quota 2M on RIV_QDATA;

drop tablespace RIV_QDATA including contents;

show user;

create table tableTask11 (
    id number,
    name varchar(10)
) tablespace RIV_QDATA;

insert into tableTask11 values (1, 'apple');
insert into tableTask11 values (2, 'apple2');
insert into tableTask11 values (3, 'apple3');

select * from tableTask11;
drop table tableTask11;



