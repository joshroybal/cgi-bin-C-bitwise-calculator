#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum operation { convert, complement, mask, shift };

/* prototype of function that gets step? parameter */
void read_query_string(char []);
int read_step1_form(char [], char *);
int read_step2_form(char [], char *, char *, int *, int *);
void print_base_form();
void print_convert_form();
void print_complement_form();
void print_mask_form();
void print_shift_form();
void print_html_line(char []);
void print_table_headers();
void print_table_row(unsigned, char *);
void print_convert_table(unsigned);
void print_complement_table(unsigned);
void print_mask_table(unsigned, unsigned, char *);
void print_shift_table(unsigned, unsigned, char *);
void display_bits(int a);
void print_error_exit_msg();

int main()
{   char base;
    char card[257], buf[257], operation[11];
    int a, choice;
    unsigned b, x;

    printf("Content-Type:text/html;charset=iso-8859-1\n\n");
    printf("\n<!DOCTYPE html>");
    printf("\n<html>");
    printf("\n<head>");
    printf("\n<meta charset=\"utf-8\">");
    printf("\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    printf("\n<link rel=\"stylesheet\" media=\"all\"  href=\"/includes/style.css\">");
    /* printf("\n<style> table td {text-align:right; white-space:nowrap;} </style>"); */
    printf("\n</head>");
    printf("\n<body>");
    printf("\n<header><p>C bitwise calculator (complements, masking, & shifting)</p></header>");
    printf("\n<div><a href=\"/index.php\">Home</a> | <a href=\"/bitcalc.html\">Back</a></div>");

    read_query_string(card);

    if (strcmp(card, "step1") == 0) {
        /* write out the initial form */
        printf("\n<form action = \"bitcalc.cgi?step2\" method = \"POST\" accept-charset=\"utf-8\">");
        printf("\n<div>select operation below</div>");
        printf("\n<ul>");
        printf("\n<li><input type=\"radio\" name=\"operation\" value=\"convert\">convert base</li>");
        printf("\n<li><input type=\"radio\" name=\"operation\" value=\"complement\">one's complement</li>");
        printf("\n<li><input type=\"radio\" name=\"operation\" value=\"mask\">bit masking</li>");
        printf("\n<li><input type=\"radio\" name=\"operation\" value=\"shift\">bit shifting</li>");
        printf("\n</ul>");
        printf("\n<div><input type = \"submit\" value = \"Submit\"></div></form>");
    }
    else if (strcmp(card, "step2") == 0) {
        choice = read_step1_form(buf, operation);

        /* bail on no base input */
        if (choice < 0) {
            print_error_exit_msg();
            return(1);
        }

        switch (choice) {
            case convert :
                print_convert_form();
                break;
            case complement :
                print_complement_form();
                break;
            case mask :
                print_mask_form();
                break;
            case shift :
                print_shift_form();
                break;
            default :
                return(1);
        }
    }
    else if (strcmp(card, "step3") == 0) {
        choice = read_step2_form(buf, operation, &base, &a, &b);

        /* bail on no base input */
        if (choice < 0) {
            print_error_exit_msg();
            return(1);
        }

      
        switch (choice) {
            case convert :
                print_convert_table(a);
                break;
            case complement :
                print_complement_table(a);
                break;
            case mask :
                print_mask_table(a, b, operation);
                break;
            case shift :
                print_shift_table(a, b, operation);
                break;
            default :
                return(1);
        }
    }
    else {
        printf("\n<p>under development</p>");
    }
    printf("\n<div><a href=\"/index.php\">Home</a> | <a href=\"/bitcalc.html\">Back</a></div>");
    printf("\n<footer><p>C bitwise calculator</p></footer>");
    printf("\n</body>");
    printf("\n</html> ");
    return(0);
}

void read_query_string(char card[])
{
    strcpy(card, getenv("QUERY_STRING"));
}

