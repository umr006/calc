#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CORRECTLY 0
#define ERROR 1
#define NO 0
#define YES 1
#define BUFF 255

typedef enum typ {
  number = 1,
  s21_x,
  open_bracket,
  close_bracket,
  s21_plus,
  s21_minus,
  s21_mult,
  s21_div,
  s21_pow,
  s21_mod,
  s21_cos,
  s21_sin,
  s21_tan,
  s21_acos,
  s21_asin,
  s21_atan,
  s21_sqrt,
  s21_log,
  s21_ln,
  s21_plus_un,
  s21_minus_un
} type_t;

typedef struct list {
  double value;
  int priority;
  type_t type;
  struct list *next;
} list_t;

int input_check(char *str);
int brackets(char *str, int len);
void calc_proc(char* str, double value_x, double *result);
void free_list(list_t *top);
int error_parser(char *str, int len) ;
int check_number(char *str, int len);
int check_x(char *str, int i);
int check_dot(char *str, int i);
int check_sign(char *str, int i);
int check_mod(char *str, int i);
int check_sign_before_func(char *str, int i);
int check_sin(char *str, int i);
int check_sqrt(char *str, int i);
int check_cos(char *str, int i);
int check_tan(char *str, int i);
int check_acos(char *str, int i);
int check_asin(char *str, int i);
int check_atan(char *str, int i);
int check_ln(char *str, int i);
int check_log(char *str, int i);
int plus_or_minus(char c);
int mul_or_div_or_pow(char c);
void parsing(char *str, list_t **list);
void create_and_add_list(list_t **list, char *num, int list_priority, type_t list_type);
list_t *create_list(char *str_value, int v_priority, type_t v_type);
void add_elem_list(list_t *head, char *str_value, int v_priority,
                   type_t v_type);
void create_stack(list_t** res, double value, int priority, type_t type);
void push_stack(list_t** res, double value, int priority, type_t type);
void create_push_stack(list_t** res, double value, int priority, type_t type);
void pop_stack(list_t **stack);
int priority_op(list_t* tmp, int priority);
void postfix_notation(list_t *list, list_t **res);
void reverse_stack(list_t **stack, double x);
void delete_stack(list_t** stack, list_t* elem);
void bi_op_calc(list_t** stack, list_t* a, list_t* b, list_t* c);
void un_op_calc(list_t** stack, list_t *a, list_t *b);
void calculate(list_t** stack);        
             
int input_check(char *str) {
  int error_code = 0;
  int len = strlen(str);
  if (len > 255) {
    error_code = 1;
  } else {
    if (brackets(str, len))
      error_code = 1;
    else if (error_parser(str, len))
      error_code = 1;
  }
  return error_code;
}

void output_list(list_t *head) {
    while (head != NULL) {
        printf("VALUE %lf\n", head->value);
        printf("PRIORITY %d\n", head->priority);
        printf("TYPE %d\n", head->type);
        head = head->next;
    }
}

int main() {
    char str[BUFF] = "cos(2)";
    double res = 0;
    calc_proc(str, 0, &res);
    printf("%lf\n", res);
    
    
    
//     // parsing(str, &list);
//     // postfix_notation(list, &res);
//     // reverse_stack(&res, 0);
//     // calculate(&res);
//     // output_list(res);
//     // free_list(list);
//     // free_list(res);
}

void calc_proc(char* str, double value_x, double *result) {
    value_x = 1;
    // char calc_str[BUFF];
    // strcpy(str, calc_str);
    // list_t* list = NULL;
    // list_t* res = NULL;
    // input_check(str);
    // parsing(str, &list);
    // postfix_notation(list, &res);
    // reverse_stack(&res, 0);
    // calculate(&res);
    // *result = res->value;
    // free_list(list);
    // free_list(res);

    value_x = 1;
    char calc_str[BUFF];
    list_t* list = NULL;
    list_t* res = NULL;
    input_check(str);
    parsing(str, &list);
    postfix_notation(list, &res);
    reverse_stack(&res, 0);
    calculate(&res);
    *result = res->value;
    free_list(list);
    free_list(res);

    // list_t* list = NULL;
    // list_t *res = NULL;
    // // double res = 0;
    // // calc_proc(str, 0, &res);
    // // printf("%lf\n", res);
    // parsing(str, &list);
    // postfix_notation(list, &res);
    // reverse_stack(&res, 0);
    // calculate(&res);
    // *result = res->value;
    // free_list(list);
    // free_list(res);
}

