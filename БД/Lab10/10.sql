create table AUDITORIUM_TYPE (
AUDITORIUM_TYPE char(20) constraint AUDITORIUM_TYPE_PK primary key,
AUDITORIUM_TYPENAME varchar2(60) constraint AUDITORIUM_TYPENAME_NOT_NULL not null
);

create table AUDITORIUM (
AUDITORIUM char(20) primary key, -- код аудитории
AUDITORIUM_NAME varchar2(200), -- аудитория
AUDITORIUM_CAPACITY number(4), -- вместимость
AUDITORIUM_TYPE char(20) not null -- тип аудитории
references AUDITORIUM_TYPE(AUDITORIUM_TYPE)
);

CREATE TABLE FACULTY (
FACULTY CHAR(20) NOT NULL,
FACULTY_NAME VARCHAR2(200),
CONSTRAINT PK_FACULTY PRIMARY KEY(FACULTY)
);

CREATE TABLE PULPIT (
PULPIT CHAR(20) NOT NULL,
PULPIT_NAME VARCHAR2(200),
FACULTY CHAR(20) NOT NULL,
CONSTRAINT FK_PULPIT_FACULTY FOREIGN KEY(FACULTY) REFERENCES FACULTY(FACULTY),
CONSTRAINT PK_PULPIT PRIMARY KEY(PULPIT)
);

CREATE TABLE TEACHER (
TEACHER CHAR(20) NOT NULL,
TEACHER_NAME VARCHAR2(200),
PULPIT CHAR(20) NOT NULL,
CONSTRAINT PK_TEACHER PRIMARY KEY(TEACHER),
CONSTRAINT FK_TEACHER_PULPIT FOREIGN KEY(PULPIT) REFERENCES PULPIT(PULPIT)
);

CREATE TABLE SUBJECT (
SUBJECT CHAR(20) NOT NULL,
SUBJECT_NAME VARCHAR2(200) NOT NULL,
PULPIT CHAR(20) NOT NULL,
CONSTRAINT PK_SUBJECT PRIMARY KEY(SUBJECT),
CONSTRAINT FK_SUBJECT_PULPIT FOREIGN KEY(PULPIT) REFERENCES PULPIT(PULPIT)
);

-- Заполняем таблицу AUDITORIUM_TYPE
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME )
values ('ЛК', 'Лекционная');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME )
values ('ЛБ-К', 'Компьютерный класс');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME )
values ('ЛК-К', 'Лекционная с уст. компьютерами');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME )
values ('ЛБ-X', 'Химическая лаборатория');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME )
values ('ЛБ-СК', 'Спец. компьютерный класс');

