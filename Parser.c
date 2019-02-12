void system_goal(void)
{
  program();
  match(SCANEOF);
}

void program(void)
{
  match(BEGIN);
  statement_list();
  match(END)
}
