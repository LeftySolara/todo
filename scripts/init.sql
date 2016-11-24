--
-- File generated with SQLiteStudio v3.0.7 on Tue Oct 25 19:08:25 2016
--
-- Text encoding used: UTF-8
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Table: tasks
CREATE TABLE IF NOT EXISTS TASKS (
    id          INTEGER PRIMARY KEY ASC ON CONFLICT ROLLBACK,
    description TEXT    NOT NULL ON CONFLICT ABORT,
    due_date    DATE,
    priority    INT,
    status      BOOLEAN NOT NULL ON CONFLICT ROLLBACK
                        DEFAULT (0),
    tags        TEXT
);


COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
