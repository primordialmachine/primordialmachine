# Concepts of Primordial Machine's Core Library

## By-Thread Error Variable
Functions write an error code to the by-thread error variable by a call to the function `void dx_set_error(dx_error)` if they fail.
The value of that variable can be read by a call to teh function `dx_error dx_get_error()`.
The default is value `DX_NO_ERROR` which is the only error code that evaluates to the value zero and indicates that there is no error.

## Logging
Output for diagnostic purposes is written via the dx_log function.

## Message Queue
The engine is to a large extend "message based".
Entities send objects to a queue and entities consume messages from that queue.
For example, if the main loop of the program receives a "quit" message, then the loop will terminate and the program will exit.

Messages can have parameters.
For example, there is an "emit" message. If posted to the queue then the string - which is an argument of the message - is emitted to the standard output.