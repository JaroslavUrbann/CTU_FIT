prompt #---------------------#
prompt #- Pomocne procedury -#
prompt #---------------------#

create or replace procedure SMAZ_VSECHNY_TABULKY AS
-- pokud v logu bude uvedeno, ze nektery objekt nebyl zrusen, protoze na nej jiny jeste existujici objekt stavi, spust proceduru opakovane, dokud se nezrusi vse
begin
  for iRec in 
    (select distinct OBJECT_TYPE, OBJECT_NAME,
      'drop '||OBJECT_TYPE||' "'||OBJECT_NAME||'"'||
      case OBJECT_TYPE when 'TABLE' then ' cascade constraints purge' else ' ' end as PRIKAZ
    from USER_OBJECTS where OBJECT_NAME not in ('SMAZ_VSECHNY_TABULKY', 'VYPNI_CIZI_KLICE', 'ZAPNI_CIZI_KLICE', 'VYMAZ_DATA_VSECH_TABULEK')
    ) loop
        begin
          dbms_output.put_line('Prikaz: '||irec.prikaz);
        execute immediate iRec.prikaz;
        exception
          when others then dbms_output.put_line('NEPOVEDLO SE!');
        end;
      end loop;
end;
/

create or replace procedure VYPNI_CIZI_KLICE as 
begin
  for cur in (select CONSTRAINT_NAME, TABLE_NAME from USER_CONSTRAINTS where CONSTRAINT_TYPE = 'R' ) 
  loop
    execute immediate 'alter table '||cur.TABLE_NAME||' modify constraint "'||cur.CONSTRAINT_NAME||'" DISABLE';
  end loop;
end VYPNI_CIZI_KLICE;
/


create or replace procedure ZAPNI_CIZI_KLICE as 
begin
  for cur in (select CONSTRAINT_NAME, TABLE_NAME from USER_CONSTRAINTS where CONSTRAINT_TYPE = 'R' ) 
  loop
    execute immediate 'alter table '||cur.TABLE_NAME||' modify constraint "'||cur.CONSTRAINT_NAME||'" enable validate';
  end loop;
end ZAPNI_CIZI_KLICE;
/

create or replace procedure VYMAZ_DATA_VSECH_TABULEK is
begin
  -- Vymazat data vsech tabulek
  VYPNI_CIZI_KLICE;
  for v_rec in (select distinct TABLE_NAME from USER_TABLES)
  loop
    execute immediate 'truncate table '||v_rec.TABLE_NAME||' drop storage';
  end loop;
  ZAPNI_CIZI_KLICE;
  
  -- Nastavit vsechny sekvence od 1
  for v_rec in (select distinct SEQUENCE_NAME  from USER_SEQUENCES)
  loop
    execute immediate 'alter sequence '||v_rec.SEQUENCE_NAME||' restart start with 1';
  end loop;
end VYMAZ_DATA_VSECH_TABULEK;
/

prompt #------------------------#
prompt #- Zrusit stare tabulky -#
prompt #------------------------#

exec SMAZ_VSECHNY_TABULKY;

prompt #-------------------------#
prompt #- Vytvorit nove tabulky -#
prompt #-------------------------#


CREATE TABLE academic_paper (
    academic_paper_id     INTEGER NOT NULL,
    academic_paper_name   CHAR(120 CHAR) NOT NULL,
    publication_date      DATE NOT NULL,
    abstract              CHAR(1000 CHAR)
);

ALTER TABLE academic_paper ADD CONSTRAINT academic_paper_pk PRIMARY KEY ( academic_paper_id );

CREATE TABLE authorship (
    researcher_id       INTEGER NOT NULL,
    academic_paper_id   INTEGER NOT NULL
);

ALTER TABLE authorship ADD CONSTRAINT authorship_pk PRIMARY KEY ( researcher_id,
                                                                  academic_paper_id );

CREATE TABLE citation (
    citing_paper_id   INTEGER NOT NULL,
    cited_paper_id    INTEGER NOT NULL
);

ALTER TABLE citation ADD CONSTRAINT citation_pk PRIMARY KEY ( cited_paper_id,
                                                              citing_paper_id );

CREATE TABLE city (
    city_id     INTEGER NOT NULL,
    city_name   CHAR(40 CHAR) NOT NULL
);

ALTER TABLE city ADD CONSTRAINT city_pk PRIMARY KEY ( city_id );

CREATE TABLE conference (
    conference_id     INTEGER NOT NULL,
    conference_name   CHAR(60 CHAR) NOT NULL
);

ALTER TABLE conference ADD CONSTRAINT conference_pk PRIMARY KEY ( conference_id );

