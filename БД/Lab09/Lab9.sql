--1. Разработайте АБ, демонстрирующий работу оператора SELECT с точной выборкой. 
declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from faculty where faculty = 'ИЭФ';
    dbms_output.put_line(rtrim(faculty_rec.faculty) || ': ' || faculty_rec.faculty_name);
exception
    when others then
        dbms_output.put_line('error = ' || sqlerrm || sqlcode);
end;

select * from faculty;

--2. Разработайте АБ, демонстрирующий работу оператора SELECT с неточной точной выборкой. Используйте конструкцию WHEN OTHERS секции исключений и встроенную функции SQLERRM, SQLCODE для диагностирования неточной выборки. 
declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from faculty where faculty like 'И%';
    dbms_output.put_line(rtrim(faculty_rec.faculty) || ': ' || faculty_rec.faculty_name);
exception
    when others then
        dbms_output.put_line('error = ' || sqlerrm || sqlcode);
end;


--3. Разработайте АБ, демонстрирующий работу конструкции WHEN TO_MANY_ROWS секции исключений для диагностирования неточной выборки. 
declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from faculty where faculty like 'И%';
    dbms_output.put_line(faculty_rec.faculty || ': ' || faculty_rec.faculty_name);
exception
    when too_many_rows
        then dbms_output.put_line('error too_many_rows: ' || sqlerrm || sqlcode);
end;

--4. Разработайте АБ, демонстрирующий возникновение и обработку исключения NO_DATA_FOUND. Разработайте АБ, демонстрирующий применение атрибутов неявного курсора.
declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from faculty where faculty = 'XXX';
    dbms_output.put_line(rtrim(faculty_rec.faculty) || ': ' || faculty_rec.faculty_name);
exception
    when no_data_found then
        dbms_output.put_line('error no_data_found: ' || sqlerrm || '-' || sqlcode);
    when others then
        dbms_output.put_line(sqlerrm);
end;

declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from faculty where faculty = 'ИЭФ';
    dbms_output.put_line(rtrim(faculty_rec.faculty) || ': ' || faculty_rec.faculty_name);

    if sql%found then
        dbms_output.put_line('%found:     true');
    else
        dbms_output.put_line('%found:     false');
    end if;

    if sql%isopen then
        dbms_output.put_line('$isopen:    true');
    else
        dbms_output.put_line('$isopen:    false');
    end if;

    if sql%notfound then
        dbms_output.put_line('%notfound:  true');
    else
        dbms_output.put_line('%notfound:  false');
    end if;

    dbms_output.put_line('%rowcount:  ' || sql%rowcount);
end;

--5. Разработайте АБ, демонстрирующий применение оператора UPDATE совместно с операторами COMMIT/ROLLBACK. 
begin
    update auditorium
    set auditorium          = '205-1',
        auditorium_name     = '205-1',
        auditorium_capacity = 100,
        auditorium_type     = 'ЛК'
    where auditorium = '206-1';
    --commit;
    rollback;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

select * from auditorium where auditorium like '206%';


--6. Продемонстрируйте оператор UPDATE, вызывающий нарушение целостности в базе данных. Обработайте возникшее исключение.
begin
    update auditorium
    set auditorium_type = 'something'
    where auditorium = '301-1';
exception
    when others then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

--7. Разработайте АБ, демонстрирующий применение оператора INSERT совместно с операторами COMMIT/ROLLBACK.
--delete from auditorium where auditorium = '205-2';
begin
    insert into auditorium VALUES ('205-2', '205-2', 100, 'ЛК');
    commit;
    insert into auditorium VALUES ('205-3', '205-3', 100, 'ЛК');
    rollback;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

select * from auditorium where auditorium like '205%';

--8. Продемонстрируйте оператор INSERT, вызывающий нарушение целостности в базе данных. Обработайте возникшее исключение.
begin
    insert into auditorium VALUES ('205-3', '205-3', '150', 'SMTH');
    commit;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

--9. Разработайте АБ, демонстрирующий применение оператора DELETE совместно с операторами COMMIT/ROLLBACK.
begin
    delete from auditorium where auditorium = '205-2';
    commit;
    delete from auditorium where auditorium like '206%';
    rollback;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

select * from auditorium where auditorium like '205%';
select * from auditorium where auditorium like '206%';

--10. Продемонстрируйте оператор DELETE, вызывающий нарушение целостности в базе данных. Обработайте возникшее исключение.
begin
    delete from auditorium_type where auditorium_type = 'ЛК';
    rollback;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

select *
from AUDITORIUM_TYPE;