void free_list(list_t *top) {
    while(top != NULL) {
        list_t* tmp = top;
        top = top->next;
        free(tmp);
    }
}


int brackets(char *str, int len) {
  int error_code = CORRECTLY;
  int open_brackets = 0, close_brackets = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] == '(') {
      open_brackets++;
      if (str[i + 1] == ')') {
        error_code = ERROR;
        break;
      }
    }
    if (str[i] == ')') {
      close_brackets++;
      if (close_brackets > open_brackets) {
        error_code = ERROR;
        break;
      }
    }
  }
    if (open_brackets != close_brackets)
        error_code = ERROR;
    return error_code;
}

int error_parser(char *str, int len) {
  int error_code = CORRECTLY;
  for (int i = 0; i < len; i++) {
    if (error_code)
      break;
    if (str[i] >= '0' && str[i] <= '9') { //|| str[i] == '(' || str[i] == ')'
      if (check_number(str, i))
        error_code = ERROR;
    } else if (str[i] == 'x' || str[i] == 'X') {
      if (check_x(str, i))
        error_code = ERROR;
    } else if (str[i] == '.') {
      if (check_dot(str, i))
        error_code = ERROR;
    } else if (str[i] == '+' || str[i] == '-') {
      if (check_sign(str, i))
        error_code = ERROR;
    } else if (str[i] == '*' || str[i] == '/') {
      if (i == 0 || str[i - 1] == '(' || check_sign(str, i))
        error_code = ERROR;
    } else if (str[i] == 'm') {
      int j = i + 3;
      if (j < len) {
        if (check_mod(str, i))
          error_code = ERROR;
        else
          i += 2;
      } else
        error_code = ERROR;
    } else if (str[i] == 's') {
      if (!check_sin(str, i))
        i += 3;
      else if (!check_sqrt(str, i))
        i += 4;
      else
        error_code = ERROR;
    } else if (str[i] == 'c' || str[i] == 't') {
      if (check_cos(str, i) && check_tan(str, i))
        error_code = ERROR;
      else
        i += 3;
    } else if (str[i] == 'a') {
      if (check_acos(str, i) && check_asin(str, i) && check_atan(str, i))
        error_code = ERROR;
      else
        i += 4;
    } else if (str[i] == 'l') {
      if (!check_ln(str, i))
        i += 2;
      else if (!check_log(str, i))
        i += 3;
      else
        error_code = ERROR;
    }
  }
  return error_code;
}

int check_number(char *str, int len) {
    int error_code = CORRECTLY;
    if (len != 0) {
        if (str[len - 1] == ')')
        error_code = ERROR;
    }
    if (str[len + 1] == '(')
        error_code = ERROR;
    if (str[len + 1] == 'c' || str[len + 1] == 's')
        error_code = ERROR;
    if (str[len + 1] == 't' || str[len + 1] == 'a')
        error_code = ERROR;
    if (str[len + 1] == 'l')
        error_code = ERROR;
    return error_code;
}

int check_x(char *str, int i) {
  int error_code = CORRECTLY;
  if (i != 0) {
    if (str[i - 1] >= '0' || str[i - 1] <= '9')
      error_code = ERROR;
    if (str[i - 1] == '.' || str[i - 1] == ')')
      error_code = ERROR;
  }
  if (str[i + 1] >= '0' || str[i + 1] <= '9')
    error_code = ERROR;
  if (str[i + 1] == '.' || str[i + 1] == '(')
    error_code = ERROR;
  if (str[i + 1] == 'c' || str[i + 1] == 's')
    error_code = ERROR;
  if (str[i + 1] == 't' || str[i + 1] == 'a')
    error_code = ERROR;
  if (str[i + 1] == 'l')
    error_code = ERROR;
  return error_code;
}

int check_dot(char *str, int i) {
  int error_code = CORRECTLY;
  if (i != 0) {
    if (str[i - 1] < 0 || str[i - 1] < 9) {
      error_code = ERROR;
    } else if (str[i + 1] < 0 || str[i + 1] < 9) {
      error_code = ERROR;
    }
  } else
    error_code = ERROR;
  return error_code;
}

