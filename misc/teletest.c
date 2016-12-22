#include "teletest.h"


int parse_from_assertion_msg(char* assertion_msg, AssertionInfo *info_ptr)
{
    char* token = strtok(assertion_msg, " ");

    if (strcmp(token, "RUN") != 0)
        return 0;


    // get function name, is this save?
    token = strtok(NULL, " ");
    strcpy(info_ptr->func_name, token);
    token = strtok(NULL, " ");

    // get all arguments
    int count = 0;
    while (token && count < MAX_ARGS)
    {
        strcpy(info_ptr->arg_list[count++], token);
        token = strtok(NULL, " ");
    }

    info_ptr->num_args = count;
    info_ptr->valid = 1;

    return 1;
}

// UNUSED, NOT READY YET
int run_function(char **arg_list, ReturnValue* ret_val_ptr)
{
    // TODO FIXME how can you pass function pointer with unknown arguments
    ret_val_ptr->fval = add(atof(arg_list[0]), atof(arg_list[1]));
    ret_val_ptr->valid;
    ret_val_ptr->type = FLOAT;
}

int run_assertion(AssertionInfo* info_ptr, ReturnValue* ret_val_ptr)
{
    // TODO parse msg
    // strok
    //ReturnValue ret_val;

    if (strcmp(info_ptr->func_name, "add") == 0)
    {
        ret_val_ptr->fval = add(atof(info_ptr->arg_list[0]), atof(info_ptr->arg_list[1]));
        ret_val_ptr->type = FLOAT;
        return 1;
    }
    if (strcmp(info_ptr->func_name, "sub") == 0)
    {
        ret_val_ptr->fval = sub(atof(info_ptr->arg_list[0]), atof(info_ptr->arg_list[1]));
        ret_val_ptr->type = FLOAT;
        return 1;
    }
    if (strcmp(info_ptr->func_name, "mult") == 0)
    {
        ret_val_ptr->fval = sub(atof(info_ptr->arg_list[0]), atof(info_ptr->arg_list[1]));
        ret_val_ptr->type = FLOAT;
        return 1;
    }

    return 0;
}

void parse_to_result_msg(ReturnValue* ret_value_ptr, char* result_msg)
{
    char ret_value_str[16];
    ret_value_str[0] = '\0';

    strcpy(result_msg, "RET ");

    if (ret_value_ptr->type == FLOAT)
        sprintf(ret_value_str, "%a", ret_value_ptr->fval);

    if (ret_value_ptr->type == INT)
        sprintf(ret_value_str, "%d", ret_value_ptr->ival);

    strcat(result_msg, ret_value_str);

}
