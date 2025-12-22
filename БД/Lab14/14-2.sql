GRANT CREATE JOB TO system;
GRANT CREATE ANY JOB TO system;
GRANT SELECT ON USER_SCHEDULER_RUNNING_JOBS TO system;

CREATE OR REPLACE PACKAGE schedule_manager IS
    PROCEDURE run_job;
    PROCEDURE create_job;
    PROCEDURE remove_job;
    PROCEDURE run_now;
    FUNCTION is_running RETURN NUMBER;
END schedule_manager;
/

CREATE OR REPLACE PACKAGE BODY schedule_manager IS

    g_job_name CONSTANT VARCHAR2(30) := 'SCHED_ARCHIVE_JOB';

    PROCEDURE run_job IS
    BEGIN
        INSERT INTO ARCHIVE_TABLE (id, data_value, created_at)
        SELECT id, data_value, created_at
        FROM SOURCE_TABLE
        WHERE created_at < SYSDATE - 30;

        DELETE FROM SOURCE_TABLE
        WHERE created_at < SYSDATE - 30;

        INSERT INTO JOB_LOG(job_name, run_date, status)
        VALUES ('DBMS_SCHEDULER_COPY', SYSDATE, 'SUCCESS');

        COMMIT;
    EXCEPTION
        WHEN OTHERS THEN
            INSERT INTO JOB_LOG(job_name, run_date, status)
            VALUES ('DBMS_SCHEDULER_COPY', SYSDATE, 'ERROR');
            ROLLBACK;
            RAISE;
    END run_job;

    PROCEDURE create_job IS
    BEGIN
        DBMS_SCHEDULER.CREATE_JOB(
            job_name => g_job_name,
            job_type  => 'STORED_PROCEDURE',
            job_action => 'SCHEDULE_MANAGER.RUN_JOB',
            start_date => SYSDATE + 7,
            repeat_interval => 'FREQ=WEEKLY',
            enabled => TRUE
        );
    END create_job;

    PROCEDURE remove_job IS
    BEGIN
        DBMS_SCHEDULER.DROP_JOB(g_job_name);
    END remove_job;

    PROCEDURE run_now IS
    BEGIN
        DBMS_SCHEDULER.RUN_JOB(g_job_name);
    END run_now;

    FUNCTION is_running RETURN NUMBER IS
        v_cnt NUMBER;
    BEGIN
        SELECT COUNT(*)
        INTO v_cnt
        FROM USER_SCHEDULER_RUNNING_JOBS
        WHERE job_name = g_job_name;

        IF v_cnt > 0 THEN
            RETURN 1;
        ELSE
            RETURN 0;
        END IF;
    END is_running;

END schedule_manager;
/


BEGIN schedule_manager.create_job; END;
/

BEGIN schedule_manager.run_now; END;
/

SELECT schedule_manager.is_running FROM dual;

SELECT * FROM JOB_LOG ORDER BY run_date DESC;

BEGIN schedule_manager.remove_job; END;
/

SELECT COUNT(*) AS running_jobs
FROM USER_SCHEDULER_RUNNING_JOBS;