int read_step1_form(char buf[], char *operation)
{
    scanf(" %[^\n]", buf);
    sscanf(buf, " %*[^=]%*c%s", operation);
    if (strcmp("convert", operation) == 0)
        return(convert);
    else if (strcmp("complement", operation) == 0)
        return(complement);
    else if (strcmp("mask", operation) == 0)
        return(mask);
    else if (strcmp("shift", operation) == 0)
        return(shift);
    else
        return(-1);
}

int read_step2_form(char buf[], char *operation, char *base, int *a, int *b)
{
    char sentinel[20];
    scanf(" %[^\n]", buf);
    sscanf(buf, " %[^=]", sentinel);

    if (strcmp("number", sentinel) == 0) { /* convert base selected */
        sscanf(buf, " %*[^=]%*c%i%*[^=]%*c%c", a, base);
        
        if (*base == '\0')
            return(-1);
        
        return(convert);
    }
    else if (strcmp("value", sentinel) == 0) { /* one's complement selected */
        sscanf(buf, " %*[^=]%*c%i%*[^=]%*c%c", a, base);

        if (*base == '\0')
            return(-1);
                
        return(complement);
    }
    else if (strcmp("operation", sentinel) == 0) { /* masking or shifting selected */
        sscanf(buf, " %*[^=]%*c%[^&]%*c%*[^=]%*c%i%*[^=]%*c%i%*[^=]%*c%c", operation, a, b, base);

        if (*base == '\0')
            return(-1);

        if (strcmp("and", operation) == 0)
            return(mask);
        else if (strcmp("or", operation) == 0)
            return(mask);
        else if (strcmp("xor", operation) == 0)
            return(mask);
        else if (strcmp("left", operation) == 0)
            return(shift);
        else if (strcmp("right", operation) == 0)
            return(shift);
        else
            return(-1);
    }
    return(-1);
}

void print_base_form()
{
    printf("\n<ul>");
    printf("\n<li><input type=\"radio\" name=\"base\" value=\"d\">decimal</li>");
    printf("\n<li><input type=\"radio\" name=\"base\" value=\"x\">hexadecimal</li>");
    printf("\n<li><input type=\"radio\" name=\"base\" value=\"o\">octal</li>");
    printf("\n</ul>");
    printf("\n<div><input type = \"submit\" value = \"Submit\"></div></form>");
}

void print_convert_form()
{
    printf("\n<form action = \"bitcalc.cgi?step3\" method = \"POST\" accept-charset=\"utf-8\">");
    print_html_line("enter no. below - enter leading 0x for hexadecimal, enter leading 0 for octal");
    printf("\n<div><input type = \"text\" name=\"number\" size=\"20\"</div>");
    print_base_form();
}

void print_complement_form()
{
    printf("\n<form action = \"bitcalc.cgi?step3\" method = \"POST\" accept-charset=\"utf-8\">");
    print_html_line("enter no. below - enter leading 0x for hexadecimal, enter leading 0 for octal");
    printf("\n<div><input type = \"text\" name=\"value\" size=\"20\"</div>");
    print_base_form();
}

void print_mask_form()
{
    printf("\n<form action = \"bitcalc.cgi?step3\" method = \"POST\" accept-charset=\"utf-8\">");
    printf("\n<div>select operation below</div>");
    printf("\n<ul>");
    printf("\n<li><input type=\"radio\" name=\"operation\" value=\"and\">& bitwise AND</li>");
    printf("\n<li><input type=\"radio\" name=\"operation\" value=\"or\">| bitwise OR</li>");
    printf("\n<li><input type=\"radio\" name=\"operation\" value=\"xor\">^ bitwise XOR</li>");
    printf("\n</ul>");
    print_html_line("enter no. below - enter leading 0x for hexadecimal, enter leading 0 for octal");
    printf("\n<div><input type = \"text\" name=\"number\" size=\"20\"</div>");
    print_html_line("enter mask below - enter leading 0x for hexadecimal, enter leading 0 for octal");
    printf("\n<div><input type = \"text\" name=\"mask\" size=\"20\"</div>");
    print_base_form();
}

