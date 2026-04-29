# Logger

Logger is my small test project, dedicated to recalling OOP principles and playing a little with multithreading.

## Project Structure

### LogRecord
`LogRecord` is a single data unit. It stores:
- severity
- category
- message
- pair of key-value fields

This is a purely toy data structure that can be tailored to real-world needs.

---

### RingBuffer
`RingBuffer` is a class designed to store up to **1024 LogRecords**.

It supports:
- `push` — accept a new record
- `pop` — release a record

If the buffer is full, the **oldest records are replaced by the newest ones**.

`RingBuffer` is implemented as a **singleton**.

---

### Producer
`Producer` is a class with a single responsibility — generate dummy `LogRecord`s.

- Runs in its own thread
- Generates a log every **10 milliseconds**
- Pushes records into `RingBuffer`

---

### Dispatcher
`Dispatcher` is responsible for processing logs.

- Pops `LogRecord`s from `RingBuffer`
- Applies a chain of **Handlers** to each record

Runs in its own thread.

---

### Handlers
`Handlers` are small decorators that modify `LogRecord`s.

They can:
- filter logs
- enrich context
- redact messages
- format logs
- write logs to different outputs (file or console)

Handlers can be combined to build a processing pipeline.

---

## Runtime Behavior

After setting up:
- multiple **Producers**
- a single **Dispatcher**

(each running in its own thread)

The program:
- generates logs every **10 ms**
- processes them continuously
- runs for **20 seconds**
- outputs:
  - console logs
  - log files (`log0.txt`, `log1.txt`, etc.)

---
