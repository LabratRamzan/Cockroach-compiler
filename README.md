# Cockroach-compiler

EBNF

statements = statement | statement ";" statements

statement = "{" statements "}" | assignment | if | ifelse | while | print | break;

print = "print" "(" expr ")"

break = "break"

assignment = IDENT "=" expr

if = "if" "(" expr ")" statement "else" statement

ifelse = if "else statement

while = "while" "(" expr ")" statement

expr = expr0 "<" expr | expr0 "<=" expr | expr0 "==" expr | expr0 "!=" expr | expr0 ">" expr | expr0 ">=" expr | expr0 

expr0 = expr1 "+" expr | expr1 "-" expr | expr1

expr1 = atom "*" expr1 | atom "/" expr1 | atom

atom = "(" expr ")" | NUMBER
