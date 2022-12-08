create table grade (
  id int primary key,
  student_id int,
  test_name text,
  subject text,
  score int,
  updated_at timestamp with time zone,
  created_at timestamp with time zone,
  unique(student_id, test_name, subject),
  foreign key(student_id) references student(id)
);
