#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STRING_LEN 10
#define COMMAND_YES "yes"
#define COMMAND_NO "no"
#define COMMAND_CUSTOM "custom"
#define COMMAND_CURRENT "current"
#define COMMAND_MONTHLY "monthly"
#define COMMAND_QUARTERLY "quarterly"
#define COMMAND_ANNUALLY "annually"

struct col {
    double inflation;
    char expense_period[MAX_STRING_LEN];
    double expenses_difference;
    double new_expenses;
};

struct loan {
    int amount;
    int term;
    double current_ir;
    double future_ir;
};

void remove_newline(char input[MAX_STRING_LEN]);
void do_print_help(void);
void buffer_clear(void);
struct col *do_col(double inflation);
struct loan *do_loan(void);
double inflation_determiner(int loan_flag);


int main (void) {
    
    printf("\nWelcome to our Financial calculator, our calculator uses user inputted data to calculate how changes in inflation and the cash rate will impact the cost of living of users\n\n");

    printf("Do you currently possess a loan that is impacted by changes in the cash rate (yes/no)? ");
    
    char command_line[MAX_STRING_LEN];
    command_line[0] = '\0';

    while (strcmp(command_line, COMMAND_NO) != 0 && strcmp(command_line, COMMAND_YES) != 0 && fgets(command_line, 4, stdin) != NULL) {
        
        remove_newline(command_line);

        if (strcmp(command_line, COMMAND_NO) == 0) {
            int loan_flag = 0;
            double inflation = inflation_determiner(loan_flag);
            struct col *col_data = do_col(inflation);
            if (inflation < 0) {
                col_data->expenses_difference = col_data->expenses_difference * -1;
                if (strcmp(col_data->expense_period, COMMAND_ANNUALLY) == 0) {
                printf("\nBased upon the information you have enterred," 
                "an annual inflation rate of %0.1lf%% would result in your annual expenses changing by -$%0.2lf to $%0.2lf\n",
                col_data->inflation, col_data->expenses_difference, col_data->new_expenses);
                } else {
                printf("\nBased upon the information you have enterred,"
                "an annual inflation rate of -%0.1lf%% would result in your %s expenses by -$%0.2lf to $%0.2lf\n",
                col_data->inflation, col_data->expense_period, col_data->expenses_difference, col_data->new_expenses);
                }   
            } else {
                if (strcmp(col_data->expense_period, COMMAND_ANNUALLY) == 0) {
                printf("\nBased upon the information you have enterred," 
                "an annual inflation rate of %0.1lf would result in your annual expenses changing by $%0.2lf to $%0.2lf\n",
                col_data->inflation, col_data->expenses_difference, col_data->new_expenses);
                } else {
                printf("\nBased upon the information you have enterred,"
                "an annual inflation rate of %0.1lf would result in your %s expenses by $%0.2lf to $%0.2lf\n",
                col_data->inflation, col_data->expense_period, col_data->expenses_difference, col_data->new_expenses);
                }
            }
        } else if (strcmp(command_line, COMMAND_YES) == 0) {
            
            struct loan *loan_data = do_loan();
            int loan_flag = 1;
            double inflation = inflation_determiner(loan_flag);
            struct col *col_data = do_col(inflation);

            int loan_periods = loan_data->term * 12;
            double ir_difference = loan_data->future_ir - loan_data->current_ir;
            double current_period_ir = (loan_data->current_ir / 1200);
            double future_period_ir = (loan_data->future_ir / 1200);
            double monthly_repayment_initial = loan_data->amount * (current_period_ir * pow(1 + current_period_ir, loan_periods)) / (pow(1+current_period_ir, loan_periods) - 1);
            double monthly_repayment_future = loan_data->amount * (future_period_ir * pow(1 + future_period_ir, loan_periods)) / (pow(1+future_period_ir, loan_periods) - 1);
            double repayment_change = monthly_repayment_future - monthly_repayment_initial;
            
            printf("\nA change in the interest rate of %0.1lf%% would result in your monthly loan repayments changing by $%0.2lf from $%0.2lf to $%0.2lf.\n", ir_difference, repayment_change, monthly_repayment_initial, monthly_repayment_future);
            printf("Based upon the expenses data you have enterred, an annual inflation rate of %0.1lf would result in your annual expenses changing by $%0.2lf to $%0.2lf.\n",
            col_data->inflation, col_data->expenses_difference, col_data->new_expenses);

        } else {
            printf("That is an invalid command, please enter \"yes\" or \"no\".\n");
            printf("Do you currently possess a loan that is impacted by changes in the cash rate (yes/no)? ");
            fgets(command_line, 4, stdin);
        }
    }
    return 0;
}

