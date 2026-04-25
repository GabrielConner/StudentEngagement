CREATE TABLE IF NOT EXISTS  badge_student (
    badge_id INT,
    student_id INT,

    PRIMARY KEY (badge_id, student_id),
    CONSTRAINT fk_badge_student_event_id_ref_badge_badge_id FOREIGN KEY (badge_id) REFERENCES badge(badge_id),
    CONSTRAINT fk_badge_student_student_id_ref_student_student_id FOREIGN KEY (student_id) REFERENCES student(student_id)
);