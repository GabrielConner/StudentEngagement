CREATE TABLE IF NOT EXISTS  student (
    student_id INT PRIMARY KEY,
    points INT,
    user_id INT,

    CONSTRAINT fk_student_user_id_ref_user_user_id FOREIGN KEY (user_id) REFERENCES user(user_id)
);