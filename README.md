# Service-Queue

implementation of a service queue which has the following operations:

|operation|description|runtime|
|---|---|---|
|sq_create()|creates and initializes an empty service queue.|**_O_**(1)|
|sq_free(SQ *q)|deconstructor for the service queue.|**_O_**(1)|
|sq_display(SQ *q)|prints out queue's state to stdout.|**_O_**(N)|
|sq_length(SQ *q)|returns length of queue.|**_O_**(1)|
|sq_give_buzzer(SQ *q)|enqueues a buzzer and returns the ID of the buzzer|**_O_**(1) *on average*|
|sq_seat(SQ *q)|dequeues a buzzer from the queue, returns the ID or -1 if queue is empty|**_O_**(1)|
|sq_kick_out(SQ *q, int buzzer)|removes given buzzer from the queue if it exists in queue|**_O_**(1)|
|sq_take_bribe(SQ *q, int buzzer)|moves buzzer to front of the queue if it exists|**_O_**(1)|

### Building binaries
```
make fdriver
```
makes driver for service queue as an example usage of the service queue.
