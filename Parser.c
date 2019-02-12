#include <stdio.h>
typedef enum token_types {
    BEGIN,
    END,
    READ,
    WRITE,
    ID,
    INTLITERAL,
    LPAREN,
    RPAREN,
    SEMICOLON,
    COMMA,
    ASSIGNOP,
    PLUSOP,
    MINUSOP,
    SCANEOF,
    TRUE
} token;

extern token scanner (void);

void parse()
{
    system_goal();
}
void system_goal(void)
{
  program();
  match(SCANEOF);
}

void program(void)
{
  match(BEGIN);
  statement_list();
  match(END);
}

void statement_list(void)
{
  statement();
  while (TRUE){
    switch(next_token()){
      case ID:
      case READ:
      case WRITE:
        statement();
        break;
      default:
        return;
    }
  }
}

void statement(void)
{
  token tok = next_token();
  switch(tok){
    case ID:
      match(ID);
      match(ASSIGNOP);
      expression();
      match(SEMICOLON);
      break;

    case READ:
      match(READ);
      match(LPAREN);
      id_list();
      match(RPAREN);
      match(SEMICOLON);
      break;

    case WRITE:
      match(WRITE);
      match(LPAREN);
      expr_list();
      match(RPAREN);
      match(SEMICOLON);
      break;

    default:
      printf("Syntax error: %d ", tok );
      //syntax_error(tok);
      break;

  }
}

void id_list(void)
{
  match(ID);
  while (next_token() == COMMA){
    match(COMMA);
    match(ID);
  }
}

void expression(void)
{
  token t;
  primary();
  for (t = next_token(); t == PLUSOP || t == MINUSOP; t = next_token()){
    add_op();
    primary();
  }
}

void expr_list(void)
{
  while (next_token() == COMMA){
    match(COMMA);
    expression();
  }
}

void add_op(void)
{
  token tok = next_token();
  if (tok == PLUSOP || tok == MINUSOP)
    match(tok);
  else
    printf("Syntax error: %d", tok );
    //syntax_error(tok);
}

void primary(void)
{
  token tok = next_token();
  switch (tok) {
    case LPAREN:
      match(LPAREN);
      expression();
      match(RPAREN);
      break;

    case ID:
      match(ID);
      break;

    case INTLITERAL:
      match(INTLITERAL);
      break;

    default:
      printf("Syntax error: %d", tok );
      //syntax_error(tok);
      break;
  }
}

/*MicroScanner::Token MicroParser::GetNextToken()
{
    MicroScanner::Token token;
    MicroScanner::TokenClass tokenClass;

    if (m_currentToken)
        token = *m_currentToken;
    else
    {
        token = m_scanner.GetToken();
        m_currentToken = std::make_shared<MicroScanner::Token>(token);
    }

    return token;
}

void MicroParser::Match(MicroScanner::TokenClass toMatch)
{
    MicroScanner::Token token;

    if (m_currentToken)
    {
        token = *m_currentToken;
        m_currentToken.reset();
    }
    else
        token = m_scanner.GetToken();

    MicroScanner::TokenClass tokenClass = token.first;

    if (tokenClass == toMatch)
    {
        const std::string& tokenValue = token.second;
        std::cout << "Matched \"" << tokenValue << "\"" << std::endl;
    }
    else
    {
        std::cout << "Syntax error!" << std::endl;
    }
}*/