int check_sign(char *str, int i) {
    int res_code = CORRECTLY;
    if (str[i + 1] == ')')
        res_code = ERROR;
    if (i != 0) {
        if (plus_or_minus(str[i - 1]) || mul_or_div_or_pow(str[i + 1]))
            res_code = ERROR;
        if (plus_or_minus(str[i - 1]) || mul_or_div_or_pow(str[i + 1]))
            res_code = ERROR;
        if (str[i - 1] == 'd' || str[i + 1] == 'm')
            res_code = ERROR;
    } else {
        if (mul_or_div_or_pow(str[i + 1]))
            res_code = ERROR;
        if (plus_or_minus(str[i + 1]))
            res_code = ERROR;
        if (str[i + 1] == 'm')
            res_code = ERROR;
    }
    return res_code;
}

int check_mod(char *str, int i) {
  int error_code = CORRECTLY;
  if (i != 0) {
    if (str[i - 1] == '(' || str[i + 3] == ')')
      error_code = ERROR;
    if (str[i - 1] == '+' || str[i - 1] == '-' || str[i + 1] == '+' ||
        str[i + 1] == '-')
      error_code = ERROR;
    if (str[i - 3] == '+' || str[i - 3] == '-' || str[i + 3] == '+' ||
        str[i + 3] == '-')
      error_code = ERROR;
    if (str[i + 1] != 'o' || str[i + 2] != 'd')
      error_code = ERROR;
    if (str[i + 3] == 'm' || str[i - 1] == 'd')
      error_code = ERROR;
  } else {
    error_code = ERROR;
  }
  return error_code;
}

int check_sign_before_func(char *str, int i) {
  int error_code = CORRECTLY;
  if (i != 0 && str[i - 1] != '(' && str[i - 1] != 'd') {
    if (!plus_or_minus(str[i - 1]) && !mul_or_div_or_pow(str[i - 1])) {
      error_code = ERROR;
    }
  }
  return error_code;
}

int check_sin(char *str, int i) {
  int error_code = CORRECTLY;
  if (strncmp(&str[i], "sin(", 4))
    error_code = ERROR;
  if (check_sign_before_func(str, i))
    error_code = ERROR;
  return error_code;
}

int check_sqrt(char *str, int i) {
  int error_code = CORRECTLY;
  if (strncmp(&str[i], "sqrt(", 5))
    error_code = ERROR;
  if (check_sign_before_func(str, i))
    error_code = ERROR;
  return error_code;
}

int check_cos(char *str, int i) {
  int error_code = CORRECTLY;
  if (strncmp(&str[i], "cos(", 4))
    error_code = ERROR;
  if (check_sign_before_func(str, i))
    error_code = ERROR;
  return error_code;
}

int check_tan(char *str, int i) {
  int error_code = CORRECTLY;
  if (strncmp(&str[i], "tan(", 4))
    error_code = ERROR;
  if (check_sign_before_func(str, i))
    error_code = ERROR;
  return error_code;
}

int check_acos(char *str, int i) {
  int error_code = ERROR;
  if (strncmp(&str[i], "acos(", 5))
    error_code = ERROR;
  if (check_sign_before_func(str, i))
    error_code = ERROR;
  return error_code;
}

int check_asin(char *str, int i) {
  int error_code = ERROR;
  if (strncmp(&str[i], "asin(", 5))
    error_code = ERROR;
  if (check_sign_before_func(str, i))
    error_code = ERROR;
  return error_code;
}

int check_atan(char *str, int i) {
  int error_code = ERROR;
  if (strncmp(&str[i], "atan(", 5))
    error_code = ERROR;
  if (check_sign_before_func(str, i))
    error_code = ERROR;
  return error_code;
}

int check_ln(char *str, int i) {
  int error_code = ERROR;
  if (strncmp(&str[i], "ln(", 3))
    error_code = ERROR;
  if (check_sign_before_func(str, i))
    error_code = ERROR;
  return error_code;
}

int check_log(char *str, int i) {
  int error_code = ERROR;
  if (strncmp(&str[i], "log(", 4))
    error_code = ERROR;
  if (check_sign_before_func(str, i))
    error_code = ERROR;
  return error_code;
}

int plus_or_minus(char c) {
  int a = 0;
  if (c == '+' || c == '-') {
    a = 1;
  }
  return a;
}

int mul_or_div_or_pow(char c) {
  int a = 0;
  if (c == '*' || c == '/' || c == '^') {
    a = 1;
  }
  return a;
}

