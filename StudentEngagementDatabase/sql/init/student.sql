CREATE TABLE IF NOT EXISTS  student (
    student_id INTEGER PRIMARY KEY,
    points INTEGER,
    user_id INTEGER,
    name VARCHAR(30),

    CONSTRAINT fk_student_user_id_ref_user_user_id FOREIGN KEY (user_id) REFERENCES user(user_id)
);