-- Заполняем таблицу AUDITORIUM
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('206-1', '206-1', 'ЛБ-К', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('301-1', '301-1', 'ЛБ-К', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('236-1', '236-1', 'ЛК', 60);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('313-1', '313-1', 'ЛК', 60);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('324-1', '324-1', 'ЛК', 50);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('413-1', '413-1', 'ЛБ-К', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('423-1', '423-1', 'ЛБ-К', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('408-2', '408-2', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('103-4', '103-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('105-4', '105-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('107-4', '107-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('110-4', '110-4', 'ЛК', 30);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('111-4', '111-4', 'ЛК', 30);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('114-4', '114-4', 'ЛК-К', 90 );
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('132-4', '132-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('02Б-4', '02Б-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('229-4', '229-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('304-4', '304-4','ЛБ-К', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('314-4', '314-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('320-4', '320-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
values ('429-4', '429-4', 'ЛК', 90);

-- Заполняем таблицу FACULTY
insert into FACULTY (FACULTY, FACULTY_NAME )
values ('ИДиП', 'Издателькое дело и полиграфия');
insert into FACULTY (FACULTY, FACULTY_NAME )
values ('ХТиТ', 'Химическая технология и техника');
insert into FACULTY (FACULTY, FACULTY_NAME )
values ('ЛХФ', 'Лесохозяйственный факультет');
insert into FACULTY (FACULTY, FACULTY_NAME )
values ('ИЭФ', 'Инженерно-экономический факультет');
insert into FACULTY (FACULTY, FACULTY_NAME )
values ('ТТЛП', 'Технология и техника лесной промышленности');
insert into FACULTY (FACULTY, FACULTY_NAME )
values ('ТОВ', 'Технология органических веществ');

-- Заполняем таблицу PULPIT
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY )
values ('ИСиТ', 'Иформационный систем и технологий ', 'ИДиП' );
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY )
values ('ПОиСОИ', 'Полиграфического оборудования и систем обработки информации ', 'ИДиП' );
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛВ', 'Лесоводства', 'ЛХФ') ;
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ОВ', 'Охотоведения', 'ЛХФ') ;
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛУ', 'Лесоустройства', 'ЛХФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛЗиДВ', 'Лесозащиты и древесиноведения', 'ЛХФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛПиСПС', 'Ландшафтного проектирования и садово-паркового строительства','ЛХФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ТЛ', 'Транспорта леса', 'ТТЛП');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛМиЛЗ', 'Лесных машин и технологии лесозаготовок', 'ТТЛП');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ОХ', 'Органической химии', 'ТОВ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ТНХСиППМ','Технологии нефтехимического синтеза и переработки полимерных материалов','ТОВ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ТНВиОХТ','Технологии неорганических веществ и общей химической технологии ','ХТиТ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ХТЭПиМЭЕ','Химии, технологии электрохимических производств и материалов электронной техники', 'ХТиТ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЭТиМ', 'экономической теории и маркетинга', 'ИЭФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('МиЭП', 'Менеджмента и экономики природопользования', 'ИЭФ');

-- Заполняем таблицу TEACHER
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('СМЛВ', 'Смелов Владимир Владиславович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('АКНВЧ', 'Акунович Станислав Иванович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('КЛСНВ', 'Колесников Леонид Валерьевич', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ГРМН', 'Герман Олег Витольдович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ЛЩНК', 'Лащенко Анатолий Пвалович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('БРКВЧ', 'Бракович Андрей Игорьевич', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ДДК', 'Дедко Александр Аркадьевич', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('КБЛ', 'Кабайло Александр Серафимович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('УРБ', 'Урбанович Павел Павлович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('РМНК', 'Романенко Дмитрий Михайлович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ПСТВЛВ', 'Пустовалова Наталия Николаевна', 'ИСиТ' );
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('?', 'Неизвестный', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ГРН', 'Гурин Николай Иванович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ЖЛК', 'Жиляк Надежда Александровна', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('БРТШВЧ', 'Барташевич Святослав Александрович', 'ПОиСОИ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ЮДНКВ', 'Юденков Виктор Степанович', 'ПОиСОИ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('БРНВСК', 'Барановский Станислав Иванович', 'ЭТиМ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('НВРВ', 'Неверов Александр Васильевич', 'МиЭП');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('РВКЧ', 'Ровкач Андрей Иванович', 'ОВ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ДМДК', 'Демидко Марина Николаевна', 'ЛПиСПС');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('МШКВСК', 'Машковский Владимир Петрович', 'ЛУ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ЛБХ', 'Лабоха Константин Валентинович', 'ЛВ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ЗВГЦВ', 'Звягинцев Вячеслав Борисович', 'ЛЗиДВ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('БЗБРДВ', 'Безбородов Владимир Степанович', 'ОХ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ПРКПЧК', 'Прокопчук Николай Романович', 'ТНХСиППМ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('НСКВЦ', 'Насковец Михаил Трофимович', 'ТЛ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('МХВ', 'Мохов Сергей Петрович', 'ЛМиЛЗ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ЕЩНК', 'Ещенко Людмила Семеновна', 'ТНВиОХТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT )
values ('ЖРСК', 'Жарский Иван Михайлович', 'ХТЭПиМЭЕ');

-- Заполняем таблицу SUBJECT
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('СУБД', 'Системы управления базами данных', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('БД', 'Базы данных', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ИНФ', 'Информацтонные технологии', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ОАиП', 'Основы алгоритмизации и программирования', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ПЗ', 'Представление знаний в компьютерных системах', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ПСП', 'Пограммирование сетевых приложений', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('МСОИ', 'Моделирование систем обработки информации', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ПИС', 'Проектирование информационных систем', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('КГ', 'Компьютерная геометрия ', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ПМАПЛ', 'Полиграфические машины, автоматы и поточные линии', 'ПОиСОИ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('КМС', 'Компьютерные мультимедийные системы', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ОПП', 'Организация полиграфического производства', 'ПОиСОИ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ДМ', 'Дискретная матеатика', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('МП', 'Математисеское программирование', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ЛЭВМ', 'Логические основы ЭВМ', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ООП', 'Объектно-ориентированное программирование', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ЭП', 'Экономика природопользования', 'МиЭП');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ЭТ', 'Экономическая теория', 'ЭТиМ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('БЛЗиПсOO','Биология лесных зверей и птиц с осн. охотов.', 'ОВ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ОСПиЛПХ','Основы садовопаркового и лесопаркового хозяйства', 'ЛПиСПС');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ИГ', 'Инженерная геодезия ', 'ЛУ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ЛВ', 'Лесоводство', 'ЛЗиДВ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ОХ', 'Органическая химия', 'ОХ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ТРИ', 'Технология резиновых изделий', 'ТНХСиППМ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ВТЛ', 'Водный транспорт леса', 'ТЛ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ТиОЛ', 'Технология и оборудование лесозаготовок', 'ЛМиЛЗ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ТОПИ', 'Технология обогащения полезных ископаемых ', 'ТНВиОХТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
values ('ПЭХ', 'Прикладная электрохимия', 'ХТЭПиМЭЕ');




--1
alter table TEACHER add BIRTHDAY date;
alter table TEACHER add SALARY number;

select * from teacher;

update TEACHER set BIRTHDAY = TO_DATE('12-02-1959', 'DD-MM-YYYY') where TEACHER = 'СМЛВ';
update TEACHER set BIRTHDAY = TO_DATE('30-01-1987', 'DD-MM-YYYY') where TEACHER = 'АКНВЧ';
update TEACHER set BIRTHDAY = TO_DATE('19-04-1991', 'DD-MM-YYYY') where TEACHER = 'КЛСНВ';
update TEACHER set BIRTHDAY = TO_DATE('16-04-1964', 'DD-MM-YYYY') where TEACHER = 'ГРМН';
update TEACHER set BIRTHDAY = TO_DATE('19-11-1988', 'DD-MM-YYYY') where TEACHER = 'ЛЩНК';
update TEACHER set BIRTHDAY = TO_DATE('05-10-1966', 'DD-MM-YYYY') where TEACHER = 'БРКВЧ';
update TEACHER set BIRTHDAY = TO_DATE('10-08-1976', 'DD-MM-YYYY') where TEACHER = 'ДДК';
update TEACHER set BIRTHDAY = TO_DATE('11-09-1989', 'DD-MM-YYYY') where TEACHER = 'КБЛ';
update TEACHER set BIRTHDAY = TO_DATE('24-12-1983', 'DD-MM-YYYY') where TEACHER = 'УРБ';
update TEACHER set BIRTHDAY = TO_DATE('03-06-1990', 'DD-MM-YYYY') where TEACHER = 'РМНК';
update TEACHER set BIRTHDAY = TO_DATE('10-05-1970', 'DD-MM-YYYY') where TEACHER = 'ПСТВЛВ';
update TEACHER set BIRTHDAY = TO_DATE('26-10-1999', 'DD-MM-YYYY') where TEACHER = '?';
update TEACHER set BIRTHDAY = TO_DATE('30-07-1984', 'DD-MM-YYYY') where TEACHER = 'ГРН';
update TEACHER set BIRTHDAY = TO_DATE('11-03-1975', 'DD-MM-YYYY') where TEACHER = 'ЖЛК';
update TEACHER set BIRTHDAY = TO_DATE('12-07-1969', 'DD-MM-YYYY') where TEACHER = 'БРТШВЧ';
update TEACHER set BIRTHDAY = TO_DATE('26-02-1983', 'DD-MM-YYYY') where TEACHER = 'ЮДНКВ';
update TEACHER set BIRTHDAY = TO_DATE('13-12-1991', 'DD-MM-YYYY') where TEACHER = 'БРНВСК';
update TEACHER set BIRTHDAY = TO_DATE('20-01-1968', 'DD-MM-YYYY') where TEACHER = 'НВРВ';
update TEACHER set BIRTHDAY = TO_DATE('21-12-1969', 'DD-MM-YYYY') where TEACHER = 'РВКЧ';
update TEACHER set BIRTHDAY = TO_DATE('28-01-1975', 'DD-MM-YYYY') where TEACHER = 'ДМДК';
update TEACHER set BIRTHDAY = TO_DATE('10-07-1983', 'DD-MM-YYYY') where TEACHER = 'МШКВСК';
update TEACHER set BIRTHDAY = TO_DATE('08-10-1988', 'DD-MM-YYYY') where TEACHER = 'ЛБХ';
update TEACHER set BIRTHDAY = TO_DATE('30-07-1984', 'DD-MM-YYYY') where TEACHER = 'ЗВГЦВ';
update TEACHER set BIRTHDAY = TO_DATE('16-04-1964', 'DD-MM-YYYY') where TEACHER = 'БЗБРДВ';
update TEACHER set BIRTHDAY = TO_DATE('12-05-1985', 'DD-MM-YYYY') where TEACHER = 'ПРКПЧК';
update TEACHER set BIRTHDAY = TO_DATE('20-10-1980', 'DD-MM-YYYY') where TEACHER = 'НСКВЦ';
update TEACHER set BIRTHDAY = TO_DATE('21-08-1990', 'DD-MM-YYYY') where TEACHER = 'МХВ';
update TEACHER set BIRTHDAY = TO_DATE('13-08-1966', 'DD-MM-YYYY') where TEACHER = 'ЕЩНК';
update TEACHER set BIRTHDAY = TO_DATE('11-11-1978', 'DD-MM-YYYY') where TEACHER = 'ЖРСК';


update TEACHER set SALARY = 1030 where TEACHER = 'СМЛВ';
update TEACHER set SALARY = 1030 where TEACHER = 'АКНВЧ';
update TEACHER set SALARY = 980 where TEACHER = 'КЛСНВ';
update TEACHER set SALARY = 1050 where TEACHER = 'ГРМН';
update TEACHER set SALARY = 590 where TEACHER = 'ЛЩНК';
update TEACHER set SALARY = 870 where TEACHER = 'БРКВЧ';
update TEACHER set SALARY = 815 where TEACHER = 'ДДК';
update TEACHER set SALARY = 995 where TEACHER = 'КБЛ';
update TEACHER set SALARY = 1460 where TEACHER = 'УРБ';
update TEACHER set SALARY = 1120 where TEACHER = 'РМНК';
update TEACHER set SALARY = 1250 where TEACHER = 'ПСТВЛВ';
update TEACHER set SALARY = 333 where TEACHER = '?';
update TEACHER set SALARY = 1520 where TEACHER = 'ГРН';
update TEACHER set SALARY = 1430 where TEACHER = 'ЖЛК';
update TEACHER set SALARY = 900 where TEACHER = 'БРТШВЧ';
update TEACHER set SALARY = 875 where TEACHER = 'ЮДНКВ';
update TEACHER set SALARY = 970 where TEACHER = 'БРНВСК';
update TEACHER set SALARY = 780 where TEACHER = 'НВРВ';
update TEACHER set SALARY = 1150 where TEACHER = 'РВКЧ';
update TEACHER set SALARY = 805 where TEACHER = 'ДМДК';
update TEACHER set SALARY = 905 where TEACHER = 'МШКВСК';
update TEACHER set SALARY = 1200 where TEACHER = 'ЛБХ';
update TEACHER set SALARY = 1500 where TEACHER = 'ЗВГЦВ';
update TEACHER set SALARY = 905 where TEACHER = 'БЗБРДВ';
update TEACHER set SALARY = 715 where TEACHER = 'ПРКПЧК';
update TEACHER set SALARY = 880 where TEACHER = 'НСКВЦ';
update TEACHER set SALARY = 735 where TEACHER = 'МХВ';
update TEACHER set SALARY = 595 where TEACHER = 'ЕЩНК';
update TEACHER set SALARY = 850 where TEACHER = 'ЖРСК';



--2
GRANT CREATE SESSION TO RIVCORE;
GRANT RESTRICTED SESSION TO RIVCORE;
GRANT CREATE ANY TABLE TO RIVCORE;
GRANT CREATE ANY VIEW TO RIVCORE;
GRANT CREATE SEQUENCE TO RIVCORE;
GRANT UNLIMITED TABLESPACE TO RIVCORE;
GRANT CREATE CLUSTER TO RIVCORE;
GRANT CREATE SYNONYM TO RIVCORE;
GRANT CREATE PUBLIC SYNONYM TO RIVCORE;
GRANT CREATE MATERIALIZED VIEW TO RIVCORE;
GRANT CREATE ANY PROCEDURE TO RIVCORE;
GRANT CREATE ANY TYPE TO RIVCORE;


CREATE OR REPLACE FUNCTION GET_FIO(TEACHER_NAME VARCHAR2)
    RETURN VARCHAR2
IS
    FIO VARCHAR2(200);
BEGIN
    FIO := SUBSTR(TEACHER_NAME, 1, INSTR(TEACHER_NAME, ' ') - 1) || ' ' ||
                 SUBSTR(TEACHER_NAME, INSTR(TEACHER_NAME, ' ') + 1, 1) || '.' ||
                 SUBSTR(TEACHER_NAME, INSTR(TEACHER_NAME, ' ', 1, 2) + 1, 1) || '.';

    RETURN FIO;
END;

select GET_FIO(TEACHER_NAME), BIRTHDAY
from TEACHER;

-- 3. Получите список преподавателей, родившихся в понедельник.
SELECT TEACHER_NAME, BIRTHDAY FROM TEACHER
WHERE TO_CHAR(BIRTHDAY, 'D') = '2';

-- 4. Создайте представление, в котором поместите список преподавателей, 
-- которые родились в следующем месяце.
create or replace view TEACHERS_NEXT_MONTH as
select GET_FIO(TEACHER_NAME) as TEACHER_NAME, -- используем функции с ФИО
       to_char(BIRTHDAY, 'DD.MM.YYYY')   as BIRTHDAY
from teacher
where to_char(BIRTHDAY, 'MM') = to_char(sysdate, 'MM') + 1;

select * from TEACHERS_NEXT_MONTH;

-- 5. Создайте представление, в котором поместите количество преподавателей, 
-- которые родились в каждом месяце.
drop table MONTHS;
drop view TEACHER_COUNT_BY_MONTH;

create table MONTHS
(
  month_name   varchar(20),
  month_number varchar(2)
);


insert into MONTHS (month_name, month_number)
values ('Январь', '01');
insert into MONTHS (month_name, month_number)
values ('Февраль', '02');
insert into MONTHS (month_name, month_number)
values ('Март', '03');
insert into MONTHS (month_name, month_number)
values ('Апрель', '04');
insert into MONTHS (month_name, month_number)
values ('Май', '05');
insert into MONTHS (month_name, month_number)
values ('Июнь', '06');
insert into MONTHS (month_name, month_number)
values ('Июль', '07');
insert into MONTHS (month_name, month_number)
values ('Август', '08');
insert into MONTHS (month_name, month_number)
values ('Сентябрь', '09');
insert into MONTHS (month_name, month_number)
values ('Октябрь', '10');
insert into MONTHS (month_name, month_number)
values ('Ноябрь', '11');
insert into MONTHS (month_name, month_number)
values ('Декабрь', '12');

create view TEACHER_COUNT_BY_MONTH as
select month_name,
       (select count(*) from TEACHER where to_char(birthday, 'MM') = month_number) as amount
from MONTHS;

select * from TEACHER_COUNT_BY_MONTH;

-- 6. Создать курсор и вывести список преподавателей, у которых в следующем году юбилей.
declare
  cursor c1 is
    select GET_FIO(TEACHER_NAME) as teacher_name,
           to_char(BIRTHDAY, 'DD.MM.YYYY')   as birthday
    from TEACHER
    where MOD((to_number(to_char(sysdate, 'YYYY')) - to_number(to_char(BIRTHDAY, 'YYYY')) + 1), 5) = 0;
begin
  for i in c1
    loop
      dbms_output.put_line(i.teacher_name || ' ' || i.birthday);
    end loop;
end;

-- 7. Создать курсор и вывести среднюю заработную плату по кафедрам с округлением вниз до целых,
-- вывести средние итоговые значения для каждого факультета и для всех факультетов в целом.
select * from TEACHER;
select * from FACULTY;

DECLARE
  CURSOR c_average_salary IS
    SELECT P.FACULTY, AVG(T.SALARY) AS AVERAGE_SALARY
    FROM TEACHER T
    INNER JOIN PULPIT P ON T.PULPIT = P.PULPIT
    GROUP BY P.FACULTY;

  v_faculty CHAR(20);
  v_average_salary NUMBER; -- среднее по одному факультету
  v_count_faculty NUMBER; -- кол-во факультетов
  v_total_average_salary NUMBER := 0;
  v_average_salary_all_faculty NUMBER; -- среднее по всем факультетам
BEGIN
  OPEN c_average_salary;
  
  DBMS_OUTPUT.PUT_LINE('Average Salary by Faculty:');
  DBMS_OUTPUT.PUT_LINE('-------------------------');
  
  LOOP
    FETCH c_average_salary INTO v_faculty, v_average_salary;
    EXIT WHEN c_average_salary%NOTFOUND;
    v_total_average_salary := v_total_average_salary + v_average_salary;
    DBMS_OUTPUT.PUT_LINE('Faculty: ' || v_faculty || ', Average Salary: ' || FLOOR(v_average_salary));
  END LOOP;
  
  SELECT COUNT(*) INTO v_count_faculty FROM FACULTY;
  v_average_salary_all_faculty := v_total_average_salary / v_count_faculty;
  DBMS_OUTPUT.PUT_LINE('-------------------------');
  DBMS_OUTPUT.PUT_LINE('Total: ' || FLOOR(v_total_average_salary));
  DBMS_OUTPUT.PUT_LINE('Total Average Salary: ' || FLOOR(v_average_salary_all_faculty));
  
  CLOSE c_average_salary;
END;

-- 8. Создайте собственный тип PL/SQL-записи (record) и продемонстрируйте работу с ним.
-- Продемонстрируйте работу с вложенными записями. Продемонстрируйте и объясните операцию присвоения.
select * from TEACHER;

DECLARE
    TYPE address_record IS RECORD (
        street   VARCHAR2(100),
        city     VARCHAR2(50)
    );

    TYPE Emp_with_address_record IS RECORD (
        emp_id   NUMBER(6),
        first_name VARCHAR2(50),
        last_name  VARCHAR2(50),
        address    address_record
    );

    emp_with_address Emp_with_address_record;
    emp_address address_record;
BEGIN
    emp_with_address.emp_id := 210;
    emp_with_address.first_name := 'Igor';
    emp_with_address.last_name := 'Romanov';
    
    emp_address.street := 'Sverdlova';
    emp_address.city := 'Minsk'; 
    emp_with_address.address := emp_address;

    DBMS_OUTPUT.PUT_LINE('Employee ID: ' || emp_with_address.emp_id);
    DBMS_OUTPUT.PUT_LINE('Name: ' || emp_with_address.first_name || ' ' || emp_with_address.last_name);
    DBMS_OUTPUT.PUT_LINE('Address: ' || emp_with_address.address.street || ', ' || 
                         emp_with_address.address.city);
END;
