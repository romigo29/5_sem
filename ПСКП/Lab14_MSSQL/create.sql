CREATE DATABASE RIV;
GO

USE RIV;
ALTER ROLE db_datareader ADD MEMBER student;
ALTER ROLE db_datawriter ADD MEMBER student;


GRANT EXECUTE TO student;

SELECT local_net_address
FROM sys.dm_exec_connections
WHERE session_id = @@SPID;