void parsing(char *str, list_t **list) {
  char num[BUFF];
  int i = 0;
  int len = strlen(str);
  for (int j = 0; j < len; j++) {
    if ((str[j] >= '0' && str[j] <= '9') || str[j] == '.') {
      num[i++] = str[j];
      if ((str[j + 1] < '0' || str[j + 1] > '9') && str[j + 1] != '.') {
        num[i] = 0;
        create_and_add_list(list, num, 0, number);
        i = 0;
      }
    } else if (str[j] == 'x' || str[j] == 'X') {
      create_and_add_list(list, "0", 0, s21_x);
    } else if (str[j] == '(') {
      create_and_add_list(list, "0", 0, open_bracket);

    } else if (str[j] == ')') {
      create_and_add_list(list, "0", 0, close_bracket);
    } else if (str[j] == '+') {
      if (str[j - 1] == '(')
        create_and_add_list(list, "0", 1, s21_plus_un);
      else
        create_and_add_list(list, "0", 1, s21_plus);
    } else if (str[j] == '-') {
      if (str[j - 1] == '(')
        create_and_add_list(list, "0", 1, s21_minus_un);
      else
        create_and_add_list(list, "0", 1, s21_minus);
    } else if (str[j] == '*') {
      create_and_add_list(list, "0", 2, s21_mult);
    } else if (str[j] == '/') {
      create_and_add_list(list, "0", 2, s21_div);
    } else if (str[j] == '^') {
      create_and_add_list(list, "0", 3, s21_pow);
    } else if (str[j] == 'm') {
      create_and_add_list(list, "0", 2, s21_mod);
      j += 2;
    } else if (str[j] == 'c') {
      create_and_add_list(list, "0", 4, s21_cos);
      j += 2;
    } else if (str[j] == 's' && str[j + 1] == 'i') {
      create_and_add_list(list, "0", 4, s21_sin);
      j += 2;
    } else if (str[j] == 't') {
      create_and_add_list(list, "0", 4, s21_tan);
      j += 2;
    } else if (str[j] == 'a' && str[j + 1] == 'c') {
      create_and_add_list(list, "0", 4, s21_acos);
      j += 3;
    } else if (str[j] == 'a' && str[j + 1] == 's') {
      create_and_add_list(list, "0", 4, s21_asin);
      j += 3;
    } else if (str[j] == 'a' && str[j + 1] == 't') {
      create_and_add_list(list, "0", 4, s21_atan);
      j += 3;
    } else if (str[j] == 's' && str[j + 1] == 'q') {
      create_and_add_list(list, "0", 4, s21_sqrt);
      j += 3;
    } else if (str[j] == 'l' && str[j + 1] == 'n') {
      create_and_add_list(list, "0", 4, s21_ln);
      j += 1;
    } else if (str[j] == 'l' && str[j + 1] == 'o') {
      create_and_add_list(list, "0", 4, s21_log);
      j += 2;
    }
  }
}

void create_and_add_list(list_t **list, char *num, int list_priority,
                         type_t list_type) {
  if (*list == NULL)
    *list = create_list(num, list_priority, list_type);
  else
    add_elem_list(*list, num, list_priority, list_type);
}

list_t *create_list(char *str_value, int v_priority, type_t v_type) {
  list_t *tmp = (struct list *)malloc(sizeof(struct list));
  tmp->value = atof(str_value);
  tmp->priority = v_priority;
  tmp->type = v_type;
  tmp->next = NULL;
  return tmp;
}

void add_elem_list(list_t *head, char *str_value, int v_priority,
                   type_t v_type) {
  list_t *tmp = head;
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = create_list(str_value, v_priority, v_type);
}


void create_stack(list_t** res, double value, int priority, type_t type) {
  list_t *tmp = (struct list *)malloc(sizeof(struct list));
  tmp->value = value;
  tmp->priority = priority;
  tmp->type = type;
  tmp->next = NULL;
  *res = tmp;
}

void push_stack(list_t** res, double value, int priority, type_t type) {
  list_t *tmp = (struct list *)malloc(sizeof(struct list));
  tmp->value = value;
  tmp->priority = priority;
  tmp->type = type;
  tmp->next = *res;
  *res = tmp;
}

void create_push_stack(list_t** res, double value, int priority, type_t type) {
  if (*res == NULL)
    create_stack(res, value, priority, type);
  else
    push_stack(res, value, priority, type);
}

void pop_stack(list_t **stack) {
  if (*stack != NULL) {
    list_t *tmp = (*stack)->next;
    free(*stack);
    *stack = tmp;
  }
}

