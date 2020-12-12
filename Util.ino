


void log(String str, char end)
{
  static const char flag_toggle_enabled = '\0';
  static bool log_enabled = true;

  char flag;
  str.getBytes(flag, 1);

  if (flag == flag_toggle_enabled)
    log_enabled = !log_enabled;
  
  if (log_enabled)
    Serial.print(str + end);
}








String SerialRead(bool wait_for_input)
{
  String output;
  if (wait_for_input)
    while (Serial.available() <= 0);
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == '\n')
      break;
    output += c;
  }
  return output;
}

















//