void remove_newline(char input[MAX_STRING_LEN]) {

    int index = 0;
    while (input[index] != '\n' && input[index] != '\0') {
        index++;
    }
    input[index] = '\0';
}

double inflation_determiner(int loan_flag) {

    printf("\nWould you like to use custom or current inflation data? ");
        
    double inflation = 5.5;
    char col_command[MAX_STRING_LEN];
    if (loan_flag == 1) {
        fgets(col_command, 8, stdin);
    }
    col_command[0] = '\0';

    while (strcmp(COMMAND_CUSTOM, col_command) != 0 && strcmp(COMMAND_CURRENT, col_command) != 0 && fgets(col_command, 8, stdin) != NULL) {
        remove_newline(col_command);

        if (strcmp(COMMAND_CURRENT, col_command) == 0) {
            return inflation;
        } else if (strcmp(COMMAND_CUSTOM, col_command) == 0) {
            printf("Please enter a value for annual inflation: ");
            while (scanf(" %lf", &inflation) != 1) {
                printf("That is not a valid value, please enter a valid number: ");
                buffer_clear();
            }
            return inflation;
        } else {
            printf("That is an invalid command, try using either \"custom\" or \"current\" ");
        }
    }
    return inflation;
}

void buffer_clear(void) {
    char c;
    do {
        c = getchar();
    } while (!isdigit(c));
    ungetc(c, stdin);
}

struct col *do_col(double inflation) {

    printf("\nIn order to calculate how changing inflation levels will impact your cost of living, please enter the period over which you record expenses: ");

    char expense_period[MAX_STRING_LEN];
    fgets(expense_period, 10, stdin);
    fgets(expense_period, 10, stdin);
    remove_newline(expense_period);
    
    while(strcmp(expense_period, COMMAND_MONTHLY) != 0 && strcmp(expense_period, COMMAND_QUARTERLY) != 0 && strcmp(expense_period, COMMAND_ANNUALLY) != 0) {
        printf("That is not a valid period, please enter either monthly, quarterly or annually: ");
        fgets(expense_period, 10, stdin);
        remove_newline(expense_period);
    }

    double period_inflation = inflation;

    if (strcmp(expense_period, COMMAND_MONTHLY) == 0) {
        period_inflation = inflation / 12;
    } else if (strcmp(expense_period, COMMAND_QUARTERLY) == 0) {
        period_inflation = inflation / 4;
    } 
    printf("Please enter your average expenses for the enterred period (not inclusive of any loan repayments): ");
    double expenses;
    
    while (scanf("%lf", &expenses) != 1) {
        printf("That answer is not valid, please enter a valid response (a numerical value): ");
        buffer_clear();
    }
    
    
    double inflation_multiplier = 1 + (period_inflation / 100);
    double new_expenses = expenses * inflation_multiplier;
    double expenses_difference = new_expenses - expenses;

    struct col *col_data = malloc(sizeof(struct col));
    col_data->inflation = inflation;
    strcpy(col_data->expense_period, expense_period);
    col_data->new_expenses = new_expenses;
    col_data->expenses_difference = expenses_difference;

    return col_data;
}

struct loan *do_loan(void) {
    
    printf("\nPlease enter the current interest rate being paid on your loan: ");
    double current_ir;
    while (scanf(" %lf", &current_ir) != 1) {
        printf("That answer is not valid, please enter a valid response (e.g. 2.79): ");
        buffer_clear();
    }

    printf("\nPlease enter the theoretical change to the interest rate you'd like to calculate: ");
    double ir_change = 0;
    while (scanf(" %lf", &ir_change) != 1 || (current_ir + ir_change) < 0) {
        printf("That answer is not valid, please enter a valid numerical value (e.g. 0.5): ");
        buffer_clear();
    }

    printf("\nPlease enter the term (in years) over which your loan is being repayed: ");
    int term = 0;
    while (scanf(" %d", &term) != 1 && term <= 0) {
        printf("That answer is not valid, please enter the number of years over which the loan will be payed in full (e.g. 25): ");
        buffer_clear();
    }

    printf("\nPlease enter the total amount of the loan: $");
    int amount = 0;
    while (scanf(" %d", &amount) != 1 && amount <= 0) {
        printf("That answer is not valid, please enter the monetary value of the loan (e.g. 500000): $");
        buffer_clear();
    }
    

    struct loan *loan_data = malloc(sizeof(struct loan));
    loan_data->current_ir = current_ir;
    loan_data->future_ir = current_ir + ir_change;
    loan_data->term = term;
    loan_data->amount = amount;

    return loan_data;
}