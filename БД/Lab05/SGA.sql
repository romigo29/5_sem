--task1
select sum(value) from v$sga;

--task2
select * from v$sga;
select component, min_size, max_size, current_size from v$sga_dynamic_components;

--task3
select component, granule_size from v$sga_dynamic_components;

--task4 
select current_size from v$sga_dynamic_free_memory;

--task5
select name, value from v$parameter
where name = 'sga_max_size'
   or name = 'sga_target';
   
show parameter sga --sqlplus  

--task6
select component, min_size, max_size, current_size
from v$sga_dynamic_components
where component in ('KEEP buffer cache', 'DEFAULT buffer cache', 'RECYCLE buffer cache');

--task7
create table KEEP_TABLE(num int) storage (buffer_pool keep) tablespace users;

insert into KEEP_TABLE values(1);

select segment_name, segment_type, tablespace_name, buffer_pool
from user_segments
where segment_name = 'KEEP_TABLE';

drop table KEEP_TABLE
--task8
create table DEFAULT_TABLE(num int) cache tablespace users;

insert into DEFAULT_TABLE values(1);

select segment_name, segment_type, tablespace_name, buffer_pool
from user_segments
where segment_name = 'DEFAULT_TABLE';

drop table DEFAULT_TABLE;

--task9
show parameter log_buffer; -- sqlplus

--task10
select * from v$sgastat where pool = 'large pool';

--task11
select 
    username, 
    service_name, 
    server 
from v$session where username is not null;

--task12
select name, description
from v$bgprocess
where paddr!=hextoraw('00') order by name;

--task13
select pname, program from v$process where background is null order by pname;

--task14
select count(*) from v$bgprocess where name like 'DBW%';
select * from v$bgprocess where name like 'DBW%';

--task15
select name, network_name, pdb from v$services;

--task16
select name, network from v$dispatcher;
show parameter dispatcher; --sqlplus

--task17
--services.msc > OracleOraDB12Home1TNSListener

--task18
--C:\app\WINBLUE_VM\product\12.2.0\dbhome_1\network\admin
