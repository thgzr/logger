Logger is my small test project, dedicated to recalling OOP principles and playing a little with multithreading.

Here is structure of the project:
LogRecord is single data unit. It stores severity, category, message, and pair of key - values. This is single piece of purely toy data, that can be tailored to real world needs.
RingBuffer is a class, designed to store up to 1024 LogRecords. RingBuffer may push (accept new record) or pop (release record). If buffer is full, oldest records will be replaced by newest ones. RingBuffer is singleton.
Producer is a class with single responsibility - generate dummy LogRecords. 
Dispatcher is a class, that doest something to LogRecords. It pops LogRecords from RingBuffer and then uses Handlers on said record.
Handlers are small decorators, that changes LogRecords. Handlers can be used to filter LogRecords, enrich its context, redact LogRecord message, format those LogRecords, and write LogRecord to different streams (file or console).
After setting up few producers and single dispatcher (each in its own thread), we launch those. Producer threads generate dummy log each 10 milliseconds, and Dispatcher is processing those logs. Program runs for 20 seconds, generating log files (log0.txt, log1.txt etc) and producing output to console.