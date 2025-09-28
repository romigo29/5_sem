create table RIV_t(x number(3) primary key, s varchar2(50));

insert into RIV_t values (1, 'Monkey');
insert into RIV_t values (2, 'Pigeon');
insert into RIV_t values (3, 'Pig');
insert into RIV_t values (4, 'Bacon');
commit;


update RIV_t set s = 'Banana' where x = 2;
update RIV_t set s = 'Milk' where x = 3;
commit;


select * from RIV_t where x > 1;
select count(*) as Amount, max(x) as Max, min(x) as Min from RIV_t;


delete from RIV_t where x = 3;
commit;



create table RIV_t1(id number(3) primary key, riv_t_id number(3), str varchar2(50),
constraint fk_riv foreign key (riv_t_id) references RIV_t(x));

insert into RIV_t1 values(1, 1, 'Something');
insert into RIV_t1 values(2, 2, 'Eats');
commit;

select t.s as RIV_t, t1.str as RIV_t1
from RIV_t t
join RIV_t1 t1 on t.x = t1.riv_t_id;

select t.s as RIV_t, t1.str as RIV_t1
from RIV_t t
left join RIV_t1 t1 on t.x = t1.riv_t_id;

select t.s as RIV_t, t1.str as RIV_t1
from RIV_t t
right join RIV_t1 t1 on t.x = t1.riv_t_id;



drop table RIV_t;
drop table RIV_t1;