CREATE TABLE conference_edition (
    conference_edition_id   INTEGER NOT NULL,
    start_date              DATE NOT NULL,
    end_date                DATE NOT NULL,
    city_id                 INTEGER NOT NULL,
    conference_id           INTEGER NOT NULL
);

ALTER TABLE conference_edition ADD CONSTRAINT conference_edition_pk PRIMARY KEY ( conference_edition_id );

CREATE TABLE field (
    field_id     INTEGER NOT NULL,
    field_name   CHAR(40 CHAR) NOT NULL
);

ALTER TABLE field ADD CONSTRAINT field_pk PRIMARY KEY ( field_id );

CREATE TABLE field_attribution (
    academic_paper_id   INTEGER NOT NULL,
    field_id            INTEGER NOT NULL
);

ALTER TABLE field_attribution ADD CONSTRAINT field_attribution_pk PRIMARY KEY ( academic_paper_id,
                                                                                field_id );

CREATE TABLE research_group (
    research_group_id     INTEGER NOT NULL,
    research_group_name   CHAR(60 CHAR) NOT NULL,
    school_id             INTEGER
);

ALTER TABLE research_group ADD CONSTRAINT research_group_pk PRIMARY KEY ( research_group_id );

CREATE TABLE researcher (
    researcher_id       INTEGER NOT NULL,
    first_name          CHAR(40 CHAR) NOT NULL,
    last_name           CHAR(40 CHAR) NOT NULL,
    research_group_id   INTEGER
);

ALTER TABLE researcher ADD CONSTRAINT researcher_pk PRIMARY KEY ( researcher_id );

CREATE TABLE school (
    school_id     INTEGER NOT NULL,
    school_name   CHAR(100 CHAR) NOT NULL,
    city_id       INTEGER NOT NULL
);

ALTER TABLE school ADD CONSTRAINT school_pk PRIMARY KEY ( school_id );

CREATE TABLE submission (
    submission_date         DATE NOT NULL,
    accepted                CHAR(1),
    academic_paper_id       INTEGER NOT NULL,
    conference_edition_id   INTEGER NOT NULL
);

ALTER TABLE submission ADD CONSTRAINT submission_pk PRIMARY KEY ( academic_paper_id,
                                                                  conference_edition_id );

ALTER TABLE authorship
    ADD CONSTRAINT authorship_academic_paper_fk FOREIGN KEY ( academic_paper_id )
        REFERENCES academic_paper ( academic_paper_id );

ALTER TABLE authorship
    ADD CONSTRAINT authorship_researcher_fk FOREIGN KEY ( researcher_id )
        REFERENCES researcher ( researcher_id );

ALTER TABLE citation
    ADD CONSTRAINT citation_academic_paper_fk FOREIGN KEY ( cited_paper_id )
        REFERENCES academic_paper ( academic_paper_id );

ALTER TABLE citation
    ADD CONSTRAINT citation_academic_paper_fkv1 FOREIGN KEY ( citing_paper_id )
        REFERENCES academic_paper ( academic_paper_id );

ALTER TABLE field_attribution
    ADD CONSTRAINT fa_academic_paper_fk FOREIGN KEY ( academic_paper_id )
        REFERENCES academic_paper ( academic_paper_id );

ALTER TABLE field_attribution
    ADD CONSTRAINT field_attribution_field_fk FOREIGN KEY ( field_id )
        REFERENCES field ( field_id );

ALTER TABLE researcher
    ADD CONSTRAINT relation_20 FOREIGN KEY ( research_group_id )
        REFERENCES research_group ( research_group_id );

ALTER TABLE submission
    ADD CONSTRAINT relation_21 FOREIGN KEY ( academic_paper_id )
        REFERENCES academic_paper ( academic_paper_id );

ALTER TABLE submission
    ADD CONSTRAINT relation_22 FOREIGN KEY ( conference_edition_id )
        REFERENCES conference_edition ( conference_edition_id );

ALTER TABLE research_group
    ADD CONSTRAINT relation_24 FOREIGN KEY ( school_id )
        REFERENCES school ( school_id );

ALTER TABLE school
    ADD CONSTRAINT relation_25 FOREIGN KEY ( city_id )
        REFERENCES city ( city_id );

ALTER TABLE conference_edition
    ADD CONSTRAINT relation_27 FOREIGN KEY ( city_id )
        REFERENCES city ( city_id );

ALTER TABLE conference_edition
    ADD CONSTRAINT relation_28 FOREIGN KEY ( conference_id )
        REFERENCES conference ( conference_id );
