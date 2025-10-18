--task1
select * from dba_tablespaces ;

--task2
create tablespace RIV_QDATA
    datafile 'C:\app\Tablespaces\RIV_QDATA.dbf'
    size 10M
    offline;

alter tablespace RIV_QDATA online;

alter user RIVCORE quota 2M on RIV_QDATA;
--r
create table RIV_T1 (
    id number primary key,
    name varchar(10)
) tablespace RIV_QDATA;

insert into RIV_T1 values (1, 'apple');
insert into RIV_T1 values (2, 'apple2');
insert into RIV_T1 values (3, 'apple3');

select * from RIV_T1;
--task3
select * from user_segments where tablespace_name = 'RIV_QDATA';

--task4
drop table RIV_T1;
select * from user_segments where tablespace_name = 'RIV_QDATA';
select * from user_recyclebin;

--task5
flashback table RIV_T1 to before drop;
select * from user_segments where tablespace_name = 'RIV_QDATA';

--task6
declare 
    i integer := 4;
begin
    for i in 4..10004 loop
        insert into RIV_T1 values (i, 'test');
    end loop;
    commit;
end;

select count(*) from RIV_T1;

--task7
select segment_name, segment_type, tablespace_name,
extents, blocks, bytes from user_segments
where tablespace_name = 'RIV_QDATA';

select * from user_extents where tablespace_name = 'RIV_QDATA';

--task8 s
drop tablespace RIV_QDATA including contents and datafiles;

--task9
select * from v$log;

--task10
select * from v$logfile;

--task11
select group#, status from v$log;
alter system switch logfile;
select TO_CHAR(SYSDATE, 'HH24:MI DD MONTH YYYY') as current_date from DUAL;

--time: 22:09 13 OCTOBER   2025

--task12
alter database add LOGFILE
group 4
'C:\APP\WINBLUE_VM\ORADATA\ORCL\REDO04.LOG'
size 50m
blocksize 512;

select * from v$log;

alter database add logfile MEMBER
'C:\APP\WINBLUE_VM\ORADATA\ORCL\REDO04_1.LOG'
to group 4

alter database add logfile MEMBER
'C:\APP\WINBLUE_VM\ORADATA\ORCL\REDO04_2.LOG'
to group 4

alter database add logfile MEMBER
'C:\APP\WINBLUE_VM\ORADATA\ORCL\REDO04_3.LOG'
to group 4

select * from v$log;
select * from v$logfile where group# = 4;

alter system switch logfile;

--task13
alter database drop logfile member 'C:\APP\WINBLUE_VM\ORADATA\ORCL\REDO04_3.LOG';
alter database drop logfile member 'C:\APP\WINBLUE_VM\ORADATA\ORCL\REDO04_2.LOG';
alter database drop logfile member 'C:\APP\WINBLUE_VM\ORADATA\ORCL\REDO04_1.LOG';
alter database drop logfile group 4; --wait for inactive

--task14
select name, log_mode from v$database;
select instance_name, archiver, active_state from v$instance;

--task15
select * from v$archived_log;

--task16 archive sqlplus
select name, log_mode from v$database;
select instance_name, archiver, active_state from v$instance;

--task17
alter system switch logfile;
select * from v$log;
select * from v$archived_log;



--task18
select name, log_mode from v$database;
select instance_name, archiver, active_state from v$instance;

--task19
select * from v$controlfile;

--task20
select type, record_size, records_total from v$controlfile_record_section;
show parameter control_files;

--task21
show parameter pfile;

--task22 connect as sysdba
--C:\app\WINBLUE_VM\product\12.2.0\dbhome_1\database
create pfile = 'RIV_PFILE.ora' from spfile;


--task23 as sys
--C:\app\WINBLUE_VM\product\12.2.0\dbhome_1\database
select * from v$pwfile_users;
show parameter remote_login_passwordfile;


--task24 as sysdba

select * from v$diag_info;

--task25
--C:\app\WINBLUE_VM\diag\rdbms\orcl\orcl\alert
--log.xml > find 'to group 4'


--task26
select SEQUENCE#, name, DELETED from V$ARCHIVED_LOG;
--C:\app\WINBLUE_VM\oradata\orcl
--C:\app\WINBLUE_VM\product\12.2.0\dbhome_1\rdbms
--C:\app\WINBLUE_VM\product\12.2.0\dbhome_1\database
--delete archive files through rman
