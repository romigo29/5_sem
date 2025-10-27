--task1 sqlnet.ora , tnsnames.ora
-- C:\app\WINBLUE_VM\product\12.2.0\dbhome_1\network\admin

--task2 sqlplus
--sqlplus system/<password> as sysdba
--SHOW PARAMETER

--task3 PDB получите список табличных пространств, файлов табличных пространств, ролей и пользователей.
ALTER SESSION SET CONTAINER = ORCLPDB;
SHOW CON_NAME;
--alter database open (if pdb is not opened)
select tablespace_name from dba_tablespaces;
select file_name from dba_data_files;
select role from dba_roles;
select username from dba_users;


--task4 Ознакомьтесь с параметрами в HKEY_LOCAL_MACHINE/SOFTWARE/ORACLE 
--WIN + R > regedit


--task5 Запустите утилиту Oracle Net Manager и подготовьте строку подключения с именем имя_вашего_пользователя_SID, где SID – идентификатор подключаемой базы данных. 
--task6 Подключитесь с помощью sqlplus под собственным пользователем и с применением подготовленной строки подключения. 
--task7 Выполните select к любой таблице, которой владеет ваш пользователь. 
select * from x;
--task8 Ознакомьтесь с командой HELP.Получите справку по команде TIMING. Подсчитайте, сколько времени длится select к любой таблице.
help timing

timing start "test"
select * from x;
timing stop "test"

--task9 Ознакомьтесь с командой DESCRIBE.Получите описание столбцов любой таблицы.
describe x;
--task10 Получите перечень всех сегментов, владельцем которых является ваш пользователь.
select * from user_segments;
--task11 Создайте представление, в котором получите количество всех сегментов, количество экстентов, блоков памяти и размер в килобайтах, которые они занимают.
create or replace view my_segments_summary as
select
    count(segment_name) as segment_count,
    sum(extents) as total_extents,
    sum(blocks) as total_blocks,
    sum(bytes)/1024 as total_size_kb
from user_segments;

select * from my_segments_summary;


