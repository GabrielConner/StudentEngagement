CREATE TABLE IF NOT EXISTS  event_student (
    event_id INTEGER,
    student_id INTEGER,

    PRIMARY KEY (event_id, student_id),
    CONSTRAINT fk_event_student_event_id_ref_event_event_id FOREIGN KEY (event_id) REFERENCES event(event_id),
    CONSTRAINT fk_event_student_student_id_ref_student_student_id FOREIGN KEY (student_id) REFERENCES student(student_id)
);