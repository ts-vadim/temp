# Log

Little class for logging.

## TODO:
- [x] Add log levels.
- [x] Format output.
- [x] Show time.
- [x] Show file, line number and function.
- [ ] Add file io.
- [ ] Color output.
- [ ] Make it asynchronous.

## FIXIT:
* Code duplication of print functions in log.cpp

## Variables:

### *enum* level
| Name | Description |
| ---- | ----------- |
| **0**: info | info log level. |
| **1**: debug | debug log level. |
| **2**: warning | warning log level. |
| **3**: error | error log level. |


## Methods:

> print(log_level, format, ...)

Prints formatted string with log level prefix *only if* **log_level** *is equals or greater than* **global log level** set by **setLogLevel()**.
#### Parameters:
| Name | Description |
| ---- | ----------- |
| log_level | *enum level*<br>message log level. |
| format | *const char\**<br>printf style format string. |
<br>


> print(format, ...)

Calls **print(log_level, format)** with log level **info**.
#### Parameters:
| Name | Description |
| ---- | ----------- |
| format | *const char\**<br>printf style format string. |
<br>


> print(log_level, format, caller_file, caller_line, caller_func, ...)

Prints formatted string with log level prefix *only if* **log_level** *is equals or greater than* **global log level** set by **setLogLevel()**.
#### Parameters:
| Name | Description |
| ---- | ----------- |
| log_level | *enum level*<br>message log level. |
| format | *const char\**<br>printf style format string. |
| caller_file | *const char\**<br>string for \_\_FILE\_\_ macro. |
| caller_line | *const int*<br>string for \_\_LINE\_\_ macro. |
| caller_func | *const char\**<br>string for \_\_FUNCTION\_\_ macro. |
<br>


> print(format, caller_file, caller_line, caller_func, ...)

Calls **print(log_level, format, caller_file, caller_line, caller_func, ...)** with log level **info**.
#### Parameters:
| Name | Description |
| ---- | ----------- |
| format | *const char\**<br>printf style format string. |
| caller_file | *const char\**<br>string for \_\_FILE\_\_ macro. |
| caller_line | *const int*<br>string for \_\_LINE\_\_ macro. |
| caller_func | *const char\**<br>string for \_\_FUNCTION\_\_ macro. |
<br>


> setLogLevel(log_level)

Sets global log level.
#### Parameters:
| Name | Description |
| ---- | ----------- |
| log_level | *enum level*<br>global message log level. |


## Static Methods:

> Print(log_level, format, caller_file, caller_line, caller_func, ...)

Prints formatted string with specified log level.
#### Parameters:
| Name | Description |
| ---- | ----------- |
| log_level | *enum level*<br>message log level. |
| format | *const char\**<br>printf style format string. |
| caller_file | *const char\**<br>string for \_\_FILE\_\_ macro. |
| caller_line | *const int*<br>string for \_\_LINE\_\_ macro. |
| caller_func | *const char\**<br>string for \_\_FUNCTION\_\_ macro. |
<br>


> Print(format, caller_file, caller_line, caller_func, ...)

Calls **Print(log_level, format, caller_file, caller_line, caller_func, ...)** with log level **info**.
#### Parameters:
| Name | Description |
| ---- | ----------- |
| log_level | *enum level*<br>message log level. |
| format | *const char\**<br>printf style format string. |
| caller_file | *const char\**<br>string for \_\_FILE\_\_ macro. |
| caller_line | *const int*<br>string for \_\_LINE\_\_ macro. |
| caller_func | *const char\**<br>string for \_\_FUNCTION\_\_ macro. |


## Usage:
```c++
Log::Print(Log::level::info, "Format %s", "message");
Log::Print(Log::level::info, "Format %s", __FILE__, __LINE__, __FUNCTION__, "message");

Log log;
log.print("Message");
log.print("Message", __FILE__, __LINE__, __FUNCTION__);
log.print(Log::level::debug, "Debug Message", __FILE__, __LINE__, __FUNCTION__);
```
### Output:
```
[00:41:30.383] INFO: Format message
[00:41:30.384 - main.cpp:11 - main()] INFO: Format message
[00:41:30.385] INFO: Message
[00:41:30.385 - main.cpp:15 - main()] INFO: Format message
[00:41:30.385 - main.cpp:16 - main()] DEBUG: Format message
```
