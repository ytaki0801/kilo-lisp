/*
 * JMC Lisp: defined in McCarthy's 1960 paper,
 *           derived from http://www.paulgraham.com/lispcode.html
 *
 * This code is Licensed under CC0.
 * https://creativecommons.org/publicdomain/zero/1.0/
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SSTR_MAX 4096

typedef uintptr_t value_t;
enum NODE_TAG { NODE_STRG, NODE_CONS };

typedef struct _node_t_ {
  value_t value;
  enum NODE_TAG tag;
} _node_t, *node_t;

node_t node(value_t value, enum NODE_TAG tag);

typedef struct _cons_t_ {
  node_t x;
  node_t y;
} _cons_t, *cons_t;

#define str_to_node(s)  (node((value_t)(s), NODE_STRG))
#define node_to_str(s)  ((char *)(s->value))
#define car(s)    (((cons_t)(s->value))->x)
#define cdr(s)    (((cons_t)(s->value))->y)

#define n_strg(s)  (s->tag == NODE_STRG)
#define n_cons(s)  (s->tag == NODE_CONS)

#define atom(s)   (eq(s, NULL) || n_strg(s))

#define S_T   (str_to_node("t"))
#define S_NIL (NULL)

int s_lex(const char *s, char* sl[])
{
  char sf[SSTR_MAX * 3];
  int i, j = 0;
  for (i = 0; i < strlen(s); i++) {
    switch (s[i]) {
      case '(':
      case ')':
      case '\'':
        sf[j++] = ' '; sf[j++] = s[i]; sf[j++] = ' ';
        break;
      case '\n': j++; break;
      default: sf[j++] = s[i];
    }
  }
  sf[j] = '\0';

  char *t;
  int len = 0;
  for (t = strtok(sf, " "); t != NULL; t = strtok(NULL, " "))
    sl[len++] = t;
  sl[len] = NULL;

  return (len);
}

node_t cons(node_t x, node_t y);

node_t s_syn(char *s[], int *pos)
{
  char *t = s[*pos];
  *pos = *pos - 1;

  if (t[0] == ')') {
    node_t r = NULL;
    while (s[*pos][0] != '(') {
      if (s[*pos][0] == '.') {
        *pos = *pos - 1;
        r = cons(s_syn(s, pos), car(r));
      } else {
        r = cons(s_syn(s, pos), r);
      }
    }
    *pos = *pos - 1;
    if (*pos != -1 && s[*pos][0] == '\'') {
      *pos = *pos - 1;
      return cons(str_to_node("quote"), cons(r, NULL));
    } else {
      return (r);
    }
  } else {
    node_t tn = str_to_node(t);
    if (*pos != -1 && s[*pos][0] == '\'') {
      *pos = *pos - 1;
      return cons(str_to_node("quote"), cons(tn, NULL));
    } else {
      return (tn);
    }
  }
}

node_t node(value_t value, enum NODE_TAG tag)
{
  node_t n = (node_t)malloc(sizeof(_node_t));
  n->value = value; n->tag = tag;
  return (n);
}

node_t cons(node_t x, node_t y)
{
  cons_t c = (cons_t)malloc(sizeof(_cons_t));
  c->x = x; c->y = y;
  node_t n = node((value_t)c, NODE_CONS);
  return (n);
}

int eq(node_t s1, node_t s2)
{
  if (s1 == NULL && s2 == NULL) return (1);
  else if (s1 == NULL || s2 == NULL) return (0);
  else if (n_cons(s1) || n_cons(s2)) return (0);
  else return (!strcmp(node_to_str(s1), node_to_str(s2)));
}

node_t caar(node_t x) { return car(car(x)); }
node_t cadr(node_t x) { return car(cdr(x)); }
node_t cadar(node_t x) { return car(cdr(car(x))); }
node_t caddr(node_t x) { return car(cdr(cdr(x))); }
node_t caddar(node_t x) { return car(cdr(cdr(car(x)))); }

node_t s_null(node_t x) {
  if (eq(x, NULL)) return S_T; else return S_NIL;
}

node_t s_append(node_t x, node_t y)
{
  if (s_null(x)) return y;
  else return cons(car(x), s_append(cdr(x), y));
}

node_t s_list(node_t x, node_t y)
{
  return cons(x, cons(y, NULL));
}

node_t s_pair(node_t x, node_t y)
{
  if (s_null(x) && s_null(y)) return NULL;
  else if (!atom(x) && !atom(y))
    return cons(s_list(car(x), car(y)),
                s_pair(cdr(x), cdr(y)));
  else return S_NIL;
}

node_t s_assoc(node_t x, node_t y)
{
  if (s_null(y)) return S_NIL;
  else if (eq(caar(y), x)) return cadar(y);
  else return s_assoc(x, cdr(y));
}

node_t evcon(node_t c, node_t a);
node_t evlis(node_t m, node_t a);

node_t s_eval(node_t e, node_t a)
{
  if      (eq(e, str_to_node("t")))   return S_T;
  else if (eq(e, str_to_node("nil"))) return S_NIL;
  else if (atom(e)) return s_assoc(e, a);
  else if (atom(car(e))) {
    if      (eq(car(e), str_to_node("quote"))) return cadr(e);
    else if (eq(car(e), str_to_node("atom")))
      if (atom(s_eval(cadr(e), a))) return S_T;
      else return S_NIL;
    else if (eq(car(e), str_to_node("eq")))
      if (eq(s_eval(cadr(e), a), s_eval(caddr(e), a))) return S_T;
      else return S_NIL;
    else if (eq(car(e), str_to_node("car")))   return car( s_eval(cadr(e), a));
    else if (eq(car(e), str_to_node("cdr")))   return cdr( s_eval(cadr(e), a));
    else if (eq(car(e), str_to_node("cons")))  return cons(s_eval(cadr(e), a),
                                                           s_eval(caddr(e), a));
    else if (eq(car(e), str_to_node("cond")))  return evcon(cdr(e), a);
    else return s_eval(cons(s_assoc(car(e), a), cdr(e)), a);
  } else if (eq(caar(e), str_to_node("lambda"))) {
    return s_eval(caddar(e),
                  s_append(s_pair(cadar(e), evlis(cdr(e), a)), a));
  } else {
    return NULL;
  }
}

node_t evcon(node_t c, node_t a)
{
  if (s_eval(caar(c), a)) return s_eval(cadar(c), a);
  else return evcon(cdr(c), a);
}

node_t evlis(node_t m, node_t a)
{
  if (s_null(m)) return NULL;
  else return cons(s_eval(car(m), a), evlis(cdr(m), a));
}

char s_eval_retval[SSTR_MAX];
void s_output(node_t s);

void s_strcons(node_t s)
{
  s_output(car(s));
  node_t sd = cdr(s);
  if (sd == NULL) {
  } else if (n_strg(sd)) {
    strcat(s_eval_retval, " . ");
    strcat(s_eval_retval, node_to_str(sd));
  } else {
    strcat(s_eval_retval, " ");
    s_strcons(sd);
  }
}

void s_output(node_t s)
{
  if (s == NULL) {
    strcat(s_eval_retval, "()");
  } else if (n_strg(s)) {
    strcat(s_eval_retval, node_to_str(s));
  } else {
    strcat(s_eval_retval, "(");
    s_strcons(s);
    strcat(s_eval_retval, ")");
  }
}

void s_eval_string(char *s)
{
  char *lr_s[SSTR_MAX];
  int s_len;

  s_len = s_lex(s, lr_s) - 1;
  node_t rs = s_syn(lr_s, &s_len);
  node_t r = s_eval(rs, NULL);
  s_eval_retval[0] = '\0';
  s_output(r);
}

/*
int main(void)
{
  s_eval_string(
    "(car (cdr '(10 20 30)))"
  );
  printf("%s\n", s_eval_retval);

  s_eval_string(
    "((lambda (x) (car (cdr x))) '(abc def ghi))"
  );
  printf("%s\n", s_eval_retval);

  s_eval_string(
    "((lambda (func x y) (func x (func y '()))) \
     'cons '10 '20)"
  );
  printf("%s\n", s_eval_retval);

  s_eval_string(
    "((lambda (func x y) (func x (func y '()))) \
     '(lambda (x y) (cons x (cons y '())))      \
     '10 '20)"
  );
  printf("%s\n", s_eval_retval);

  s_eval_string(
    "((lambda (assoc k v) (assoc k v))  \
     '(lambda (k v)                     \
        (cond ((eq v '()) nil)          \
              ((eq (car (car v)) k)     \
               (car v))                 \
              ('t (assoc k (cdr v)))))  \
     'Orange \
     '((Apple . 120) (Orange . 210) (Lemmon . 180)))"
  );
  printf("%s\n", s_eval_retval);

  return (0);
}
*/
