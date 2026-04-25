CREATE TABLE IF NOT EXISTS  event (
    event_id INT PRIMARY KEY,
    title VARCHAR(40),
    location VARCHAR(50),
    time DATETIME,
    exp_attendance INT,
    audio_video BIT,
    budget INT
);