--------------------
--11. Создайте анонимный блок, распечатывающий таблицу TEACHER с применением явного курсора LOOP-цикла. Считанные данные должны быть записаны в переменные, объявленные с применением опции %TYPE.
declare
    cursor curs_teachers is select TEACHER, TEACHER_NAME, PULPIT
                            from TEACHER;
    m_teacher      TEACHER.TEACHER%type;
    m_teacher_name TEACHER.TEACHER_NAME%type;
    m_pulpit       TEACHER.PULPIT%type;
begin
    open curs_teachers;
    loop
        fetch curs_teachers into m_teacher, m_teacher_name, m_pulpit;
        exit when curs_teachers%notfound;
        dbms_output.put_line(' ' || curs_teachers%rowcount || ' '
            || m_teacher || ' '
            || m_teacher_name || ' '
            || m_pulpit);
    end loop;
    close curs_teachers;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

--12. Создайте АБ, распечатывающий таблицу SUBJECT с применением явного курсора иWHILE-цикла. Считанные данные должны быть записаны в запись (RECORD), объявленную с применением опции %ROWTYPE.
declare
    cursor curs_subject is
        select subject, subject_name, pulpit
        from subject;
    rec_subject subject%rowtype;
begin
    open curs_subject;
    fetch curs_subject into rec_subject;
    while (curs_subject%found)
        loop
            dbms_output.put_line(' ' || curs_subject%rowcount || ' '
                || rec_subject.subject || ' '
                || rec_subject.subject_name || ' '
                || rec_subject.pulpit);
            fetch curs_subject into rec_subject;
        end loop;
    close curs_subject;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

--13. Создайте АБ, распечатывающий все кафедры (таблица PULPIT) и фамилии всех преподавателей (TEACHER) использовав, соединение (JOIN) PULPIT и TEACHER и с применением явного курсора и FOR-цикла.
declare
    cursor curs_pulpit is
        select pulpit.pulpit, teacher.teacher_name
        from pulpit
                 join teacher
                      on pulpit.pulpit = teacher.pulpit;
    rec_pulpit curs_pulpit%rowtype;
begin
    for rec_pulpit in curs_pulpit
        loop
            dbms_output.put_line(' ' || curs_pulpit%rowcount || ' '
                || rec_pulpit.pulpit || ' '
                || rec_pulpit.teacher_name);
        end loop;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

--14. Создайте АБ, распечатывающий следующие списки аудиторий: все аудитории (таблица AUDITORIUM) с вместимостью меньше 20, от 21-30, от 31-60, от 61 до 80, от 81 и выше. Примените курсор с параметрами и три способа организации цикла по строкам курсора.
declare
    cursor curs (from_cap auditorium.auditorium_capacity%type, to_cap auditorium.auditorium_capacity%type)
        is select auditorium, auditorium_capacity, auditorium_type
           from auditorium
           where auditorium_capacity >= from_cap
             and auditorium_capacity <= to_cap;
    record curs%rowtype;
begin
    dbms_output.put_line('capacity < 20 :');
    for aum in curs(0, 20)
        loop
            dbms_output.put_line(aum.auditorium || ' ');
        end loop;

    dbms_output.put_line('21 < capacity < 30 :');
    open curs(21, 30);
    fetch curs into record;
    while curs%found
        loop
            dbms_output.put_line(record.auditorium || ' ');
            fetch curs into record;
        end loop;
    close curs;

    dbms_output.put_line('31 < capacity < 60 :');
    for aum in curs(31, 60)
        loop
            dbms_output.put_line(aum.auditorium || ' ');
        end loop;

    dbms_output.put_line('61 < capacity < 80 :');
    open curs(61, 80);
    fetch curs into record;
    loop
        dbms_output.put_line(record.auditorium || ' ');
        fetch curs into record;
        exit when curs%notfound;
    end loop;
    close curs;

    dbms_output.put_line('81 < capacity:');
    for aum in curs(81, 500)
        loop
            dbms_output.put_line(aum.auditorium || ' ');
        end loop;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

--15. Создайте AБ. Объявите курсорную переменную с помощью системного типа refcursor. Продемонстрируйте ее применение для курсора c параметрами. 
declare
    type auditorium_ref is ref cursor return auditorium%rowtype;
    curs     auditorium_ref;
    curs_row curs%rowtype;    
begin
    open curs for select * from auditorium;
    fetch curs into curs_row;
    loop
        exit when curs%notfound;
        dbms_output.put_line(' ' || curs_row.auditorium || ' ' || curs_row.auditorium_capacity);
        fetch curs into curs_row;
    end loop;
    close curs;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

--16. Создайте AБ. Продемонстрируйте понятие курсорный подзапрос?
declare
    cursor curs_aut
        is
        select auditorium_type,
               cursor
                   (select auditorium
                    from auditorium
                    where auditorium_type.auditorium_type = auditorium.auditorium_type)
        from auditorium_type;
    curs_aum sys_refcursor;
    aut      auditorium_type.auditorium_type%type;
    txt      varchar2(1000);
    aum      auditorium.auditorium%type;
