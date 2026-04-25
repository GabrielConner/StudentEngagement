CREATE TABLE IF NOT EXISTS user (
    user_id INTEGER PRIMARY KEY,
    email VARCHAR(255),
    password VARCHAR(32),
    permission INTEGER
);