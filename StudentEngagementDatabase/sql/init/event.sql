CREATE TABLE IF NOT EXISTS  event (
    event_id INTEGER PRIMARY KEY,
    title VARCHAR(40),
    location VARCHAR(50),
    time DATETIME,
    exp_attendance INTEGER,
    audio_video BIT,
    budget INTEGER
);