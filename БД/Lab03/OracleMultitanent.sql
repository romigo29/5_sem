--task1
select * from dba_pdbs;

--task2
select * from V$INSTANCE;

--task3?
select * from v$option;
select * from dba_registry;

--task4-5
select * from dba_pdbs;


--task6-----------------------------------------
select * from v$pdbs;
alter session set "_ORACLE_SCRIPT" = true;
alter session set container = RIV_PDB;
alter pluggable database RIV_PDB open;
alter session set container = CDB$ROOT;
show con_name;

--tablespaces
create tablespace TS_U1
datafile 'C:\app\Tablespaces\TS_U1.dbf'
size 7M
autoextend on next 5M
maxsize 100M;

select TABLESPACE_NAME, STATUS, contents logging from SYS.DBA_TABLESPACES;

--temp tablespace
create temporary tablespace TS_U1_TEMP
tempfile 'C:\app\Tablespaces\TS_U1_TEMP.dbf'
size 5M
autoextend on next 3M
maxsize 30M;

--role
alter session set "_ORACLE_SCRIPT" = true

create ROLE RL_U1CORE;
grant create session,
    create table,
    create view,
    create procedure,
    drop any table,
    drop any view,
    drop any procedure
to RL_U1CORE;

select * from dba_roles where role like 'RL%';
select * from dba_sys_privs where grantee like 'RL%';
     
--security profile
alter session set "_ORACLE_SCRIPT" = false;

create profile PF_U1CORE limit
password_life_time 180
sessions_per_user 3 
failed_login_attempts 7
password_lock_time 1
password_reuse_time 10
password_grace_time default
connect_time 180 
idle_time 30

select * from DBA_PROFILES where profile like 'PF_U1%';


--create user
create user U1_RIV_PDB identified by k9hTP0mF
default tablespace TS_U1
temporary tablespace TS_U1_TEMP
profile PF_U1CORE
account unlock
quota 50M ON TS_U1;
grant RL_U1CORE to U1_RIV_PDB;

SELECT username FROM all_users WHERE username = 'U1_RIV_PDB';

----------------------------------
--task7
create table RIV_table
(
    id int primary key,
    text varchar(50)
);

insert into RIV_table values (1, 'test1'); 
insert into RIV_table values (2, 'test2');
insert into RIV_table values (3, 'test3');

select * from RIV_table;

drop table RIV_table;

--task8
select * from dba_tablespaces where TABLESPACE_NAME like 'TS%';
select * from dba_temp_files;
select * from dba_roles where ROLE like 'RL%';
select * from dba_sys_privs where GRANTEE like 'RL%';
select * from dba_profiles where PROFILE like 'PF_U1%';
select * from dba_users where USERNAME like 'U1%';

--task9
alter session set container = CDB$ROOT;
alter session set container = RIV_PDB;
show con_name


alter pluggable database riv_pdb open;

grant create session to C##RIV container=all;

CREATE USER C##RIV IDENTIFIED BY 1111
CONTAINER=ALL;
grant 
    create session, 
    alter session, 
    create any table,
    drop any table
to C##RIV container = all;

select * from dba_users where username = 'C##RIV';
select username, common, con_id from cdb_users where username = 'C##RIV';
select * from dba_sys_privs where grantee = 'C##RIV';

alter session set container = RIV_PDB;



drop user C##RIV;
REVOKE CREATE SESSION FROM C##RIV CONTAINER=ALL;
REVOKE ALTER SESSION FROM C##RIV CONTAINER=ALL;
REVOKE CREATE ANY TABLE FROM C##RIV CONTAINER=ALL;
REVOKE DROP ANY TABLE FROM C##RIV CONTAINER=ALL;


create table x (id int);
drop table x;


--task11
drop pluggable database RIV_PDB including datafiles;

