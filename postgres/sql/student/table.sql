create table student (
  id serial not null primary key,
  name text not null,
  age int not null,
  updated_at timestamptz default now(),
  created_at timestamptz default now()
);

SELECT
  CASE
    WHEN (
      SELECT
        age
      FROM
        student
      WHERE
        id = 1
    ) = 10 THEN true
    ELSE false
  END;
