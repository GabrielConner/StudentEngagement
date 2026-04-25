INSERT INTO user (email, password, permission) VALUES 
("user@otc.edu", "password", 0),
("pied@otc.edu", "p@ssword", 0),
("piper@otc.edu", "pa$$word", 0),
("pickle@otc.edu", "passw0rd", 0),
("admin@otc.edu", "admin1", 1),
("services@otc.edu", "services", 2);


INSERT INTO student (student_id, points, user_id, name) VALUES
(1004002, 20, 1, "Garry Snail"),
(1234567, 15, 2, "Mickey Mouse"),
(9876543, 2, 3, "Kermit Frog"),
(2415234, 9, 4, "Harry Potter");


INSERT INTO event (title, location, time, exp_attendance, audio_video, budget) VALUES
("Hackathon", "PMC232", "04-24-2026T04:30", 10, 1, 10),
("Buffet", "NKM204", "05-03-2027T02:15", 200, 0, 10000),
("Grad Ceremony", "Bank Arena", "05-13-2026T03:30", 5, 0, 0);


INSERT INTO event_student (event_id, student_id) VALUES
(1,1),
(2,1),
(1,2),
(3,2),
(1,3),
(2,3),
(3,3);