void print_shift_form()
{
    printf("\n<form action = \"bitcalc.cgi?step3\" method = \"POST\" accept-charset=\"utf-8\">");
    printf("\n<div>select operation below</div>");
    printf("\n<ul>");
    printf("\n<li><input type=\"radio\" name=\"operation\" value=\"left\">left shift</li>");
    printf("\n<li><input type=\"radio\" name=\"operation\" value=\"right\">right shift</li>");
    printf("\n</ul>");
    print_html_line("enter no. below - enter leading 0x for hexadecimal, enter leading 0 for octal");
    printf("\n<div><input type = \"text\" name=\"number\" size=\"20\"</div>");
    printf("\n<div>enter shift below</div>");
    printf("\n<div><input type = \"text\" name=\"shift\" size=\"10\"</div>");        
    print_base_form();
}

void print_html_line(char buf[])
{
    printf("\n<div>%s</div>", buf);
}

void print_table_headers() /* print table headers */
{   
    printf("\n<tr><th>n</th><th>binary</th><th>hexadecimal</th><th>octal</th><th>decimal</th></tr>");
}

void print_table_row(unsigned a, char *astr) /* print table data row */
{
    printf("\n<tr><td>%s</td><td>", astr);
    display_bits(a);
    printf("</td><td>%#x</td><td>%#o</td><td>%u</td></tr>", a, a, a, a);
}

void print_convert_table(unsigned a) /* print html table */
{
    printf("\n<table class=\"right-nowrap\">");
    print_table_headers();
    print_table_row(a, "a");
    printf("\n</table>");
}

void print_complement_table(unsigned n) /* print html table */
{    
    printf("\n<table class=\"right-nowrap\">");
   print_table_headers();
    print_table_row(n, "a");
    print_table_row(~n, "~a");
    printf("\n</table>");
}

void print_mask_table(unsigned a, unsigned b, char *operation)
{
   unsigned x;
   char opstr[6];

   if (strcmp("and", operation) == 0) {
      strcpy(opstr, "a & b");
      x = a & b;
   }
   else if (strcmp("or", operation) == 0) {
      strcpy(opstr, "a | b");
      x = a | b;
   }
   else if (strcmp("xor", operation) == 0) {
      strcpy(opstr, "a ^ b");
      x = a ^ b;
   }
   /* print html table */
   printf("\n<table class=\"right-nowrap\">");
   print_table_headers();
   print_table_row(a, "a");
   print_table_row(b, "b");
   print_table_row(x, opstr);
   printf("\n</table>");
}

void print_shift_table(unsigned a, unsigned n, char *operation)
{
   unsigned x;
   char opstr[10];

   if (strcmp("left", operation) == 0) {
      sprintf(opstr, "a << %u", n);
      x = a << n;
   }
   else if (strcmp("right", operation) == 0) {
      sprintf(opstr, "a >> %u", n);
      x = a >> n;
   }
   /* print html table */
   printf("\n<table class=\"right-nowrap\">");
   print_table_headers();
   print_table_row(a, "a");
   print_table_row(x, opstr);
   printf("\n</table>");
}    

/* output the bit pattern */
void display_bits(int a)
{
   int b, m, count, nbits;
   unsigned mask;

   /* determine the word size in bits and set the initial mask */
   nbits = 8 * sizeof(int);
   m = 0x1 << (nbits - 1);         /* place 1 in leftmost position */

   mask = m;
   for (count = 1; count <= nbits; count++) {
      b = (a & mask) ? 1 : 0;     /* set display bit on or off */
      printf("%x", b);            /* print display bit */
      if (count % 4 == 0)
         printf(" ");            /* blank spaces after every 4th digit */
      mask >>= 1;
    }
}

void print_error_exit_msg()
{
   printf("\n<p>error - processing terminated</p>");
   printf("\n<footer><p>C bitwise calculator</p></footer>");
   printf("\n</body>");
   printf("\n</html> ");
}