int priority_op(list_t* tmp, int priority) {
  int res = 0;
  if (tmp != NULL) {
    if (priority > tmp->priority)
      res = 1;
  } else
      res = 1;
  return res;
}

void postfix_notation(list_t *list, list_t **res) {
  list_t *stack_tmp = NULL;
  while (list) {
    if (list->type != 4) {
      if (list->type == 1 || list->type == 2) {
        create_push_stack(res, list->value, list->priority,
                              list->type);
      } else {
        while (1) {
          if (priority_op(stack_tmp, list->priority) || list->type == 3) {
            create_push_stack(&stack_tmp, list->value, list->priority,
                                  list->type);
            break;
          } else {
            create_push_stack(res, stack_tmp->value,
                                  stack_tmp->priority, stack_tmp->type);
            pop_stack(&stack_tmp);
          }
        }
      }
    } else {
      while (stack_tmp->type != 3) {
        create_push_stack(res, stack_tmp->value, stack_tmp->priority,
                              stack_tmp->type);
        pop_stack(&stack_tmp);
      }
      pop_stack(&stack_tmp);
    }
    list = list->next;
  }
  while (stack_tmp) {
    create_push_stack(res, stack_tmp->value, stack_tmp->priority,
                          stack_tmp->type);
    pop_stack(&stack_tmp);
  }
}

void reverse_stack(list_t **stack, double x) {
  list_t *stack_rev = NULL;
  while (*stack) {
    if ((*stack)->type == s21_x) {
      (*stack)->type = number;
      (*stack)->value = x;
    }
    create_push_stack(&stack_rev, (*stack)->value, (*stack)->priority,(*stack)->type);
    pop_stack(stack);
  }
  *stack = stack_rev;
}

void delete_stack(list_t** stack, list_t* elem) {
    list_t *tmp;
    tmp = *stack;
    if (*stack == elem) {
        tmp = (*stack)->next;
        free(*stack);
        *stack = tmp;
    } else {
        while (tmp->next != elem) {
            tmp = tmp->next;
        }
        tmp->next = elem->next;
        free(elem);
    }
}


void bi_op_calc(list_t** stack, list_t* a, list_t* b, list_t* c) {
    double op1, op2, res = 0;
    op1 = a->value;
    op2 = b->value;
    if(c->type == s21_plus) {
        res = op1 + op2;
    } else if (c->type == s21_minus) {
        res = op1 - op2;
    } else if (c->type == s21_mult) {
        res = op1 * op2;
    } else if (c->type == s21_div) {
        res = op1 / op2;
    } else if (c->type == s21_pow) {
        res = pow(op1, op2);
    } else if (c->type == s21_mod) {
        res = fmod(op1, op2);
    }
    a->priority = 0;
    a->type = number;
    a->value = res;
    delete_stack(stack, c);
    delete_stack(stack, b);
}

void un_op_calc(list_t** stack, list_t *a, list_t *b) {
    double op1 = 0, res = 0;
    op1 = a->value;
    if (b->type == s21_plus_un) {
        res = +op1;
    } else if (b->type == s21_minus_un) {
        res = -op1;
    } else if (b->type == s21_sin) {
        res = sin(op1);
    } else if (b->type == s21_cos) {
        res = cos(op1);
    } else if (b->type == s21_tan) {
        res = tan(op1);
    } else if (b->type == s21_asin) {
        res = asin(op1);
    } else if (b->type == s21_acos) {
        res = acos(op1);
    } else if (b->type == s21_atan) {
        res = atan(op1);
    } else if (b->type == s21_sqrt) {
        res = sqrt(op1);
    } else if (b->type == s21_log) {
        res = log10(op1);
    } else if (b->type == s21_ln) {
        res = log(op1);
    }
    a->value = res;
    a->priority = 0;
    a->type = number;
    delete_stack(stack, b);
}

void calculate(list_t** stack) {
    while((*stack)->next != NULL) {
        list_t *a, *b, *c;
        a = *stack;
        b = a->next;
        if (b->next != NULL)
            c = b->next;
        if (b->type == number) {
            while(1) {
                if (c->priority) {
                    break;
                }
                a = b;
                b = a->next;
                c = b->next;
            }
            if (c->type >= 5 && c->type <= 9) {
                bi_op_calc(stack, a, b, c);
            } else {
                un_op_calc(stack, b, c);
            }
        } else {
            un_op_calc(stack, a, b);
        }
    }
}