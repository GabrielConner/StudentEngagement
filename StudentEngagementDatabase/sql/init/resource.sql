CREATE TABLE IF NOT EXISTS  resource (
    resource_id INT PRIMARY KEY,
    event_id INT,
    name VARCHAR(30),

    CONSTRAINT fk_resource_event_id_ref_event_event_id FOREIGN KEY (event_id) REFERENCES event(event_id)
);