begin
    open curs_aut;
    fetch curs_aut into aut, curs_aum;
    while(curs_aut%found)
        loop
            txt := rtrim(aut) || ': ';

            loop
                fetch curs_aum into aum;
                exit when curs_aum%notfound;
                txt := txt || rtrim(aum) || '; ';
            end loop;

            dbms_output.put_line(txt);
            fetch curs_aut into aut, curs_aum;
        end loop;

    close curs_aut;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

--17. Создайте AБ. Уменьшите вместимость всех аудиторий (таблица AUDITORIUM) вместимостью от 40 до 80 на 10%. Используйте явный курсор с параметрами, цикл FOR, конструкцию UPDATE CURRENT OF. 
select * from auditorium where auditorium_capacity between 40 and 80 order by auditorium;

declare
    cursor curs_auditorium(from_cap auditorium.auditorium%type, to_cap auditorium.auditorium%type)
        is
        select auditorium, auditorium_capacity
        from auditorium
        where auditorium_capacity >= from_cap
          and AUDITORIUM_CAPACITY <= to_cap
            for update;
    aum auditorium.auditorium%type;
    cty auditorium.auditorium_capacity%type;
begin
    open curs_auditorium(40, 80);
    fetch curs_auditorium into aum, cty;

    while(curs_auditorium%found)
        loop
            cty := cty * 0.9;
            update auditorium
            set auditorium_capacity = cty
            where current of curs_auditorium;
            dbms_output.put_line(' ' || aum || ' ' || cty);
            fetch curs_auditorium into aum, cty;
        end loop;

    close curs_auditorium;
    --commit;
    rollback;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

--18. Создайте AБ. Удалите все аудитории (таблица AUDITORIUM) вместимостью от 0 до 20. Используйте явный курсор с параметрами, цикл WHILE, конструкцию UPDATE CURRENT OF. 
------------------------------------ �
select * from auditorium where auditorium_capacity between 0 and 20 order by auditorium;

declare
    cursor curs_auditorium(from_cap auditorium.auditorium%type, to_cap auditorium.auditorium%type)
        is
        select auditorium, auditorium_capacity
        from auditorium
        where auditorium_capacity >= from_cap
          and AUDITORIUM_CAPACITY <= to_cap
            for update;
    aum auditorium.auditorium%type;
    cty auditorium.auditorium_capacity%type;
begin
    open curs_auditorium(0, 20);
    fetch curs_auditorium into aum, cty;

    while(curs_auditorium%found)
        loop
            delete auditorium
            where current of curs_auditorium;
            fetch curs_auditorium into aum, cty;
        end loop;
    close curs_auditorium;

    for pp in curs_auditorium(0, 120)
        loop
            dbms_output.put_line(' ' || pp.auditorium || ' ' || pp.auditorium_capacity);
        end loop;
    --commit;
    rollback;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

--19. Создайте AБ. Продемонстрируйте применение псевдостолбца ROWID в операторах UPDATE и DELETE. 
select * from auditorium where auditorium_capacity between 20 and 150 order by auditorium;
declare
    cursor cur(capacity auditorium.AUDITORIUM_CAPACITY%type)
        is select auditorium, auditorium_capacity, rowid
           from auditorium
           where auditorium_capacity >= capacity for update;
    aum auditorium.auditorium%type;
    cap auditorium.auditorium_capacity%type;
begin
    for rec in cur(20)
        loop
            if rec.auditorium_capacity >= 90
            then
                delete auditorium where rowid = rec.rowid and rec.auditorium_capacity >= 90;
            elsif rec.auditorium_capacity >= 20
            then
                update auditorium
                set auditorium_capacity = auditorium_capacity + 3
                where rowid = rec.rowid;
            end if;
        end loop;
    for rec in cur(20)
        loop
            dbms_output.put_line(rec.auditorium || ' ' || rec.auditorium_capacity);
        end loop;
    --commit;
    rollback;
end;


--20. Распечатайте в одном цикле всех преподавателей (TEACHER), разделив группами по три (отделите группы линией -------------). 
declare
    cursor curs_teachers is select TEACHER, TEACHER_NAME, PULPIT
                            from teacher;
    m_teacher      teacher.teacher%type;
    m_teacher_name teacher.teacher_name%type;
    m_pulpit       teacher.pulpit%type;
begin
    open curs_teachers;
    loop
        fetch curs_teachers into m_teacher, m_teacher_name, m_pulpit;
        exit when curs_teachers%notfound;
        dbms_output.put_line(' ' || curs_teachers%rowcount || ' ' || m_teacher || ' ' || m_teacher_name ||
                             ' ' || m_pulpit);
        if (mod(curs_teachers%rowcount, 3) = 0) then
            dbms_output.put_line('-----------------------');
        end if;
    end loop;
    close curs_teachers;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;











