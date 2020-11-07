#include "../configLoader.h"

/*
 * returns the position in the string the function exited at
*/
int extract_field(string_t str, unsigned int *pos, string_t *dest)
{
    dest->len = 0;
    unsigned int i;
    if (str.len <= *pos)
    {
        return FALSE;
    }

    /*
     * Ignore leading whitespaces
    */
    for (i = *pos; i < str.len; i++)
    {
        if (str.str[i] != CONFIG_IGNORE_CHAR)
        {
            // Done with leading whitespaces
            break;
        }
    }
    *pos = i;

    /*
     * If we've reached an = as its own field
    */
    if (str.str[*pos] == CONFIG_FIELD_DECLARATION_CHAR)
    {
        string_write_char(dest, str.str[*pos]);
        string_null_terminate(dest);
        *pos += 1;
        return TRUE;
    }

    /*
     * If we've reached a # as its own field
    */
    if (str.str[*pos] == CONFIG_COMMENT_CHAR)
    {
        string_write_char(dest, str.str[*pos]);
        string_null_terminate(dest);
        *pos += 1;
        return TRUE;
    }

    /*
     * If we've reached an array declaration square brace
    */
    if (str.str[*pos] == CONFIG_ARRAY_START_CHAR || str.str[*pos] == CONFIG_ARRAY_END_CHAR)
    {
        string_write_char(dest, str.str[*pos]);
        string_null_terminate(dest);
        *pos += 1;
        return TRUE;
    }

    /*
     * If the current field is within quotes
    */
    int inQuotes = FALSE;
    if (str.str[*pos] == CONFIG_STRING_ENCLOSE_CHAR)
    {
        *pos += 1;
        inQuotes = TRUE;
    }

    if (inQuotes)
    {
        /*
         * Read until the string is escaped
        */
        for (i = *pos; i < str.len; i++)
        {
            char c = str.str[i];

            if (c == CONFIG_STRING_ENCLOSE_CHAR)
            {
                // Move to the next character then exit the loop
                i++;
                break;
            }
            if (c == CONFIG_ESCAPE_CHAR && str.str[i + 1] == CONFIG_STRING_ENCLOSE_CHAR)
            {
                // reached a quote intended to be within a string
                string_write_char(dest, str.str[i + 1]);
                i++;
            }
            else
            {
                string_write_char(dest, c);
            }
        }
    }
    else
    {
        /*
         * Read until a whitespace or special char related to
         * the config file is read
        */
        for (i = *pos; i < str.len; i++)
        {
            char c = str.str[i];

            if (c == CONFIG_IGNORE_CHAR)
            {
                // Move to the next character then exit the loop
                i++;
                break;
            }
            else if (c == CONFIG_FIELD_DECLARATION_CHAR ||
                     c == CONFIG_STRING_ENCLOSE_CHAR ||
                     c == CONFIG_COMMENT_CHAR ||
                     c == CONFIG_ARRAY_START_CHAR ||
                     c == CONFIG_ARRAY_END_CHAR)
            {
                break;
            }
            else
            {
                string_write_char(dest, c);
            }
        }
    }

    // Null terminate the field we've extracted
    string_null_terminate(dest);

    // Return the position in the string of the next unread char
    *pos = i;
    return TRUE;
}

array_t read_config_file(char *filePath)
{
    // Storing the processed data
    dynamic_array_t data = new_dynamic_array(sizeof(config_var_t));
    config_var_t newVar;

    // reading from the file
    FILE *configFile = fopen(filePath, MODE_READ);
    string_t buffer = new_string(DEFAULT_BUFFER_LEN);
    string_t field = new_string(DEFAULT_BUFFER_LEN);

    /*
     * Read every line in the file
    */
    int fieldType = CONFIG_FIELD_NAME;
    int inArray = FALSE;
    
    while (fileio_next_line(configFile, &buffer))
    {
        int currField = CONFIG_FIELD_NAME;
        unsigned int pos = 0;
        while (extract_field(buffer, pos, &field))
        {
            if (field.len == 1)
            {
                if (field.str[0] == CONFIG_COMMENT_CHAR)
                {
                    break;
                }
                else if (field.str[0] == CONFIG_FIELD_DECLARATION_CHAR)
                {
                    currField = CONFIG_FIELD_DECLARATION;
                }
                else if (field.str[0] == CONFIG_ARRAY_START_CHAR)
                {
                    inArray = TRUE;
                }
                else if (field.str[0] == CONFIG_ARRAY_END_CHAR)
                {
                    inArray = FALSE;
                }
            }

            if (currField == CONFIG_FIELD_NAME)
            {
                newVar.varName = string_copy(field);

                // Reset the array for storing the new data
                data.len = 0;
            }
            else if (currField == CONFIG_FIELD_DECLARATION)
            {
                string_t fieldCpy = string_copy(field);

                // Add the current field to the dynamic array
                dynamic_array_append(&data, &fieldCpy);
            }
        }
    }
    // if (configFile)
    // {
    //     // For every line in the file
    //     while (fileio_next_line(configFile, &fileBuffer))
    //     {
    //         processLine(&fileBuffer);
    //     }
    // }

    // Dealing with memory
    string_destroy(&buffer);

    // convert the list
    array_t vars = list_to_array(data);

    // TODO delete the list

    return vars;
}
